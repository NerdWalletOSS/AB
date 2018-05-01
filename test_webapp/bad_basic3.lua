return 
{
  TestType =  "XYTest",
  Channel =  "facebook",
  BinType =  "anonymous",
  description =  "some bogus description",
  Creator =  "joe",
  name =  "Test1",
  Variants =  { {
    name =  "Control",
    percentage =  "-1",
    -- ERROR: Percentage out of bounds
    url =  "http://www.yahoo.com"
  },
  {
    name =  "Variant_A",
    percentage =  "101",
    -- ERROR: Percentage out of bounds
    url =  "http://www.gmail.com"
  },
  {
    name =  "Variant_B",
    percentage =  "0",
    url =  "http://www.cnn.com"
  }, }
}
--[[Total 100, but individually crosiing boundary condition]]--
