import json
import kafka
import signal
import sys
consumer_stream = kafka.KafkaConsumer('ab') # default bootstrap_servers is localhost:9092
total_msgs = 0

def signal_handler(signal, frame):
    print("Total messages", total_msgs)
    sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)


for msg in consumer_stream:
    print("Out tracer id", json.loads(msg.value)["out_tracer_id"])
    total_msgs += 1

