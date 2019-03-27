import json

import numpy as np
import pandas as pd


class Tree(object):
    """
    Building block for all tree-based models.
    """

    def __init__(self, model, feature_names, mdl_idx=0, tree_idx=0, debug=False):
        """
        :param model: Model object, anything so long as it is converted into a pandas df in get_raw_model_df
        :param feature_names: list of strings, feature names corresponding to actual features
        :param mdl_idx: integer representing "which model is this?" 0-indexed
        :param tree_idx: integer representing "which tree is this"? 0-indexed
        """
        self.model = model
        self.mdl_idx = mdl_idx
        self.tree_idx = tree_idx
        self.feature_names = feature_names
        self.old_idx_to_new_map = None
        self.debug = debug
        self.tree_df = self.tree_to_df()

    def get_tree_df(self):
        """
        Simple utility getting back the tree dataframe.

        :return: `pd.DataFrame`
        """
        return self.tree_df

    def save(self, fname):
        """
        Saves to CSV.

        :param fname: file name, string
        :return: None
        """
        self.tree_df.to_csv(fname, index=None)

    def save_dt_features(self, fname):
        dt_features_str = 'local T = {}\n' + \
                          '\n'.join([
                              "T[{}]\t= '{}'".format(idx + 1, feat_nm)
                              for idx, feat_nm in enumerate(self.feature_names)]) + \
                          '\nreturn T\n'
        with open(fname, 'w+') as f:
            f.write(dt_features_str)

    def get_raw_model_df(self, model):
        """
        Takes the model and makes it into a dataframe with the following columns:
        ['node_idx', 'lchild_idx', 'rchild_idx', 'feature_idx', 'threshold']
        Optional: ['neg', 'pos', 'xgb_val']
        :param model: anything representing the model
        :return: `pd.DataFrame` with said 5 compulsory columns and optional columns
        """
        raise NotImplementedError('This is a base class, so this function is not implemented.')

    def tree_to_df(self):
        """
        Converts trees to a properly formatted dataframe.

        :return: `pd.DataFrame`
        """
        df = self.get_raw_model_df(self.model)
        assert all(col in df.columns for col in ['node_idx', 'lchild_idx', 'rchild_idx', 'feature_idx', 'threshold'])
        df['mdl_idx'] = int(self.mdl_idx)
        df['tree_idx'] = int(self.tree_idx)
        for col in ['pos', 'neg', 'xgb_val']:
            if col not in df:
                df[col] = None
        for col in ['threshold', 'xgb_val']:
            if df[col].notnull().any():
                df[col] = df[col].astype(np.float32)
        for col in ['pos', 'neg']:
            if df[col].notnull().any():
                df[col] = df[col].astype(np.int64)  # arbitrary?
        df = df[
            ['mdl_idx', 'tree_idx', 'node_idx', 'lchild_idx', 'rchild_idx', 'feature_idx', 'threshold', 'neg', 'pos',
             'xgb_val']]  # reordering
        feat_to_feat_idx = {feat: idx for idx, feat in enumerate(self.feature_names)}
        # Most sklearn indices are already done (ie np) for feature_idx. This is just in case it is not a sklearn index
        if df['feature_idx'].dtype != np.int64:
            df['feature_idx'] = df['feature_idx'].apply(
                lambda x: feat_to_feat_idx[x] if x in feat_to_feat_idx else -2)  # mapping here
        return self._fix_index(df)

    def _fix_index(self, df):
        """
        Sometimes the xgb node indices (especially) aren't consecutive, so this tries to remedy it

        :param df: old faulty pandas.DataFrame with ['lchild_idx', 'node_idx', 'rchild_idx'] columns
        :return: same df but with the indices changed to make them consecutive from 0
        """
        # Don't necessarily assume that the df's indices are not already in 0-X
        old_idx_to_new_map = {old: new for new, old in enumerate(df['node_idx'])}
        if self.debug:
            self.old_idx_to_new_map = old_idx_to_new_map
        for col in ['lchild_idx', 'node_idx', 'rchild_idx']:
            df[col] = df[col].apply(lambda x: old_idx_to_new_map[x] if x in old_idx_to_new_map else -1)
        return df.sort_values('node_idx').reset_index(drop=True)


class Ensemble(Tree):
    """
    Building block for all object-based models.

    Anything that extends Ensemble NEEDS to implement the following functions:

    1. an init
    2. a get_models staticmethod, that obtains the relevant models for each tree in a list format
    3. a get_raw_model_df method, that takes each tree in (2) and converts it into a pd.DataFrame format as per
    documentation in Tree's own get_raw_model_df method.
    """

    def __init__(self, model_obj, feature_names, mdl_idx=0, debug=False):
        """
        :param model_obj: The model object in question
        :param feature_names: list of strings, feature names corresponding to actual features
        :param mdl_idx: integer representing "which model is this?" 0-indexed
        """
        self.indiv_models = self.get_models(model_obj)
        self.mdl_idx = mdl_idx
        self.feature_names = feature_names
        self.ensemble_df = self.models_to_df()
        self.debug = debug

    @staticmethod
    def get_models(model):
        """
        Should return a list of models

        :param model: however the object came in
        :return: list of models
        """
        raise NotImplementedError()

    def save(self, fname):
        """
        Saves to CSV. Overrides base class

        :param fname: file name, string
        :return: None
        """
        self.ensemble_df.to_csv(fname, index=None)

    def models_to_df(self):
        """
        Converts the indiv_models generated by get_models into a dataframe. Basically calls Tree on each item
        in indiv_models, with a couple modifications to make sure nodes are labelled correctly.

        :return: `pd.DataFrame`
        """
        trees = []
        past_node_ct = 0
        old_idx_to_new_map = []
        for i, model in enumerate(self.indiv_models):
            super(Ensemble, self).__init__(model, self.feature_names, self.mdl_idx, i, debug=self.debug)
            df = self.get_tree_df()
            if self.debug:
                old_idx_to_new_map.append({old: new + past_node_ct for old, new in self.old_idx_to_new_map.iteritems()})
            df['node_idx'] = df['node_idx'].apply(int) + past_node_ct
            for col in ['lchild_idx', 'rchild_idx']:
                df[col] = df[col].apply(lambda x: int(x + past_node_ct) if x != -1 else -1)
            past_node_ct = past_node_ct + df.shape[0]
            trees.append(df)
        self.tree_df = None
        if self.debug:
            self.old_idx_to_new_map = old_idx_to_new_map
        return pd.concat(trees).reset_index(drop=True)


class XGBoostEnsemble(Ensemble):
    """
    Extends Ensemble to take care of Python's XGBoost API.
    """

    def __init__(self, model, mdl_idx=0, debug=False):
        """
        XGBoost stores feature names, so it is not necessary to feed the init the feature names.
        :param model: `xgboost.XGBClassifier`
        :param mdl_idx: integer representing "which model is this?" 0-indexed
        """
        self.booster = model.get_booster()
        self.debug = debug
        super(XGBoostEnsemble, self).__init__(self.booster, self.booster.feature_names, mdl_idx, debug=debug)

    @staticmethod
    def get_models(model_obj):
        """
        Method to obtain the models hiding in an `xgboost.XGBClassifier`.
        :param model_obj: `xgboost.XGBClassifier`
        :return: list of dictionaries, each one being a representation of a XGBoost tree
        """
        dumps = model_obj.get_dump(dump_format='json')
        return [json.loads(d) for d in dumps]

    def _tree_to_df_helper(self, tree):
        """
        Helper function that recursively goes through the tree.
        """
        if u'leaf' in tree:
            return [{'node_idx': tree['nodeid'], 'lchild_idx': -1, 'rchild_idx': -1, 'feature_idx': -2, 'threshold': -2,
                     'xgb_val': np.float32(tree['leaf'])}]
        # assert checks on integrity of tree
        assert tree['yes'] == tree['children'][0]['nodeid']
        assert tree['no'] == tree['children'][1]['nodeid']
        current_node = [{
            'node_idx': tree['nodeid'],
            'lchild_idx': tree['yes'],  # TODO in the future: account for booleans
            'rchild_idx': tree['no'],
            'feature_idx': tree['split'],
            'threshold': np.float32(tree['split_condition']),
            'xgb_val': None
        }]
        for child in tree['children']:
            current_node = current_node + self._tree_to_df_helper(child)
        return current_node

    def get_raw_model_df(self, model):
        """
        A XGBoost dump looks like this:
        - [u'no', u'missing', u'nodeid', u'children', u'depth', u'split', u'yes', u'split_condition']

        Converts it to a XGBoost dump. Uses the helper to do a recursive search on the tree.
        :param model: dictionary that corresponds to an XGBoost dump.
        :return: `pd.DataFrame` of model representation
        """
        df = pd.DataFrame(self._tree_to_df_helper(model))
        for col in ['pos', 'neg']:
            df[col] = None
        return df


class RandomForestEnsemble(Ensemble):
    """
    Extends Ensemble to take care of Python's scikit-learn API, specifically for RandomForests. Not yet tested on
    other ensembles in the `sklearn.ensemble` class, but believed to work.
    """

    def __init__(self, model, feature_names, mdl_idx=0, debug=False):
        """
        :param model_obj: `sklearn.ensemble.forest.RandomForestClassifier` or similar
        :param feature_names: list of strings, feature names corresponding to actual features
        :param mdl_idx: integer representing "which model is this?" 0-indexed
        """
        self.debug = debug
        super(RandomForestEnsemble, self).__init__(model.estimators_, feature_names, mdl_idx, debug=debug)

    @staticmethod
    def get_models(model_obj):
        """
        Luckily no processing is required to get out individual estimators.
        """
        return model_obj

    def get_raw_model_df(self, model):
        """
        The tree representation of each `DecisionTreeClassifier` object is accessible in this code. Remember,
        a `RandomForestClassifier` is made up of many `DecisionTreeClassifiers`
        :param model: `sklearn.tree.tree.DecisionTreeClassifier`
        :return: `pd.DataFrame` of model representation
        """
        df = pd.DataFrame(model.tree_.__getstate__()['nodes']).join(pd.DataFrame(model.tree_.value[:, 0, :]))
        df = df.reset_index().rename(
            columns={
                'index': 'node_idx',
                'left_child': 'lchild_idx',
                'right_child': 'rchild_idx',
                'feature': 'feature_idx',
                'threshold': 'threshold',
                0: 'neg',
                1: 'pos'
            })
        return df
