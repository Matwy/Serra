

// Import MQTT service
import { MQTTService } from "./mqttService.js";
var serre = JSON.parse(document.getElementById('graphs-container').dataset.serre);

console.log(serre);


const host = "192.168.0.191";
const port = 9001;
const topic = "serra/0";// + serre[0].serra;

let liveValues = [
    document.getElementById("live-value-1"),
    document.getElementById("live-value-2"),
    document.getElementById("live-value-3"),
    document.getElementById("live-value-4"),
    document.getElementById("live-value-5")
];

var temperatureCanvas = document.getElementById('temperature-chart').getContext('2d');
var humiditysCanvas = document.getElementById('humiditys-chart').getContext('2d');
var pressureCanvas = document.getElementById('pressure-chart').getContext('2d');
var altitudeCanvas = document.getElementById('altitude-chart').getContext('2d');

const temperatureData = {
    labels: Array.from({ length: 31 }, (_, i) => i),
    datasets: [{
        label: 'Temperature',
        borderColor: 'rgba(255, 0, 0, 1)',
        fill: false,
    }]
};
const pressureData = {
    labels: Array.from({ length: 31 }, (_, i) => i),
    datasets: [{
        label: 'Pressure',
        borderColor: 'rgba(0, 0, 255, 1)',
        fill: false,
    }]
};

const altitudeData = {
    labels: Array.from({ length: 31 }, (_, i) => i),
    datasets: [{
        label: 'Altitude',
        borderColor: 'rgba(255, 165, 0, 1)',
        fill: false,
    }]
};

const humiditysData = {
    labels: Array.from({ length: 31 }, (_, i) => i),
    datasets: [{
        label: 'Air Humidity',
        borderColor: 'rgba(143, 255, 231, 1)',
        fill: false
    }, {
        label: 'Terrain Humidity',
        borderColor: 'rgba(153, 79, 0, 1)',
        fill: false
    }]
};

const datas = [temperatureData, humiditysData, pressureData, altitudeData];
const canvas = [temperatureCanvas, humiditysCanvas, pressureCanvas, altitudeCanvas];
const suggestedMinMax = [[15, 30], [40, 60], [950, 1050], [0, 500]]
let liveCharts = [];
for (let i = 0; i < 4; i++) {
    const config = {
        type: 'line',
        data: datas[i],
        options: {
            responsive: true,
            plugins: {
                title: {
                    display: false
                }
            },
            maintainAspectRatio: false,
            scales: {
                x: {
                    title: {
                        display: false,
                    }
                },
                y: {
                    suggestedMin: suggestedMinMax[i][0],
                    suggestedMax: suggestedMinMax[i][1],
                    title: {
                        display: false,
                    }
                }
            },
        },
    };
    let chart = new Chart(canvas[i], config);
    liveCharts.push(chart);
}


function addData(data, chartIndex, datasetIndex = 0) {
    liveCharts[chartIndex].data.datasets[datasetIndex].data.push(data);
    if (liveCharts[chartIndex].data.datasets[datasetIndex].data.length >= 32) {
        liveCharts[chartIndex].data.datasets[datasetIndex].data.shift();
    }
    liveCharts[chartIndex].update(); // aggiorna il grafico
}
function updateLiveCharts(messageResponse) {
    addData(messageResponse[0], 0); // temperature
    addData(messageResponse[1], 1); // air humidity
    addData(messageResponse[2], 2); //pressure
    addData(messageResponse[3], 3); //altitude
    addData(messageResponse[4], 1, 1); //terrain humidity

    liveValues[0].innerHTML = messageResponse[0] + " °C"; // temperature
    liveValues[1].innerHTML = messageResponse[4] + " %"; //terrain humidity
    liveValues[2].innerHTML = messageResponse[1] + " %"; // air humidity
    liveValues[3].innerHTML = messageResponse[2] + " hPa"; //pressure
    liveValues[4].innerHTML = messageResponse[3] + " m"; //altitude
}

/*
  MQTT Message Handling Code
*/
const mqttStatus = document.querySelector(".status");

function onConnect(message) {
    mqttStatus.textContent = "Connected";
}
function onMessage(message) {
    var messageResponse = message.payloadString.split(",");
    updateLiveCharts(messageResponse);
}

function onError(error) {
    console.log(`Error encountered :: ${error}`);
    mqttStatus.textContent = "Error";
}

function onClose() {
    console.log(`MQTT connection closed!`);
    mqttStatus.textContent = "Closed";
}

function initializeMQTTConnection() {
    console.log(
        `Initializing connection to :: ${host}:${port} topic :: ${topic}`
    );
    var fnCallbacks = { onConnect, onMessage, onError, onClose };

    var mqttService = new MQTTService(host, port, topic, fnCallbacks);

    let waterButton = document.getElementById("water-button");
    waterButton.addEventListener("click", function () {
        waterButton.style.border = "4px solid  blue";
        mqttService.publishMessage();

    });
}



window.addEventListener("load", (event) => {
    initializeMQTTConnection();

});