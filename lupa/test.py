import dt_interp
import sys


g_interp = dt_interp.init('../DT/spam', "random_forest")
json_body = None
with open("../DT/spam/sample_input.json", "r") as f:
    json_body = f.read()
for i in range(100):
    result = dt_interp.classify(g_interp, json_body)
    print(i)
print("Classify Result:")
print(result)
dt_interp.release(g_interp)
print("success")
