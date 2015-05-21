//
//  Serial.cpp
//  GloveApp
//
//  Created by Marco Marchesi on 2/12/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#include "Serial.h"

#include <stdio.h>    // Standard input/output definitions
#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>    // File control definitions
#include <errno.h>    // Error number definitions
#include <termios.h>  // POSIX terminal control definitions
#include <string.h>   // String function definitions
#include <iostream>
#include <math.h>

#define BT_DEVICE_PORT  "/dev/cu.Glove-AMP-SPP"
#define USB_DEVICE_PORT "/dev/cu.usbserial-DA00RAK6"
#define START_COMMAND   "\x01\x02\x01\x03"
#define STOP_COMMAND    "\x01\x02\x00\x03"

#define TERM_SPEED      B57600
#define PACKET_SIZE     55
 
using namespace std;



float reverseFloat( const float inFloat )
{
    float retVal;
    Serial::BYTE *floatToConvert = ( Serial::BYTE* ) & inFloat;
    Serial::BYTE *returnFloat = ( Serial::BYTE* ) & retVal;
    
    // swap the bytes into a temporary buffer //VERIFY
    returnFloat[0] = floatToConvert[2];
    returnFloat[1] = floatToConvert[3];
    returnFloat[2] = floatToConvert[0];
    returnFloat[3] = floatToConvert[1];
    
    
    return retVal;
    
//    return inFloat;
}


int Serial::init(){
    
    isConnected = false;

    glove = open(BT_DEVICE_PORT, O_RDWR | O_NOCTTY);
    /* Error Handling */
    if ( glove < 0 )
    {
        cout << "Error " << errno << " opening " << BT_DEVICE_PORT << ": " << strerror (errno) << endl;
        return EXIT_FAILURE;
    }
    
    /* *** Configure Port *** */
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    
    /* Error Handling */
    if ( tcgetattr ( glove, &tty ) != 0 )
    {
        cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
        return EXIT_FAILURE;
    }
    
    /* Set Baud Rate */
    cfsetospeed (&tty, TERM_SPEED);
    cfsetispeed (&tty, TERM_SPEED);
    
    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;        // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;
    tty.c_cflag     &=  ~CRTSCTS;       // no flow control
    tty.c_lflag     =   0;          // no signaling chars, no echo, no canonical processing
    tty.c_oflag     =   0;                  // no remapping, no delays
    tty.c_cc[VMIN]      =   PACKET_SIZE;                  // read doesn't block
    tty.c_cc[VTIME]     =   0;                  // 0.5 seconds read timeout
    
    tty.c_cflag     |=  CREAD;
    tty.c_cflag     |=  CLOCAL;     // turn on READ & ignore ctrl lines
    tty.c_iflag     &=  ~(IXON | IXOFF | IXANY);// turn off s/w flow ctrl
    tty.c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    tty.c_oflag     &=  ~OPOST;              // make raw
    
    /* Flush Port, then applies attributes */
    tcflush( glove, TCIFLUSH );
    
    if ( tcsetattr ( glove, TCSANOW, &tty ) != 0)
    {
        cout << "Glove Connection Error " << errno << " from tcsetattr" << endl;
    }

    return EXIT_SUCCESS;
};


int Serial::connect(){
    
    /* *** WRITE READ COMMAND *** */
    int n_written = (int)write( glove, START_COMMAND, sizeof(START_COMMAND) -1 );
    
    if(!n_written)
    {
        isConnected = false;
        cout << "Error " << errno << " opening " << BT_DEVICE_PORT << ": " << strerror (errno) << endl;
        return EXIT_FAILURE;
    }else{
        isConnected = true;
        cout << "Glove is ready" << endl;
        return glove;
    }

};

int Serial::disconnect(){
    
    /* *** WRITE READ COMMAND *** */
    int n_written = (int)write( glove, STOP_COMMAND, sizeof(STOP_COMMAND) -1 );
    
    if(!n_written)
    {
        isConnected = true;
        cout << "Error " << errno << " closing " << BT_DEVICE_PORT << ": " << strerror (errno) << endl;
        return EXIT_FAILURE;
    }else{
        isConnected = false;
        cout << "Glove has been disconnected" << endl;
        return EXIT_SUCCESS;
    }
    
};

Serial::glove_packet Serial::process_packet(Serial::serial_packet* p) {
    
    Serial::glove_packet packet;
    
    packet.acc_x = reverseFloat(p->acc_x);
    packet.acc_y = reverseFloat(p->acc_y);
    packet.acc_z = reverseFloat(p->acc_z);
    packet.gyr_x = reverseFloat(p->gyr_x);
    packet.gyr_y = reverseFloat(p->gyr_y);
    packet.gyr_z = reverseFloat(p->gyr_z);
    packet.mag_x = reverseFloat(p->mag_x);
    packet.mag_y = reverseFloat(p->mag_y);
    packet.mag_z = reverseFloat(p->mag_z);
    packet.theta = reverseFloat(p->theta);
    packet.rx = reverseFloat(p->rx);
    packet.ry = reverseFloat(p->ry);
    packet.rz = reverseFloat(p->rz);
    
    return packet;
};

