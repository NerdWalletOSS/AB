$(document).ready(function() {
  $("#error").css('display', 'none', 'important');
  $('#CheckTests').submit(function() {
    $.ajax({
      type: "POST",
      url: "processor/check_tests_processor.php",
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
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#error_message").html('All well between RTS & Database');
        window.location = "check_tests.php";

        }
      },
      success: function(response, textStatus, XHR) {
        //var id = XHR.getResponseHeader('TestID');
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#error_message").html('All well between RTS & Database');
        window.location = "check_tests.php";

      },
      beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }
    });
    return false;
  });
});
