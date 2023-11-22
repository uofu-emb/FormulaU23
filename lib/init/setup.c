// init/setup.c

#include <stdio.h>

void PID_SetMode(PID* pid, int mode) {
    // Implement this function to set the mode of your PID
}

void VESC_setSerialPort(VESC* vesc, int port) {
    // Implement this function to set the serial port of your VESC
}

void setup() {
    PID_SetMode(&myPID1, AUTOMATIC);
    PID_SetMode(&myPID2, AUTOMATIC);

    // Initialize your serial ports here
    int Serial1 = 115200;
    int Serial2 = 115200;
    int Serial = 115200;

    VESC_setSerialPort(&vesc1, Serial1);
    VESC_setSerialPort(&vesc2, Serial2);

    // Print your header here
    printf("t,StVin,ThVin,TrlOut,TrrOut,Trl,Trr,deltaTorque,Nrr,Nrl,YawRate,Vy,\n");
}