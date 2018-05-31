Execute "bash chk.sh <directory>" to run basic validations
Once iC server is running
Execute "bash test_make_feature_vector.sh <directory>
If you do 
  bash test_make_feature_vector.sh spam
You will get back something like 
{"0":0,"1":0.64,"2":0.64,"3":0,"4":0.32,"5":0,"6":0,"7":0,"8":0,"9":0,"10":0,"11":0.64,"12":0,"13":0,"14":0,"15":0.32,"16":0,"17":1.29,"18":1.93,"19":0,"20":0.96,"21":0,"22":0,"23":0,"24":0,"25":0,"26":0,"27":0,"28":0,"29":0,"30":0,"31":0,"32":0,"33":0,"34":0,"35":0,"36":0,"37":0,"38":0,"39":0,"40":0,"41":0,"42":0,"43":0,"44":0,"45":0,"46":0,"47":0,"48":0,"49":0,"50":0,"51":0.778,"52":0,"53":0,"54":3.756,"55":61,"56":278}

Execute "bash test_post_proc_preds.sh 
If spam has been loaded, you will get back something like
{"spam":0.97090911865234}
Execute "bash test_classify.sh <directory>
If you do
  bash test_classify.sh spam
You will get back something like 
  {"spam":0.97090911865234}

Each directory under here corresponds to one classification problem
The name of the directory is the name of the problem e.g.,
spam
or some such

In that directory, we have
1) dt.csv ---  the decision tree as a CSV file
2) dt_feature.lua --- a Lua table that has keys as feature names and 
	values are 1, 2, 3, ...
        Example:
        X = {}
        X[0] = 1; X[1] = 2, ....
        X = {}
        X["abc"] = 1; X["def"] = 2, ....
        X = {}
        X["abc"] = 1; X["def"] = { g = 2, h = 3 }
		1-indexed integer positions in g_dt_feature_vector
		a Lua table with key: value as:
			valid categorical value: 1-indexed integer position
3) mdl_map.lua --- a Lua table that has keys as 0-indexed integers
	and values as names of every model
4) generate_features.lua
	Returns a function, generate_features:
		input: Lua table, key-value pairs corresponding to the inputs
			- assume all keys and values are string
		output: Lua table, key-value pairs corresponding to the variable 
			names and their values. All variable names must appear as 
			keys in dt_feature.lua and all values must be either numeric
			or (categorical) appear as the nested Lua table's keys in 
			dt_feature.lua

5) A directory called test. Inside this, all tests that need to be 
executed will be in files called test_*.lua

Currently, we have a script that takes dt.csv and converts it into 3 binary
files called
dt.bin
rf.bin
mdl.bin
which are loaded by the C server

6) You must have a file called sample_input.json. This is used as an 
input to the /MakeFeatureVector API as a debugging check

-------

How to use:

1. If there is a pickled Decision Tree-based model 
(assume its name is MODEL.pkl), run the following to convert 
that model into a CSV (assume we want it
as dt.csv), and mdl_map.lua:

virtualenv venv
source venv/bin/activate
pip install -u upgrade pip
pip install -r requirements.txt
python convert_dt.py MODEL.pkl dt.csv mdl_map.lua
deactivate
rm -rf venv

This will generate dt.csv and mdl_map.lua. 

Requirements:

MODEL.pkl should either be a sklearn tree-based model, or a 
dictionary of:
	{model_name: sklearn tree-based model}

If it is the first, then mdl_map.lua looks like:
----- LUA CODE BELOW ------
return {[0]='model'}
----- LUA CODE ABOVE ------
 
If the second, then mdl_map.lua looks like:
----- LUA CODE BELOW ------
return {[0]='model_1', [1]='model_2', [2]='model_3'}
----- LUA CODE ABOVE ------
for all the keys (['model_1', 'model_2', 'model_3']) 
in MODEL.pkl.


2. To generate dt_feature.lua:

The role of dt_feature.lua is to make sure that the vector that 
goes into the model is the same vector (ordering and features) 
as the one built in Python. This is not trivial because:

- 1. Handling categorical variables can be tricky (missing 
values, omission of base value or otherwise)
- 2. Ordering issues can make a model null and void.

Conceptually, dt_feature.lua is a cookbook/config for the 
variables (after preprocessing) that brings them from a wide 
dataframe with variables of varying type to a wide dataframe with
variables of numeric types.

A sample format is here:
----- LUA CODE BELOW ------
return {height = 1, weight = 2, age = 3,
		country = {['RU'] = 4, ['US'] = 5}, [0] = 6}
----- LUA CODE ABOVE ------

In this sample, note the invariants:

1. All non-dictionary values should be distinct integers
beginning from 1. 
	- For non-categorical values, the value has to be an integer
	- For categorical values, the value has to be a dictionary:
		- Keys are the possible values (in this case country can only 
		be 'RU' or 'US')
		- Values are the 1-indexed positions these take when
		training the model
		- Usually the keys are ordered alphabetically (i.e. sorted)

So in this case, there are 5 variables in this order:
	- 'height' (numeric)
	- 'weight' (numeric)
	- 'age' (numeric)
	- 'country' (categorical) with values 'RU' and 'US'
	- 0 (numeric)

Something else to note re categorical:
	- The values provided in there must be the same set of values 
	when one-hot-encoded. This means that in practice there MAY be 
	values that are different (aka we could get 'CN'), and what 
	happens is that it will be treated as NOT 'RU' and NOT 'US'
		- i.e. if the vector was 
			{'height': 0.2, 'weight': 2303, 'age': 23,
			'country': 'CN', 0: 334}
		then the corresponding vector will be:
			[0.2, 2303, 23, 0, 0, 334]
		- on the other hand, if the vector was
			{'height': 0.3, 'weight': 32, 'age': 11,
			'country': 'US', 0: 45}
		then the corresponding vector will be:
			[0.3, 32, 11, 0, 1, 45]
		and if country in this case was 'RU' then:
			[0.3, 32, 11, 1, 0, 45]


3. To write code for generate_features.lua:

All of the necessary code and supplementary files must be in that
same folder. Especially if you require external data files, they should
be called from there.

There should only be one function returned:

----- LUA CODE BELOW ------
local function generate_features(in_features)
  --[[ 
  	in_features is a dictionary of
  		- raw_variable_name: raw_value

  	Goal: process it however the model requires to get out:
  	out_features, a dictionary of
  		- variable_name: value

  	Where variable_name MUST correspond to those in dt_feature.lua.

  	This effectively is the preprocessing (and not one-hot-encoding)
  	step in model prediction.
  ]]--
  local out_features = {}
  -- DO STUFF HERE TO OUT_FEATURES BASED ON IN_FEATURES
  return out_features
end

return generate_features
----- LUA CODE ABOVE ------


Tips/Rules:

1. All require statements in LUA shall start from the AB directory. 
As an example,
	"require 'DT/spam/some_code'"
2. All Lua data files (dictionaries stored in Lua) will be called 
via 'require'
3. Before running, go to AB and run 
	- source to_source
	to set the LUA_PATH so it knows where to import.
4. If you need to store any data files, use the cache:
	- "local cache = require 'lua/cache'"
	- When inserting: "cache.put(VAR_NAME, var)"
	- When dereferencing: "local var = cache.get(VAR_NAME)"
	Now it is accessible from any other piece of code.

5) Do not do any package.path or package.cpath changes.
6) Instead, any require must be relative to AB/ as an example
  local assertx = require 'lua/assertx'
  means that there is a file  AB/lua/assertx.lua


----- INITIALIZING DT / HOW IT WORKS -----

All files required to preprocess, one-hot-encode and predict / 
post-process predictions will be loaded in the

g_L_DT state, which is initialized in (AB) src/libe_ab_httpd.c, in the
line that calls init_lua(), which comes from init.c (1st step of 3).

In init.c's init_lua(), there is a stated requirement to have
DT/dt.lua in there. DT.lua will be 'require'd (basically a 
set of global functions that are required for running, and it should
not change.)

The functions in DT/dt.lua are as follows: 

- load_config (loads the config file)
- hard_code_config (for hard-coding/debugging)
- update_config (for loading the files required for running + checking)

and 3 functions that will be called by C:
- make_feature_vector (given a C char*, convert it into a float* with
	numeric values that will be fed to the decision tree)
- get_num_features (number of elements in the float*, 
	to figure out how much memory to memset the above float*) 
- post_proc_preds (given the prediction values as a float*, convert them 
	back into a JSON string (C char*) with keys as model names)

In dt.lua, nothing is checked, just these functions loaded into the
Lua space.

When it gets properly initialized is later (2nd step of 3): 

Either l_hard_code_config() (after calling init_lua)
OR
l_load_config()

will load the correct set of configs into the Lua state. This usually
means defining the directory of the model being run.

The corresponding file for this is:
DT/dt_load_config.lua

OR

lua/dt_hard_config.lua

After that, the third step (of 3) will involve adding the files in that
DT directory (which was loaded in step 2).

In C (i.e. src/libe_ab_httpd.c) it is called right after, in the
function l_update_config(), which calls DT/dt.lua's update_config().

In that (which is in DT/dt_update_config.lua), it:

- adds the 3 files in the repo into the cache:
	- dt_feature.lua (named in cache as dt_feature)
	- mdl_map.lua (named in cache as mdl_map)
	- generate_features.lua (named in cache as generate_features)
	NOTE THAT it assumes that the path from which this is called is
	from AB/.
	It searches through DT/XXXX/ (where XXXX is supplied by either
	l_hard_code_config or l_load_config, in the global LUA variable 
	config.DT.DT_DIR.VALUE or in the global C value c_cfg.dt_dir
	i.e. XXXX is C's value of c_cfg.dt_dir i.e. Lua's value of
	config.DT.DT_DIR.VALUE)
- checks if these files are present and valid
- dies if it is not

So by this point, as a summary:

3 C functions have been called:
- init_lua() which initialises the g_L_DT Lua state and adds all 6 
	functions in DT/dt.lua into that state
- EITHER l_hard_code_config() or l_load_config(), which adds the 
	value of the directory name XXXX (C's value of c_cfg.dt_dir 
	i.e. Lua's value of config.DT.DT_DIR.VALUE)
- l_update_config(), which uses that value of XXXX to search for 
	the 3 Lua files (dt_feature.lua, mdl_map.lua, 
	generate_features.lua) and adds them into the cache.

----- USING DT -----

In the whole Decision Tree pipeline there are a few steps:

1. Taking in a string containing the 'raw' inputs and processing
	them into a float*
	[src/ab_process_req.c calls the function l_make_feature_vector(), 
	present in src/l_make_feature_vector.c, which calls the Lua
	function make_feature_vector, whose source code is in 
	DT/lua/make_feature_vector.lua]
	- sub-step 1 of 2: Processing the raw inputs into variables
	- sub-step 2 of 2: Putting these variables into the float* in 
	the correct order

	Note about DT/lua/make_feature_vector.lua: This function does 
	not really change for different models, thus it is in the 
	DT/lua (common Lua code for models) directory. How it works 
	is as follows:
		1. It takes in a C char* (body), which is to be a JSON 
			string with raw inputs
		2. It parses the C char* out of its JSON format into a
			dictionary, in_features
		3. It finds the function 'generate_features' (which was
			to be provided in the model's directory) and runs it on
			in_features, to get out_features (sub-step 1 of 2)
		4. It runs one-hot-encoding (code in 
			DT/lua/one_hot_encoding.lua, which requires the cache
			to have dt_feature.lua) to figure out how to fill the
			float* (fvec)
		5. It then checks that the number of features in the vector
			matches the number that was prescribed in the 
			get_num_features function (code in 
			DT/lua/get_num_features.lua, which requires the cache 
			to have dt_feature.lua). 
		6. It fills the float* and returns it

2. Predicting using the Decision Tree
	This will be done on the C side. It looks at the dt.csv file
	(which has a few columns: 
		- mdl_idx: a non-decreasing 0-indexed integer + 0 column
			that describes the model number (most cases there
			is only 1 model, only exception is if you are doing
			prediction using the same set of variables for 
			different products and you combine it together))
			
			This one has to match up with mdl_map.lua; each unique
			integer in mdl_idx has to be present (as a key) in 
			mdl_map.lua and vice versa

		- tree_idx: a non-decreasing 0-indexed integer + 0 column
			that describes the tree number for each model. Each
			model could have a variable number of trees (e.g. a 
			random forest that was set to 50 trees, versus 1 
			decision tree, or an xgboost of 100 trees).

		- node_idx: a unique increasing 0-indexed integer, gives 
			unique node number
		- lchild_idx: left child node number. If -1, is a leaf
		- rchild_idx: right child node number. If -1, is a leaf
		- feature_idx: 0-indexed feature index (corresponding to
			the 1-indexed version in dt_feature.lua). If -2, is
			 a leaf.
		- threshold: value of that feature that determines 
			whether to go to left or right child. If less than
			or equal to, left child. Otherwise, right. If -2 
			usually means a leaf (bad idea to use this to
			determine leaf)
		- neg: number of elements in this node that are negative
		- pos: number of elements in this node that are positive

	The final calculation of each model is:
		- For each tree_idx in that model:
			- At the leaf node, take prob := pos / pos + neg
		- Return the mean of all prob for each tree_idx (not 
			weighted)

3. Returning these values as a JSON string
	[src/ab_process_req.c calls C's l_post_proc_preds (code in 
	src/l_post_proc_preds.c), which calls Lua's post_proc_preds 
	(code in DT/lua/post_proc_preds.lua)]
	
	It takes in 
	- the feature vector
		(float* g_predictions in C, opvec in Lua), 
	- the length of it 
		(g_n_mdl in C - called n_opvec in Lua; g_n_mdl is set
		in src/update_config.c, which calls code in 
		src/load_mdl.c, which looks at the max of dt.csv's 
		mdl_idx and uses that value as g_n_mdl)
	and does the following:

	- It takes all the name mapping in mdl_map.lua and fills 
		up a new dictionary:
	
		(element 0 in mdl_map is key, with value mdl_map[0], 
		element 2 in mdl_map is key, with value mdl_map[2] etc)

	- It then converts the dictionary into a JSON string, and 
		then writes it into (char* ) out_buf
			(in Lua. In C it's called g_rslt)

---- TESTING AND DEBUGGING ----

0. Before testing, ensure you have run `source to_source` at
the AB directory.

1. First, test if the AB state is loading. To do so, run 
`luajit -e "require 'DT.dt'"`. It should not make noise.

2. Next, add on loading configs:
`luajit -e "require 'DT.dt'; hard_code_config()"`
and
`luajit -e "require 'DT.dt';hard_code_config(); update_config()"`
should not complain.

If any of them complain:

- check if the spam folder has the 3 lua files mentioned above
	(dt_feature.lua, mdl_map.lua, generate_features.lua)
- check to see those files are properly formatted

3. To test get_num_features:
Run 
`luajit -e "require 'DT.dt';hard_code_config(); update_config(); print(get_num_features())"`

You should get a number at the very end.

4. To test make_feature_vector:
CD to /DT/lua/ and run 
`luajit test_make_feature_vector.lua`

5. To test post_proc_preds:
Run 
`luajit /DT/lua/test_post_proc_preds.lua`

6. Make sure that in the directory you set your Lua scripts
to (in load_config), you run all tests in there.
