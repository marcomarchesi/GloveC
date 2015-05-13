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

#define SIZE 4096

// range of accelerometer is (-16,16) and standard deviation approximated to range/4 = 32/4
// range of gyroscope is (-2000,2000) and standard deviation approximated to range/4 = 4000/4


int main(int argc, char **argv) {
    // insert code here...
    std::cout << "Hello, Glove!\n";
    bool isStarted = true;
    
    
    GestureRecognizer gestureRecognizer;
    VectorDouble realTimeData(6);
    
    gestureRecognizer.init();
//    gestureRecognizer.train();
    Serial::glove_packet glove_data;

    
    // 1) Open a serial port to get data from the glove controller
    Serial serialPort;
    serialPort.init();
    /* Allocate memory for read buffer */
    char buffer [SIZE];
    int checker = 0;
    char check_buffer = '\0';
    memset (&buffer, '\0', sizeof buffer);
    int glove = serialPort.connect();
    int buffer_index = 0;
    
    // 2) on connection
    while (serialPort.isConnected)
    {
        while(isStarted){
            int c = (int)read(glove,&check_buffer,sizeof(char));
            int end = (int)check_buffer;
            cout << "end is " << end << endl;
            if(end == 3)
                break;
        }
        isStarted = false;
        int n = (int)read( glove, &buffer[buffer_index], sizeof(buffer) - buffer_index);
//        
//        printf("%i %i %i %i %i %i %i\n",(int)buffer[0],(int)buffer[1],(int)buffer[2],(int)buffer[3],(int)buffer[4],(int)buffer[5], buffer[54]);
        
        
    
//        printf ("float is %02x %02x %02x %02x\n",buffer[2],buffer[3],buffer[4],buffer[5]);
        
//        buffer_index += n;
////        if(buffer_index == SIZE){
        
        glove_data = serialPort.process_packet((Serial::serial_packet*)buffer);
        buffer_index = 0;
//        }
        
        
//        memcpy(&buffer, &glove_data, sizeof(glove_data));
        
        

//        
            Math::update(glove_data.theta,glove_data.rx,glove_data.ry,glove_data.rz);
            cout << Math::getYaw() << endl;
//
//        realTimeData[0] = (glove_data.acc_x - MEAN_X)/STD_X;
//        realTimeData[1] = (glove_data.acc_y - MEAN_Y)/STD_Y;
//        realTimeData[2] = (glove_data.acc_z - MEAN_Z)/STD_Z;
//        realTimeData[3] = (glove_data.gyr_x - MEAN_GX)/STD_GX;
//        realTimeData[4] = (glove_data.gyr_y - MEAN_GY)/STD_GY;
//        realTimeData[5] = (glove_data.gyr_z - MEAN_GZ)/STD_GZ;
        

        
//         CLASSIFY THE LAST 100 SAMPLES 
//        gestureRecognizer.classify(realTimeData);
        
        
        /**********************/
        
        /* C++ */
        
        memset (&buffer, '\0', sizeof buffer);
        
    }
    
//    testing::InitGoogleTest(&argc, argv);
    return 0;
}
