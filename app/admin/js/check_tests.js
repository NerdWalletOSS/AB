$(document).ready(function() {
  $("#error").css('display', 'none', 'important');
  $('#CheckTests').submit(function() {
    $.ajax({
      type: "POST",
      url: "processor/check_tests_processor.php",
      error: function(response, textStatus, XHR) {
        if (response != true) {
          var cssLink = "../css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#stack_trace").css('display', 'inline', 'important');
          $("#error_message").html(response.getResponseHeader('Error-Message'));
          $("#stack_trace").html(response.getResponseHeader('Error-BackTrace'));
        } else {
          var cssLink = "../css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#error_message").html('All well between RTS & Database');
        }
      },
      success: function(response, textStatus, XHR) {
          var cssLink = "../css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#error_message").html('All well between RTS & Database');
      },
      beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }
    });
    return false;
  });
});
