#include "LegKinematics.h"
#include <math.h>


const uint8_t LegKinematics::servo_orientation_[4] = {0, 1, 0, 1}; // trigonometric orientation for legs 0-3 ( legs 1 and 3 are mirrored so the rotation direction is inverted) 

LegKinematics::LegKinematics() {}

LegJoints LegKinematics::calculateJointAngles(LegId leg, const LegPosition& position) const {
    LegJoints joints;
    
    // Calculate hip angle from y and z
    joints.hipAngle = calculateHipAngle(leg, position.y, position.z);
    
    // Calculate y1 (projected Y after hip rotation)
    float y1;
    if (servo_orientation_[leg]) {  // legs 1 and 3 (right side)
        float hyp = sqrt(pow(position.y, 2) + pow(position.z + SHOULDER_TO_FOOT_Z, 2));
        y1 = sqrt(pow(hyp, 2) - pow(SHOULDER_TO_FOOT_Z, 2));
    } else {  // legs 0 and 2 (left side)
        float hyp = sqrt(pow(position.y, 2) + pow(position.z - SHOULDER_TO_FOOT_Z, 2));
        y1 = sqrt(pow(hyp, 2) - pow(SHOULDER_TO_FOOT_Z, 2));
    }
    
    // Calculate hypotenuse in the leg plane (X, Y1)
    float h = sqrt(pow(position.x, 2) + pow(y1, 2));
    
    // Calculate base shoulder angle from hypotenuse
    float base_shoulder_angle = calculateShoulderAngle(h);
    
    // Add correction angle from X offset
    float add_shoulder_angle = atan2(position.x, y1) * RAD_TO_DEG;
    
    // Apply orientation-specific adjustments
    if (servo_orientation_[leg]) {  // right side legs
        joints.shoulderAngle = -base_shoulder_angle + add_shoulder_angle;
    } else {  // left side legs
        joints.shoulderAngle = base_shoulder_angle - add_shoulder_angle;
    }
    
    // Calculate knee angle
    float knee_angle = calculateKneeAngle(h);
    
    // Apply orientation-specific knee adjustment
    if (servo_orientation_[leg]) {  // right side
        joints.kneeAngle = 90 - knee_angle;
    } else {  // left side
        joints.kneeAngle = -(90 - knee_angle);
    }
    
    return joints;
}

float LegKinematics::calculateShoulderAngle(float hypotenuse) const {
    float num = pow(hypotenuse, 2) + pow(THIGH_LENGTH, 2) - pow(SHIN_LENGTH, 2);
    float den = 2 * hypotenuse * THIGH_LENGTH;
    return acos(num / den) * RAD_TO_DEG;
}

float LegKinematics::calculateKneeAngle(float hypotenuse) const {
    float num = pow(THIGH_LENGTH, 2) + pow(SHIN_LENGTH, 2) - pow(hypotenuse, 2);
    float den = 2 * SHIN_LENGTH * THIGH_LENGTH;
    return acos(num / den) * RAD_TO_DEG;
}

float LegKinematics::calculateHipAngle(LegId leg, float y, float z) const {
    float phi;
    
    if (servo_orientation_[leg]) {  // legs 1 and 3 (right side)
        float hyp = sqrt(pow(y, 2) + pow(z + SHOULDER_TO_FOOT_Z, 2));
        float teta = atan2((z + SHOULDER_TO_FOOT_Z), y) * RAD_TO_DEG;
        float psi = acos(SHOULDER_TO_FOOT_Z / hyp) * RAD_TO_DEG;
        phi = -90 + teta + psi;
    } else {  // legs 0 and 2 (left side)
        float hyp = sqrt(pow(y, 2) + pow(z - SHOULDER_TO_FOOT_Z, 2));
        float teta = atan2((z - SHOULDER_TO_FOOT_Z), y) * RAD_TO_DEG;
        float psi = acos(SHOULDER_TO_FOOT_Z / hyp) * RAD_TO_DEG;
        phi = 90 + teta - psi;
    }
    
    // Apply front/back adjustment
    if (leg <= LEG_FRONT_RIGHT) {  // front legs (0 and 1)
        phi = -phi;
    }
    
    return phi;
}
