import dt_interp
import sys


status = dt_interp.init('../DT/spam', "random_forest")
json_body = None
with open("../DT/spam/sample_input.json", "r") as f:
    json_body = f.read()
result = dt_interp.classify(json_body)
print("Classify Result:")
print(result)
print("success")
# sys.exit(0)
