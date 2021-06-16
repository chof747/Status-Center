#ifndef CONFIG_H
#define CONFIG_H


#define WIFI_SSID       ""
#define WIFI_PASSWORD   ""

#define MQTT_BROKER     "127.0.0.1"
#define MQTT_USER       ""
#define MQTT_PASSWORD   ""

#define MQTT_SUBSCRIPTIONS 2

#ifndef CONTROLLER_TIMEOUT 
    #define CONTROLLER_TIMEOUT 30000
#endif


#endif //CONFIG_H