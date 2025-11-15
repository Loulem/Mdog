#ifndef LEG_STATE_H
#define LEG_STATE_H

// Position d'une patte en coordonnées cartésiennes (mm)
struct LegPosition {
    float x;  // Avant/Arrière
    float y;  // Haut/Bas
    float z;  // Gauche/Droite
};

// Angles des joints d'une patte (degrés)
struct LegJoints {
    float hipAngle;       // Rotation latérale
    float shoulderAngle;  // Rotation avant/arrière
    float kneeAngle;      // Flexion
};

// État complet des 4 pattes en positions
struct AllLegsPosition {
    LegPosition front_left;
    LegPosition front_right;
    LegPosition back_left;
    LegPosition back_right;
    
    // Accès par index
    LegPosition& operator[](uint8_t index) {
        switch(index) {
            case 0: return front_left;
            case 1: return front_right;
            case 2: return back_left;
            case 3: return back_right;
            default: return front_left;
        }
    }
    
    const LegPosition& operator[](uint8_t index) const {
        switch(index) {
            case 0: return front_left;
            case 1: return front_right;
            case 2: return back_left;
            case 3: return back_right;
            default: return front_left;
        }
    }
    #ifdef DEBUG_PRINT_ENABLE
    void print() const {
        Serial.println("Leg Positions:");
        Serial.print("Front Left: ("); Serial.print(front_left.x); Serial.print(", "); Serial.print(front_left.y); Serial.print(", "); Serial.print(front_left.z); Serial.println(")");
        Serial.print("Front Right: ("); Serial.print(front_right.x); Serial.print(", "); Serial.print(front_right.y); Serial.print(", "); Serial.print(front_right.z); Serial.println(")");
        Serial.print("Back Left: ("); Serial.print(back_left.x); Serial.print(", "); Serial.print(back_left.y); Serial.print(", "); Serial.print(back_left.z); Serial.println(")");
        Serial.print("Back Right: ("); Serial.print(back_right.x); Serial.print(", "); Serial.print(back_right.y); Serial.print(", "); Serial.print(back_right.z); Serial.println(")");
    }
    #endif
};

// État complet des 4 pattes en angles
struct AllLegsJoints {
    LegJoints front_left;
    LegJoints front_right;
    LegJoints back_left;
    LegJoints back_right;
    
    // Accès par index
    LegJoints& operator[](uint8_t index) {
        switch(index) {
            case 0: return front_left;
            case 1: return front_right;
            case 2: return back_left;
            case 3: return back_right;
            default: return front_left;
        }
    }
    
    const LegJoints& operator[](uint8_t index) const {
        switch(index) {
            case 0: return front_left;
            case 1: return front_right;
            case 2: return back_left;
            case 3: return back_right;
            default: return front_left;
        }
    }
    #ifdef DEBUG_PRINT_ENABLE
    void print() const {
        Serial.println("Leg Joints:");
        Serial.print("Front Left: ("); Serial.print(front_left.hipAngle); Serial.print(", "); Serial.print(front_left.shoulderAngle); Serial.print(", "); Serial.print(front_left.kneeAngle); Serial.println(")");
        Serial.print("Front Right: ("); Serial.print(front_right.hipAngle); Serial.print(", "); Serial.print(front_right.shoulderAngle); Serial.print(", "); Serial.print(front_right.kneeAngle); Serial.println(")");
        Serial.print("Back Left: ("); Serial.print(back_left.hipAngle); Serial.print(", "); Serial.print(back_left.shoulderAngle); Serial.print(", "); Serial.print(back_left.kneeAngle); Serial.println(")");
        Serial.print("Back Right: ("); Serial.print(back_right.hipAngle); Serial.print(", "); Serial.print(back_right.shoulderAngle); Serial.print(", "); Serial.print(back_right.kneeAngle); Serial.println(")");
    }
    #endif
};

#endif // LEG_STATE_H
