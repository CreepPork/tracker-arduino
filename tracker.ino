#include "constants.hpp"
#include "secrets.hpp"
#include "http.cpp"

#include "vendor/TinyGSM/src/TinyGsmClient.h"
#include "vendor/TinyGPSPlus/src/TinyGPS++.h"

#include <SoftwareSerial.h>

SoftwareSerial gps_serial(PIN_GPS_RX, PIN_GPS_TX);
TinyGPSPlus gps;

SoftwareSerial gprs_serial(PIN_GPRS_RX, PIN_GPRS_TX);
TinyGsm modem(gprs_serial);
TinyGsmClientSecure client(modem);
Http http(client);

double latitude, longitude, speed, altitude = 0;

void setup()
{
    gps_serial.begin(GPS_BAUD);

    gprs_serial.begin(GPRS_BAUD);

    modem.restart();

    while (!modem.waitForNetwork())
    {
        //
    }

    while (!modem.gprsConnect(GPRS_APN, GPRS_USER, GPRS_PASSWORD))
    {
        //
    }
}

void loop()
{
    gps_serial.listen();

    latitude = gps.location.lat();
    longitude = gps.location.lng();
    speed = gps.speed.kmph();
    altitude = gps.altitude.meters();

    delay(3000);

    gprs_serial.listen();
    send_gps_data();
}

void send_gps_data()
{
    http.post(
        "/api/ingest",
        (String("")
            + "&secret=" + TRACKER_SECRET
            + "&latitude=" + latitude
            + "&longitude=" + longitude
            + "&speed=" + speed
            + "&alt=" + altitude
        ).c_str());
}
