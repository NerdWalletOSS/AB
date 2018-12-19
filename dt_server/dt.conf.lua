local conf = {}
conf.PORT = 8000
conf.DECISION_TREE_DIRECTORY = "../DT/spam"
conf.MODEL_NAME = "spam"
conf.FOREST_TYPE = "RANDOM_FOREST"
-- conf.DECISION_TREE_DIRECTORY = "../DT/eg_xgboost"
-- conf.MODEL_NAME = "eg_xgboost"
-- conf.FOREST_TYPE = "XGBOOST"
return conf
