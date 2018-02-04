<?php require_once "common/header_1.php"; ?>
<script src="js/insert_test.js"></script>
<?php require_once "common/header_2.php"; ?>
<div class="row">
<div class="col-xs-12">
<div class="panel panel-primary">
<div class="panel-heading">
<h3 class="panel-title">Add Test</h3>
</div>
<div class="panel-body">
<!-- Error messages will move to the div-->
<?php require_once "common/error_div.php"; ?>
<form id='addTest'>
Test Name:&nbsp;&nbsp;
<input type="text" id="TestName" name="TestName"  value="" />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Test Description:&nbsp;&nbsp;
<input type="text" id="TestDescription" name="TestDescription"  value="" />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input  id="submit" type="submit" value="Add Test" />
</form>
</div></div></div></div>
<?php require_once "common/footer.php"; ?>
