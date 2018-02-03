#!/bin/bash
mysql -u root --database abdb < create_table.sql
bash create_triggers.sh > _create_triggers.sql
mysql -u root --database abdb < _create_triggers.sql
