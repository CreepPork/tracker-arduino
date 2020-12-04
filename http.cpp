#include "constants.hpp"
#include "secrets.hpp"
#include <Arduino.h>

#include "vendor/TinyGSM/src/TinyGsmClient.h"

class Http
{
private:
    TinyGsmClientSecure client;

public:
    Http(TinyGsmClientSecure client)
    {
        this->client = client;
    }

    String get(char *resource)
    {
        if (!client.connect(TRACKER_SERVER, HTTPS_PORT))
        {
            delay(10000);
            return;
        };

        client.println(String("GET ") + resource + " HTTP/1.1");
        client.println(String("Host: ") + TRACKER_SERVER);
        client.println("Connection: close\r\n");
        client.println();

        int timeout = millis();

        String data = "";
        int data_counter = 0;

        while (client.connected() && millis() - timeout < 10000)
        {
            while (client.available())
            {
                data[data_counter] = client.read();
                timeout = millis();
                data_counter++;
            }
        }

        data[data_counter] = '\r\n';

        client.stop();

        return data;
    }

    String post(char *resource, const char *data)
    {
        if (!client.connect(TRACKER_SERVER, HTTPS_PORT))
        {
            delay(10000);
            return;
        };

        String data_string = String(data);

        client.println(String("POST ") + resource + " HTTP/1.1");
        client.println(String("Host: ") + TRACKER_SERVER);
        client.println("Connection: close\r\n");
        client.println("Content-Type: application/x-www-form-urlencoded;");
        client.print("Content-Length: ");
        client.println(data_string.length());
        client.println();
        client.println(data_string);

        int timeout = millis();

        String data = "";
        int data_counter = 0;

        while (client.connected() && millis() - timeout < 10000)
        {
            while (client.available())
            {
                data[data_counter] = client.read();
                timeout = millis();
                data_counter++;
            }
        }

        data[data_counter] = '\r\n';

        client.stop();

        return data;
    }
};
