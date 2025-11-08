#define NB_ANGLE 100
#define NB_LEFT_TO_RIGHT 40
#define QUARTER_NB_ANGLE NB_ANGLE / 4
#define Z_LEFT -4.0
#define Z_RIGHT 4.0

// int angle[40]{354, 348, 342, 336, 330, 324, 318, 312, 306, 300, 294, 288, 282, 276, 270, 264, 258, 252, 246, 240, 234, 228, 222, 216, 210, 204, 198, 192, 186, 180, 162, 144, 126, 108, 90, 72, 54, 36, 18, 0};
float crawling_x_pos[NB_ANGLE]; // liste des coordonnées x des pates
float y_pos[NB_ANGLE];
float z_pos[NB_ANGLE];
float offset_y = 21;
#define X_OFFSET -4 // offset pour la position x des pattes
#define OFFSET_Z 0 // offset pour la position z des pattes
#define WALK_RADIUS 6
int n_walk = 0;
long unsigned int delay_walk = 1;
long unsigned int precedent_movement = 0;
bool isWalkingEnabled = 1;

int angle[NB_ANGLE];
float leg0_x_pos[NB_ANGLE + NB_LEFT_TO_RIGHT];
float leg1_x_pos[NB_ANGLE + NB_LEFT_TO_RIGHT];
float leg2_x_pos[NB_ANGLE + NB_LEFT_TO_RIGHT];
float leg3_x_pos[NB_ANGLE + NB_LEFT_TO_RIGHT];

float leg0_y_pos[NB_ANGLE + NB_LEFT_TO_RIGHT];
float leg1_y_pos[NB_ANGLE + NB_LEFT_TO_RIGHT];
float leg2_y_pos[NB_ANGLE + NB_LEFT_TO_RIGHT];
float leg3_y_pos[NB_ANGLE + NB_LEFT_TO_RIGHT];

float leg_z_pos[NB_ANGLE + NB_LEFT_TO_RIGHT];

void setup_angle()
{
  int quart = NB_ANGLE / 4;
  double decrement1 = (360.0 - 180.0) / (3 * quart); // 3 derniers quarts
  double decrement2 = (180.0 - 0.0) / quart;         // Premier quart

  // Remplissage du premier quart (180 à 0)
  for (int i = 0; i < quart; i++)
  {
    // angle[i] = 180.0 - (i - 3 * quart) * decrement2;
    angle[i] = 180.0 - i * decrement2;
  }

  // Remplissage des 3 derniers quarts (360 à 180)
  for (int i = quart; i < NB_ANGLE; i++)
  {
    angle[i] = 360.0 - (i-quart) * decrement1;
  }
}

void setup_walk_path()
{
  setup_angle();
  int quart = NB_ANGLE / 4;
  // on remplit les tableaux des positions des pates qui se lèvent pour le premier quart des positions
  for (int i = 0; i < quart; i++)
  {
    crawling_x_pos[i] = cos((float)angle[i] * 2 * PI / 360.0) * 4;
    y_pos[i] = offset_y - sin((float)angle[i] * 2 * PI / 360.0) * WALK_RADIUS;
  }

  // on remplit les tableaux des positions des pates au sol pour les trois quarts restant des positions
  for (int i = quart; i < NB_ANGLE; i++)
  {
    crawling_x_pos[i] = cos((float)angle[i] * 2 * PI / 360.0) * 4;
    y_pos[i] = offset_y;
  }

  for (int i = 0; i < 2 * quart; i++)
  {
    z_pos[i] = Z_LEFT;
  }

  for (int i = 2 * quart; i < NB_ANGLE; i++)
  {
    z_pos[i] = Z_RIGHT;
  }
}

void setup_walk_path_with_right_left()
{
  setup_walk_path();
  for (int i = 0; i < NB_ANGLE / 2; i++)
  {
    // first leg to go up
    leg2_x_pos[i] = crawling_x_pos[i];
    leg2_y_pos[i] = y_pos[i];
    leg_z_pos[i] = z_pos[i];

    // second leg to go up
    leg0_x_pos[i] = crawling_x_pos[(i + QUARTER_NB_ANGLE * 3) % NB_ANGLE];
    leg0_y_pos[i] = y_pos[(i + QUARTER_NB_ANGLE * 3) % NB_ANGLE];

    // third leg to go up
    leg3_x_pos[i] = crawling_x_pos[(i + QUARTER_NB_ANGLE * 2) % NB_ANGLE];
    leg3_y_pos[i] = y_pos[(i + QUARTER_NB_ANGLE * 2) % NB_ANGLE];

    // fourth leg to go up
    leg1_x_pos[i] = crawling_x_pos[(i + QUARTER_NB_ANGLE) % NB_ANGLE];
    leg1_y_pos[i] = y_pos[(i + QUARTER_NB_ANGLE) % NB_ANGLE];

  }
  float increment_z = (Z_RIGHT - Z_LEFT) / (float)(NB_LEFT_TO_RIGHT / 2.0);
  for (int i = NB_ANGLE / 2; i < NB_ANGLE / 2 + NB_LEFT_TO_RIGHT / 2; i++)
  {
    leg0_x_pos[i] = leg0_x_pos[i - 1];
    leg0_y_pos[i] = leg0_y_pos[i - 1];
    leg_z_pos[i] = leg_z_pos[i - 1] + increment_z;

    leg1_x_pos[i] = leg1_x_pos[i - 1];
    leg1_y_pos[i] = leg1_y_pos[i - 1];

    leg2_x_pos[i] = leg2_x_pos[i - 1];
    leg2_y_pos[i] = leg2_y_pos[i - 1];

    leg3_x_pos[i] = leg3_x_pos[i - 1];
    leg3_y_pos[i] = leg3_y_pos[i - 1];

  }
  for (int i = NB_ANGLE / 2 + NB_LEFT_TO_RIGHT / 2; i < NB_ANGLE + NB_LEFT_TO_RIGHT / 2; i++)
  {
    
    leg0_x_pos[i] = crawling_x_pos[i - NB_LEFT_TO_RIGHT/2];
    leg0_y_pos[i] = y_pos[i - NB_LEFT_TO_RIGHT/2];
    leg_z_pos[i] = z_pos[i - NB_LEFT_TO_RIGHT/2];

    leg3_x_pos[i] = crawling_x_pos[(i - NB_LEFT_TO_RIGHT/2 + QUARTER_NB_ANGLE) % NB_ANGLE];
    leg3_y_pos[i] = y_pos[(i - NB_LEFT_TO_RIGHT/2 + QUARTER_NB_ANGLE) % NB_ANGLE];

    leg1_x_pos[i] = crawling_x_pos[(i - NB_LEFT_TO_RIGHT/2 + QUARTER_NB_ANGLE * 2) % NB_ANGLE];
    leg1_y_pos[i] = y_pos[(i - NB_LEFT_TO_RIGHT/2 + QUARTER_NB_ANGLE * 2) % NB_ANGLE];

    leg2_x_pos[i] = crawling_x_pos[(i - NB_LEFT_TO_RIGHT/2 + QUARTER_NB_ANGLE * 3) % NB_ANGLE];
    leg2_y_pos[i] = y_pos[(i - NB_LEFT_TO_RIGHT/2 + QUARTER_NB_ANGLE * 3) % NB_ANGLE];

  }
  for (int i = NB_ANGLE + NB_LEFT_TO_RIGHT / 2; i < NB_ANGLE + NB_LEFT_TO_RIGHT; i++)
  {
    leg0_x_pos[i] = leg0_x_pos[i - 1];
    leg0_y_pos[i] = leg0_y_pos[i - 1];
    leg_z_pos[i] = leg_z_pos[i - 1] - increment_z;

    leg1_x_pos[i] = leg1_x_pos[i - 1];
    leg1_y_pos[i] = leg1_y_pos[i - 1];

    leg2_x_pos[i] = leg2_x_pos[i - 1];
    leg2_y_pos[i] = leg2_y_pos[i - 1];

    leg3_x_pos[i] = leg3_x_pos[i - 1];
    leg3_y_pos[i] = leg3_y_pos[i - 1];

  }
}

void walk(int speed /*vittesse en %*/, int sens /*1 = avancer -1 reculer*/)
{

  if (isWalkingEnabled && speed != 0)
  {
    if ((millis() - precedent_movement) >= delay_walk)
    {
      Serial.print(leg0_x_pos[n_walk]);
      Serial.print(leg0_y_pos[n_walk]);
      Serial.println(leg_z_pos[n_walk]);
      setLegXYZ(leg0_x_pos[n_walk]+X_OFFSET, leg0_y_pos[n_walk], leg_z_pos[n_walk]-OFFSET_Z, 0);
      setLegXYZ(leg1_x_pos[n_walk]+X_OFFSET, leg1_y_pos[n_walk], leg_z_pos[n_walk]+OFFSET_Z, 1);
      setLegXYZ(leg2_x_pos[n_walk]+X_OFFSET, leg2_y_pos[n_walk], leg_z_pos[n_walk]-OFFSET_Z, 2);
      setLegXYZ(leg3_x_pos[n_walk]+X_OFFSET, leg3_y_pos[n_walk], leg_z_pos[n_walk]+OFFSET_Z, 3);
      //setLegXYZ(crawling_x_pos[n_walk], y_pos[n_walk], z_pos[n_walk], 0);
      //setLegXYZ(crawling_x_pos[(n_walk + QUARTER_NB_ANGLE + NB_ANGLE) % NB_ANGLE], y_pos[(n_walk + QUARTER_NB_ANGLE) % NB_ANGLE], z_pos[n_walk], 3);
      //setLegXYZ(crawling_x_pos[(n_walk + QUARTER_NB_ANGLE * 2 + NB_ANGLE) % NB_ANGLE], y_pos[(n_walk + QUARTER_NB_ANGLE * 2) % NB_ANGLE], z_pos[n_walk], 1);
      //setLegXYZ(crawling_x_pos[(n_walk + QUARTER_NB_ANGLE * 3 + NB_ANGLE) % NB_ANGLE], y_pos[(n_walk + QUARTER_NB_ANGLE * 3) % NB_ANGLE], z_pos[n_walk], 2);
      
      if (sens == 1)
      {
        n_walk = (n_walk + 1) % (NB_ANGLE + NB_LEFT_TO_RIGHT);
      }
      else if (sens == -1)
      {
        n_walk = (n_walk - 1 + NB_ANGLE + NB_LEFT_TO_RIGHT) % (NB_ANGLE + NB_LEFT_TO_RIGHT);
      }

      precedent_movement = millis();
    }
  }
}
void walk(int speed)
{
  walk(speed, 1);
}
