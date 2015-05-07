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
    float getRoll(Serial::glove_packet* data){
        return atan(data->acc_y / sqrt(data->acc_x*data->acc_x + data->acc_z*data->acc_z));
    };
    /* PITCH */
    float getPitch(Serial::glove_packet* data){
        return atan(data->acc_x / data->acc_z);
    };
    float getYaw(Serial::glove_packet* data){
       float yaw = 0;
            return yaw;
    };
    /* MATH UTILS */
    float degreesToRadians(float degree){
        return degree*(M_PI/180);
    };
    
    /* IMU PROCESSING */
    void update(float ax,float ay,float az,float gx,float gy,float gz,float mx,float my,float mz){
        float norm;
        float hx, hy, _2bx, _2bz, _8bx, _8bz;
        float s1, s2, s3, s4;
        float qDot1, qDot2, qDot3, qDot4;
        float excludeAccel;
        
        float _2q1mx, _2q1my, _2q1mz, _2q2mx, _4bx, _4bz, _2q1, _2q2, _2q3, _2q4;
        float q1q1, q1q2, q1q3, q1q4, q2q2, q2q3, q2q4, q3q3, q3q4, q4q4, _2q1q3, _2q3q4;
        
        _2q1 = 2.0 * q1;
        _2q2 = 2.0 * q2;
        _2q3 = 2.0 * q3;
        _2q4 = 2.0 * q4;
        _2q1q3 = 2.0 * q1 * q3;
        _2q3q4 = 2.0 * q3 * q4;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q1q4 = q1 * q4;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q2q4 = q2 * q4;
        q3q3 = q3 * q3;
        q3q4 = q3 * q4;
        q4q4 = q4 * q4;
        
        excludeAccel = 0; //Only use gyros and magnetos for updating the filter
        
        // Normalise accelerometer measurement
        norm = sqrt(ax * ax + ay * ay + az * az);
        if (norm > 0.3 && excludeAccel == 0){ //normal larger than the sensor noise floor during freefall
            norm = 1.0 / norm;
            ax *= norm;
            ay *= norm;
            az *= norm;
        }
        else{
            ax = 0;
            ay = 0;
            az = 0;
        }
        
        // Normalise magnetometer measurement
        norm = sqrt(mx * mx + my * my + mz * mz);
        if (norm > 0.0){
            norm = 1.0 / norm;
            mx *= norm;
            my *= norm;
            mz *= norm;
        }
        // else{
        //   break; //something is wrong with the magneto readouts
        // }
        
        // Reference direction of Earth's magnetic field
        _2q1mx = 2 * q1 * mx;
        _2q1my = 2 * q1 * my;
        _2q1mz = 2 * q1 * mz;
        _2q2mx = 2 * q2 * mx;
        
        hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
        hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
        _2bx = sqrt(hx * hx + hy * hy);
        _2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
        _4bx = 2.0 * _2bx;
        _4bz = 2.0 * _2bz;
        _8bx = 2.0 * _4bx;
        _8bz = 2.0 * _4bz;
        
        // Gradient descent algorithm corrective step
        s1 = -_2q3 * (2.0 * q2q4 - _2q1q3 - ax) + _2q2 * (2.0 * q1q2 + _2q3q4 - ay) - _4bz * q3 * (_4bx * (0.5 - q3q3 - q4q4) + _4bz * (q2q4 - q1q3) - mx) + (-_4bx * q4 + _4bz * q2) * (_4bx * (q2q3 - q1q4) + _4bz * (q1q2 + q3q4) - my) + _4bx * q3 * (_4bx * (q1q3 + q2q4) + _4bz * (0.5 - q2q2 - q3q3) - mz);
        s2 = _2q4 * (2.0 * q2q4 - _2q1q3 - ax) + _2q1 * (2.0 * q1q2 + _2q3q4 - ay) - 4.0 * q2 * (1.0 - 2.0 * q2q2 - 2.0 * q3q3 - az) + _4bz * q4 * (_4bx * (0.5 - q3q3 - q4q4) + _4bz * (q2q4 - q1q3) - mx) + (_4bx * q3 + _4bz * q1) * (_4bx * (q2q3 - q1q4) + _4bz * (q1q2 + q3q4) - my) + (_4bx * q4 - _8bz * q2) * (_4bx * (q1q3 + q2q4) + _4bz * (0.5 - q2q2 - q3q3) - mz);
        s3 = -_2q1 * (2.0 * q2q4 - _2q1q3 - ax) + _2q4 * (2.0 * q1q2 + _2q3q4 - ay) - 4.0 * q3 * (1.0 - 2.0 * q2q2 - 2.0 * q3q3 - az) + (-_8bx * q3 - _4bz * q1) * (_4bx * (0.5 - q3q3 - q4q4) + _4bz * (q2q4 - q1q3) - mx) + (_4bx * q2 + _4bz * q4) * (_4bx * (q2q3 - q1q4) + _4bz * (q1q2 + q3q4) - my) + (_4bx * q1 - _8bz * q3) * (_4bx * (q1q3 + q2q4) + _4bz * (0.5 - q2q2 - q3q3) - mz);
        s4 = _2q2 * (2.0 * q2q4 - _2q1q3 - ax) + _2q3 * (2.0 * q1q2 + _2q3q4 - ay) + (-_8bx * q4 + _4bz * q2) * (_4bx * (0.5 - q3q3 - q4q4) + _4bz * (q2q4 - q1q3) - mx) + (-_4bx * q1 + _4bz * q3) * (_4bx * (q2q3 - q1q4) + _4bz * (q1q2 + q3q4) - my) + _4bx * q2 * (_4bx * (q1q3 + q2q4) + _4bz * (0.5 - q2q2 - q3q3) - mz);
        
        norm = sqrt(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);
        if (norm > 0.0){
            norm = 1.0 / norm;  //normalise gradient step
            s1 *= norm;
            s2 *= norm;
            s3 *= norm;
            s4 *= norm;
        }
        // else{
        //    break;
        // }
        
        // Compute rate of change of quaternion
        qDot1 = 0.5 * (-q2 * gx - q3 * gy - q4 * gz) - (1 - ALPHA) * s1;
        qDot2 = 0.5 * (q1 * gx + q3 * gz - q4 * gy) - (1 - ALPHA) * s2;
        qDot3 = 0.5 * (q1 * gy - q2 * gz + q4 * gx) - (1 - ALPHA) * s3;
        qDot4 = 0.5 * (q1 * gz + q2 * gy - q3 * gx) - (1 - ALPHA) * s4;
        
        // Integrate to yield quaternion
        q1 += qDot1 * SAMPLE_TIME;
        q2 += qDot2 * SAMPLE_TIME;
        q3 += qDot3 * SAMPLE_TIME;
        q4 += qDot4 * SAMPLE_TIME;
        
        norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
        if (norm > 0.0){
            norm = 1.0 / norm;  //normalise quaternion
            q1 = q1 * norm;
            q2 = q2 * norm;
            q3 = q3 * norm;
            q4 = q4 * norm;
        }
    };
    
    //
    //float getRoll(){
    //    roll = atan2(2*(q1 * q2 + q3 * q4),1 - 2 * (q2 * q2 + q3 * q3));
    //    return radiansToDegrees(roll);
    //};
    ////
    //float getPitch(){
    //    pitch = asin(2*(q1 * q3 - q4 * q2));
    //    return radiansToDegrees(pitch);
    //};
    ////
    //float getYaw(){
    //    yaw = atan2(2*(q1 * q4 + q2 * q3),1 - 2 * (q3 * q3 + q4 * q4));
    //    return yaw;
    //};

    
    
    
}

#endif /* defined(__GloveRuntime__Math__) */
