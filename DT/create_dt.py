import joblib

import pandas as pd

from xgboost import XGBClassifier
from sklearn.ensemble import RandomForestClassifier

from tree import RandomForestEnsemble, XGBoostEnsemble


def train_models(df):
    rf = RandomForestClassifier(random_state=42, n_estimators=100)
    rf.fit(df.iloc[:, :-1], df[df.columns[-1]])
    joblib.dump(rf, 'spam/rf_mdl.pkl')
    xgb = XGBClassifier(n_estimators=100, random_state=42)
    xgb.fit(df.iloc[:, :-1], df[df.columns[-1]])
    joblib.dump(rf, 'spam/rf_mdl.pkl')
    return {'rf': rf, 'xgb': xgb}


def save_mdl_map(mdl_nm='spam'):
    """
    For now it's just one model, really depends on how the whole thing is set up.
    :return:
    """
    output_str = 'return {\n\t[1]="%s"\n}' % mdl_nm
    with open('spam/mdl_map.lua', 'w+') as f:
        f.write(output_str)


def generate_files(df, mdl_nm='spam'):
    models = train_models(df)

    # Generate the structures
    xgb_ensem = XGBoostEnsemble(models['xgb'])
    rf_ensem = RandomForestEnsemble(models['rf'], df.columns[:-1])

    # Save the following files:
    # dt_features.lua
    xgb_ensem.save_dt_features('spam/dt_features.lua')

    # mdl_map.lua
    save_mdl_map(mdl_nm)

    # xgb_dt.csv
    xgb_ensem.save('spam/xgb_dt.csv')

    # rf_dt.csv
    rf_ensem.save('spam/rf_dt.csv')

    # input.json
    df.iloc[:, :-1].to_json('spam/input.json', orient='records')

    # xgb_output.json
    xgb_preds = models['xgb'].predict_proba(df.iloc[:, :-1])[:, 1]
    pd.DataFrame(xgb_preds, columns=[mdl_nm]).to_json('spam/xgb_output.json', orient='records')

    # rf_output.json
    rf_preds = models['rf'].predict_proba(df.iloc[:, :-1])[:, 1]
    pd.DataFrame(rf_preds, columns=[mdl_nm]).to_json('spam/rf_output.json', orient='records')


if __name__ == '__main__':
    df = pd.read_csv('spam/spam_dataset.csv')
    generate_files(df, 'spam')
    exit(0)
