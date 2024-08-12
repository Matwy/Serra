import random
from paho.mqtt import client as mqtt_client
from threading import Thread
from time import sleep
from datetime import datetime
import mysql.connector

broker = '192.168.1.209'
port = 1883
topic = "Serra5CIIgruppoMGA"
client_id = f'python-mqtt-{random.randint(0, 100)}'

arr_data_sensors = [0, 0, 0, 0, 0, 0]
n_for_avarage_datas = 0
seconds = 0
mycursor = None
mydb = None


def timer():
    global n_for_avarage_datas, seconds, arr_data_sensors
    while True:
        seconds += 1
        if seconds >= 60:
            seconds = 0
            if n_for_avarage_datas > 0:
                InsertQuery(avarage(arr_data_sensors))
                arr_data_sensors = [0, 0, 0, 0, 0, 0]
            n_for_avarage_datas = 0

        sleep(1)


def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def InsertQuery(arr_datas):
    global mycursor, mydb
    #my_tuple = (content, type, datetime.now())
    sql = "INSERT INTO sensori (`umidità_terreno`, `temperatura`, `umidità`, `pressione`, `altitudine`, `id_serra`, `data`) VALUES (%s, %s,%s,%s, %s,%s,%s)"
    arr_datas.insert(6,datetime.now())
    mycursor.execute(sql, arr_datas)
    mydb.commit()
    print("aggiunto")

def avarage(arr):
    for i in range(0, 5):
        arr[i] /= n_for_avarage_datas
        arr[i] = round(arr[i],1)
    return arr


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        global n_for_avarage_datas
        n_for_avarage_datas += 1
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        arr_data_sensors1 = msg.payload.decode().split(',')
        try:
            for i in range(0, 5):
                arr_data_sensors[i] += float(arr_data_sensors1[i])
            arr_data_sensors[5] = arr_data_sensors1[5]
        except ValueError:
            print("ValueError in converting data from MQTT message to int")

    client.subscribe(topic)
    client.on_message = on_message


def run():
    global mycursor, mydb
    mydb = mysql.connector.connect(
        host="localhost",
        user="root",
        password="",
        database="serra"
    )
    mycursor = mydb.cursor()
    client = connect_mqtt()
    subscribe(client)

    thread_subscriber = Thread(target=client.loop_forever)
    thread_subscriber.start()

    thread_timer = Thread(target=timer)
    thread_timer.start()


if __name__ == '__main__':
    run()
