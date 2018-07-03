The data in the knn dataset comes from: https://archive.ics.uci.edu/ml/datasets/Occupancy+Detection+

It tries to predict whether a room is occupied or not.

It has these variables (standardized to have mean 0 and variance 1):

Temperature, in Celsius 
Relative Humidity, % 
Light, in Lux 
CO2, in ppm 
Humidity Ratio, Derived quantity from temperature and relative humidity, in kgwater-vapor/kg-air 

The output variable, yvar, is:

Occupancy, 0 or 1, 0 for not occupied, 1 for occupied status

The predictions in the dataset are generated using Python's sklearn module, in particular the
KNearestClassification function: http://scikit-learn.org/stable/modules/generated/sklearn.neighbors.KNeighborsClassifier.html

Firstly, the dataset was split into two, train and test. This was randomly sampled, with ratios of 4:1 (out of 8143 samples)

Secondly, 3 different variants of the K-Nearest Neighbours was trained: 

5-nearest neighbour,
15-nearest neighbour,
100-nearest neighbour

Lastly, the model was applied to all in the dataset (both trained and test).
