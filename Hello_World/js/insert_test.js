$(document).ready(function(){
  $("#error").css('display', 'none', 'important');
		$('#addTest').submit(function(){
			$.ajax({
				type: "POST",
		   	url: "processor/insert_test_processor.php",
			 	data: $(this).serialize(),
			 	//dataType: "json",//set to JSON
		   	success: function(response, textStatus, xhr){  
				console.log(response)
				console.log(xhr.getAllResponseHeaders())
				//var json_obj = $.parseJSON(response);//parse JSON
				//alert(json_obj.status)
				if(xhr.getResponseHeader('Error-Code') != 400)    {
			 		window.location="home.php";
				}
				else    {
					$("#error").css('display', 'inline', 'important');
					var cssLink = "css/error.css";
        	$("head").append("  <link href=" + cssLink + " rel='stylesheet' /> ");
			 		$("#error_message").html(xhr.getResponseHeader('Error-Message'));
			 		$("#stack_trace").html(xhr.getResponseHeader('Error-BackTrace'));
				}
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
