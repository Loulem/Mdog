#include "CrawlingTrajectory.h"

CrawlingTrajectory::CrawlingTrajectory()
    : current_step_index_(0), last_step_time_ms_(0), step_interval_ms_(1) {}

void CrawlingTrajectory::generateTrajectory() {
    generateWalkPath();
    generateLateralShift();
}

bool CrawlingTrajectory::advanceStep(int direction, uint32_t current_ms) {
    if (current_ms - last_step_time_ms_ < step_interval_ms_) {
        return false;
    }
    
    if (direction > 0) {
        current_step_index_ = (current_step_index_ + 1) % TOTAL_STEPS;
    } else if (direction < 0) {
        current_step_index_ = (current_step_index_ - 1 + TOTAL_STEPS) % TOTAL_STEPS;
    }
    
    last_step_time_ms_ = current_ms;
    return true;
}

LegPosition CrawlingTrajectory::getLegPosition(uint8_t leg_index) const {
    if (leg_index >= 4) {
        return LegPosition{0.0f, 0.0f, 0.0f};
    }
    return trajectory_[current_step_index_][leg_index];
}

void CrawlingTrajectory::setStepInterval(uint16_t interval_ms) {
    step_interval_ms_ = interval_ms;
}

void CrawlingTrajectory::generateWalkPath() {
    // Generate angle array (first quarter: 180->0, rest: 360->180)
    // Angles start from 180 and go to 180 in clockwise direction (anti-trigo)
    float angles[NB_STEPS];
    float decrement1 = (360.0 - 180.0) / (3.0 * QUARTER_STEPS);
    float decrement2 = (180.0 - 0.0) / (float)QUARTER_STEPS;
    
    for (uint8_t i = 0; i < QUARTER_STEPS; i++) {
        angles[i] = 180.0 - i * decrement2;
    }
    for (uint8_t i = QUARTER_STEPS; i < NB_STEPS; i++) {
        angles[i] = 360.0 - (i - QUARTER_STEPS) * decrement1;
    }
    
    // Generate basic walk pattern (X, Y positions) without leg right/left shift
    float base_x[NB_STEPS];
    float base_y[NB_STEPS];
    float base_z[NB_STEPS];
    
    // First quarter: legs lift (circular arc)
    for (uint8_t i = 0; i < QUARTER_STEPS; i++) {
        base_x[i] = cos(angles[i] * DEG_TO_RAD) * 4.0;
        base_y[i] = STANCE_HEIGHT - sin(angles[i] * DEG_TO_RAD) * WALK_RADIUS;
    }
    
    // Remaining quarters: legs on ground (constant height)
    for (uint8_t i = QUARTER_STEPS; i < NB_STEPS; i++) {
        base_x[i] = cos(angles[i] * DEG_TO_RAD) * 4.0;
        base_y[i] = STANCE_HEIGHT;
    }
    
    // Z positions (left/right)
    for (uint8_t i = 0; i < NB_STEPS / 2; i++) {
        base_z[i] = Z_LEFT;
    }
    for (uint8_t i = NB_STEPS / 2; i < NB_STEPS; i++) {
        base_z[i] = Z_RIGHT;
    }
    
    // Distribute positions to each leg with phase shifts
    // Leg order: 2 -> 0 -> 3 -> 1 (diagonal gait)
    for (uint8_t i = 0; i < NB_STEPS / 2; i++) {
        uint8_t idx0 = (i + 3 * QUARTER_STEPS) % NB_STEPS;
        uint8_t idx1 = (i + QUARTER_STEPS) % NB_STEPS;
        uint8_t idx2 = i;
        uint8_t idx3 = (i + 2 * QUARTER_STEPS) % NB_STEPS;
        
        trajectory_[i][0].x = base_x[idx0] + X_OFFSET;
        trajectory_[i][1].x = base_x[idx1] + X_OFFSET;
        trajectory_[i][2].x = base_x[idx2] + X_OFFSET;
        trajectory_[i][3].x = base_x[idx3] + X_OFFSET;
        
        trajectory_[i][0].y = base_y[idx0];
        trajectory_[i][1].y = base_y[idx1];
        trajectory_[i][2].y = base_y[idx2];
        trajectory_[i][3].y = base_y[idx3];
        
        trajectory_[i][0].z = base_z[i];
        trajectory_[i][1].z = base_z[i];
        trajectory_[i][2].z = base_z[i];
        trajectory_[i][3].z = base_z[i];
    }
}

void CrawlingTrajectory::generateLateralShift() {
    // First half: shift from left to right
    float increment_z = (Z_RIGHT - Z_LEFT) / (float)(NB_LATERAL_STEPS / 2);
    
    // Transition 1: Z goes from left to right (steps 50-69)
    for (uint8_t i = NB_STEPS / 2; i < NB_STEPS / 2 + NB_LATERAL_STEPS / 2; i++) {
        trajectory_[i][0].x = trajectory_[i - 1][0].x;
        trajectory_[i][1].x = trajectory_[i - 1][1].x;
        trajectory_[i][2].x = trajectory_[i - 1][2].x;
        trajectory_[i][3].x = trajectory_[i - 1][3].x;
        
        trajectory_[i][0].y = trajectory_[i - 1][0].y;
        trajectory_[i][1].y = trajectory_[i - 1][1].y;
        trajectory_[i][2].y = trajectory_[i - 1][2].y;
        trajectory_[i][3].y = trajectory_[i - 1][3].y;
        
        float new_z = trajectory_[i - 1][0].z + increment_z;
        trajectory_[i][0].z = new_z;
        trajectory_[i][1].z = new_z;
        trajectory_[i][2].z = new_z;
        trajectory_[i][3].z = new_z;
    }
    
    // Continue walking (steps 70-119)
    for (uint8_t i = NB_STEPS / 2 + NB_LATERAL_STEPS / 2; i < NB_STEPS + NB_LATERAL_STEPS / 2; i++) {
        uint8_t base_idx = i - NB_LATERAL_STEPS / 2;
        uint8_t idx0 = (base_idx + 3 * QUARTER_STEPS) % NB_STEPS;
        uint8_t idx1 = (base_idx + QUARTER_STEPS) % NB_STEPS;
        uint8_t idx2 = base_idx;
        uint8_t idx3 = (base_idx + 2 * QUARTER_STEPS) % NB_STEPS;
        
        trajectory_[i][0].x = trajectory_[idx0][0].x;
        trajectory_[i][1].x = trajectory_[idx1][1].x;
        trajectory_[i][2].x = trajectory_[idx2][2].x;
        trajectory_[i][3].x = trajectory_[idx3][3].x;
        
        trajectory_[i][0].y = trajectory_[idx0][0].y;
        trajectory_[i][1].y = trajectory_[idx1][1].y;
        trajectory_[i][2].y = trajectory_[idx2][2].y;
        trajectory_[i][3].y = trajectory_[idx3][3].y;
        
        float z_val = (base_idx < NB_STEPS / 2) ? Z_LEFT : Z_RIGHT;
        trajectory_[i][0].z = z_val;
        trajectory_[i][1].z = z_val;
        trajectory_[i][2].z = z_val;
        trajectory_[i][3].z = z_val;
    }
    
    // Transition 2: Z goes from right to left (steps 120-139)
    for (uint8_t i = NB_STEPS + NB_LATERAL_STEPS / 2; i < TOTAL_STEPS; i++) {
        trajectory_[i][0].x = trajectory_[i - 1][0].x;
        trajectory_[i][1].x = trajectory_[i - 1][1].x;
        trajectory_[i][2].x = trajectory_[i - 1][2].x;
        trajectory_[i][3].x = trajectory_[i - 1][3].x;
        
        trajectory_[i][0].y = trajectory_[i - 1][0].y;
        trajectory_[i][1].y = trajectory_[i - 1][1].y;
        trajectory_[i][2].y = trajectory_[i - 1][2].y;
        trajectory_[i][3].y = trajectory_[i - 1][3].y;
        
        float new_z = trajectory_[i - 1][0].z - increment_z;
        trajectory_[i][0].z = new_z;
        trajectory_[i][1].z = new_z;
        trajectory_[i][2].z = new_z;
        trajectory_[i][3].z = new_z;
    }
}
