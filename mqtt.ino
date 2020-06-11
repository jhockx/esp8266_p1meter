void sendMQTTMessage(const char *topic, char *payload)
{
    bool result = mqttClient.publish(topic, payload, false);
}

bool mqttReconnect()
{
    int MQTT_RECONNECT_RETRIES = 0;

    while (!mqttClient.connected() && MQTT_RECONNECT_RETRIES < MQTT_MAX_RECONNECT_TRIES)
    {
        MQTT_RECONNECT_RETRIES++;

        if (mqttClient.connect(HOSTNAME, MQTT_USER, MQTT_PASS))
        {
            char *message = new char[16 + strlen(HOSTNAME) + 1];
            strcpy(message, "p1 meter alive: ");
            strcat(message, HOSTNAME);
            mqttClient.publish("hass/status", message);
        }
        else
        {
            delay(5000);
        }
    }

    if (MQTT_RECONNECT_RETRIES >= MQTT_MAX_RECONNECT_TRIES)
    {
        return false;
    }

    return true;
}

void sendMetric(String name, long metric)
{
    if (metric > 0)
    {
        char output[10];
        ltoa(metric, output, sizeof(output));

        String topic = String(MQTT_ROOT_TOPIC) + "/" + name;
        sendMQTTMessage(topic.c_str(), output);
    }
}

void sendDataToBroker()
{
    for (int i = 0; i < NUMBER_OF_READOUTS; i++)
    {
        sendMetric(telegramObjects[i].name, telegramObjects[i].value);
    }
}
