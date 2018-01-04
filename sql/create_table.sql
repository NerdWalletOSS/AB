-- DROP DATABASE abdb; || true
DROP DATABASE abdb;
CREATE DATABASE abdb;
USE abdb;


DROP TABLE IF EXISTS cat_attr; -- load in LoadDBConfigs
CREATE TABLE cat_attr (
  pk int(8) not null auto_increment,
  name varchar(16) not null,
  description varchar(128),
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (pk),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;


DROP TABLE IF EXISTS cat_attr_val; -- load in LoadDBConfigs
CREATE TABLE cat_attr_val (
  pk int(8) not null auto_increment,
  name varchar(16) not null,
  cat_attr_id int(8) not null,
  description varchar(128),
  is_del int(2) not null default 0, -- for soft deletes
  PRIMARY KEY (pk),
  CONSTRAINT fk_cat_attr_id FOREIGN KEY (cat_attr_id) REFERENCES cat_attr(pk),
  CONSTRAINT uq_name_cat_attr_id UNIQUE (name, cat_attr_id)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

-- TODO: Better way to initialize these?

insert into cat_attr values (NULL, 'device', 'device', 0);
insert into cat_attr values (NULL, 'is_paid', 'is_paid', 0);
insert into cat_attr_val values (NULL, 'pc', 1, 'Device PC', 0);
insert into cat_attr_val values (NULL, 'tablet', 1, 'Device tablet', 0);
insert into cat_attr_val values (NULL, 'mobile', 1, 'Device mobile', 0);
insert into cat_attr_val values (NULL, 'true', 2, 'paid ', 0);
insert into cat_attr_val values (NULL, 'false', 2, 'not paid ', 0);

DROP TABLE IF EXISTS admin;
CREATE TABLE admin (
  pk int(8) not null auto_increment,
  name varchar(32) not null,
  PRIMARY KEY (pk),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into admin values (NULL, 'joe'); 
insert into admin values (NULL, 'blow'); 

DROP TABLE IF EXISTS channel; -- load in LoadDBConfigs
CREATE TABLE channel (
  pk int(8) not null auto_increment,
  name varchar(16) not null,
  PRIMARY KEY (pk),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into channel values (NULL, 'google'); 
insert into channel values (NULL, 'facebook'); 

DROP TABLE IF EXISTS state; -- load in LoadDBConfigs
CREATE TABLE state (
  pk int(8) not null auto_increment,
  name varchar(16) not null,
  PRIMARY KEY (pk),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into state values (NULL, 'dormant'); 
insert into state values (NULL, 'started'); 
insert into state values (NULL, 'terminated'); 
insert into state values (NULL, 'archived'); 

-- TODO Need a better way to represent test hierarchy
DROP TABLE IF EXISTS test_type; -- load in LoadDBConfigs
CREATE TABLE test_type (
  pk int(8) not null auto_increment,
  name varchar(16) not null,
  PRIMARY KEY (pk),
  CONSTRAINT uq_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into test_type values (NULL, 'ABTest'); 
insert into test_type values (NULL, 'XYTest'); 

-- TODO: 
-- Can we use journaling triggers to avoid having explicit journaling?
DROP TABLE IF EXISTS abtest;
CREATE TABLE abtest (
  pk int(8) not null auto_increment,
  name varchar(128) not null,
  test_type_id int(8) not null, -- 1 = ABTest, 2 = XYTest
  pred_id int(8), -- can be null
  channel_id int(8),
  description varchar(256) not null,
  is_private int(2) not null default 0, 
  is_sequential int(2) not null default 0, 
  is_dev_specific  boolean default false not null,
  seed bigint(22) not null default 0,
  regex varchar(4096),
  date_created datetime not null,
  t_create bigint not null default 0,
  external_id bigint not null default 0,
  most_recent_update datetime not null,-- redundant but thats OK
  t_update bigint not null default 0,
  ramp_num int(8) not null,
  num_variants int(8) not null,
  creator_id int(8) not null, 
  state_id int(8) not null, -- initial value is "dormant"
  PRIMARY KEY (pk),
  CONSTRAINT chk_ramp_num CHECK (ramp_num >= 1),
  CONSTRAINT chk_nv_lb CHECK (num_variants >= 2),
  CONSTRAINT chk_nv_ub CHECK (num_variants <= 4),
  CONSTRAINT fk_admin_id FOREIGN KEY (creator_id) REFERENCES admin(pk),
  CONSTRAINT fk_state_id FOREIGN KEY (state_id) REFERENCES state(pk),
  CONSTRAINT fk_channel_id FOREIGN KEY (channel_id) REFERENCES channel(pk),
  CONSTRAINT chk_is_private CHECK (is_private >= 0),
  CONSTRAINT chk_is_private CHECK (is_private <= 1),
  CONSTRAINT fk_test_type_id FOREIGN KEY (test_type_id) REFERENCES test_type(pk),
  CONSTRAINT fk_pred_id FOREIGN KEY (pred_id) REFERENCES abtest(pk)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS variant;
CREATE TABLE variant (
  pk int(8) not null auto_increment,
  name varchar(32) not null,
  abtest_id int(8) not null,
  percentage int(8) not null, -- percentages for test should add to 100
  count int(8) default 0 not null, -- set by call to RTS
  dwh_count int(8) default 0 not null, -- set by DWH
  is_final  boolean default false not null,
  description varchar(256),
  url varchar(256),
  custom_data  varchar(2048),
  t_update bigint not null default 0,
  PRIMARY KEY (pk),
  CONSTRAINT chk_perc_lb CHECK (percentage >= 0),
  CONSTRAINT chk_perc_ub CHECK (percentage <= 100),
  CONSTRAINT chk_count CHECK (count >= 0),
  CONSTRAINT uq_name_abtest_id UNIQUE (name, abtest_id),
  CONSTRAINT fk_abtest_id FOREIGN KEY (abtest_id) REFERENCES abtest(pk)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS cat_attr_val_test;
CREATE TABLE cat_attr_val_test (
  pk int(8) not null auto_increment,
  abtest_id int(8) not null,
  cat_attr_id int(8) not null, -- redundant
  cat_attr_val_id int(8) not null,
  CONSTRAINT fk_cavt_abtest_id FOREIGN KEY (abtest_id) REFERENCES abtest(pk),
  CONSTRAINT fk_cavt_cat_attr_val_id FOREIGN KEY (cat_attr_val_id) REFERENCES cat_attr_val(pk),
  CONSTRAINT uq_abtest_id_cat_attr_val_id UNIQUE (abtest_id, cat_attr_val_id),
  PRIMARY KEY (pk)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS state_history;
CREATE TABLE state_history (
  pk int(8) not null auto_increment,
  abtest_id int(8) not null,
  modifier_id int(8) not null,
  state_id int(8),
  ramp_num int(8) not null, -- what was ramp at time change occurred
  when_changed datetime not null,
  t_change bigint not null default 0,
  PRIMARY KEY (pk),
  CONSTRAINT rh_chk_ramp CHECK (ramp_num >= 1),
  CONSTRAINT sh_fk_abtest_id FOREIGN KEY (abtest_id) REFERENCES abtest(pk),
  CONSTRAINT sh_fk_admin_id  FOREIGN KEY (modifier_id) REFERENCES admin(pk),
  CONSTRAINT sh_fk_state_id  FOREIGN KEY (state_id) REFERENCES state(pk)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;


DROP TABLE IF EXISTS ramp_history;
CREATE TABLE ramp_history (
  pk int(8) not null auto_increment,
  ramp_num int(8) not null, 
  state_id int(8), -- what was state at time ramp occurred
  abtest_id int(8) not null, -- redundant but OK to have 
  modifier_id int(8) not null,
  variant_id int(8) not null,
  when_changed datetime not null,
  t_change bigint not null default 0,
  percentage int(8) not null, -- percentages for test should add to 100
  count int(8) default 0 not null, -- set on add_to_ramp_history()
  CONSTRAINT chk_rh_perc_lb CHECK (percentage >= 0),
  CONSTRAINT chk_rh_perc_ub CHECK (percentage <= 100),
  PRIMARY KEY (pk),
  CONSTRAINT rh_fk_state_id  FOREIGN KEY (state_id) REFERENCES state(pk),
  CONSTRAINT rh_fk_abtest_id FOREIGN KEY (abtest_id) REFERENCES abtest(pk),
  CONSTRAINT rh_fk_admin_id FOREIGN KEY (modifier_id) REFERENCES admin(pk),
  CONSTRAINT rh_fk_var_id   FOREIGN KEY (variant_id) REFERENCES variant(pk)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS config;
CREATE TABLE config (
  pk int(8) not null auto_increment,
  name varchar(32) not null,
  value varchar(128) not null,
  PRIMARY KEY (pk),
  CONSTRAINT uq_config UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into config values (NULL, 'check_url', 'true');

-- TODO: Do we need reserved words?
DROP TABLE IF EXISTS reserved_words;
CREATE TABLE reserved_words (
  pk int(8) not null auto_increment,
  name varchar(32) not null,
  PRIMARY KEY (pk),
  CONSTRAINT uq_words UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into reserved_words values (NULL, 'Action');
insert into reserved_words values (NULL, 'AddTest'); 
insert into reserved_words values (NULL, 'Attribute');
insert into reserved_words values (NULL, 'Control');
insert into reserved_words values (NULL, 'Diagnostics'); 
insert into reserved_words values (NULL, 'DeleteTest'); 
insert into reserved_words values (NULL, 'FinalVariant');
insert into reserved_words values (NULL, 'ExternalID');
insert into reserved_words values (NULL, 'getcounts'); 
insert into reserved_words values (NULL, 'getvariant'); 
insert into reserved_words values (NULL, 'healthcheck'); 
insert into reserved_words values (NULL, 'ignore'); 
insert into reserved_words values (NULL, 'halt'); 
insert into reserved_words values (NULL, 'Ineligible');
insert into reserved_words values (NULL, 'IsSequential');
insert into reserved_words values (NULL, 'ListTests'); 
insert into reserved_words values (NULL, 'LogFile'); 
insert into reserved_words values (NULL, 'NumTests'); 
insert into reserved_words values (NULL, 'NumUsers'); 
insert into reserved_words values (NULL, 'NumVariants'); 
insert into reserved_words values (NULL, 'RampNum');
insert into reserved_words values (NULL, 'restart');
insert into reserved_words values (NULL, 'Seed');
insert into reserved_words values (NULL, 'Service');
insert into reserved_words values (NULL, 'setpercentages'); 
insert into reserved_words values (NULL, 'setstate'); 
insert into reserved_words values (NULL, 'TestName');
insert into reserved_words values (NULL, 'TestID');
insert into reserved_words values (NULL, 'testinfo'); 
insert into reserved_words values (NULL, 'TestType');
insert into reserved_words values (NULL, 'Unset');
insert into reserved_words values (NULL, 'UUID');
insert into reserved_words values (NULL, 'UserAgent');
insert into reserved_words values (NULL, 'Value');
insert into reserved_words values (NULL, 'Variant');
insert into reserved_words values (NULL, 'variant0'); 
insert into reserved_words values (NULL, 'variant1'); 
insert into reserved_words values (NULL, 'variantid0'); 
insert into reserved_words values (NULL, 'variantid1'); 
insert into reserved_words values (NULL, 'variantid2'); 
insert into reserved_words values (NULL, 'variantid3');
insert into reserved_words values (NULL, 'VariantName');
insert into reserved_words values (NULL, 'VariantURL');

DROP TABLE IF EXISTS device; -- load in LoadDBConfigs
CREATE TABLE device (
  pk int(8) not null auto_increment,
  name varchar(16) not null,
  description varchar(127),
  PRIMARY KEY (pk),
  CONSTRAINT uq_dev_name UNIQUE (name)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

insert into device values (NULL, 'Desktop', '');
insert into device values (NULL, 'Mobile_iOS', '');
insert into device values (NULL, 'Mobile_Android', '');
insert into device values (NULL, 'Tablet_iOS', '');
insert into device values (NULL, 'Tablet_Android', '');
insert into device values (NULL, 'Other', '');

DROP TABLE IF EXISTS device_x_variant;
CREATE TABLE device_x_variant (
  pk int(8) not null auto_increment,
  device_id  int(8) not null,
  variant_id int(8) not null,
  ramp_num int(8) not null,
  percentage int(8) not null,
  CONSTRAINT dxv_fk_dev_id   FOREIGN KEY (device_id) REFERENCES device(pk),
  CONSTRAINT dxv_fk_var_id   FOREIGN KEY (variant_id) REFERENCES variant(pk),
  CONSTRAINT dxv_chk_perc_lb CHECK (percentage >= 0),
  CONSTRAINT dxv_chk_perc_ub CHECK (percentage <= 100),
  CONSTRAINT dxv_chk_ramp_num CHECK (ramp_num >= 1),
  PRIMARY KEY (pk)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;


DROP TABLE IF EXISTS log_ui_to_webapp;
CREATE TABLE log_ui_to_webapp (
  pk int(8) not null auto_increment,
  d_create datetime not null,
  t_create bigint not null default 0,
  msg varchar(4096),
  err_msg varchar(1024),
  api varchar(32),
  is_success int(2),
  PRIMARY KEY (pk)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;

DROP TABLE IF EXISTS log_webapp_to_rts;
CREATE TABLE log_u_to_rtsi_to_webapp (
  d_create datetime not null,
  t_create bigint not null default 0,
  msg varchar(4096)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=UTF8;
