$(document).ready(function() {

  function confirm() {
    confirm("Are you sure, you want to change the state?");
  }

  function action_state(state_id) {
    switch (state_id) {
      case '1':
        return 'Publish';
        break;

      case '2':
        return 'Start';
        break;

      case '3':
        return 'Stop';
        break;

      case '4':
        return 'Archive';
        break;
    }
  }
  function mode(state_id) {
    switch ( state_id ) {
    case "1" : 
    case "2" : 
    case "3" : 
       return "Edit";
      break;
    case "4" : 
    case "5" : 
       return "View";
      break;
    default : 
      // HANDLE ERROR PROPERLY TODO
      break;
    }
  }
  $('#jsTestTable').DataTable({
    "order": [
      [0, "asc"]
    ]
  });
  $("#error").css('display', 'none', 'important');
  $('input[type="radio"]').click(function() {
  $("#error").css('display', 'none', 'important');
    var option = $(this).val();
    $.ajax({
      type: "POST",
      url: "processor/filter_test.php",
      data: $(this).serialize(),
      error: function(response, textStatus, XHR) {
        console.log(response);
        console.log(textStatus);
        console.log(XHR);
        console.log(response.getAllResponseHeaders());
        if (response.getResponseHeader('Error-Code') != 200) {
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#stack_trace").css('display', 'inline', 'important');
          $("#error_message").html(response.getResponseHeader('Error-Message'));
          $("#stack_trace").html(response.getResponseHeader('Error-BackTrace'));
        }
      },
      success: function(response, textStatus, XHR) {
        // Make customised table
        $.makeTable = function(jsonData) {
          var table = $('<table id="jsTestTable" class="display"  style="word-wrap: break-word"><thead> <tr><th>ID</  th><th>Name</th><th>Check Test</th><th>Action</th> </tr></thead><tfoot> <tr><th>ID</  th><th>Name</th><th>Check Test</th><th>Action</th></tr></tfoot>');
          for (var k in jsonData[0])
            var tblHeader = "";
          tblHeader += "<th>" + k[0] + "</th>";
          $.each(jsonData, function(index, value) {
            var TableRow = "<tr>";
            TableRow += "<td><a href='aev_test_1.php?TestID=" + value['id'] + "'>" + value['id'] + "</a></td>";
            TableRow += "<td><a href='aev_test_1.php?TestID=" + value['id'] + "'>" + value['name'] + "</a></td>";
            if ((value['state_id'] == 3) || (value['state_id'] == 4)) {
            TableRow +=  "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><button class='check_test' data-key ='"+ value['name'] + "'>Check Test</button></td>";
						} else {  
            TableRow +=  "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>N/A</td>";
             }
   /*         if (value['state_id'] < 5) {
            TableRow += "<td><a href='processor/set_state_processor.php?TestID=" + value['id'] + "&state_id=" + value['state_id'] + "'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>" + action_state(value['state_id']) + "</button>"
*/  
					if (value['state_id'] == 3) {
            TableRow += "<td><a href='fix_to_a_winner.php?TestID=" + value['id'] + "'><button type='button' class='btn btn-primary btn-xs'>" + action_state(value['state_id']) + "</button></a>";
} else {
TableRow += "<td><a href='processor/set_state_processor.php?TestID=" + value['id'] + "&state_id=" + value['state_id'] + "'><button type='button' class='check_test btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>" + action_state(value['state_id']) + "</button></a></td>"; 
}          
 if (value['state_id'] == 1) {
            TableRow += "&nbsp;&nbsp;<a href='processor/set_state_processor.php?TestID=" + value['id'] + "&state_id=" + value['state_id'] + "&action=delete" + "'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>Delete</button>";
            } 
            if (value['state_id'] == 4) {
            TableRow += "&nbsp;&nbsp;<a href='processor/set_state_processor.php?TestID=" + value['id'] + "&state_id=" + value['state_id'] + "&action=resurrect" + "'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>Resuurect</button></td>";}      
						// else {
						//TableRow += "<strong>No Action</strong></td>";
						//}
            TableRow += "</tr>";

            $(table).append(TableRow);
          });
          return ($(table));
        };
        var jsonData = eval(response);
        if (jsonData == null) {
          var TableRow = "";
          var table = '<table id="jsTestTable" class="display"  style="word-wrap: break-word"><thead> <tr><th>ID</  th><th>Name</th><th>Action</th> </tr></thead><tfoot> <tr><th>ID</  th><th>Name</th><th>Action</th> </tr></tfoot>'
        } else {
          var table = $.makeTable(jsonData);
        }
        $("#show-data").html(table);
        $('#jsTestTable').DataTable({
          "order": [
            [0, "asc"]
          ]
        });
      }
      /*beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }*/
    });
    option.attr( "checked" );//option.prop('checked', true);
    return false;
  });


 $(document).on("click", ".check_test", function() {
    var name = $(this).attr('data-key'); // $(this) refers to button that was clicked
    //e.preventDefault();
    $.ajax({
      type: "POST",
      url: "processor/check_test_processor.php?TestName="+name,
      //data: $(this).serialize(),
      error: function(response, textStatus, XHR) {
        if (response.getResponseHeader('Error-Code') != 200) {
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#stack_trace").css('display', 'inline', 'important');
          $("#error_message").html(response.getResponseHeader('Error-Message'));
          $("#stack_trace").html(response.getResponseHeader('Error-BackTrace'));
        } else {
          //var id = response.getResponseHeader('TestID');
          window.location = "home.php";
        }
      },
      success: function(response, textStatus, XHR) {
        //var id = XHR.getResponseHeader('TestID');
        window.location = "home.php";
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#stack_trace").css('display', 'inline', 'important');
          $("#error_message").html('Test is in sync with RTS & Db');
      },
      beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }
    });
    return false;
  });

});