$(document).ready(function() {
  $("#error").css('display', 'none', 'important');
  $('#addAdmin').submit(function() {
    $.ajax({
      type: "POST",
      url: "processor/add_admin_processor.php",
      data: $(this).serialize(),
      error: function(response, textStatus, XHR) {
          $("#AddAdmin .close").click(); 
        if (response.getResponseHeader('Error-Code') != 200) {
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#stack_trace").css('display', 'inline', 'important');
          $("#error_message").html(response.getResponseHeader('Error-Message'));
          $("#stack_trace").html(response.getResponseHeader('Error-BackTrace'));
        } else {
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#error_message").html('Admin Added');
        window.location = "list_admin.php";

        }
      },
      success: function(response, textStatus, XHR) {
        //var id = XHR.getResponseHeader('TestID');
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#error_message").html('Admin Added');
        window.location = "list_admin.php";

      },
      beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }
    });
    return false;
  });
});
