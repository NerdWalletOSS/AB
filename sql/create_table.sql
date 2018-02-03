-- DROP DATABASE abdb; || true
DROP DATABASE abdb;
CREATE DATABASE abdb;
USE abdb;

DROP TABLE IF EXISTS api; -- config
CREATE TABLE api (
  id int(8) not null auto_increment,
  name varchar(32) not null,
  description varchar(128) not null,
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into api values(NULL, 'insert_test_edit_test_basic', 
"Create a test or edit basic information about test", 0);

DROP TABLE IF EXISTS admin; -- config 
CREATE TABLE admin (
  id int(8) not null auto_increment,
  name varchar(32) not null,
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into admin values (NULL, 'joe', 0); 
insert into admin values (NULL, 'blow', 0); 

DROP TABLE IF EXISTS attr_type; -- config 
CREATE TABLE attr_type (
  id int(8) not null auto_increment,
  name varchar(32) not null,
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into attr_type values (NULL, 'categorical', 0); 
insert into attr_type values (NULL, 'boolean', 0); 
insert into attr_type values (NULL, 'numeric', 0); 

DROP TABLE IF EXISTS attr; -- config
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

DROP TABLE IF EXISTS cat_attr_val; -- config
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

DROP TABLE IF EXISTS channel; -- config
CREATE TABLE channel (
  id int(8) not null auto_increment,
  name varchar(16) not null,
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into channel values (NULL, 'aol', 0); 
insert into channel values (NULL, 'bing', 0); 
insert into channel values (NULL, 'cnn', 0); 
insert into channel values (NULL, 'dianomi', 0); 
insert into channel values (NULL, 'facebook', 0); 
insert into channel values (NULL, 'google', 0); 
insert into channel values (NULL, 'instagram', 0); 
insert into channel values (NULL, 'outbrain', 0); 
insert into channel values (NULL, 'pandora', 0); 
insert into channel values (NULL, 'pinterest', 0); 
insert into channel values (NULL, 'pocket', 0); 
insert into channel values (NULL, 'taboola', 0); 
insert into channel values (NULL, 'twitter', 0); 
insert into channel values (NULL, 'weather_channel', 0); 
insert into channel values (NULL, 'yahoo', 0); 
insert into channel values (NULL, 'youtube', 0); 

DROP TABLE IF EXISTS state; -- config
CREATE TABLE state (
  id int(8) not null auto_increment,
  name varchar(16) not null,
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into state values (NULL, 'draft', 0); 
insert into state values (NULL, 'dormant', 0); 
insert into state values (NULL, 'started', 0); 
insert into state values (NULL, 'terminated', 0); 
insert into state values (NULL, 'archived', 0); 

DROP TABLE IF EXISTS bin_type; -- config
CREATE TABLE bin_type (
  id int(8) not null auto_increment,
  name varchar(64) not null,
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into bin_type values (NULL, 'free_for_all', 0); 
insert into bin_type values (NULL, 'memory_c_to_v_ok', 0); 
insert into bin_type values (NULL, 'memory_no_changes', 0); 
insert into bin_type values (NULL, 'c_to_v_ok_v_to_c_ok_v_to_v_not_ok', 0); 

DROP TABLE IF EXISTS test_type; -- config
CREATE TABLE test_type (
  id int(8) not null auto_increment,
  name varchar(16) not null,
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (id),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into test_type values (NULL, 'ABTest', 0); 
insert into test_type values (NULL, 'XYTest', 0); 

DROP TABLE IF EXISTS device; -- config
CREATE TABLE device (
  id int(8) not null auto_increment,
  name varchar(16) not null,
  description varchar(127),
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (id),
  CONSTRAINT uq_dev_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into device values (NULL, 'Desktop', '', 0);
insert into device values (NULL, 'Mobile_iOS', '', 0);
insert into device values (NULL, 'Mobile_Android', '', 0);
insert into device values (NULL, 'Tablet_iOS', '', 0);
insert into device values (NULL, 'Tablet_Android', '', 0);
insert into device values (NULL, 'Other', '', 0);

DROP TABLE IF EXISTS config; -- config
CREATE TABLE config (
  id int(8) not null auto_increment,
  name varchar(32) not null,
  value varchar(128) not null,
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (id),
  CONSTRAINT uq_config UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into config values(NULL, 'default_landing_page', 'www.nerdwallet.com', 0);
insert into config values(NULL, 'check_url_reachable', 'true', 0);
insert into config values(NULL, 'num_retries',         '10', 0);
insert into config values(NULL, 'max_len_admin_name',  '31', 0);
insert into config values(NULL, 'max_len_channel_name','15', 0);
insert into config values(NULL, 'max_len_cat_attr',    '15', 0);
insert into config values(NULL, 'max_len_cat_attr_val','15', 0);
insert into config values(NULL, 'max_len_regex',       '1023', 0);
insert into config values(NULL, 'max_num_devices',     '8', 0);
insert into config values(NULL, 'max_num_tests',       '1024', 0);
insert into config values(NULL, 'min_num_variants',    '2', 0);
insert into config values(NULL, 'max_num_variants',    '8', 0);
insert into config values(NULL, 'max_len_custom_data', '2047', 0);
insert into config values(NULL, 'max_len_test_name',   '127', 0);
insert into config values(NULL, 'max_len_test_dscr',   '255', 0);
insert into config values(NULL, 'max_len_variant_name','31', 0);
insert into config values(NULL, 'max_len_variant_dscr','255', 0);
insert into config values(NULL, 'max_len_url',         '255', 0);

DROP TABLE IF EXISTS request_webapp; -- debugging
CREATE TABLE request_webapp (
  id int(8) not null auto_increment,
  created_at datetime not null,
  t_create bigint not null default 0,
  payload varchar(4096),
  api_id int(8) not null, 
  msg_stdout varchar(4096),
  msg_stderr varchar(4096),
  status_code int(8) not null default 200,
  CONSTRAINT fk_api_id FOREIGN KEY (api_id) REFERENCES api(id),
  PRIMARY KEY (id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS request_rts; -- debugging
CREATE TABLE request_rts (
  id int(8) not null auto_increment,
  server_address varchar(32) not null, 
  server_port int(8) not null,
  created_at datetime not null,
  t_create bigint not null default 0,
  payload varchar(4096),
  msg_stdout varchar(4096),
  msg_stderr varchar(4096),
  status_code int(8) not null default 200,
  PRIMARY KEY (id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS test; -- test info 
CREATE TABLE test (
  id int(8) not null auto_increment,
  name varchar(128) not null,
  api_id int(8) not null, 
  request_webapp_id int(8) not null, 
  test_type_id int(8) not null, 
  bin_type_id int(8) not null, -- 
  pred_id int(8), -- can be null
  channel_id int(8),
  description varchar(256),

  is_dev_specific  boolean default false not null,
  has_filters  boolean default false not null,

  seed bigint(22) not null default 0,
  external_id bigint not null default 0,

  created_at datetime not null,
  t_create bigint not null default 0,

  updated_at datetime not null,
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
  CONSTRAINT fk_t_api_id FOREIGN KEY (api_id) REFERENCES api(id),
  CONSTRAINT fk_bin_type_id FOREIGN KEY (bin_type_id) REFERENCES bin_type(id),
  CONSTRAINT fk_rq_web_id FOREIGN KEY (request_webapp_id) REFERENCES request_webapp(id),
  CONSTRAINT fk_pred_id FOREIGN KEY (pred_id) REFERENCES test(id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS variant; -- test info
CREATE TABLE variant (
  id int(8) not null auto_increment,
  name varchar(32) not null,
  api_id int(8) not null, 
  request_webapp_id int(8) not null, 
  description varchar(256),
  test_id int(8) not null,
  percentage float not null, -- percentages for test should add to 100
  is_final  boolean default false not null,
  url varchar(256),
  custom_data  varchar(2048),

  created_at datetime not null,
  t_create bigint not null default 0,

  updated_at datetime not null,
  t_update bigint not null default 0,

  PRIMARY KEY (id),
  CONSTRAINT chk_perc_lb CHECK (percentage >= 0),
  CONSTRAINT chk_perc_ub CHECK (percentage <= 100),
  CONSTRAINT uq_name_test_id UNIQUE (name, test_id),
  CONSTRAINT fk_v_api_id FOREIGN KEY (api_id) REFERENCES api(id),
  CONSTRAINT fk_v_rq_web_id FOREIGN KEY (request_webapp_id) REFERENCES request_webapp(id),
  CONSTRAINT fk_test_id FOREIGN KEY (test_id) REFERENCES test(id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS cat_attr_val_test; -- test info
CREATE TABLE cat_attr_val_test (
  id int(8) not null auto_increment,
  api_id int(8) not null, 
  request_webapp_id int(8) not null, 
  test_id int(8) not null,
  cat_attr_val_id int(8) not null,
  CONSTRAINT fk_cavt_test_id FOREIGN KEY (test_id) REFERENCES test(id),
  CONSTRAINT fk_cavt_cat_attr_val_id FOREIGN KEY (cat_attr_val_id) REFERENCES cat_attr_val(id),
  CONSTRAINT fk_c_api_id FOREIGN KEY (api_id) REFERENCES api(id),
  CONSTRAINT uq_test_id_cat_attr_val_id UNIQUE (test_id, cat_attr_val_id),
  CONSTRAINT fk_c_rq_web_id FOREIGN KEY (request_webapp_id) REFERENCES request_webapp(id),
  PRIMARY KEY (id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS bool_attr_test; -- test info
CREATE TABLE bool_attr_test (
  id int(8) not null auto_increment,
  api_id int(8) not null, 
  request_webapp_id int(8) not null, 
  test_id int(8) not null,
  val int(2) not null, -- 0 is false, 1 is true 
  CONSTRAINT fk_b_api_id FOREIGN KEY (api_id) REFERENCES api(id),
  CONSTRAINT fk_b_rq_web_id FOREIGN KEY (request_webapp_id) REFERENCES request_webapp(id),
  CONSTRAINT b_uq_test_id UNIQUE (test_id),
  PRIMARY KEY (id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS device_x_variant; -- test info
CREATE TABLE device_x_variant (
  id int(8) not null auto_increment,
  api_id int(8) not null, 
  request_webapp_id int(8) not null, 
  device_id  int(8) not null,
  variant_id int(8) not null,
  test_id int(8) not null, -- redundant but useful 
  ramp_num int(8) not null,
  percentage int(8) not null,
  CONSTRAINT dxv_fk_dev_id   FOREIGN KEY (device_id) REFERENCES device(id),
  CONSTRAINT dxv_fk_var_id   FOREIGN KEY (variant_id) REFERENCES variant(id),
  CONSTRAINT dxv_chk_perc_lb CHECK (percentage >= 0),
  CONSTRAINT dxv_chk_perc_ub CHECK (percentage <= 100),
  CONSTRAINT dxv_chk_ramp_num CHECK (ramp_num >= 1),
  CONSTRAINT fk_d_api_id FOREIGN KEY (api_id) REFERENCES api(id),
  CONSTRAINT fk_d_variant_id FOREIGN KEY (variant_id) REFERENCES variant(id),
  CONSTRAINT fk_d_test_id FOREIGN KEY (test_id) REFERENCES test(id),
  CONSTRAINT fk_d_rq_web_id FOREIGN KEY (request_webapp_id) REFERENCES request_webapp(id),
  PRIMARY KEY (id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;


