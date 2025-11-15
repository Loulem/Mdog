// Walking gait generation utilities
// - Generates a simple 4-phase crawling trajectory over NB_ANGLE samples
// - X follows a cosine; Y stays at offset except during swing (last quarter)
#pragma once
#include <Arduino.h>
#define NB_ANGLE 100

// int angle[40]{354, 348, 342, 336, 330, 324, 318, 312, 306, 300, 294, 288, 282, 276, 270, 264, 258, 252, 246, 240, 234, 228, 222, 216, 210, 204, 198, 192, 186, 180, 162, 144, 126, 108, 90, 72, 54, 36, 18, 0};
double crawling_x_pos[NB_ANGLE]; // X trajectory samples for one full cycle
double y_pos[NB_ANGLE];           // Y trajectory samples for one full cycle
float offset_y = 26;  // Neutral Y offset (stance height)
float WALK_RADIUS = 6; // Swing arc amplitude on Y during the last quarter
byte n_walk = 0;                        // Current index in the trajectory
long unsigned int delay_walk = 1;       // Minimum time between steps (ms)
long unsigned int precedent_movement = 0; // Last time we advanced n_walk
bool isWalkingEnabled = 1;              // Master enable
#define QUARTER_NB_ANGLE (NB_ANGLE/4)

int angle[NB_ANGLE]; // Degrees for each sample (monotonic 360..0 across the cycle)

// Fill 'angle' array with a smooth 360->180 (3 quarters) then 180->0 (last quarter)
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

// Build X/Y trajectories for a full cycle
void setup_walk_path()
{
  setup_angle();
  int quart = NB_ANGLE / 4;
  for (int i = 3 * quart; i < NB_ANGLE; i++)
  {
    crawling_x_pos[i] = cos((float)angle[i] * 2 * PI / 360.0) * 4;
    y_pos[i] = offset_y - sin((float)angle[i] * 2 * PI / 360.0) * WALK_RADIUS;
  }
  for (int i = 0; i < 3 * quart; i++)
  {
    crawling_x_pos[i] = cos((float)angle[i] * 2 * PI / 360.0) * 4;
    y_pos[i] = offset_y;
  }
}

// Advance walking trajectory if enabled
// speed: percent (unused here but kept for API compatibility)
// sens: 1 forward, -1 backward
void walk(int speed /*vittesse en %*/, int sens /*1 = avancer -1 reculer*/)
{
  
  if (isWalkingEnabled && speed != 0)
  {
    if ((millis() - precedent_movement) >= delay_walk)
    {
      setLegXY(crawling_x_pos[n_walk], y_pos[n_walk], 0);
      setLegXY(crawling_x_pos[(n_walk + QUARTER_NB_ANGLE + NB_ANGLE) % NB_ANGLE], y_pos[(n_walk + QUARTER_NB_ANGLE) % NB_ANGLE], 1);
      setLegXY(crawling_x_pos[(n_walk + QUARTER_NB_ANGLE*2 + NB_ANGLE) % NB_ANGLE], y_pos[(n_walk + QUARTER_NB_ANGLE*2) % NB_ANGLE], 2);
      setLegXY(crawling_x_pos[(n_walk + QUARTER_NB_ANGLE*3 + NB_ANGLE) % NB_ANGLE], y_pos[(n_walk + QUARTER_NB_ANGLE*3) % NB_ANGLE], 3);
      if (sens == 1)
      {
        n_walk = (n_walk + 1) % NB_ANGLE;
      }
      else if (sens == -1)
      {
        n_walk = (n_walk - 1 + NB_ANGLE) % NB_ANGLE;
      }

      precedent_movement = millis();
    }
  }
}
// Overload: defaults to forward
void walk(int speed)
{
  walk(speed, 1);
}
