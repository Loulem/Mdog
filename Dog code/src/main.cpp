/*A faire :
- changer atan par atan2 et tester
- rajouter la marche arrière
- rajouter la rotation du robot
- ajouter les différents mode de fonctionnement
- la réception RF

*/

// Libraries

int command;
int controller_x;
int controller_y;

#include <math.h>
#include "robot_leg_control.h"
#include "crawling.h"
#include "communication.h"






///////////////// Déclaration pour la communication///////////////////////







#include "test.h"

void setup()
{
  delay(5000);
  
  
  servo_uc1.begin();
  servo_uc1.setOscillatorFrequency(26250000); // a determiner à l’oscillocope pour chaque driver (vérifier qu’on obtient 2000 microsecond pour un writemicrosecond de 2000)
  servo_uc1.setPWMFreq(300); // Analog servos run at ~50 Hz updates

  servo_uc2.begin();
  servo_uc2.setOscillatorFrequency(25360000);
  servo_uc2.setPWMFreq(300);         // Analog servos run at ~50 Hz updates
  
  for (int i = 4; i < 16; i++)
  {
    
    servo_uc1.writeMicroseconds(i, servoNeutralPos[i]);
    servo_uc2.writeMicroseconds(i, servoNeutralPos[i]);
  }

  Serial.begin(250000);
  nrf24_init(115);

  
  test_setup();
  //setup_walk_path();

  setup_walk_path_with_right_left();
  /*
  delay(4000);
  for (int i = 0; i < NB_ANGLE + NB_LEFT_TO_RIGHT;i++)
  {
    Serial.print(leg0_x_pos[i]);
    Serial.print(" ");
    Serial.print(leg0_y_pos[i]);
    Serial.print(" ");
    Serial.print(leg0_z_pos[i]);
    Serial.println();

    delay(10);
  
  }
  delay(4000);
  */
}

void loop()
{
  test_sequence();

  
  /*unsigned long t1 = micros();
  get_command();
  walk( speed,command);
  unsigned long t2 = micros();
  unsigned long t = t2 - t1;
  Serial.println(t);
  */

  
}



void controller_main_loop()
{
    get_command();
    if (command == 1){
      setLegXYAll(controller_x, controller_y);
    }
    else if (command == 2){
      all_leg_go_to_x_y_z_with_rotation(SERVO_X_DIST, -13, SERVO_Z_DIST, controller_x * PI / 180.0, 0, 0);
    }
    
  }
