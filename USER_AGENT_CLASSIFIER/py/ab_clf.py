# Input:
#   DWH dump of user agent table: ./data/dwh_data.csv
#   Justin mapping:               ./data/justin.csv'
#
# Builds a SGD classifier
#   The classifier predicts the device browser for unknown user agents
#
# Output:
#   Model coefficients: ./data/model_coefficients.csv'
#   Model intercepts:   ./data/category_intercept.csv
#   Sample predictions: ./data/sample_predictions.csv
#   Sample user agents: ./data/sample_ua.csv
#
import collections
import matplotlib.pyplot as plt
import nltk
import numpy as np
import os
import pandas as pd
import pathlib
import plac

from sklearn import metrics
from sklearn.metrics import roc_curve, auc
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.linear_model import SGDClassifier
from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import train_test_split
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import LabelEncoder
from sklearn.preprocessing import label_binarize
from ua_parser import user_agent_parser


def parse(ua):
    try:
        h = user_agent_parser.Parse(ua)
        return (
            h['device']['family'], h['os']['family'], h['user_agent']['family'])
    except:
        return None, None, None


def read_csv(fn):
    """Streams the first five values in each row of the input file."""

    for line in open(fn, 'r'):
        values = line.strip().split(',')
        row = values[:4]
        row.append(','.join(values[4:]))
        yield row


def read_data(fn):
    """Creates a DataFrame from the first five cols of the input data file."""

    columns = ['device', 'device_type', 'os_type', 'browser_type', 'ua']
    data = list(read_csv(fn))
    df = pd.DataFrame(data=data, columns=columns, index=range(len(data)))
    df = df[df.ua.notnull()]
    return df


def tokenize_ua(df, parse_ua=False):
    """Tokenize user agent."""

    df['tokens'] = df.ua.apply(
        lambda ua: [
            x.lower() for x in nltk.wordpunct_tokenize(ua)
            if x.isalpha()])
    if parse_ua:
        df['device_parsed'], df['os_parsed'], df['ua_parsed'] = zip(
            *df.ua.apply(parse))
    df['sent'] = df.tokens.apply(lambda x: ' '.join(x))
    df['len'] = df.ua.apply(len)
    df['cnt'] = df.tokens.apply(len)
    del df['tokens']
    df = df[(df.cnt > 0) & (df.sent != 'null')]
    return df


def split_on_space(s):
    return s.split()


def create_grid_search_sgd_clf(df):
    sgd_clf = Pipeline([
        ('vect', CountVectorizer(
            analyzer=split_on_space, min_df=20000, binary=True)),
        # loss='hinge', penalty='l2', max_iter=6, tol=None, shuffle=True
        ('clf', SGDClassifier(random_state=42))])

    parameters = {
        'vect__min_df': (1, 2, 3, 4),
        'tfidf__use_idf': (True, False),
        'clf__alpha': [10 ** x for x in range(-6, 1)],
        'clf__li_ratio': [0, 0.05, 0.1, 0.2, 0.5, 0.8, 0.9, 0.95, 1],
        'clf__loss': ('log', 'hinge')}

    grid_search_sgd_clf = GridSearchCV(sgd_clf, parameters, n_jobs=-1)
    grid_search_sgd_clf.fit(df.sent.values, df.target.values)
    print(grid_search_sgd_clf.best_score_)
    for param_name in sorted(parameters.keys()):
        print("%s: %r" %
              (param_name, grid_search_sgd_clf.best_params_[param_name]))


def create_sgd_clf(df, test_size=0.2):
    def calc_accuracy():
        predicted = sgd_clf.predict(test.sent.values)
        accuracy = np.mean(predicted == test.target.values)
        print(accuracy)
        print(metrics.classification_report(
            test.target.values, predicted,
            target_names=[
                'Mobile_Android', 'Desktop', 'Tablet_iOS', 'Mobile_iOS',
                'Other', 'Tablet_Android']))

    train, test = train_test_split(df, test_size=test_size, random_state=42)

    sgd_clf = Pipeline([
        ('vect', CountVectorizer(
            analyzer=split_on_space, min_df=0.01, binary=True)),
        ('clf', SGDClassifier(
            loss='hinge', penalty='l2', alpha=1e-3, random_state=42,
            max_iter=5, tol=None, shuffle=True))])
    print('Training the SGD classifier may take several minutes to complete.')
    sgd_clf.fit(train.sent.values, train.target.values)

    calc_accuracy()
    return sgd_clf


def build_justin_mapping(fn):
    mdf = pd.read_csv(fn)
    keys = list(
        (mdf.device + '::' + mdf.device_type + '::' + mdf.os_type + '::' +
         mdf.browser_type).values)
    values = list(mdf.justin_class.values)
    return collections.OrderedDict((k, v) for k, v in zip(keys, values))


def model_to_csv(sgd_clf, category_encoder):
    vectorizer = sgd_clf.named_steps['vect']
    clf = sgd_clf.named_steps['clf']

    vocab = sorted(
        [(x, y) for x, y in vectorizer.vocabulary_.items()],
        key=lambda t: t[1])
    coefficients_df = pd.DataFrame(index=[x for x, y in vocab])
    for x in range(clf.coef_.shape[0]):
        coefficients_df[x] = clf.coef_[x]
    fn = './data/model_coefficients.csv'
    coefficients_df.to_csv(fn, header=False)
    print('Model coefficients:', fn)

    categories_df = pd.DataFrame(
        index=category_encoder.classes_,
        data=clf.intercept_, columns=['intercept'])
    fn = './data/category_intercept.csv'
    categories_df.to_csv(fn, header=False)
    print('Model intercepts:', fn)

    return coefficients_df, categories_df


def gen_sample(df, sample_size=10000):
    """Generate sample files with user agents and predictions."""

    sample_df = df.sample(sample_size)
    fn = './data/sample_ua.csv'
    sample_df.ua.to_csv(fn, index=False, header=False)
    print('Sample UAs:', fn)
    fn = './data/sample_predictions.csv'
    sample_df.predict_category.to_csv(fn, index=False, header=False)
    print('Sample predictions:', fn)


def c_predict(df, sgd_clf):
    vectorizer = sgd_clf.named_steps['vect']
    dtm = vectorizer.transform(df.sent.values)
    df['vector'] = dtm.todense().tolist()
    df['vector'] = df.vector.apply(lambda x: np.array(x))
    clf = sgd_clf.named_steps['clf']
    df['c'] = df.vector.apply(
        lambda x: (np.dot(x, clf.coef_.T) + clf.intercept_).argmax())
    print('Number of different predictions:', len(df[df.c != df.predict]))


def plot_roc_curve(df, category_encoder):
    # http://scikit-learn.org/stable/auto_examples/model_selection/plot_roc.html
    n_classes = len(category_encoder.classes_)
    y = label_binarize(df.target.ravel(), classes=range(n_classes))
    y_hat = label_binarize(df.predict.ravel(), classes=range(n_classes))

    fpr = {}
    tpr = {}
    roc_auc = {}
    for i in range(n_classes):
        fpr[i], tpr[i], _ = roc_curve(y[:, i], y_hat[:, i])
        roc_auc[i] = auc(fpr[i], tpr[i])

    plt.figure()
    plt.plot(
        fpr[2], tpr[2], color='darkorange', lw=2,
        label='ROC curve (area = %0.2f)' % roc_auc[2])
    plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.05])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title('Receiver operating characteristic example')
    plt.legend(loc="lower right")
    plt.show()


def gen_target_column(df, map_fn):
    def gen_key(row):
        cols = ['device', 'device_type', 'os_type', 'browser_type']
        return '::'.join([row[x] for x in cols])

    mapping = build_justin_mapping(map_fn)
    df['justin_key'] = df.apply(gen_key, axis=1)
    df['target_category'] = df.justin_key.apply(lambda x: mapping[x])
    le = LabelEncoder()
    le.fit(df.target_category.ravel())
    df['target'] = le.transform(df.target_category.ravel())
    return le


def run(fn, mfn):
    df = read_data(fn)
    df = tokenize_ua(df)
    category_encoder = gen_target_column(df, mfn)
    sgd_clf = create_sgd_clf(df)
    model_to_csv(sgd_clf, category_encoder)
    df['predict'] = sgd_clf.predict(df.sent.ravel())
    df['predict_category'] = category_encoder.inverse_transform(
        df.predict.ravel())
    gen_sample(df)
    return df, sgd_clf


@plac.annotations(
    fn=('Input file with UAs', 'option', 'i', pathlib.Path),
    mfn=('Mapping file name', 'option', 'm', pathlib.Path))
def main(fn='./data/dwh_data.csv', mfn='./data/justin.csv'):
    errors = []
    if not os.path.isfile(fn):
        errors.append('Input file with UAs does not exist.')
    if not os.path.isfile(mfn):
        errors.append('Mapping file does not exist.')
    if errors:
        for x in errors:
            print(x)
        print('Exiting...')
        return
    if not os.path.isdir('data'):
        os.makedirs('data')
    run(fn, mfn)


if __name__ == '__main__':
    plac.call(main)
