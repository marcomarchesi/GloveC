//
//  Serial.h
//  GloveApp
//
//  Created by Marco Marchesi on 2/12/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#ifndef __GloveApp__Serial__
#define __GloveApp__Serial__

#include <stdio.h>    // Standard input/output definitions
#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>    // File control definitions
#include <errno.h>    // Error number definitions
#include <termios.h>  // POSIX terminal control definitions
#include <string.h>   // String function definitions
#include <iostream>
class Serial{
    public:
        struct serial_packet{
            int16_t header;
            float acc_x;
            float acc_y;
            float acc_z;
            float gyr_x;
            float gyr_y;
            float gyr_z;
            float mag_x;
            float mag_y;
            float mag_z;
            float theta;
            float rx;
            float ry;
            float rz;
            int8_t chksum;
        };
        struct glove_packet {
            float acc_x;
            float acc_y;
            float acc_z;
            float gyr_x;
            float gyr_y;
            float gyr_z;
            float mag_x;
            float mag_y;
            float mag_z;
            float theta;
            float rx;
            float ry;
            float rz;
        };
        /* serial setup */
        int init();
        /* connect to acquire glove data */
        int connect();
        /* get complete packet of glove data */
        Serial::glove_packet process_packet(Serial::serial_packet* p);
        /* disconnect glove streaming */
        int disconnect();
        bool isConnected;
    private:
        /* VARIABLES */
        /* glove serial port handler */
        int glove;
        /* connection status */
    
        /* METHODS */

};

#endif /* defined(__GloveApp__Serial__) */
