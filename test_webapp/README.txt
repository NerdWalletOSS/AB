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

