//
//  TimeManager.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-02.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "TimeManager.h"
#include <cmath>
#include "Utils.h"

TimeManager::TimeManager()
{
    frame = 0;
    elapsed = 0;
    duration = 0;
    totalElapsed = 0;
}

TimeManager::~TimeManager()
{
    
}

void TimeManager::update(float dt)
{
    totalElapsed += dt;
    elapsed += dt;
    frame++;
    if (elapsed >= 1.0) {
        duration++;
        int hrs = (int) (floor((float) (duration / 3600)));
        int mins = (int) (floor((float) ((duration / 60) % 60)));
        int secs = duration % 60;
        
        // Format output to HH:MM:SS
        time_str = "";
        sprintf(buf, "%d:", hrs);
        hrs_str = buf;
        time_str += hrs_str;
        
        if (mins < 10) {
            char buf[10];
            sprintf(buf, "0%d:", mins);
            mins_str = buf;
            time_str += mins_str;
        }
        else {
            char buf[10];
            sprintf(buf, "%d:", mins);
            mins_str = buf;
            time_str += mins_str;
        }
        
        if (secs < 10) {
            char buf[10];
            sprintf(buf, "0%d", secs);
            secs_str = buf;
            time_str += secs_str;
        }
        else {
            char buf[10];
            sprintf(buf, "%d", secs);
            secs_str = buf;
            time_str += secs_str;
        }
        
//        LOGI("time: %s, dt: %f, fps: %d\n", time_str.c_str(), dt, frame);
        frame = 0;
        elapsed = 0.0;
    }
}