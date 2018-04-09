return 
{
<<<<<<< HEAD
  TestType =  "XYTest",
  Channel =  "facebook",
  BinType =  "anonymous",
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
    name =  "Variant_A",
    --[[Duplicate Variant Name]]--
    percentage =  "20",
    url =  "www.cnn.com"
  }, }
=======
  TestType =  "ABTest",
  Creator =  "joe",
  name =  "Test1",
  Variants =  { 
    { name =  "V1", percentage =  "10", },
    { name =  "V2", percentage =  "10", },
    { name =  "V3", percentage =  "10", },
    { name =  "V4", percentage =  "10", },
    { name =  "V5", percentage =  "10", },
    { name =  "V6", percentage =  "10", },
    { name =  "V7", percentage =  "10", },
    { name =  "V8", percentage =  "10", },
    -- ERROR: No variant named control for ABTest
  },
>>>>>>> dev
}
