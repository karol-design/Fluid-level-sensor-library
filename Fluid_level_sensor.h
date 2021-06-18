/*  Name: Fluid_level_sensor.h
 *  Description: IoT Fluid level sensor based on ESP32 and HC-SR04 | Device library
 *  Author: Karol Wojsław
 *  Date: 18/06/2021 (last release)
 */

#ifndef Fluid_level_sensor_h
#define Fluid_level_sensor_h

#include <Arduino.h> // Include standard Arduino header file

/* Board IO pins in use definition */
#define BUTTON 27     // Button on GPIO 27
#define LED 2         // Blue built-in LED on GPIO 2
#define WS2812_PIN 5  // RGB WS2812 LED on GPIO 5

/* Distance measurement sensor (HC-SR04) macro definitions */
#define TRIG_PIN 25
#define ECHO_PIN 26
#define N_MEASUREMENTS 10
#define MIN_DISTANCE 2.0
#define MAX_DISTANCE 200.0

/* Define core information for SMTP connection*/
#define SMTP_HOST "smtp.gmail.com" // SMTP host adress for Gmail
#define SMTP_PORT 465              // SMTP port for Gmail SSL connection
#define AUTHOR_EMAIL "esp32.karol@gmail.com" // Email adress of the sender (! less secure app option should be enabled)
#define AUTHOR_PASSWORD "TestESP1234"        // Password to the sender account


class Fluid_level_sensor {
  public:
    Fluid_level_sensor();
    float measureDistance();
    float getDistance();
    String createMessage(long rssi, int errors, int bat_SOC, float distance);

  private:
    float pulseTime, distance, readings_sum, _distance;
    String msg_header, msg_body_1, msg_body_2, msg_body_3, msg_body_4, msg_body_5, msg_body_6, msg_footer, message;
    String _errors, _battery_SOC, _wifi_range;
};

#endif