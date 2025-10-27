/* Roadmap / A faire:
 - Add reverse walking
 - Add body rotation mode
 - Add multiple operating modes
 - RF reception
*/

// Application entry point: setup() and loop()
// Libraries
#include <Arduino.h>
#include <math.h>

// Global controller state
int command;
float controller_x;
float controller_y;

#include "robot_leg_control.h"
#include "crawling.h"
#include "communication.h"

void controller_main_loop();





///////////////// Déclaration pour la communication///////////////////////







#include "test.h"

void setup()
{
  delay(5000);
  
  
  // Initialize both servo drivers
  servo_uc1.begin();
  servo_uc1.setOscillatorFrequency(26250000); // tune per board with oscilloscope
  servo_uc1.setPWMFreq(300); // PCA9685 PWM frequency (tuned for your hardware)

  servo_uc2.begin();
  servo_uc2.setOscillatorFrequency(25360000);
  servo_uc2.setPWMFreq(300);         // Keep same frequency on both boards
  
  for (int i = 4; i < 16; i++)
  {
    // Move all driven channels to their mechanical neutral
    servo_uc1.writeMicroseconds(i, servoNeutralPos[i]);
    servo_uc2.writeMicroseconds(i, servoNeutralPos[i]);
  }

  Serial.begin(250000);
  nrf24_init(115); // Initialize RF link (channel/id as used by your setup)

  
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
    Serial.print(leg_z_pos[i]);
    Serial.println();

    delay(10);
  
  }
  delay(4000);
  */
}

void loop()
{
  // Demo/test sequence; replace by controller_main_loop when ready
  test_sequence();
  /*Serial.print(command);
  Serial.print(" ");
  Serial.print(controller_x);
  Serial.print(" ");
  Serial.println(controller_y);
*/
  //controller_main_loop();
  /*unsigned long t1 = micros();
  get_command();
  walk( speed,command);
  unsigned long t2 = micros();
  unsigned long t = t2 - t1;
  Serial.println(t);
  */

  
}


