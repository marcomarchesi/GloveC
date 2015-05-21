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
    
    float q0,q1,q2,q3;
    float roll,pitch,yaw;
    
    /* BIT CHECK */
    bool IsBitSet( unsigned char byte, int index )
    {
        int mask = 1<<index;
        return (byte & mask) != 0;
    }
    
    /* MATH UTILS */
    float degreesToRadians(float degree){
        return degree*(M_PI/180);
    };
    float radiansToDegrees(float angle){
        return angle*(180/M_PI);
    };
    
    
    float getRoll(){
        roll = asin(-2*(q1*q3 - q0*q2));
        return radiansToDegrees(roll);
    };
    float getPitch(){
        pitch = atan2(q0 * q3 + q1 * q2, 0.5 - (q2 * q2 + q3 * q3));
        return radiansToDegrees(pitch);
    };
    float getYaw(){
        yaw = atan2(q0 * q1 + q2 * q3,0.5 - (q1 * q1 + q2 * q2));
        return radiansToDegrees(yaw);
    };
    
    


    
    void update(float theta, float rx, float ry, float rz){
        
        q0 = rx * sin(theta/2);
        q1 = ry * sin(theta/2);
        q2 = rz * sin(theta/2);
        q3 = cos(theta/2);
        
    };
    
    float getTurnGloveDirection(){
        return getYaw();
    };
    
}

#endif /* defined(__GloveRuntime__Math__) */
