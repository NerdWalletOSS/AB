<!-- Fixed navbar -->
<nav class="navbar navbar-inverse navbar-fixed-top">
  <div class="container">
    <div class="navbar-header">
      <!-- Mobile Device Toggle -->
      <button aria-controls="navbar" aria-expanded="false" class="navbar-toggle collapsed" data-target="#navbar" data-toggle="collapse" type="button"><span class="sr-only">Toggle navigation</span> <span class="icon-bar"></span> <span class="icon-bar"></span> <span class="icon-bar"></span></button> <!-- Application Title -->
       <a class="navbar-brand" href="home.php" id="modLink">
<?php
if ($TestType == "ABTest")
	{
		echo "A/B Test Manager";
	}
elseif ($TestType == "XYTest")
	{
		echo "URL Router";
	}
else
	{
		echo "SOMETHING WRONG";
	}
?>
       </a>
    </div>
    <div class="navbar-collapse collapse" id="navbar">
      <ul class="nav navbar-nav navbar-right">
        <li>
          <a href="#">
          <h5>
<?php
  echo "Hello! " . $User;
?>
         </h5>
         </a>
        </li>
        <li>
          <a href="logout.php"><button class="btn btn-sm btn-danger" type="button">Log Out</button></a>
        </li>
      </ul>
    </div><!--/.nav-collapse -->
  </div>
</nav>
