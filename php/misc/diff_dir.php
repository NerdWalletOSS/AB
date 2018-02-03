<?php
function diff_dir(
  $dir1,
  $dir2
)
{
  $D1 = scandir($dir1); 
  if ( !$D1 ) { 
    exit("error"); 
  }
  $D2 = scandir($dir2); 
  if ( !$D2 ) { 
    exit("error"); 
  }
  foreach ( $D1 as $file ) {
    $d1 = $dir1 . "/" . $file;
    // Ignore files that start with period 
    if ( substr($file, 0, 1 ) == "." ) { 
      continue; 
    }
    // Recursive call for directories 
    if ( is_dir($d1) ) { 
      diff_dir($dir1 . "/" . $file, $dir2 . "/" . $file);
    }
    if ( in_array($file, $D2) ) { 
      // Do comparison 
      $x1 = $dir1 . "/" . $file;
      $x2 = $dir2 . "/" . $file;
      $s1 = file_get_contents($x1);
      $s2 = file_get_contents($x2);
      if ( $s1 != $s2 ) { 
        echo "Mismatch for file $file in $dir1 versus $dir2 \n";
      }
    }
    else { 
      echo "$file in $dir1 not found in $dir2 \n";
    }
  }
}

if ( $argc != 3 ) { exit("Need 2 arguments directory 1 and directory 2");}
$dir1 = $argv[1];
$dir2 = $argv[2];
if ( $dir1 == "" ) { exit("error"); }
if ( $dir1 == $dir2 ) { exit("error"); }
diff_dir($dir1, $dir2);
echo "\n================================\n";
diff_dir($dir2, $dir1);
?>
