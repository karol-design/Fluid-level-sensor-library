/*  Name: Fluid_level_sensor.cpp
 *  Description: IoT Fluid level sensor based on ESP32 and HC-SR04 | Device library
 *  Author: Karol Wojsław
 *  Date: 18/06/2021 (last release)
 */

 #include "Fluid_level_sensor.h" // Library header file


/* ------------------------------------- */
/* Fluid level sensor class constructor  */
Fluid_level_sensor::Fluid_level_sensor() {
  /* Output/input IO pins assignment */
  pinMode(LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  /* Set outputs states at startup */
  digitalWrite(LED, LOW);
}


/* ------------------------------------------------------------------------ */
/* Method that measures and returns in cm the distance measured by HC-SR04  */
float Fluid_level_sensor::measureDistance() {
  digitalWrite(TRIG_PIN, LOW);  // Begin by setting Trigger pin low
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); // Set Trigger pin HIGH for 15 us
  delayMicroseconds(15);
  digitalWrite(TRIG_PIN, LOW);
  noInterrupts(); // Deactivate interrupts
  float pulseTime = pulseIn(ECHO_PIN, HIGH); // Measure pulse time from ECHO_PIN
  interrupts(); // Turn interrupts back on
  float distance = pulseTime / 58.0; // Calculate distance by dividing pulse time by 58

  if(distance < 200.0 && distance > 2.0) { // Check if the result fits within the possible range
    return distance; // Return measured distance
  } else if(distance <= MIN_DISTANCE) {
    return 0.0;
  } else if(distance >= MAX_DISTANCE) {
    return 200.0;
  }

  return -1.0; // If something goes wrong return -1 as a sign of an error
}


/* ------------------------------------------------------------------------------- */
/* Method that calculate and returns in cm the average of N_MEASUREMENTS readings  */
float Fluid_level_sensor::getDistance() {
  float readings_sum = 0.0;

  for(int i = 0; i < N_MEASUREMENTS; i++) { // Try to take N_MEASUREMENTS measurements
    readings_sum += measureDistance();   // Add measured distance to readings_sum
    delay(100); // Wait 100 ms between readings for better performance
  }

  return (readings_sum / N_MEASUREMENTS); //Return the average of N_MEASUREMENTS readings in cm
}


/* ---------------------------------------------------------------------------- */
/* Method that create and returns mail message for the user as String variable  */
String Fluid_level_sensor::createMessage(long rssi, int errors, int bat_SOC, float distance) {
  /* Variable definition & initialisation */
  String msg_header, msg_body_1, msg_body_2, msg_body_3, msg_body_4, msg_body_5, msg_body_6, msg_footer, message;
  String _errors, _bat_SOC;
  float _distance = (distance / 100.0);

  /* Check if any errors occured */
  if(errors) {
    _errors = String(errors);
  } else if (!errors) {
    _errors = String("Brak");
  }

  if(bat_SOC > 90) {
    _bat_SOC = String("Znakomity");
  } else if (bat_SOC > 60) {
    _bat_SOC = String("Dobry");
  } else if (bat_SOC > 30) {
    _bat_SOC = String("Niski - niezbędna wymiana!");
  } else if (bat_SOC > 0) {
    _bat_SOC = String("Krytyczny - niezbędna wymiana!");
  }

  msg_header = String("<div><h2><span style=\"color:#2980b9\">Czujnik poziomu nieczystości </span><span style=\"color:#3498db\">| Informacja:</span></h2><p>Zainstalowany&nbsp;przez&nbsp;ciebie&nbsp;czujnik poziomu nieczystości&nbsp;wykrył&nbsp;<strong>wysoki&nbsp;stan&nbsp;płyn&oacute;w&nbsp;w&nbsp;zbiorniku</strong>.</p>");
  msg_body_1 = String("<p>Ostatni&nbsp;pomiar&nbsp;wskazuje&nbsp;na&nbsp;");
  msg_body_2 = String("&nbsp;m&nbsp;do&nbsp;stanu&nbsp;maksymalnego.</p><p>Pomyślnie wysłano maila z zam&oacute;wieniem wywiezienia nieczystości na ustawiony wcześniej adres.</p>");
  msg_body_3 = String("<table align=\"left\" border=\"0\" cellpadding=\"1\" cellspacing=\"1\" style=\"width:330px\"><thead><tr><th colspan=\"2\" scope=\"col\" style=\"width: 320px;\"><hr/></th></tr></thead><tbody><tr><td style=\"width:169px\">📶 &nbsp;|&nbsp;<span style=\"color:#3498db\">Zasięg sieci WiFi&nbsp;</span></td><td style=\"width:148px\"> ");
  msg_body_4 = String(" dB</td></tr><tr><td style=\"width:169px\">🔋&nbsp; |&nbsp;<span style=\"color:#2ecc71\">Stan&nbsp;baterii&nbsp;</span></td><td style=\"width:148px\">");
  msg_body_5 = String("</td></tr><tr><td style=\"width:169px\"><span style=\"color:#3498db\">&nbsp;</span>&nbsp;&nbsp;⚠&nbsp;&nbsp;&nbsp;|&nbsp;<span style=\"color:#7f8c8d\">Wykryte błędy</span></td><td style=\"width:148px\">");
  msg_body_6 = String("</td></tr></tbody></table><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><hr /><p><span style=\"font-size:11px\">");
  msg_footer = String("Dziękujemy&nbsp;za&nbsp;korzystanie&nbsp;z naszego produktu.&nbsp;Masz&nbsp;pytania lub potrzebujesz&nbsp;pomocy?</span></p><p><span style=\"font-size:11px\"><strong>Skontaktuj&nbsp;się&nbsp;ze&nbsp;wsparciem&nbsp;technicznym</strong> mailowo (simple.tech@gmail.com) lub zadzwoń na numer: +48 332958467.</span></p></div>");

  message = String();
  message = msg_header + msg_body_1 + _distance + msg_body_2 + msg_body_3 + rssi + msg_body_4 + _bat_SOC + " (" + bat_SOC + " %)" +msg_body_5 + _errors + msg_body_6 + msg_footer;

  return message;
}