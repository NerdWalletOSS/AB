#!/bin/bash
body='{"utm_camp": "cc_mktg_paid_120115_beta", "domain": "google.com", "country": "US", "utm_med": "cpc", "host": "www.google.com", "os_fmly_nm": "Windows", "browser_fmly_nm": "Mobile Safari", "device_type": "Android", "timestamp": 1457115204, "utm_src": "goog", "timezone": "America/North_Dakota/New_Salem", "postal_cd": 91706}'
curl -XPOST http://localhost:8000/MakeFeatureVector \
  --data "$body" \
  --header "Content-Type: application/json"
