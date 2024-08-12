import mqtt from 'paho-mqtt';

export class MQTTService {
    constructor(host, port, topic, messageCallbacks) {
        this.client = null;
        this.topic = topic;
        this.messageCallbacks = messageCallbacks;

        // Connessione al broker MQTT
        this.client = new mqtt.Client(host, port, 'mqttjs_' + Math.random().toString(16).substr(2, 8));
        this.client.onMessageArrived = this.messageCallbacks.onMessage.bind(this);

        this.client.connect({

            onSuccess: this.onConnected.bind(this),
            onFailure: (err) => {
                console.log(`Connection failed: ${err.errorMessage}`);
            },
            useSSL: false // oppure true se usi SSL/TLS
        });
    }

    onConnected() {
        console.log('Connesso al broker MQTT');
        console.log(`subscribing to topic ${this.topic}`);
        this.client.subscribe(this.topic);
    }

    onMessageArrived(message) {
        console.log(`onMessageArrived: ${message.payloadString}`);
    }

    publishMessage() {
        this.client.publish("serra/acqua", "1");
        console.log('Publishing message');
    }
}
