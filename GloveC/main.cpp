//
//  main.cpp
//  GloveC
//
//  Created by Marco Marchesi on 4/13/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#include <iostream>
#include "gtest/gtest.h"

#include "Serial.h"
#include "Math.h"
#include "GestureRecognizer.h"

#define NUM_OF_SAMPLES 125
#define MEAN_X 0
#define MEAN_Y 0
#define MEAN_Z 0
#define MEAN_GX 0
#define MEAN_GY 0
#define MEAN_GZ 0
#define STD_X 8
#define STD_Y 8
#define STD_Z 8
#define STD_GX 1000
#define STD_GY 1000
#define STD_GZ 1000

// range of accelerometer is (-16,16)
// range of gyroscope is (-2000,2000)


int main(int argc, char **argv) {
    // insert code here...
    std::cout << "Hello, Glove!\n";
    
    
    GestureRecognizer gestureRecognizer;
    VectorDouble realTimeData(6);
    
    gestureRecognizer.init();
//    gestureRecognizer.train();
    Serial::glove_packet glove_data;

    
    // 1) Open a serial port to get data from the glove controller
    Serial serialPort;
    serialPort.init();
    int glove = serialPort.connect();
    
    /* Allocate memory for read buffer */
    char buffer [21];
    memset (&buffer, '\0', sizeof buffer);
    int buffer_index = 0;
    
    // 2) on connection
    while (serialPort.isConnected)
    {
        int n = (int)read( glove, &buffer[buffer_index], sizeof(buffer)-buffer_index);
        
        
        buffer_index += n;
        if(buffer_index == 21){
            glove_data = serialPort.process_packet((Serial::serial_packet*)buffer);
            buffer_index = 0;
        }
        
        
        memcpy(&buffer, &glove_data, sizeof(glove_data));
        
        realTimeData[0] = (glove_data.acc_x - MEAN_X)/STD_X;
        realTimeData[1] = (glove_data.acc_y - MEAN_Y)/STD_Y;
        realTimeData[2] = (glove_data.acc_z - MEAN_Z)/STD_Z;
        realTimeData[3] = (glove_data.gyr_x - MEAN_GX)/STD_GX;
        realTimeData[4] = (glove_data.gyr_y - MEAN_GY)/STD_GY;
        realTimeData[5] = (glove_data.gyr_z - MEAN_GZ)/STD_GZ;
        
        
//         CLASSIFY THE LAST 100 SAMPLES 
        gestureRecognizer.classify(realTimeData);
        
        
        /**********************/
        
        /* C++ */
        
        memset (&buffer, '\0', sizeof buffer);
    }
    
//    testing::InitGoogleTest(&argc, argv);
    return 0;
}
