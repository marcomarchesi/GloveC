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


GTEST_API_ int main(int argc, char **argv) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    
    GestureRecognizer gestureRecognizer;
    gestureRecognizer.init();
//    gestureRecognizer.generate_random_set(6, 3);
    MatrixDouble realTimeData = MatrixDouble(NUM_OF_SAMPLES,6);
    
    gestureRecognizer.train();
//    Serial::glove_packet glove_data;
//    int realTimeDataIndex = 0;
//    
//    // 1) Open a serial port to get data from the glove controller
//    Serial serialPort;
//    serialPort.init();
//    int glove = serialPort.connect();
//    
//    /* Allocate memory for read buffer */
//    char buffer [21];
//    memset (&buffer, '\0', sizeof buffer);
//    int buffer_index = 0;
//    
//    // 2) on connection
//    while (serialPort.isConnected)
//    {
//        int n = (int)read( glove, &buffer[buffer_index], sizeof(buffer)-buffer_index);
//        
//        
//        buffer_index += n;
//        if(buffer_index == 21){
//            glove_data = serialPort.process_packet((Serial::serial_packet*)buffer);
//            buffer_index = 0;
//        }
//        
//        
//        memcpy(&buffer, &glove_data, sizeof(glove_data));
//        
//        if(realTimeDataIndex == NUM_OF_SAMPLES-1){
//            for(int i = 0; i<NUM_OF_SAMPLES-1;++i){
//                realTimeData[i][0] = realTimeData[i+1][0];
//                realTimeData[i][1] = realTimeData[i+1][1];
//                realTimeData[i][2] = realTimeData[i+1][2];
//                realTimeData[i][3] = realTimeData[i+1][3];
//                realTimeData[i][4] = realTimeData[i+1][4];
//                realTimeData[i][5] = realTimeData[i+1][5];
//            }
//        }
//        
//        realTimeData[realTimeDataIndex][0] = glove_data.acc_x;
//        realTimeData[realTimeDataIndex][1] = glove_data.acc_y;
//        realTimeData[realTimeDataIndex][2] = glove_data.acc_z;
//        realTimeData[realTimeDataIndex][3] = glove_data.gyr_x;
//        realTimeData[realTimeDataIndex][4] = glove_data.gyr_y;
//        realTimeData[realTimeDataIndex][5] = glove_data.gyr_z;
//        
//        if(realTimeDataIndex<NUM_OF_SAMPLES-1)
//            realTimeDataIndex++;
//        
//        //            //Save the real time data
//        //            if( !recognizer.saveModelToFile((basic_fstream<char>&)"RealTimeData.txt") ){
//        //                cout << "Failed to save the classifier model!\n";
//        //            }
//        
////         CLASSIFY THE LAST 100 SAMPLES 
//        gestureRecognizer.classify(realTimeData);
//        
//        
//        /**********************/
//        
//        /* C++ */
//        
//        memset (&buffer, '\0', sizeof buffer);
//    }
//    
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
