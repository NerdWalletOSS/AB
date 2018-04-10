<?php 

// -- CALL REQUIRED FILES
set_include_path(get_include_path() . PATH_SEPARATOR . "php/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/db_helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/helpers/");
set_include_path(get_include_path() . PATH_SEPARATOR . "php/rts/");
// -- GET TESTS
require_once "db_get_rows.php";
require_once "utility_functions.php";
$state = "'2','3','4'";
$result = db_get_rows("test", "test_type_id = 2 and state_id IN (" . $state . ")");
$nR = count($result);
?>
<!html>
<head>
  <title>Demo Advertisement Page</title>
  <!-- Bootstrap core CSS -->
  <link href="bootstrap-3.3.5-dist/css/bootstrap.min.css" rel="stylesheet">
  <!-- Bootstrap theme -->
  <link href="bootstrap-3.3.5-dist/css/bootstrap-theme.min.css" rel="stylesheet">
  <!-- Custom styles for this template -->
  <link href="css/theme.css" rel="stylesheet">
  <!-- jQuery  -->
  <script src="js/jquery-1.11.1.min.js"></script>
  <script src="js/bootstrap.min.js"></script>
  <link href="css/bootstrap-editable.css" rel="stylesheet">
  <script src="tcc/js/bootstrap-editable.js"></script>
  <script src="js/bootstrap-tooltip.js"></script>
  <script src="js/bootstrap-confirmation.js"></script>
</head>
<body>
  <nav class="navbar navbar-inverse navbar-fixed-top">
  <div class="container">
  <div class="navbar-header">
  <a class="navbar-brand" href="ad.php">Demo Advertisement Page</a>
  </div>
  <!--/.nav-collapse -->
  </div>
  </nav>
  <div align="center">
  <div class="container theme-showcase" role="main" align="center">
  <div class="row" align="center">
  <div class="col-xs-4">
  <div class="panel panel-primary">
  <div class="panel-heading">
  <h3 class="panel-title">Ad | Nerdwallet</h3>
  </div>
  <div class="panel-body">
  <form class="form-signin" action="http://173.255.252.157:8000/Router?nw_campaign_id=<?php echo $_GET['nw_campaign_id'];?>" method="GET" >
  <div class="form-group">
  <div class="table-responsive">
  <table >
  <tbody>
  <tr>
  <td><img src="images/ad.jpg" width="300px">
  <label for="sel1">Pick an Experiment: &nbsp;<span class='glyphicon glyphicon-question-sign' data-placement='top' data-toggle='tooltip' href='#' data-original-title='Picking a winner fixes the experiment to a single variant or the control group. `'></span></label><br/>
  <select class="form-control" id="sel1" name="nw_campaign_id">
<?php for ( $i = 0; $i < $nR; $i++ ) {
  echo "<option value='".$result[$i]['t_create']."'>".$result[$i]['name']."</option>";
} ?>
  </select><br/>
  </td></tr><tr><td>Age: <?php echo $age=rand(12,99); ?><input type="hidden" name="age" value="<?php echo  $age; ?>">
  </td></tr><tr><td>Gender: <?php $gender=rand(1,2); switch ($gender) {case 1: echo "Male"; break; case 2: echo "Female";} ?><input type="hidden" name="gender" value="<?php echo  $gender; ?>">
  </td></tr><tr><td>Geolocation: <?php $a=array("Washington","New York","Palo Alto","San Fransisco","California");
$random_keys=array_rand($a,2); echo $a[$random_keys[0]];  ?><input type="hidden" name="geolocation" value="<?php echo $a[$random_keys[0]]; ?>">
  <button class="btn btn-lg btn-danger btn-block" type="submit" data-title="Pick an Experiment" data-message="Pick an Experiment">Click</button>
  </td>
  </tr>
  </tbody>
  </table>
  </div>
  </div>
  </form>	
  </div>
  </div>
  </div>
  </div>
  </div>
<!-- FOOTER -->
<?php require_once "common/footer.php"; ?>
