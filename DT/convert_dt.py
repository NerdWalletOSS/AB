"""
This Python script takes in a pickled file ('.pkl'),
the output of any Python sklearn tree-based model, 
and converts it to csv.
"""

import joblib
import numpy as np
import pandas as pd
import os
import sys
from sklearn.model_selection import RandomizedSearchCV
from sklearn.model_selection import RepeatedStratifiedKFold
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn import preprocessing
import sklearn
import logging
from slpp import slpp as lua ## from https://github.com/SirAnthony/slpp, I was lazy to create a Lua-to-Python parser
logr = logging.getLogger(__name__)
logging.basicConfig(level=0)

COLNAMES = ['model_idx', 'tree_idx', 'node_idx', 'lchild_idx', 'rchild_idx', 'feature_idx', 'threshold', 'neg', 'pos']
DEFAULT_MODEL_NM = 'model'

def generate_csv_indiv_model(model, idx, past_node_ct, past_tree_ct):
    '''
    For one model only.
    '''
    assert hasattr(model, 'best_estimator_'), 'Model seems to not be a decision tree.'
    final_df_list = []
    if isinstance(model, sklearn.tree.tree.DecisionTreeClassifier):
        est = [model]
    elif isinstance(model, sklearn.model_selection._search.RandomizedSearchCV):
        est = model.best_estimator_.estimators_
    else:
        est = model.estimators_
    for i, m in enumerate(est):
        m_tree = pd.DataFrame(m.tree_.__getstate__()['nodes']).join(pd.DataFrame(m.tree_.value[:, 0, :]))
        m_tree = m_tree.drop(columns=['impurity', 'n_node_samples', 'weighted_n_node_samples']).reset_index()
        m_tree['tree_idx'] = i + past_tree_ct
        m_tree['index'] = past_node_ct + m_tree['index']
        m_tree['left_child'] = m_tree['left_child'].apply(lambda x: past_node_ct + x if x != -1 else -1)
        m_tree['right_child'] = m_tree['right_child'].apply(lambda x: past_node_ct + x if x != -1 else -1)
        final_df_list.append(m_tree)
        past_node_ct = past_node_ct + m_tree.shape[0]
    final_df = pd.concat(final_df_list)
    final_df['model_idx'] = idx
    final_df = final_df[final_df.columns.tolist()[-1:] + final_df.columns.tolist()[-2:-1] + final_df.columns.tolist()[:-2]].copy()
    final_df.columns = COLNAMES
    final_df['neg'] = final_df['neg'].apply(int)
    final_df['pos'] = final_df['pos'].apply(int)
    return {'df': final_df, 'nodes': past_node_ct, 'n_trees': len(est)}


if __name__ == '__main__':
    assert len(sys.argv) == 4, 'Arg1 is model file. Arg2 is output csv fname, arg3 is mapping Lua fname.'
    assert os.path.isfile(sys.argv[1]), 'File {} is not a file.'.format(sys.argv[1])
    try:
        model = joblib.load(open(sys.argv[1], 'r+'))
        model_idx_count = 0
        past_node_ct = 0
        past_tree_ct = 0
        mdl_map = {}
        if isinstance(model, dict):
            ## this case, it's a model of models.
            final_model_list = []
            for k, v in model.iteritems():
                model_op = generate_csv_indiv_model(v, model_idx_count, past_node_ct, past_tree_ct)
                past_node_ct = model_op['nodes']
                past_tree_ct = past_tree_ct + model_op['n_trees']
                model_idx_count = model_idx_count + 1
                mdl_map[model_idx_count] = k
                final_model_list.append(model_op['df'])
            final_df = pd.concat(final_model_list)
        else:
            final_df = generate_csv_indiv_model(model, model_idx_count, past_node_ct, past_tree_ct)['df']
            mdl_map[str(model_idx_count)] = DEFAULT_MODEL_NM
        final_df.to_csv(sys.argv[2], index=False)
        print lua.encode(mdl_map)
        lua_dict = 'return {}'.format(lua.encode(mdl_map).strip('\n'))
        print lua_dict
        with open(sys.argv[3], 'w+') as outf:
            outf.write(lua_dict)
    except Exception as e:
        logr.error('Error: {}'.format(str(e)))
