#ifndef CRAWLING_TRAJECTORY_H
#define CRAWLING_TRAJECTORY_H

#include <Arduino.h>
#include "LegState.h"

class CrawlingTrajectory {
public:
    CrawlingTrajectory();
    
    void generateTrajectory();
    bool advanceStep(int direction, uint32_t current_ms);
    LegPosition getLegPosition(uint8_t leg_index) const;
    void setStepInterval(uint16_t interval_ms);

private:
    
    static constexpr uint16_t NB_STEPS = 100;
    static constexpr uint16_t NB_LATERAL_STEPS = 40;
    static constexpr uint16_t TOTAL_STEPS = NB_STEPS + NB_LATERAL_STEPS;
    static constexpr uint8_t QUARTER_STEPS = NB_STEPS / 4;
    
    static constexpr float WALK_RADIUS = 6.0;
    static constexpr float STANCE_HEIGHT = 21.0;
    static constexpr float X_OFFSET = -4.0;
    static constexpr float Z_LEFT = -4.0;
    static constexpr float Z_RIGHT = 4.0;
    
    AllLegsPosition trajectory_[TOTAL_STEPS];
    uint16_t current_step_index_;
    uint32_t last_step_time_ms_;
    uint16_t step_interval_ms_;
    
    void generateWalkPath();
    void generateLateralShift();
};

#endif // CRAWLING_TRAJECTORY_H
