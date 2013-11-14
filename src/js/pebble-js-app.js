Pebble.addEventListener("ready",
    function(e) {
        console.log("Hello world! - Sent from your javascript application.");
		
		var transactionId = Pebble.sendAppMessage({'0': 42, '1': 'string value'},
												  function(e) {
												  	  console.log("Successfully delivered message with transactionId="
															 + e.data.transactionId);
												  },
												  function(e) {
													  console.log("Unable to deliver message with transactionID="
																 + e.data.transactionId
																 + " Error is: " + e.error.message);
												  }
	   );
		
		Pebble.showSimpleNotificationOnPebble("Test Notify", "This is a test and only a test, if it was real this would be cool");
    }
);

Pebble.addEventListener("showConfiguration",
						function(e) {
							Pebble.openURL("http://www.google.com");
						}					   
);

Pebble.addEventListener("configurationClosed",
						function(e) {
							var configuration = JSON.parse(e.configurationData);
							console.log("Configuration window returned: " + e.configurationData);
						}					   
);
