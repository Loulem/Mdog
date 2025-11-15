//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <Adafruit_PWMServoDriver.h> //https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
//#include "robot_leg_control.h"

int i1 = 0;
float x = 0;
byte sens = 0;

int axeX = A0;     // signal de l'axe X sur entrée A0
int axeY = A1;     // signal de l'axe Y sur entrée A1
int buttonPin = 10; // Bouton-poussoir en broche 7


void test_sequence();
void test_sequence();
void test_setup();
void test_servo_max();
void put_all_leg_to_zero();
void test_offset();
void test_h();
void test_angle();
void test_y();
void test_x();
void test_x_all_legs();
void test_x_y();
void test_walk();
void test_z();
void test_z_all_legs();
void test_angle_roll();
void right_left();


void test_sequence(){
  //test_offset();
  //test_servo_max();
  //test_h();
  test_walk();
  //test_z();
  //test_z_all_legs();
  // test_y();
  // test_x();
  // test_x_all_legs();
  // put_all_leg_to_zero();
  // test_x_y();
  // test_angle_roll();
  //test_angle();
  //right_left();
  
}

void test_setup(){
    pinMode(axeX, INPUT);             // définition de A0 comme une entrée
    pinMode(axeY, INPUT);             // définition de A1 comme une entrée
    pinMode(buttonPin, INPUT_PULLUP); // définition de 7 comme une entrée
}


void test_servo_max()
{

  if (!digitalRead(buttonPin))
  {
    i1 = (i1 + 1) % 16;
    delay(500);
  }
  if (sens)
  {
    x = (x + 10);
    if (x >= 2000)
    {
      sens = !sens;
    }
  }
  else
  {
    x = (x - 10);
    if (x <= 0)
    {
      sens = !sens;
    }
  }

  Serial.print("On est sur le moteur");
  Serial.print(i1);
  Serial.print("       ");
  Serial.print("x est égale à ");
  Serial.println(x);
  delay(25);

  servo_uc1.writeMicroseconds(i1, 500 + x);
  servo_uc2.writeMicroseconds(i1, 500 + x);
}

void put_all_leg_to_zero()
{
  for (int i = 0; i < 16; i++)
  {
    set_servo_angle(0, i);
  }
}

void test_offset()
{
  if (!digitalRead(buttonPin))
  {
    i1 = (i1 + 1) % 16;
    delay(500);
  }
  double X = analogRead(axeX) / 5 + servoNeutralPos[i1] - 100;
  Serial.print("On est sur le moteur");
  Serial.print(i1);
  Serial.print("       ");
  Serial.print("x est égal à ");
  Serial.println(X);
  servo_uc2.writeMicroseconds(i1, X);
  servo_uc1.writeMicroseconds(i1, X);
  delay(25);
}

void test_h()
{
  if (!digitalRead(buttonPin))
  {
    i1 = (i1 + 1) % 4;
    delay(500);
  }
  double X = (double)analogRead(axeX) / 100 + 14;
  Serial.print("On est sur le moteur");
  Serial.print(i1);
  Serial.print("       ");
  Serial.print("h est égale à ");
  Serial.println(X);
  write_h(X, i1);
  delay(25);
}

void test_angle()
{
  if (!digitalRead(buttonPin))
  {
    i1 = (i1 + 1) % 16;
    delay(500);
  }
  double X = (double)analogRead(axeX) / 20;
  Serial.print("On est sur le moteur");
  Serial.print(i1);
  Serial.print("       ");
  Serial.print("O° est égale à ");
  Serial.println(X);
  set_servo_angle(X, i1);
  delay(25);
}

void test_y()
{

  double X = (double)analogRead(axeX) / 100 + 4;

  Serial.print("La hauteur est égale à ");
  Serial.println(X);

  setLegHeightAll(X);
  delay(25);
}

void test_x()
{
  if (!digitalRead(buttonPin))
  {
    i1 = (i1 + 1) % 16;
    delay(500);
  }
  double X = (double)analogRead(axeX) / 100 - 5;

  Serial.print("On est sur le moteur");
  Serial.print(i1);
  Serial.print("       ");
  Serial.print("La longueur est égale à ");
  Serial.println(X);

  setLegXY(X, 15, i1);
  delay(25);
}

void test_x_all_legs()
{
  double X = (double)analogRead(axeX) / 100 - 5;

  Serial.print("La longueur est égale à ");
  Serial.println(X);
  setLegXYAll(X, 15);
  delay(25);
}

void test_x_y()
{

  float X, Y;

  X = analogRead(axeX) / 150 + 10;
  Y = analogRead(axeY) / 150 + 10;
  // X = analogRead (axeX) * (5.0 / 1023.0);
  // Y = analogRead (axeY) * (5.0 / 1023.0);

  Serial.print("Axe X:");
  Serial.print(X, 4);
  Serial.print("V, ");

  Serial.print("Axe Y:");
  Serial.print(Y, 4);
  Serial.print("V, ");

  Serial.println("Bouton:");

  setLegXYAll(X, Y);
}

void test_walk()
{
  double X = (double)analogRead(axeX) / 10;

  // Serial.print("La vittesse est égale à ");
  // Serial.print(X);
  // Serial.println(" %");

  walk(X);
}

void test_z()
{
  if (!digitalRead(buttonPin))
  {
    i1 = (i1 + 1) % 4;
    delay(500);
  }
  double X = (double)analogRead(axeX) / 100 - 2.5;
  // Serial.print("On est sur le moteur");
  // Serial.print(i1);
  // Serial.print("       ");
  // Serial.print("z est égale à ");
  // Serial.println(X);
  setLegXYZ(0, 20, X, i1);
  delay(25);
}

void test_z_all_legs()
{
  double X = (double)analogRead(axeX) / 100 - 2.5;
  // Serial.print("On est sur le moteur");
  // Serial.print(i1);
  // Serial.print("       ");
  // Serial.print("z est égale à ");
  // Serial.println(X);
  setLegXYZAll(0, 18, X);
  delay(25);
}

void test_angle_roll()
{
  double X = (double)analogRead(axeX) / 50.0 - 10;
  // Serial.print("On est sur le moteur");
  // Serial.print(i1);
  // Serial.print("       ");
  // Serial.print("z est égale à ");
  // Serial.println(X);

  all_leg_go_to_x_y_z_with_rotation(SERVO_X_DIST, -13, SERVO_Z_DIST, X * PI / 180.0, 0, 0);
  delay(25);
}

void right_left()
{
  if (sens)
  {
    x = (x + 0.1);
    if (x >= 5)
    {
      sens = !sens;
    }
  }
  else
  {
    x = (x - 0.1);
    if (x <= -5)
    {
      sens = !sens;
    }
  }
  setLegXYZAll(0, 18, x);
  delay(25);
}