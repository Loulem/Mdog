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
};

#endif // LEG_STATE_H
