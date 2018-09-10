$(document).ready(function() {
// DECLARING REQUIRED HELPER FUNCTIONS
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
//----------------------------------------------------//

//----------------------------------------------------//
// FILTER TEST WITH RESPECT TO STATES
  $('#jsTestTable').DataTable({
    "order": [
      [6, "desc"]
    ]
  });
  $("#error").css('display', 'none', 'important');
  $('input[type="radio"]').click(function() {
  $("#error").css('display', 'none', 'important');
    var option = $(this).attr('id');
    var admin = $('#TestAdmin').val();
    $.ajax({
      type: "GET",
      //url: "processor/filter_test.php",
      url: "processor/filter_test.php?admin="+admin+"&option="+option,
      //data: JSON.stringify( { "option": option, "admin": admin } ), //$(this).serialize(),
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
          var table = $('<table id="jsTestTable" class="display"  style="word-wrap: break-word"><thead> <tr><th>ID</th><th>Clone</th><th>Name</th><th>Campaign ID</th><th>Check Test</th><th>Action</th> <th>Updated On</th></tr></thead><tfoot> <tr><th>ID</th><th>Clone</th><th>Name</th><th>Campaign ID</th><th>Check Test</th><th>Action</th><th>Updated On</th></tr></tfoot>');
          for (var k in jsonData[0])
            var tblHeader = "";
          tblHeader += "<th>" + k[0] + "</th>";
          $.each(jsonData, function(index, value) {
            var TableRow = "<tr>";
            TableRow += "<td><a href='aev_test_1.php?TestID=" + value['id'] + "'>" + value['id'] + "</a></td>";
            TableRow +=  "<td><a href='/'  class='OpenCloneModal'  data-id="+ value['id'] +" data-name="+ value['name'] +"><button type='button' class='btn btn-warning btn-sm'><span style='font-size:15px;'><strong>+</strong></span></button></a></th>";
            TableRow += "<td><a href='aev_test_1.php?TestID=" + value['id'] + "'>" + value['name'] + "</a></td>";
            TableRow += "<td>" + value['external_id'] + "</td>";
            if ((value['state_id'] == 3) || (value['state_id'] == 4)) {
            TableRow +=  "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><button class='check_test btn btn-warning btn-xs' data-key ='"+ value['name'] + "'>Check Test</button></td>";
						} else {  
            TableRow +=  "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>N/A</td>";
             }
 
					if (value['state_id'] == 3) {
            TableRow += "<td><a href='fix_to_a_winner.php?TestID=" + value['id'] + "'><button type='button' class='btn btn-primary btn-xs'>" + action_state(value['state_id']) + "</button></a>";
} else if (value['state_id'] == 5) {
  TableRow += "<td><strong>No Action</strong>";
}  else {
TableRow += "<td><a href='processor/set_state_processor.php?TestID=" + value['id'] + "&state_id=" + value['state_id'] + "'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>" + action_state(value['state_id']) + "</button></a>"; 
}          
 if (value['state_id'] == 1) {
            TableRow += "&nbsp;&nbsp;<a href='processor/set_state_processor.php?TestID=" + value['id'] + "&state_id=" + value['state_id'] + "&action=delete" + "'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>Delete</button>";
            } 
            if ((value['state_id'] == 4) && (value['test_type_id'] == 2)) {
            TableRow += "&nbsp;&nbsp;<a href='processor/set_state_processor.php?TestID=" + value['id'] + "&state_id=" + value['state_id'] + "&action=resurrect" + "'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>Resuurect</button>";}      
            TableRow += "</td>";

						var display_date = new Date(value['updated_at']).toLocaleDateString('en-GB', {
    				day : 'numeric',
    				month : 'short',
    				year : 'numeric'
						}).split(' ').join('-');
  				  TableRow += "<td>" + display_date + "</td>";
						TableRow +="</tr>";
            $(table).append(TableRow);
          });

          return ($(table));
        };
        var jsonData = eval(response); 
        if (jsonData == null) {
          var TableRow = "";
          var table = '<table id="jsTestTable" class="display"  style="word-wrap: break-word"><thead> <tr><th>ID</th><th>Clone</th><th>Name</th><th>Campaign ID</th><th>Check Test</th><th>Action</th> <th>Updated On</th></tr></thead><tfoot> <tr><th>ID</th><th>Clone</th><th>Name</th><th>Campaign ID</th><th>Check Test</th><th>Action</th> <th>Updated On</th></tr></tfoot>'
        } else {
          var table = $.makeTable(jsonData);
        }
        $("#show-data").html(table);
        $('#jsTestTable').DataTable({
          "order": [
            [6, "desc"]
          ]
        });
      },
      beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }
    });
    option.prop('checked', true);
    return false;
  });

//----------------------------------------------------//
// FILTER TEST WITH RESPECT TO ADMIN NAME
  $("#error").css('display', 'none', 'important');
  $('#TestAdmin').change(function() {
  $("#error").css('display', 'none', 'important');
    var admin = $(this).val();
    var option = $('input[name=option]:checked').attr('id');
    $.ajax({
      type: "GET",
      //url: "processor/filter_test.php",
      url: "processor/filter_test.php?admin="+admin+"&option="+option,
      //data: JSON.stringify( { "option": option, "admin": admin } ), //$(this).serialize(),
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
          var table = $('<table id="jsTestTable" class="display"  style="word-wrap: break-word"><thead> <tr><th>ID</th><th>Clone</th><th>Name</th><th>Campaign ID</th><th>Check Test</th><th>Action</th> <th>Updated On</th></tr></thead><tfoot> <tr><th>ID</th><th>Clone</th><th>Name</th><th>Campaign ID</th><th>Check Test</th><th>Action</th><th>Updated On</th></tr></tfoot>');
          for (var k in jsonData[0])
            var tblHeader = "";
          tblHeader += "<th>" + k[0] + "</th>";
          $.each(jsonData, function(index, value) {
            var TableRow = "<tr>";
            TableRow += "<td><a href='aev_test_1.php?TestID=" + value['id'] + "'>" + value['id'] + "</a></td>";
            TableRow +=  "<td><a href='#'  class='OpenCloneModal'  data-id="+ value['id'] +" data-name="+ value['name'] +"><button type='button' class='btn btn-warning btn-sm'><span style='font-size:15px;'><strong>+</strong></span></button></a></th>";
            TableRow += "<td><a href='aev_test_1.php?TestID=" + value['id'] + "'>" + value['name'] + "</a></td>";
            TableRow += "<td>" + value['external_id'] + "</td>";
            if ((value['state_id'] == 3) || (value['state_id'] == 4)) {
            TableRow +=  "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'><button class='check_test btn btn-warning btn-xs' data-key ='"+ value['name'] + "'>Check Test</button></td>";
						} else {  
            TableRow +=  "<td style='word-wrap: break-word;min-width: 160px;max-width: 160px;'>N/A</td>";
             }
 
					if (value['state_id'] == 3) {
            TableRow += "<td><a href='fix_to_a_winner.php?TestID=" + value['id'] + "'><button type='button' class='btn btn-primary btn-xs'>" + action_state(value['state_id']) + "</button></a>";
} else if (value['state_id'] == 5) {
  TableRow += "<td><strong>No Action</strong>";
}  else {
TableRow += "<td><a href='processor/set_state_processor.php?TestID=" + value['id'] + "&state_id=" + value['state_id'] + "'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>" + action_state(value['state_id']) + "</button></a>"; 
}          
 if (value['state_id'] == 1) {
            TableRow += "&nbsp;&nbsp;<a href='processor/set_state_processor.php?TestID=" + value['id'] + "&state_id=" + value['state_id'] + "&action=delete" + "'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>Delete</button>";
            } 
            if ((value['state_id'] == 4) && (value['test_type_id'] == 2)) {
            TableRow += "&nbsp;&nbsp;<a href='processor/set_state_processor.php?TestID=" + value['id'] + "&state_id=" + value['state_id'] + "&action=resurrect" + "'><button type='button' class='btn btn-primary btn-xs' data-toggle='confirmation' data-title='Are you sure?'>Resuurect</button>";}      
            TableRow += "</td>";
						var display_date = new Date(value['updated_at']).toLocaleDateString('en-GB', {
    				day : 'numeric',
    				month : 'short',
    				year : 'numeric'
						}).split(' ').join('-');
  				  TableRow += "<td>" + display_date + "</td>";
						TableRow +="</tr>";
            $(table).append(TableRow);
          });
          return ($(table));
        };
        var jsonData = eval(response);
        if (jsonData == null) {
          var TableRow = "";
          var table = '<table id="jsTestTable" class="display"  style="word-wrap: break-word"><thead> <tr><th>ID</th><th>Clone</th><th>Name</th><th>Campaign ID</th><th>Check Test</th><th>Action</th> <th>Updated On</th></tr></thead><tfoot> <tr><th>ID</th><th>Clone</th><th>Name</th><th>Campaign ID</th><th>Check Test</th><th>Action</th> <th>Updated On</th></tr></tfoot>'
        } else {
          var table = $.makeTable(jsonData);
        }
        $("#show-data").html(table);
       $('#jsTestTable').DataTable({
          "order": [
            [6, "desc"]
          ]
        });
      },
      beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }
    });
    option.prop('checked', true);
    return false;
  });

//---------------------------------------------------------------------//
// CHECK TEST

 $(document).on("click", ".check_test", function() {
    var name = $(this).attr('data-key');
    $.ajax({
      type: "POST",
      url: "processor/check_test_processor.php?TestName="+name,
      error: function(response, textStatus, XHR) {
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
//------------------------------------------------------------------------------//

//});

});
