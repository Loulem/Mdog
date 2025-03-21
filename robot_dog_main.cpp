/*A faire :
- changer atan par atan2 et tester
- rajouter la marche arrière
- rajouter la rotation du robot
- ajouter les différents mode de fonctionnement
- la réception RF

*/

// Libraries

#include <math.h>
#include "robot_leg_control.h"
#include "walk.h"
#include "communication.h"






///////////////// Déclaration pour la communication///////////////////////







#include "test.h"

void setup()
{
  test_setup();
  setup_walk_path();

  servo_uc1.begin();
  servo_uc1.setPWMFreq(50); // Analog servos run at ~50 Hz updates

  servo_uc2.begin();
  servo_uc2.setPWMFreq(50);         // Analog servos run at ~50 Hz updates

  for (int i = 4; i < 16; i++)
  {
    servo_uc1.writeMicroseconds(i, servoNeutralPos[i]);
    servo_uc2.writeMicroseconds(i, servoNeutralPos[i]);
  }

  Serial.begin(250000);
  nrf24_init(115);
}

void loop()
{
  Serial.println("here");
  // test_offset();
  //  test_h();
  //test_walk();
  radioNRF.startListening();
  if (radioNRF.available())
  {
    // total = total + 1;
    // Serial.println(total);
    radioNRF.read(&receivedData, sizeof(receivedData));
    radioNRFData = receivedData;
    // Serial.println(radioNRFData);
  }
  Serial.println(radioNRFData);
  int index_of_comma = radioNRFData.indexOf(',');
  int command = radioNRFData.substring(0, index_of_comma).toInt();
  int speed = radioNRFData.substring(index_of_comma + 1).toInt();
  Serial.print(speed);
  Serial.println(command);
  
  walk(speed, command);
  // test_z();
  // test_z_all_legs();
  // test_y();
  // test_x();
  // test_x_all_legs();
  // put_all_leg_to_zero();
  // test_x_y();

  // test_angle_roll();
  /* float X, Y;
  int Bouton;
  X = analogRead(axeX)-(float)(1023/2);
  Y = analogRead(axeY)-(float)(1023/2);
  //X = analogRead (axeX) * (5.0 / 1023.0);
  //Y = analogRead (axeY) * (5.0 / 1023.0);
  Bouton = digitalRead (buttonPin);

  Serial.print ("Axe X:");
  Serial.print (X, 4);
  Serial.print ("V, ");

  Serial.print ("Axe Y:");
  Serial.print (Y, 4);
  Serial.print ("V, ");

  Serial.print ("Bouton:");
  */
}
