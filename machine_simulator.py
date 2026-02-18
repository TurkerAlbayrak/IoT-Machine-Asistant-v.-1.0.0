import time
import math
import random
import json
import paho.mqtt.client as mqtt

BROKER = "broker.hivemq.com"
PORT = 1883

REQUEST_TOPIC = "factory/request"
DATA_TOPIC = "factory/data"

t = 0

client = mqtt.Client()

def on_connect(client, userdata, flags, rc):
    print("Broker baglandi")
    client.subscribe(REQUEST_TOPIC)

def on_message(client, userdata, msg):
    global t

    print("ESP32 veri istedi")

    t += 0.1

    vibration = {
        "x": round(2 * math.sin(2 * math.pi * 2 * t) + random.uniform(-0.2,0.2),2),
        "y": round(2 * math.cos(2 * math.pi * 2 * t) + random.uniform(-0.2,0.2),2),
        "z": 9.81
    }

    temperature = round(25 + 3 * math.sin(0.1 * t),2)
    humidity = round(50 + 10 * math.cos(0.05 * t),2)

    gas = 300 + random.randint(-20,20)
    if random.randint(0,100) > 95:
        gas = 800 + random.randint(-50,50)

    data = {
        "vibration": vibration,
        "temperature": temperature,
        "humidity": humidity,
        "gas": gas
    }

    payload = json.dumps(data)

    client.publish(DATA_TOPIC, payload)
    print("Veri gonderildi:", payload)

client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT, 60)
client.loop_forever()
