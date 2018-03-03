$(document).ready(function(){

 $('#myModal').on('show.bs.modal', function (e) {
  var rowid = $(e.relatedTarget).data('id');
  var position = $(e.relatedTarget).data('position');
  var vname = $("#VName_" + position).html();
  var vdesc = $("#Vdesc_" + position).html();
  var vcd = $("#Vcd_" + position).html();

  //$("input[name=VID]").val(rowid);
  $("#VariantID").html(rowid);   
  $("input[name=VariantID]").val(rowid);
  $("#VariantName").html(vname);   
  $("input[name=VariantName").val(vname);
  $("#Description").val(vdesc);
  $("#CustomData").val(vcd);
				//$("input[name=VID]").val(rowid);
/*
        $.ajax({
            type : 'post',
            url : 'fetch_record.php', //Here you will fetch records 
            data :  'rowid='+ rowid, //Pass $id
            success : function(data){
            $('.fetched-data').html(data);//Show fetched data from database
            }
        });
*/
     });

  $("#error").css('display', 'none', 'important');
		$('#addVI').submit(function(){
			$.ajax({
				type: "POST",
		   	url: "processor/add_addln_variant_info.php",
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
			 		window.location="aev_test_3.php?TestID=" + id;
				}
		   	},
        success: function(response, textStatus, XHR) {
					var id = XHR.getResponseHeader('TestID');
			 		window.location="aev_test_3.php?TestID=" + id;
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
