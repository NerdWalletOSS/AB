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
    url =  "www.yahoo.com"
  },
  {
    name =  "Variant_A",
    percentage =  "101",
<<<<<<< HEAD
    url =  "www.gmail.com"
=======
    -- ERROR: Percentage out of bounds
    url =  "www.yahoo.com"
>>>>>>> dev
  },
  {
    name =  "Variant_B",
    percentage =  "0",
    url =  "www.cnn.com"
  }, }
}
--[[Total 100, but individually crosiing boundary condition]]--
