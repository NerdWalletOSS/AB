  int
    hammer(
        int tid
        )
    {
      int status = 0;
      for ( int tid = 0; tid < 1; tid++ ) {
        for ( int iter = 0; iter < num_iters; iter++ ) {
          // T is used to measure individual times 
          for ( int uid = 0; uid < num_users; uid++ ) {
            for ( int test_id = 0; test_id < num_tests; test_id++ ) { 
              char url[AB_MAX_LEN_URL+1];
              char tracer[AB_MAX_LEN_TRACER+1];
              for ( int i = 0; i < AB_MAX_LEN_URL+1; i++ ) { url[i] = '\0'; }
              for ( int i = 0; i < AB_MAX_LEN_TRACER+1; i++ ) { tracer[i] = '\0';}
              status = make_guid(NULL, tracer, AB_MAX_LEN_TRACER); 
              cBYE(status);
              sprintf(url, "%s:%d/GetVariant?TestName=%s&TestType=%s&UUID=%d&Tracer=%s", 
                  server, port,
                  T[test_id].test_name, 
                  T[test_id].test_type, 
                  start_uuid+uid, tracer);
              t1 = get_time_usec();
              // fprintf(stderr, "%s\n", url);
              execute(g_ch[tid], url, &http_code);
              if ( num_bad > 10000 ) { goto DONE; }
              if ( http_code != 200 ) { continue; num_bad++; }
              t2 = get_time_usec();
              if ( t2 < t1 ) {  continue; }
              t = t2 - t1;
              if ( t > max_time ) { max_time = t; }
              if ( t < min_time ) { min_time = t; }
              avg_time += t;
              hit_ctr++;
              t = t % 1000; // convert to msec
              if ( (int)t >= nH ) { t = nH; num_over++; }
              H[t]++;
              if ( ( hit_ctr % 500 ) == 0 ) {
                sprintf(url, "%s:%d/Diagnostics?Source=C", server, port);
                status = execute(g_ch[tid], url, &http_code); cBYE(status);
                if ( http_code != 200 ) { go_BYE(-1); }
                ndots++; 
                fprintf(stderr, ".");
                if ( ndots >= 64 ) { 
                  fprintf(stderr, "%d/%d\n", hit_ctr, exp_num_hits);
                  ndots = 0;
                }
              }
            }
          }
        }
      }
BYE:
      return status;
    }
