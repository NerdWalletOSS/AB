$(document).ready(function() {

//-------------------------------------------------------//
// ADD TEST //
  $("#error").css('display', 'none', 'important');
  $('#addTest').submit(function(e) {
    e.preventDefault();
    $.ajax({
      type: "POST",
      url: "processor/insert_test_processor.php",
      data: $(this).serialize(),
      error: function(response, textStatus, XHR) {
        console.log(response);
        if (response.getResponseHeader('Error-Code') != 200) {
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#stack_trace").css('display', 'inline', 'important');
          $("#error_message").html(response.getResponseHeader('Error-Message'));
          $("#stack_trace").html(response.getResponseHeader('Error-BackTrace'));
        } else {
          var id = response.getResponseHeader('TestID');
          window.location = "aev_test_2.php?TestID=" + id;
        }
      },
      success: function(response, textStatus, XHR) {
        console.log(response);
        console.log(XHR);
        console.log(textStatus);
        var id = XHR.getResponseHeader('TestID');
        console.log("TEST ID = " + id);
        window.location = "aev_test_2.php?TestID=" + id;
      },
      beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }
    });

    return false;

  });
//--------------------------------------------------------------------------//
// TEST URL //
  $("#error").css('display', 'none', 'important');
  $('#TestURL').submit(function(e) {
    e.preventDefault();
    $.ajax({
      type: "POST",
      url: "processor/test_url_processor.php",
      data: $(this).serialize(),
      error: function(response, textStatus, XHR) {
        console.log(response);
        if (response.getResponseHeader('Error-Code') != 200) {
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
          $("#stack_trace").css('display', 'inline', 'important');
					var URLReturned = response.match(/(?:"[^"]*"|^[^"]*$)/)[0].replace(/"/g, "").replace(/Redirecting to /g,'');
          $("#error_message").html(URLReturned);
          $("#stack_trace").html(response.getResponseHeader('Error-BackTrace'));
        }
      },
      success: function(response, textStatus, XHR) {
        console.log(response);
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error").css('display', 'inline', 'important');
          $("#error_message").css('display', 'inline', 'important');
					var URLReturned = response.match(/(?:"[^"]*"|^[^"]*$)/)[0].replace(/"/g, "").replace(/Redirecting to /g,'');
          $("#error_message").html(URLReturned);
      },
      beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }
    });
    return false;
  });

//------------------------------------------------------------------------//
// CLONE A TEST
  $(".OpenCloneModal").click(function(e) {
      e.preventDefault();       
      $("#CloneModalTitle").html('Do you wish to clone the test '+$(this).data('name')+' ?');
      $("#TestID").val($(this).data('id'));
      $("#TestName").val($(this).data('name'));
      $("#Creator").val($(this).data('creator'));

      $('#CloneModal').modal('show');

  });
  $("#error_clone").css('display', 'none', 'important');
$("#CloneModalSubmit").click(function(){
      $.ajax({
             url: 'processor/clone_test_processor.php',
             data: {
               id: $("#TestID").val(),
               name: $("#TestName").val(),
               creator: $("#Creator").val(),
               clone: $("#CloneTestName").val()
             },
             dataType: 'json',
        error: function(response, textStatus, XHR) {
        if (response.getResponseHeader('Error-Code') != 200) {
          var cssLink = "css/error.css";
          $("head").append("<link href=" + cssLink + " rel='stylesheet' />");
          $("#error_clone").css('display', 'inline', 'important');
          $("#error_message_clone").css('display', 'inline', 'important');
          $("#stack_trace_clone").css('display', 'inline', 'important');
          $("#error_message_clone").html(response.getResponseHeader('Error-Message'));
          $("#stack_trace_clone").html(response.getResponseHeader('Error-BackTrace'));
        } else {
        $('#CloneModal').find('#modalAlert').addClass('alert-success');
        $('#CloneModal').find('#modalAlert').html(data.message).show; 
        $('#CloneModal').find('#modalAlert').removeClass('hidden');
        $('#CloneModal').modal('toggle');
        }
      },
      success: function(response, textStatus, XHR) {
        $('#modalAlert').addClass('alert-success');
        $('#modalAlert').html(response.message).show; 
        $('#modalAlert').removeClass('hidden');
        $('#CloneModal').modal('toggle');
      },
      beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }

       });    
  });
//-------------------------------------------------------------
// VARIANT COUNT TABLE

 $("#is_vc_stats").change(function (e) {
    e.preventDefault();
    var ischecked= $(this).is(':checked');
    if(ischecked) {
    var id = $(this).data('id');
    $.ajax({
      type: "POST",
      url: "processor/variant_count_rts_processor.php?id="+id,
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
          $("#vc_stats").html(response);
          $('#vc_stats').removeClass("table hidden");
      },
      beforeSend: function() {
        $("#error_message").css('display', 'inline', 'important');
        $("#error_message").html("Loading...")
      }
    });
    } else {
    $('#vc_stats').addClass('table hidden');
    }
    return false;
  });
});
