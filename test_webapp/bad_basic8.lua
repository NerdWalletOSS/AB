return 
{
  TestType =  "XYTest",
  Channel =  "facebook",
  BinType =  "c_to_v_ok_v_to_c_ok_v_to_v_not_ok",
  --[[Invalid BinType with respect to TestType]]--
  description =  "some bogus description",
  Creator =  "joe",
  name =  "Test1",
  Variants =  { {
    name =  "Control",
    percentage =  "50",
    url =  "www.gmail.com"
  },
  {
    name =  "Variant_A",
    percentage =  "30",
    url =  "www.yahoo.com"
  },
  {
    name =  "Variant_B",
    percentage =  "20",
    url =  "www.cnn.com"
  }, }
}
