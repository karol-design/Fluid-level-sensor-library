/* ESP32 based IoT fluid level sensor | Device test
 * Created by Karol Wojslaw on 18th June 2021
 * Description: The code aim is to test core functionalities of the device (measuring distance and sending mails)
*/

#include "Freenove_WS2812_Lib_for_ESP32.h" // WS2812 RGB LED library
#include <Fluid_level_sensor.h> // Fluid level sensor library
#include <ESP_Mail_Client.h>    // ESP Mail sending/receiving library
#include <WiFi.h>               // ESP WiFi library

#define WIFI_SSID "TP_LINK_7522"        // WiFi SSID (name)
#define WIFI_PASSWORD "TwojaStara7522"  // WiFi password

#define RECIPIENT_EMAIL "wojslaw.tech@gmail.com" // Email adress of the receipient

Fluid_level_sensor device;  // Fluid level sensor object
SMTPSession smtp;           // The SMTP Session object used for Email sending
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(1, WS2812_PIN, 0, TYPE_GRB); // WS2812 RGB LED object


/* ---------------------------------------- Setup ---------------------------------------- */
void setup() {
  Serial.begin(115200);
  strip.begin(); // WS2812 RGB LED initialised
	strip.setBrightness(45);

  /* Try to connect to WiFi access point */
  Serial.print("\nConnecting to WiFi access point (AP).");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    strip.setLedColorData(0, 255, 0, 0);
		strip.show();
    delay(200);
  }
  
  strip.setLedColorData(0, 100, 0, 250);
	strip.show();
  Serial.print("\n\nWiFi connected.\nIP address: ");
  Serial.print(WiFi.localIP());
  Serial.print("\nWiFi signal strength: ");
  Serial.println(WiFi.RSSI());

  smtp.debug(0); // Disable the debug via Serial port: none debug or 0 / basic debug or 1
}


/* ---------------------------------------- Loop ---------------------------------------- */
void loop() {
  ESP_Mail_Session session; // Declare the session config data object

  // float distance = device.measureDistance();  // Measure distance using HC-SR04
  // Serial.print("Measured distance [cm]: ");   // Print measured value
  // Serial.println(distance);

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = "Czujnik poziomu nieczystości";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Poziom płynów zbliża się do maksimum";
  message.addRecipient("Użytkownik", RECIPIENT_EMAIL);

  /*Send HTML message*/
  String htmlMsg = device.createMessage(WiFi.RSSI(), 0x0, 95, 120.5);
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  if(digitalRead(BUTTON) == LOW) {
    strip.setLedColorData(0, 250, 250, 250);
	  strip.show();
    digitalWrite(LED, HIGH);
    /* Connect to server with the session config */
    if (!smtp.connect(&session))
      return;

    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&smtp, &message))
      Serial.println("Error sending Email, " + smtp.errorReason());
    digitalWrite(LED, LOW);
    strip.setLedColorData(0, 100, 0, 250);
	  strip.show();
  }
}