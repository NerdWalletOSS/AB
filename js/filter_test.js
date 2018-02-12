$(document).ready(function(){
      $('#TestTable').DataTable({
        "order": [
          [0, "asc"]
        ]
      });
  $("#error").css('display', 'none', 'important');
     $('input[type="radio"]').click(function(){  
		$(this).attr('checked', true)
     var option = $(this).val();
		//$('#addTest').submit(function(){
			$.ajax({
				type: "POST",
		   	url: "processor/filter_test.php",
			 	data: $(this).serialize(),
			 	//dataType: "json",//set to JSON
		   	error: function(response, textStatus, XHR){  
				console.log(response);
				console.log(textStatus);
				console.log(XHR);
				console.log(response.getAllResponseHeaders());
				if(response.getResponseHeader('Error-Code') != 200)    {
					var cssLink = "css/error.css";
        	$("head").append("<link href=" + cssLink + " rel='stylesheet' />");
				  $("#error").css('display', 'inline', 'important');
				  $("#error_message").css('display', 'inline', 'important');
				  $("#stack_trace").css('display', 'inline', 'important');
			 		$("#error_message").html(response.getResponseHeader('Error-Message'));
			 		$("#stack_trace").html(response.getResponseHeader('Error-BackTrace'));
				}
				/*else    {
					var id = response.getResponseHeader('TestID');
			 		window.location="aev_test_2.php?id=" + id;
				}*/
		   	},
        success: function(response, textStatus, XHR) {
				//console.log(response);
      // Make customised table
      $.makeTable = function(jsonData) {
        var table = $('<table id="jsTestTable_' + option + '" class="display"  style="word-wrap: break-word"><thead> <tr><th>ID</  th><th>Name</th><th>State</th> </tr></thead><tfoot> <tr><th>ID</  th><th>Name</th><th>State</th> </tr></tfoot>');
        for (var k in jsonData[0])
					var tblHeader = "";
          tblHeader += "<th>" + k[0] + "</th>";
        $.each(jsonData, function(index, value) {
          var TableRow = "<tr>";
          TableRow += "<td><a href='aev__test_1?TestID=" + value['id'] + "'>" + value['id'] + "</td>";
          TableRow += "<td>" + value['name'] + "</td>";
          TableRow += "<td>" + value['state_id'] + "</td>";
          TableRow += "</tr>";
          $(table).append(TableRow);
        });
        return ($(table));
      };
      var jsonData = eval(response);
      if (jsonData == null ) { var TableRow = ""; var table = '<table id="jsTestTable_' + option + '" class="display"  style="word-wrap: break-word"><thead> <tr><th>ID</  th><th>Name</th><th>State</th> </tr></thead><tfoot> <tr><th>ID</  th><th>Name</th><th>State</th> </tr></tfoot>'} else { var table = $.makeTable(jsonData);}
		  $("#show-data").html(table);
      //$(table).appendTo("#show-data");
      $('#jsTestTable_' + option + '').DataTable({
        "order": [
          [0, "asc"]
        ]
      });
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
