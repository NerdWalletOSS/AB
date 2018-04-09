return 
{

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
}
