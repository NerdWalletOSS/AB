#!/bin/bash
spambody='{"0":0.0,"1":0.64,"2":0.64,"3":0.0,"4":0.32,"5":0.0,"6":0.0,"7":0.0,"8":0.0,"9":0.0,"10":0.0,"11":0.64,"12":0.0,"13":0.0,"14":0.0,"15":0.32,"16":0.0,"17":1.29,"18":1.93,"19":0.0,"20":0.96,"21":0.0,"22":0.0,"23":0.0,"24":0.0,"25":0.0,"26":0.0,"27":0.0,"28":0.0,"29":0.0,"30":0.0,"31":0.0,"32":0.0,"33":0.0,"34":0.0,"35":0.0,"36":0.0,"37":0.0,"38":0.0,"39":0.0,"40":0.0,"41":0.0,"42":0.0,"43":0.0,"44":0.0,"45":0.0,"46":0.0,"47":0.0,"48":0.0,"49":0.0,"50":0.0,"51":0.778,"52":0.0,"53":0.0,"54":3.756,"55":61,"56":278}'

curl -XPOST http://localhost:8000/MakeFeatureVector \
  --data "$spambody" \
  --header "Content-Type: application/json"
echo DONE; exit;

#===================
curl --url "localhost:8000/ClassifyUA?UserAgent=Mozilla%2F5.0%20%28Linux%3B%20Android%207.0%3B%20Grand%20XL%20Build%2FNRD90M%29%20AppleWebKit%2F537.36%20%28KHTML%2C%20like%20Gecko%29%20Chrome%2F60.0.3112.116%20Mobile%20Safari%2F537.36"
# should return 
# {        "JustinCat": "Mobile_Android", "JustinCatID" : "3",       "OS": "Android", "OSID" : "1",       "Browser": "Chrome", "BrowserID" : "2", "DeviceType": "Android", "DeviceTypeID" : "1" }

echo ""
echo "============================"
echo ""
curl --url "localhost:8000/UTMKV?URL=https://www.nerdwallet.com/l/banking/best-cd-rates?nw_campaign_id=150845972646889300&utm_source=goog&utm_medium=cpc&utm_campaign=bk_mktg_paid_121417_beta_CDs&utm_term=%2Bcd&utm_content=ta&mktg_hline=7517&mktg_body=1699&mktg_place=5&gclid=Cj0KCQiAzMDTBRDDARIsABX4AWzrC8Y_TgM6-tfQomcrrVrI21SFPj946phFQQtsJX_8l3xiIwBPpZEaAgCPEALw_wcB&gclsrc=aw.ds&VariantID=3334&GUID=110043&DeviceID=0"
