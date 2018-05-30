<?php
if (isset($_GET['msg']) )
{
?>
<script type="text/javascript" language="javascript">
document.getElementById('msg').innerHTML='<?php echo ($_GET["msg"]);?>';
document.getElementById('msg').style.display='block';
//$("div.msg").delay(5000).fadeOut();
</script>
<?php
} else{
	//DO Nothing
}
?>
<footer class="footer" style="background-color: black; text-align:center; height:50px; ">
<div class="container">
  <p class="text-muted"><br/><b>QA Team @ NerdWallet</b></p>
</div>
</footer>
</body>
</html>
