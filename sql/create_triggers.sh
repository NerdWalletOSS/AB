# get table names
get_line(){
  cat create_table.sql | sed 's/--.*//g' | awk -v entry=$1 '
  BEGIN{
    FS=" ";
    RS=";";
  } 
  {
    if ($1 == "CREATE" && $3 == entry){
      print $0 ";" ;
    }
  }' | grep -v "CONSTRAINT" | grep -v "PRIMARY KEY" | sed '/^\s*$/d'

}

cat create_table.sql | grep "\-\- test info" | sed 's/; --.*//g' | awk '{print $NF}' | while read line
do
  LEN=`get_line $line|wc -l`
  # echo $LEN
  # get_line $line
  get_line $line | awk -v f_length=$LEN '
  BEGIN{
    fields="";
    new_fields="";
    name="";
  }
  {
    if (NR == 1 || NR == f_length){
      if (NR == 1) {
        name=$3;
        $3 = "log_"$3;
        print $0;
        print "log_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
        print "log_ime_stamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
        print "modification_type CHAR(8),"
      } else{
        print $0;
      }
    } else {
    if (NR > 2) { 
      fields = fields", "$1;
      new_fields = new_fields", NEW."$1;
    } else {
      fields = $1;
      new_fields = "NEW."$1;

    }
      my_str = "";
      count = 1;
      for(i=1; i<=NF && count<=2; i++) {
        if ($i != "") {
        my_str = my_str" "$i;
        count = count + 1;
      }
      }
      if (NR != f_length -1) {
        my_str = my_str",";
      }
      print my_str;
    }
  }
  END{
    print "delimiter |"
    print "CREATE TRIGGER " name"_update_trigger"
    print "\tBEFORE UPDATE ON "name
    print "\tFOR EACH ROW"
    print "\tBEGIN"
    print "\t\tINSERT INTO log_" name" (modification_type, " fields") VALUES(\"UPDATE\", " new_fields ");"
    print "\tEND;"
    print "|"
    print "delimiter ;"
    print ""
    print "delimiter |"
    print "CREATE TRIGGER " name"_insert_trigger"
    print "\tBEFORE INSERT ON "name
    print "\tFOR EACH ROW"
    print "\tBEGIN"
    print "\t\tINSERT INTO log_" name" (modification_type, " fields") VALUES(\"INSERT\", " new_fields ");"
    print "\tEND;"
    print "|"
    print "delimiter ;"

  }' | sed 's/,,/,/g'
done
