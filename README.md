# Serra e IoT

![serra](https://github.com/Giorgio0004/Serra/assets/93153474/ec2b1572-41ec-4c99-a419-7eeffece1b20)

Questo progetto utilizza un Arduino MKR1000 come publisher per i dati di sei sensori di temperatura e umidità, un Raspberry Pi come broker utilizzando Mosquitto, e un sito web Laravel che sfrutta MQTT over WebSocket per ricevere in tempo reale i dati e visualizzarli su grafici.
Ecco la presentazione della serra https://docs.google.com/presentation/d/1DXfOI4KkMKe0ySXlvxLb1iJ_ovHE9GpdOrmsOPRQnmY/edit?usp=sharing

## Installazione e Configurazione

### Arduino MKR1000

1. Caricare lo sketch `btnDisplaySensors3.ino` sull'Arduino MKR1000 utilizzando l'IDE di Arduino.
2. Collegare i sensori ai pin corrispondenti dell'Arduino MKR1000.
3. Configurare le impostazioni di connessione Wi-Fi e MQTT.

### Raspberry Pi

1. Installare Mosquitto utilizzando il comando `sudo apt-get install mosquitto`.
2. Configurare Mosquitto per utilizzare WebSockets aggiungendo le seguenti righe al file di configurazione `/etc/mosquitto/mosquitto.conf`:`listener 9001 protocol websockets`
3. Avviare il servizio Mosquitto utilizzando il comando `sudo systemctl start mosquitto`.

### Sito Web Laravel

1. Clonare il repository del progetto nel proprio server web.
2. Installare le dipendenze utilizzando il comando `composer install`.
3. Configurare le impostazioni di connessione al database nel file `.env`.
4. Avviare il server web utilizzando il comando  `npm run dev` e `php artisan serve`.

## Utilizzo

1. Accedere al sito web tramite il proprio browser.
2. I grafici mostreranno in tempo reale i dati dei sensori di temperatura e umidità pubblicati dall'Arduino MKR1000 attraverso il Raspberry Pi.

## Vantaggi

Questa serra con sensori di temperatura e umidità offre numerosi vantaggi, tra cui:

- **Miglior controllo ambientale:** i sensori di temperatura e umidità consentono di monitorare e mantenere i livelli di temperatura e umidità ideali per la crescita delle piante, migliorando la qualità e la quantità del raccolto.

- **Risparmio energetico:** grazie al controllo preciso dei livelli di temperatura e umidità, è possibile ridurre i costi energetici associati alla climatizzazione della serra.

- **Maggiore efficienza:** la serra con sensori di temperatura e umidità consente di ottimizzare le risorse e di ridurre gli sprechi, migliorando l'efficienza complessiva del processo di coltivazione.

Inoltre, grazie alla presenza di un sistema di annaffiatura automatica controllato da remoto, è possibile garantire una corretta irrigazione delle piante anche in assenza di personale, migliorando ulteriormente l'efficienza e la produttività della serra. 

## Annaffiatura Remota

Il sistema di annaffiatura automatica controllato da remoto è basato su un Arduino MKR1000 che utilizza un sensore di umidità del suolo per rilevare il livello di umidità del terreno e attivare l'irrigazione quando necessario. Il sistema può essere controllato da remoto tramite un'interfaccia web, che consente di attivare l'irrigazione e di monitorare lo stato del sistema.

Grazie a questa funzionalità, è possibile garantire una corretta irrigazione delle piante anche in assenza di personale, riducendo il rischio di stress idrico e migliorando la salute e la produttività delle piante.
