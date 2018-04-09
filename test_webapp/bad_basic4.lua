return 
{
<<<<<<< HEAD
  TestType =  "YZTest",
  --[[Non Existant Test Type]]--
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
    name =  "Variant_B",
    percentage =  "20",
    url =  "www.cnn.com"
  }, }
=======
  TestType =  "ABTest",
  Creator =  "joe",
  name =  "Test1",
  Variants =  { 
    { name =  "Control", percentage =  "20", },
    { name =  "V1", percentage =  "10", },
    { name =  "V2", percentage =  "10", },
    { name =  "V3", percentage =  "10", },
    { name =  "V4", percentage =  "10", },
    { name =  "V5", percentage =  "10", },
    { name =  "V6", percentage =  "10", },
    { name =  "V7", percentage =  "10", },
    { name =  "V8", percentage =  "10", },
    -- ERROR: More than 8 variants
  },
>>>>>>> dev
}
