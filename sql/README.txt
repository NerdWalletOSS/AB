abdb < create_table.sql #-- creates the database
bash create_trigger.sh > _create_triggers.sql # -- creates the triggers
abdb < _create_triggers.sql # loads the triggers
