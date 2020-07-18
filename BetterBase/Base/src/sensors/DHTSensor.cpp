#include "../inc/BaseConfig.h"

#include "DHTSensor.h"

DHTSensor::DHTSensor() {
    dht = new DHT(PINOUT_DHT22_INPUT, DHT22);
}

void DHTSensor::init() {
    dht->begin();
}

float DHTSensor::getHumidity() {
    return dht->readHumidity();
}

float DHTSensor::getTemperature() {
    return dht->readTemperature();
}

