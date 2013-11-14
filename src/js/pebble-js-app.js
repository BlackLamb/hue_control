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
    }
);
