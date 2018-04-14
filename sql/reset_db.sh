#!/bin/bash
mysql -u root --database abdb2 < create_table.sql
bash create_triggers.sh > _create_triggers.sql
mysql -u root --database abdb2 < _create_triggers.sql
