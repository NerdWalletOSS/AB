</head>
<body>
<!-- Fixed navbar -->
	<nav class="navbar navbar-inverse navbar-fixed-top">
	<div class="container">
	<div class="navbar-header">
	<!-- Mobile Device Toggle -->
	<button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
	<span class="sr-only">Toggle navigation</span>
	<span class="icon-bar"></span>
	<span class="icon-bar"></span>
	<span class="icon-bar"></span>
	</button>
<!-- Application Title -->
	<a class="navbar-brand" id="modLink" href="home.php"><?php if ($TestType == "ABTest"){ echo "A/B Test Manager"; } elseif ($TestType == "XYTest") 
{echo "URL Router";} else { echo "SOMETHING WRONG";}?></a>
	</div>
	<div id="navbar" class="navbar-collapse collapse">
<ul class="nav navbar-nav navbar-right">
      <li><a href="#"><h5><?php echo "Hello! ".$User; ?></h5></a></li>
    <li ><a href="logout.php"><button type="button" class="btn btn-sm btn-danger">Log Out</button></a></li>
  </ul>
  </div><!--/.nav-collapse -->
  </div>
  </nav>
