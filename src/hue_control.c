#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
static char mesg[10];

enum {
  APP_STATE = 0x1,
  LIGHTS = 0x2,
  LIGHT_STATE = 0x3,
};

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {

}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  Tuple down_tuple = TupletCString(LIGHT_STATE, "Down Sent");
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
	
	if (iter == NULL) {
		return;
	}
	
	dict_write_tuplet(iter, &down_tuple);
	dict_write_end(iter);
	
	app_message_outbox_send();
}

void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
}

void in_received_handler(DictionaryIterator *received, void *context) {
	//incoming message received
	Tuple *app_state_tuple = dict_find(received, APP_STATE);
	Tuple *lights_tuple = dict_find(received, LIGHTS);
	Tuple *light_state_tuple = dict_find(received, LIGHT_STATE);
	
	if (app_state_tuple) {
		strncpy(mesg, app_state_tuple->value->cstring, 10);
		text_layer_set_text(text_layer, mesg);
	}
}

void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped!");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void app_message_init(void) {
  // Register message handlers
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  // Init buffers
  app_message_open(64, 64);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Loading...");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  app_message_init();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
