#!/bin/bash
body='{"utm_camp": "cc_mktg_paid_120115_beta", "domain": "google.com", "country": "US", "utm_med": "cpc", "host": "www.google.com", "os_fmly_nm": "Windows", "browser_fmly_nm": "Mobile Safari", "device_type": "Android", "timestamp": 1457115204, "utm_src": "goog", "timezone": "America/North_Dakota/New_Salem", "postal_cd": 91706}'
curl -XPOST http://localhost:8000/MakeFeatureVector \
  --data "$body" \
  --header "Content-Type: application/json"

#===================
curl --url "localhost:8000/ClassifyUA?UserAgent=Mozilla%2F5.0%20%28Linux%3B%20Android%207.0%3B%20Grand%20XL%20Build%2FNRD90M%29%20AppleWebKit%2F537.36%20%28KHTML%2C%20like%20Gecko%29%20Chrome%2F60.0.3112.116%20Mobile%20Safari%2F537.36"
# should return 
# {        "JustinCat": "Mobile_Android", "JustinCatID" : "3",       "OS": "Android", "OSID" : "1",       "Browser": "Chrome", "BrowserID" : "2", "DeviceType": "Android", "DeviceTypeID" : "1" }
