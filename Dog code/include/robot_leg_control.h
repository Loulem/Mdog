#ifndef ROBOT_LEG_CONTROL_H
#define ROBOT_LEG_CONTROL_H
#include <Arduino.h>
#include <math.h>
#include <Adafruit_PWMServoDriver.h> //https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library




Adafruit_PWMServoDriver servo_uc1 = Adafruit_PWMServoDriver(0x40); // driver de servo moteur avec l’adresse 0x40
Adafruit_PWMServoDriver servo_uc2 = Adafruit_PWMServoDriver(0x41); // driver de servo moteur avec l’adresse 0x40



const int servoNeutralPos[16] = {1500, 1500, 1500, 1500, 1500, 1500, 1490, 1430, 1460, 1590, 1440, 1560, 1550, 1500, 1490, 1533}; // correspond à la commande en micro seconde pour mettre le servo à 0°
const int servo_max_ms[16] = {2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500}; // valeur max en micro seconde accepté par le servo moteur
const int servo_min_ms[16] = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}; // valeur min en micro seconde accepté par le servo moteur
const int servo_min_angle[16] = {-90, -90, -90, -90, -90, -90, -90, -90, -90, -90, -90, -90, -90, -90, -90, -90}; // angle max des servos
const int servo_max_angle[16] = {90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90}; // angle min des servos
const double shoulderToFootZ[4] = {5.375, 5.375, 5.375, 5.375}; // correspondant à la distance suivant Z séparant l’épaule du pied (double supports fractional mm)


double thighLength[4] = {11, 11, 11, 11};
double shinLength[4] = {14.3, 14.3, 14.3, 14.3};
byte sens_servo[4] = {0, 1, 0, 1}; // 0 sens trigo => ex 500ms = -90 et 2500ms = 90

#define SERVO_X_DIST 15.7
#define SERVO_Z_DIST 8
double leg_x_distance[4] = {SERVO_X_DIST, SERVO_X_DIST, -SERVO_X_DIST, -SERVO_X_DIST};
double leg_z_distance[4] = {-SERVO_Z_DIST, SERVO_Z_DIST, -SERVO_Z_DIST, SERVO_Z_DIST};



int angle_to_ms(double angle, int servo_i)
{ // Convertit un angle en durée d'impulsion en microsecondes pour le servo

  return ((double)(servo_max_ms[servo_i] - servo_min_ms[servo_i]) / ( servo_max_angle[servo_i] - servo_min_angle[servo_i]) * angle);
}

void set_servo_angle(double angle, int servo_i)
{ // commande au servo de se mettre à l’angle demandé dans le sens trigo et 0° correspond au milieu
  servo_uc1.writeMicroseconds(servo_i, angle_to_ms(angle, servo_i) + servoNeutralPos[servo_i]);
  servo_uc2.writeMicroseconds(servo_i, angle_to_ms(angle, servo_i) + servoNeutralPos[servo_i]);
}

double calculate_shoulder_angle(double h, int leg_i)
{
  return acos((pow(h, 2) + pow(thighLength[leg_i], 2) - pow(shinLength[leg_i], 2)) / (2 * h * thighLength[leg_i])) * 180.0 / PI;
}

double calculate_knee_angle(double h, int leg_i)
{
  return acos((pow(thighLength[leg_i], 2) + pow(shinLength[leg_i], 2) - pow(h, 2)) / (2 * shinLength[leg_i] * thighLength[leg_i])) * 180.0 / PI;
}

void write_h(double h, int leg_i)
{ // Définit la hauteur d'une jambe en ajustant les angles de l'épaule et du genou
  double shoulder_angle = calculate_shoulder_angle(h, leg_i);
  double knee_angle = calculate_knee_angle(h, leg_i);

  if (sens_servo[leg_i])
  { // si le servo est positionné d’un certain côté
    set_servo_angle(-shoulder_angle, leg_i + 12);
    set_servo_angle((90 - knee_angle), leg_i + 8);
  }
  else
  {
    set_servo_angle(shoulder_angle, leg_i + 12);
    set_servo_angle(-(90 - knee_angle), leg_i + 8);
  }
}

void setLegHeightAll(double h)
{
  for (int i = 0; i < 4; i++)
  {
    write_h(h, i);
  }
}

void setLegXY(double x, double y, int leg_i)
{

  double h = sqrt(pow(y, 2) + pow(x, 2));
  // double h = y/(cos(add_shoulder_angle*PI/180));

  double add_shoulder_angle = atan2((double)x, (double)y) * 180.0 / PI;
  double shoulder_angle = calculate_shoulder_angle(h, leg_i);
  double knee_angle = calculate_knee_angle(h, leg_i);

  if (sens_servo[leg_i])
  {
    set_servo_angle(-shoulder_angle + add_shoulder_angle, leg_i + 12);
    set_servo_angle((90 - knee_angle), leg_i + 8);
  }
  else
  {
    set_servo_angle(shoulder_angle - add_shoulder_angle, leg_i + 12);
    set_servo_angle(-(90 - knee_angle), leg_i + 8);
  }
}

void setLegXYAll(double x, double y)
{
  for (int i = 0; i < 4; i++)
  {
    setLegXY(x, y, i);
  }
}

void setLegXYZ(double x, double y, double z, int leg_i)
{
  /*Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(z);
  Serial.print(" ");
  Serial.println(leg_i);*/

  double y1;
  double phi;
  if (leg_i % 2)
  {
    double hyp = sqrt(pow(y, 2) + pow(z +  shoulderToFootZ[leg_i], 2));
    y1 = sqrt(pow(hyp, 2) - pow( shoulderToFootZ[leg_i], 2));
  double teta = atan2((z + shoulderToFootZ[leg_i]), y) * 180.0 / PI;
  double psi = acos(shoulderToFootZ[leg_i] / hyp) * 180.0 / PI;
    phi = -90 + teta + psi;
    /*Serial.print(hyp);
    Serial.print(" ");
    Serial.print(y1);
    Serial.print(" ");
    Serial.println(phi);*/
  }

  else
  {
    double hyp = sqrt(pow(y, 2) + pow(z -  shoulderToFootZ[leg_i], 2));
    y1 = sqrt(pow(hyp, 2) - pow( shoulderToFootZ[leg_i], 2));
  double teta = atan2((z - shoulderToFootZ[leg_i]), y) * 180.0 / PI;
  double psi = acos(shoulderToFootZ[leg_i] / hyp) * 180.0 / PI;
    phi = 90 + teta - psi;
  }

  if (leg_i <= 1)
  {
    set_servo_angle(-phi, leg_i + 4);
  }
  else
  {
    set_servo_angle(phi, leg_i + 4);
  }
  setLegXY(x, y1, leg_i);
}

void setLegXYZAll(double x, double y, double z)
{
  for (int i = 0; i < 4; i++)
  {
    setLegXYZ(x, y, z, i);
  }
}


void leg_go_to_x_y_z_with_rotation(double x, double y, double z, double teta, double phi, double psy, int leg_i)
{ // x,y,z sont les coordonnées associés au repaire du sol centré au centre du chien, l’angle teta est la rotation autour de z, phi est la rotation autour de x1 et psi autour de y2
  // on calcule le vecteur (x,y,z) dans la base R2
  double cteta = cos(teta);
  double steta = sin(teta);
  double cphi = cos(phi);
  double sphi = sin(phi);
  double cpsy = cos(psy);
  double spsy = sin(psy);

  double x3 = x * (cteta * cpsy - spsy * steta * sphi) + y * (cpsy * steta + spsy * sphi * cteta) - z * spsy * cphi - leg_x_distance[leg_i];
  double y3 = -x * (steta * cphi) + y * cphi * cteta + z * sphi;
  double z3 = x * (cteta * spsy + cpsy * steta * sphi) + y * (spsy * steta - cpsy * sphi * cteta) + z * cpsy * cphi - leg_z_distance[leg_i];
  setLegXYZ(x3, -y3, z3, leg_i);
}

void all_leg_go_to_x_y_z_with_rotation(double x, double y, double z, double teta, double phi, double psy)
{
  leg_go_to_x_y_z_with_rotation(x, y, -z, teta, phi, psy, 0);
  leg_go_to_x_y_z_with_rotation(x, y, z, teta, phi, psy, 1);
  leg_go_to_x_y_z_with_rotation(-x, y, -z, teta, phi, psy, 2);
  leg_go_to_x_y_z_with_rotation(-x, y, z, teta, phi, psy, 3);
}

#endif