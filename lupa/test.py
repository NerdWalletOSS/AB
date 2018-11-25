import dt_interp
import sys


status = dt_interp.init('../DT/spam', "random_forest")
json_body = None
with open("../DT/spam/sample_input.json", "r") as f:
    json_body = f.read()
for i in range(1, 100):
    result = dt_interp.classify(json_body)
    print(i)
print("Classify Result:")
print(result)
dt_interp.release()
print("success")
# sys.exit(0)
# DT = dt_interp.init('../DT/spam', "random_forest")
# result = dt_interp.classify(DT, json_body)
# X:add(...) == add(X, ....)
# DT:release()
