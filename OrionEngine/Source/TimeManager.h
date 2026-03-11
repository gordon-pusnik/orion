//
//  TimeManager.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-02.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__TimeManager__
#define __Orion__TimeManager__

#include <string>

class TimeManager
{
public:
    TimeManager();
    ~TimeManager();
    void update(float dt);
    double getElapsed() { return totalElapsed; };
    
private:
    int frame;
    double elapsed;
    int duration;
    char buf[10];
    std::string time_str;
    std::string hrs_str;
    std::string mins_str;
    std::string secs_str;
    double totalElapsed;
};

#endif /* defined(__Orion__TimeManager__) */
