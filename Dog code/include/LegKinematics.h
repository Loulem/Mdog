#ifndef LEG_KINEMATICS_H
#define LEG_KINEMATICS_H

#include <Arduino.h>
#include "LegState.h"

enum LegId {
    LEG_FRONT_LEFT = 0,
    LEG_FRONT_RIGHT = 1,
    LEG_BACK_LEFT = 2,
    LEG_BACK_RIGHT = 3
};

class LegKinematics {
public:
    LegKinematics();
    
    LegJoints calculateJointAngles(LegId leg, const LegPosition& position) const;

private:
    
    static constexpr float THIGH_LENGTH = 11.0;
    static constexpr float SHIN_LENGTH = 14.3;
    static constexpr float SHOULDER_TO_FOOT_Z = 5.375;
    static constexpr float SERVO_X_DIST = 15.7;
    static constexpr float SERVO_Z_DIST = 8.0;
    
    static const uint8_t servo_orientation_[4];
    
    float calculateShoulderAngle(float hypotenuse) const;
    float calculateKneeAngle(float hypotenuse) const;
    float calculateHipAngle(LegId leg, float y, float z) const;
};

#endif // LEG_KINEMATICS_H
