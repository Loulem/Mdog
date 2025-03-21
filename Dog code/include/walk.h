// int angle[40]{354, 348, 342, 336, 330, 324, 318, 312, 306, 300, 294, 288, 282, 276, 270, 264, 258, 252, 246, 240, 234, 228, 222, 216, 210, 204, 198, 192, 186, 180, 162, 144, 126, 108, 90, 72, 54, 36, 18, 0};
double x_pos[40];
double y_pos[40];
float offset_z = 17;
byte n_walk = 0;
long unsigned int delay_walk = 10;
long unsigned int precedent_movement = 0;
bool isWalkingEnabled = 1;

#define NB_ANGLE 99
int angle[NB_ANGLE];

void setup_angle()
{
  int quart = NB_ANGLE / 4;
  double decrement1 = (360.0 - 180.0) / (3 * quart); // 3 premiers quarts
  double decrement2 = (180.0 - 0.0) / quart;         // Dernier quart

  // Remplissage des 3 premiers quarts (360 à 180)
  for (int i = 0; i < 3 * quart; i++)
  {
    angle[i] = 360.0 - i * decrement1;
  }

  // Remplissage du dernier quart (180 à 0)
  for (int i = 3 * quart; i < NB_ANGLE; i++)
  {
    angle[i] = 180.0 - (i - 3 * quart) * decrement2;
  }
}

void setup_walk_path()
{
  for (int i = 30; i < 40; i++)
  {
    x_pos[i] = cos((float)angle[i] * 2 * PI / 360.0) * 4;
    y_pos[i] = offset_z - sin((float)angle[i] * 2 * PI / 360.0) * 5;
  }
  for (int i = 0; i < 30; i++)
  {
    x_pos[i] = cos((float)angle[i] * 2 * PI / 360.0) * 4;
    y_pos[i] = offset_z;
  }
}

void walk(int speed /*vittesse en %*/, int sens /*1 = avancer -1 reculer*/)
{

  if (isWalkingEnabled && speed != 0)
  {
    if ((millis() - precedent_movement) >= delay_walk)
    {

      Serial.println(x_pos[n_walk]);
      Serial.println(y_pos[n_walk]);
      Serial.println(n_walk);
      setLegXY(x_pos[n_walk], y_pos[n_walk], 0);
      setLegXY(x_pos[(n_walk + 10 + 40) % 40], y_pos[(n_walk + 10) % 40], 1);
      setLegXY(x_pos[(n_walk + 20 + 40) % 40], y_pos[(n_walk + 20) % 40], 2);
      setLegXY(x_pos[(n_walk + 30 + 40) % 40], y_pos[(n_walk + 30) % 40], 3);
      if (sens == 1)
      {
        n_walk = (n_walk + 1) % 40;
      }
      else if (sens == -1)
      {
        n_walk = (n_walk - 1 + 40) % 40;
      }

      precedent_movement = millis();
    }
  }
}
void walk(int speed)
{
  walk(speed, 1);
}
