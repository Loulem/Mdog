/*A faire :
- changer atan par atan2 et tester
- rajouter la marche arrière
- rajouter la rotation du robot
- ajouter les différents mode de fonctionnement
- la réception RF

*/

// Libraries
int speed;
int command;
#include <math.h>
#include "robot_leg_control.h"
#include "walk.h"
#include "communication.h"






///////////////// Déclaration pour la communication///////////////////////







#include "test.h"

void setup()
{
  delay(5000);
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
  delay(1000);

}

void loop()
{
  //test_sequence();

  
  unsigned long t1 = micros();
  get_command();
  walk( speed,command);
  unsigned long t2 = micros();
  unsigned long t = t2 - t1;
  Serial.println(t);
  

  
}



