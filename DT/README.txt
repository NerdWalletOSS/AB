Each directory under here corresponds to one classification problem
The name of the directory is the name of the problem e.g.,
credit_card_approval_prediction
or some such

In that directory, we have
1) dt.csv ---  the decision tree as a CSV file
2) make_feature_vector.lua --- 
  Input =  input point to be classified in  JSON format e.g., 
  \verb+ { "age" : 50, "is_male" : true } +
  Output: writes to \verb+float *g_feature_vector // [g_n_feature_vector]

3) get_num_featires.lua
  allows C server to figure out g_n_feature_vector at boot up time

4) post_proc_preds.lua
Input = float *g_predictions [g_n_predictions] 
where g_n_predictions is the number of models in the problem
Ouput = char *X // [nX] 
