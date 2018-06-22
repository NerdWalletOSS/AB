$(document).ready(function() {
  $("#error").css('display', 'none', 'important');
  $("#check_test").click(function(e) {
    var name = $(this).attr('data-key'); // $(this) refers to button that was clicked
    e.preventDefault();
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
