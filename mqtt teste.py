import paho.mqtt.client as mqtt
import time
import random
import json

# MQTT Broker settings
broker_address = "dev.saiot2.ect.ufrn.br"
port = 8000
topic = "measurements/273e255d-125b-47ca-979c-66b29263fd35"
client_id = "273e255d-125b-47ca-979c-66b29263fd35"
username = "saiotect@gmail.com"
password = "@2345678"

value = 0

# Function to send MQTT messages with variable value and frequency
def send_mqtt_messages():
    client = mqtt.Client(client_id=client_id)

    # Set username and password if provided
    if username and password:
        client.username_pw_set(username, password)

    try:
        client.connect(broker_address, port)

        message_id = "SupaSens"

        while True:
            global value
            message = {"id": message_id, "value": value}
            
            value = value + 1
            
            if(value>30):
                value = 0

            # Convert the message to JSON format
            json_message = json.dumps(message)

            # Publish the message to the specified topic
            client.publish(topic, json_message)

            # Print the sent message for verification
            print(f"Sent: {json_message}")

            # Adjust the sleep duration for variable frequency
            time.sleep(1)

    except KeyboardInterrupt:
        print("Script terminated by user.")
    finally:
        client.disconnect()

if __name__ == "__main__":
    send_mqtt_messages()
