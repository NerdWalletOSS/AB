return { 
  port = 8000, -- Port on which A/B server will run

  log_server = "localhost", -- Name of Server running Data Pipeline
  log_port = 8004, -- Port on which Data Pipeline is listening
  log_url = "Log", -- URL on which Data Pipeline needs data POSTed
  log_health_url = "Ignore", --Health check URL for Data Pipeline Server

  dim_server = "localhost", -- Name of Server running Session Service
  dim_port = 8024, -- Port on which Session Service is srunning
  dim_url = "session", -- URL for Session Service
  dim_health_url = "api/v1/health_check", -- Health check URL for Session Service

  num_post_retries = 1, -- Number of times it will attempt to POST to log server. 
  verbose = false, -- Set to true if you want logs on stderr

  mysql_server = "localhost", -- Name of MySQL server that back-ends WebApp
  mysql_user = "root", --  User for MySQL database
  mysql_password = "", -- Password for user for MySQL server
  mysql_database = "abdb", -- Name of database in MySQL server
  
  statsd_host = "localhost", -- Whatever needed for DataDog
  statsd_port = 8125, -- Determined by DataDog
  statsd_inc = "nw.metrics.ab.requests", -- Determined by DataDog
  statsd_timing = "nw.metrics.ab.request_time", -- Determined by DataDog
  
  sz_log_q = 65536, --[[Size of queue of messages to Log Server.  Provides buffering so that POSTing to Log Server can be done by separate thread and does not slow down
  --]]
  sz_uuid_ht = 4194304, -- Number of unique UUIDs that can be supported. 
  
  default_url,"http://localhost:8080/AB/UR/tcc/landing_page_0.php",
  -- For URL Router
  test_uuid_len = false, -- If true, then UUID len must match exactly
  reload_on_startup = false, -- If true then Reload occurs when server comes up
  xy_guid = 1, -- Used for testing.Set to 0 or omit this line in production
  uuid_len = 8, -- Maximum Length of UUID. If test_uuid_len=true, then this is exact length of UUID
  dev_file = "opt/ab/device.csv",
  ua_to_dev_map_file = "opt/ab/ua_to_dev.bin",
  num_log_entries = 65536, 
}
