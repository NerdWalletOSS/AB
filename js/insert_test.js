$(document).ready(function(){
  $("#error").css('display', 'none', 'important');
		$('#addTest').submit(function(){
			$.ajax({
				type: "POST",
		   	url: "processor/insert_test_processor.php",
			 	data: $(this).serialize(),
			 	//dataType: "json",//set to JSON
		   	error: function(response, textStatus, XHR){  
				//console.log(response);
				//console.log(textStatus);
				//console.log(XHR);
				//console.log(response.getAllResponseHeaders());
				if(response.getResponseHeader('Error-Code') != 200)    {
					var cssLink = "css/error.css";
        	$("head").append("<link href=" + cssLink + " rel='stylesheet' />");
				  $("#error").css('display', 'inline', 'important');
				  $("#error_message").css('display', 'inline', 'important');
				  $("#stack_trace").css('display', 'inline', 'important');
			 		$("#error_message").html(response.getResponseHeader('Error-Message'));
			 		$("#stack_trace").html(response.getResponseHeader('Error-BackTrace'));
				}
				else    {
			 		window.location="home.php";
				}
		   	},
        success: function(response, textStatus, XHR) {
    	 		window.location="home.php";
        },
		   	beforeSend:function()
		   {
					$("#error_message").css('display', 'inline', 'important');
					$("#error_message").html("Loading...")
		   }
		  });
		return false;
	});
});
