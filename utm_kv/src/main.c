#include "url.h"
#include "utm_incs.h"
#include "utm_types.h"
#include "utm_kv.h"
int
main(
    void
    )
{
  int status = 0;
  int sz = 1024;
  char buf[sz];
  UTM_REC_TYPE utm_rec;
  const char *instr = "https://www.nerdwallet.com/l/banking/best-cd-rates?nw_campaign_id=150845972646889300&utm_source=goog&utm_medium=cpc&utm_campaign=bk_mktg_paid_121417_beta_CDs&utm_term=%2Bcd&utm_content=ta&mktg_hline=7517&mktg_body=1699&mktg_place=5&gclid=Cj0KCQiAzMDTBRDDARIsABX4AWzrC8Y_TgM6-tfQomcrrVrI21SFPj946phFQQtsJX_8l3xiIwBPpZEaAgCPEALw_wcB&gclsrc=aw.ds&VariantID=3334&GUID=110043&DeviceID=0";
  status = utm_kv(instr, &utm_rec);
  cBYE(status);
  print_utm_kv(utm_rec, buf, sz);
  fprintf(stderr, "%s\n", buf);
  free_utm_kv(utm_rec);
BYE:
  return status;
}
