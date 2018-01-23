-- DROP DATABASE abdb; || true
DROP DATABASE abdb;
CREATE DATABASE abdb;
USE abdb;

DROP TABLE IF EXISTS admin;
CREATE TABLE admin (
  id int(8) not null auto_increment,
  name varchar(32) not null,
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into admin values (NULL, 'joe'); 
insert into admin values (NULL, 'blow'); 

DROP TABLE IF EXISTS attr_type; -- load in LoadDBConfigs
CREATE TABLE attr_type (
  id int(8) not null auto_increment,
  name varchar(32) not null,
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into attr_type values (NULL, 'categorical'); 
insert into attr_type values (NULL, 'boolean'); 
insert into attr_type values (NULL, 'numeric'); 

DROP TABLE IF EXISTS attr; -- load in LoadDBConfigs
CREATE TABLE attr (
  id int(8) not null auto_increment,
  name varchar(16) not null,
  description varchar(128),
  attr_type_id int(8) not null, 
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name),
  CONSTRAINT fk_attr_type_id FOREIGN KEY (attr_type_id) REFERENCES attr_type(id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into attr values (NULL, 'device', 'device', 1, 0);
insert into attr values (NULL, 'is_paid', 'is_paid', 2, 0);
insert into attr values (NULL, 'fscore', 'F Score', 3, 0);

DROP TABLE IF EXISTS cat_attr_val; -- load in LoadDBConfigs
CREATE TABLE cat_attr_val (
  id int(8) not null auto_increment,
  name varchar(16) not null,
  attr_id int(8) not null,
  description varchar(128),
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (id),
  CONSTRAINT fk_attr_id FOREIGN KEY (attr_id) REFERENCES attr(id),
  CONSTRAINT uq_name_cat_attr_id UNIQUE (name, attr_id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into cat_attr_val values (NULL, 'pc', 1, 'Device PC', 0);
insert into cat_attr_val values (NULL, 'tablet', 1, 'Device tablet', 0);
insert into cat_attr_val values (NULL, 'mobile', 1, 'Device mobile', 0);

DROP TABLE IF EXISTS channel; -- load in LoadDBConfigs
CREATE TABLE channel (
  id int(8) not null auto_increment,
  name varchar(16) not null,
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into channel values (NULL, 'aol'); 
insert into channel values (NULL, 'bing'); 
insert into channel values (NULL, 'cnn'); 
insert into channel values (NULL, 'dianomi'); 
insert into channel values (NULL, 'facebook'); 
insert into channel values (NULL, 'google'); 
insert into channel values (NULL, 'instagram'); 
insert into channel values (NULL, 'outbrain'); 
insert into channel values (NULL, 'pandora'); 
insert into channel values (NULL, 'pinterest'); 
insert into channel values (NULL, 'pocket'); 
insert into channel values (NULL, 'taboola'); 
insert into channel values (NULL, 'twitter'); 
insert into channel values (NULL, 'weather_channel'); 
insert into channel values (NULL, 'yahoo'); 
insert into channel values (NULL, 'youtube'); 

DROP TABLE IF EXISTS state; -- load in LoadDBConfigs
CREATE TABLE state (
  id int(8) not null auto_increment,
  name varchar(16) not null,
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into state values (NULL, 'draft'); 
insert into state values (NULL, 'dormant'); 
insert into state values (NULL, 'started'); 
insert into state values (NULL, 'terminated'); 
insert into state values (NULL, 'archived'); 

DROP TABLE IF EXISTS bin_type; -- load in LoadDBConfigs
CREATE TABLE bin_type (
  id int(8) not null auto_increment,
  name varchar(64) not null,
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into bin_type values (NULL, 'free_for_all'); 
insert into bin_type values (NULL, 'memory_c_to_v_ok'); 
insert into bin_type values (NULL, 'memory_no_changes'); 
insert into bin_type values (NULL, 'c_to_v_ok_v_to_c_ok_v_to_v_not_ok'); 

DROP TABLE IF EXISTS test_type; -- load in LoadDBConfigs
CREATE TABLE test_type (
  id int(8) not null auto_increment,
  name varchar(16) not null,
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into test_type values (NULL, 'ABTest'); 
insert into test_type values (NULL, 'XYTest'); 

DROP TABLE IF EXISTS device; -- load in LoadDBConfigs
CREATE TABLE device (
  id int(8) not null auto_increment,
  name varchar(16) not null,
  description varchar(127),
  PRIMARY KEY (id),
  CONSTRAINT uq_dev_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into device values (NULL, 'Desktop', '');
insert into device values (NULL, 'Mobile_iOS', '');
insert into device values (NULL, 'Mobile_Android', '');
insert into device values (NULL, 'Tablet_iOS', '');
insert into device values (NULL, 'Tablet_Android', '');
insert into device values (NULL, 'Other', '');

-- TODO: 
-- Can we use journaling triggers to avoid having explicit journaling?
DROP TABLE IF EXISTS test;
CREATE TABLE test (
  id int(8) not null auto_increment,
  name varchar(128) not null,
  test_type_id int(8) not null, 
  bin_type_id int(8) not null, -- 
  pred_id int(8), -- can be null
  channel_id int(8),
  description varchar(256),
  is_dev_specific  boolean default false not null,
  seed bigint(22) not null default 0,
  external_id bigint not null default 0,
  d_create datetime not null,
  t_create bigint not null default 0,
  d_update datetime not null,-- redundant but thats OK
  t_update bigint not null default 0,
  creator_id int(8) not null, 
  updater_id int(8) not null, 
  state_id int(8) not null, -- initial value is "draft"
  PRIMARY KEY (id),
  CONSTRAINT fk_crtr_id FOREIGN KEY (creator_id) REFERENCES admin(id),
  CONSTRAINT fk_updtr_id FOREIGN KEY (updater_id) REFERENCES admin(id),
  CONSTRAINT fk_state_id FOREIGN KEY (state_id) REFERENCES state(id),
  CONSTRAINT fk_channel_id FOREIGN KEY (channel_id) REFERENCES channel(id),
  CONSTRAINT fk_test_type_id FOREIGN KEY (test_type_id) REFERENCES test_type(id),
  CONSTRAINT fk_bin_type_id FOREIGN KEY (bin_type_id) REFERENCES bin_type(id),
  CONSTRAINT fk_pred_id FOREIGN KEY (pred_id) REFERENCES test(id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS variant;
CREATE TABLE variant (
  id int(8) not null auto_increment,
  name varchar(32) not null,
  description varchar(256),
  test_id int(8) not null,
  percentage float not null, -- percentages for test should add to 100
  is_final  boolean default false not null,
  url varchar(256),
  custom_data  varchar(2048),
  d_update datetime not null,-- redundant but thats OK
  t_update bigint not null default 0,
  PRIMARY KEY (id),
  CONSTRAINT chk_perc_lb CHECK (percentage >= 0),
  CONSTRAINT chk_perc_ub CHECK (percentage <= 100),
  CONSTRAINT uq_name_test_id UNIQUE (name, test_id),
  CONSTRAINT fk_test_id FOREIGN KEY (test_id) REFERENCES test(id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS cat_attr_val_test;
CREATE TABLE cat_attr_val_test (
  id int(8) not null auto_increment,
  test_id int(8) not null,
  cat_attr_val_id int(8) not null,
  CONSTRAINT fk_cavt_test_id FOREIGN KEY (test_id) REFERENCES test(id),
  CONSTRAINT fk_cavt_cat_attr_val_id FOREIGN KEY (cat_attr_val_id) REFERENCES cat_attr_val(id),
  CONSTRAINT uq_test_id_cat_attr_val_id UNIQUE (test_id, cat_attr_val_id),
  PRIMARY KEY (id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS state_history;
CREATE TABLE state_history (
  id int(8) not null auto_increment,
  test_id int(8) not null,
  modifier_id int(8) not null,
  state_id int(8),
  ramp_num int(8) not null, -- what was ramp at time change occurred
  when_changed datetime not null,
  t_change bigint not null default 0,
  PRIMARY KEY (id),
  CONSTRAINT rh_chk_ramp CHECK (ramp_num >= 1),
  CONSTRAINT sh_fk_test_id FOREIGN KEY (test_id) REFERENCES test(id),
  CONSTRAINT sh_fk_admin_id  FOREIGN KEY (modifier_id) REFERENCES admin(id),
  CONSTRAINT sh_fk_state_id  FOREIGN KEY (state_id) REFERENCES state(id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;


DROP TABLE IF EXISTS ramp_history;
CREATE TABLE ramp_history (
  id int(8) not null auto_increment,
  ramp_num int(8) not null, 
  state_id int(8), -- what was state at time ramp occurred
  test_id int(8) not null, -- redundant but OK to have 
  modifier_id int(8) not null,
  variant_id int(8) not null,
  when_changed datetime not null,
  t_change bigint not null default 0,
  percentage int(8) not null, -- percentages for test should add to 100
  count int(8) default 0 not null, -- set on add_to_ramp_history()
  CONSTRAINT chk_rh_perc_lb CHECK (percentage >= 0),
  CONSTRAINT chk_rh_perc_ub CHECK (percentage <= 100),
  PRIMARY KEY (id),
  CONSTRAINT rh_fk_state_id  FOREIGN KEY (state_id) REFERENCES state(id),
  CONSTRAINT rh_fk_test_id FOREIGN KEY (test_id) REFERENCES test(id),
  CONSTRAINT rh_fk_admin_id FOREIGN KEY (modifier_id) REFERENCES admin(id),
  CONSTRAINT rh_fk_var_id   FOREIGN KEY (variant_id) REFERENCES variant(id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS config;
CREATE TABLE config (
  id int(8) not null auto_increment,
  name varchar(32) not null,
  value varchar(128) not null,
  PRIMARY KEY (id),
  CONSTRAINT uq_config UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into config values(NULL, 'default_landing_page', 'www.nerdwallet.com');
insert into config values(NULL, 'check_url_reachable', 'true');
insert into config values(NULL, 'num_retries',         '10');
insert into config values(NULL, 'max_len_admin_name',  '31');
insert into config values(NULL, 'max_len_channel_name','15');
insert into config values(NULL, 'max_len_cat_attr',    '15');
insert into config values(NULL, 'max_len_cat_attr_val','15');
insert into config values(NULL, 'max_len_regex',       '1023');
insert into config values(NULL, 'max_num_devices',     '8');
insert into config values(NULL, 'max_num_tests',       '1024');
insert into config values(NULL, 'min_num_variants',    '2');
insert into config values(NULL, 'max_num_variants',    '8');
insert into config values(NULL, 'max_len_custom_data', '2047');
insert into config values(NULL, 'max_len_test_name',   '127');
insert into config values(NULL, 'max_len_test_dscr',   '255');
insert into config values(NULL, 'max_len_variant_name','31');
insert into config values(NULL, 'max_len_variant_dscr','255');
insert into config values(NULL, 'max_len_url',         '255');

DROP TABLE IF EXISTS device_x_variant;
CREATE TABLE device_x_variant (
  id int(8) not null auto_increment,
  device_id  int(8) not null,
  variant_id int(8) not null,
  ramp_num int(8) not null,
  percentage int(8) not null,
  CONSTRAINT dxv_fk_dev_id   FOREIGN KEY (device_id) REFERENCES device(id),
  CONSTRAINT dxv_fk_var_id   FOREIGN KEY (variant_id) REFERENCES variant(id),
  CONSTRAINT dxv_chk_perc_lb CHECK (percentage >= 0),
  CONSTRAINT dxv_chk_perc_ub CHECK (percentage <= 100),
  CONSTRAINT dxv_chk_ramp_num CHECK (ramp_num >= 1),
  PRIMARY KEY (id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;


DROP TABLE IF EXISTS log_ui_to_webapp;
CREATE TABLE log_ui_to_webapp (
  id int(8) not null auto_increment,
  d_create datetime not null,
  t_create bigint not null default 0,
  msg varchar(4096),
  err_msg varchar(4096),
  api varchar(32),
  is_err int(2) not null default 0,
  PRIMARY KEY (id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS log_webapp_to_rts;
CREATE TABLE log_u_to_rtsi_to_webapp (
  d_create datetime not null,
  t_create bigint not null default 0,
  msg varchar(4096),
  err_msg varchar(4096),
  is_err int(2) not null default 0
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;
