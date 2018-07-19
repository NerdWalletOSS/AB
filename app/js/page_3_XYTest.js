$(document).ready(function() {
  $("#error").css('display', 'none', 'important');
  $('#device_x_variant').submit(function(e) {
    e.preventDefault();
    $.ajax({
      type: "POST",
      url: "processor/page_3_processor.php",
      data: $(this).serialize(),
      error: function(response, textStatus, XHR) {
        if (response.getResponseHeader('Error-Code') != 200) {
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#stack_trace").css('display', 'inline', 'important');
          $("#error_message").html(response.getResponseHeader('Error-Message'));
          $("#error_message").html(response.getResponseHeader('Error-Msg-Out'));
          $("#stack_trace").html(response.getResponseHeader('Error-BackTrace'));
        } else {
          var id = response.getResponseHeader('TestID');
          window.location = "aev_test_4.php?TestID=" + id;
        }
      },
      success: function(response, textStatus, XHR) {
        var id = XHR.getResponseHeader('TestID');
        window.location = "aev_test_4.php?TestID=" + id;
      },
      beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }
    });
    return false;
  });
});
