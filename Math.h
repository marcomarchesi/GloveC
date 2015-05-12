//
//  Math.h
//  GloveRuntime
//
//  Created by Marco Marchesi on 3/25/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#ifndef __GloveRuntime__Math__
#define __GloveRuntime__Math__

#include <stdio.h>
#include <math.h>
#include "Serial.h"

namespace Math {
    
    float q1,q2,q3,q4;
    
    /* functions for getting rotation angles */
    /* ROLL */
    float getRoll(){
        return q1 * q2;
    };
    /* PITCH */
    float getPitch(){
        return q1 * q3;
    };
    /* YAW */
    float getYaw(){
        return q1 * q4;
    };
    
    /* MATH UTILS */
    float degreesToRadians(float degree){
        return degree*(M_PI/180);
    };
    
    void update(Serial::glove_packet* data){
        q1 = data->theta;
        q2 = data->rx;
        q3 = data->ry;
        q4 = data->rz;
        
    };
    
}

#endif /* defined(__GloveRuntime__Math__) */
