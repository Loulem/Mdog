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

// Global controller state (angle for future use)
float controller_x;

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
  // Read controller command as a struct (angle, power)
  ControllerCommand cmd = get_command();

  int speed = 0;
  int sens = 1; // 1 forward, -1 backward
  if (cmd.ok) {
    controller_x = cmd.angle;
    // Use power magnitude as speed, sign as direction
    sens = (cmd.power < 0) ? -1 : 1;
    speed = (int)fabs(cmd.power);
  }
  // Demo/test sequence; replace by controller_main_loop when ready
  test_sequence();

  walk(speed, sens);


  
}


