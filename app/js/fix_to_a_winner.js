$(document).ready(function(){
  $("#error").css('display', 'none', 'important');
		$('#fix_to_a_winner').submit(function(){
			$.ajax({
				type: "POST",
		   	url: "processor/fix_to_a_winner_processor.php",
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
					var id = response.getResponseHeader('TestID');
			 		window.location="home.php?TestID=" + id;
				}
		   	},
        success: function(response, textStatus, XHR) {
					var id = XHR.getResponseHeader('TestID');
			 		window.location="home.php?TestID=" + id;
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
