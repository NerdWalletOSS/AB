# Some useful environment variables
# When testing from command line, export AB_PHP_CLI=true
# When you don't want to use RTS, export AB_WEBAPP_STANDALONE=true
# 
# set up alias as follows
abdb='mysql -u root --database abdb'
# to clean out database
abdb < ../sql/create_table.sql
# to set up environment variables
source to_source 
# to execute basic test
php cli_test_basic.php good_test_basic_1.json
# to add device specific variants
php cli_set_device_specific_variant.php good_dev_spec_1.json
#---------
