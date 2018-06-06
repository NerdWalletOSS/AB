<?php
error_reporting( E_ALL );
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/db_helpers");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php/rts");
set_include_path(get_include_path() . PATH_SEPARATOR . "../../php");

require_once "../../php/rts/list_rts.php";
require_once "../includes/header_diagnostics.php"; 
?>

<div class="container theme-showcase" role="main" >
<?php
$SP = list_rts();
if ( $SP ) { 
   foreach ( $SP as $sp ) { 
     $server = $sp['server']; $port = $sp['port'];
     echo "server = $server, port = $port \n";
  }
}
else {
  echo "Not using RTS\n";
}
?> 
</div>
<!-- /container -->
	<!-- FOOTER -->
<?php require_once "../includes/footer.php"; ?>
