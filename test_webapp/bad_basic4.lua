return 
{

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
}
