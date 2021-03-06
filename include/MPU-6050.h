#pragma once

#include <Arduino.h>
#include <Wire.h>

class MPU6050 {
    private:
        TwoWire& Wire;
        const int address = 0x68;
        int AcX, AcY, AcZ; // accelerometer data
        int Tmp; // temperature data
        int GyX, GyY, GyZ; // gyro data
        void getData();
        // gyroscope corrections
        int GyXcal = 480;
        int GyYcal = 170;
        int GyZcal = 210;

        // accelerometer corrections
        int AcXcal = -950;
        int AcYcal = -300;
        int AcZcal = 0;

        // temp correction
        int tcal = -16000;

    public:
        MPU6050(TwoWire& Wire);
        int getAccX();
        int getAccY();
        int getAccZ();
        int getGyroX();
        int getGyroY();
        int getGyroZ();
        int getTempC();
        int getTempF();
        void calibrateTempOffset(float currentTempF);
        void getRawGyro(int& X, int& Y, int& Z);
        void getRawAcc(int& X, int& Y, int& Z);
        String outputCSV();
};

MPU6050::MPU6050(TwoWire& Wire) : Wire(Wire) {
    Wire.beginTransmission(address);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
}

void MPU6050::getData() {
    Wire.beginTransmission(address); //begin transmission to I2C slave device
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false); //restarts transmission to I2C slave device
    Wire.requestFrom(address,14,true); //request 14 registers in total  
    //read accelerometer data
    AcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) 0x3C (ACCEL_XOUT_L)  
    AcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) 0x3E (ACCEL_YOUT_L) 
    AcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) 0x40 (ACCEL_ZOUT_L)
  
    //read temperature data 
    Tmp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) 0x42 (TEMP_OUT_L) 
  
    //read gyroscope data
    GyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) 0x48 (GYRO_ZOUT_L) 
}

void MPU6050::getRawAcc(int& X, int& Y, int& Z) {
    getData();
    X = AcX;
    Y = AcY;
    Z = AcZ;
}

int MPU6050::getAccX() {
    getData();
    return AcX + AcXcal;
} 

int MPU6050::getAccY() {
    getData();
    return AcY + AcYcal;
}

int MPU6050::getAccZ() {
    getData();
    return AcZ + AcZcal;
}

void MPU6050::getRawGyro(int& X, int& Y, int& Z) {
    getData();
    X = GyX;
    Y = GyY;
    Z = GyZ;
}

int MPU6050::getGyroX() {
    getData();
    return GyX + GyXcal;
}

int MPU6050::getGyroY() {
    getData();
    return GyY + GyYcal;
}

int MPU6050::getGyroZ() {
    getData();
    return GyZ + GyZcal;
}

void MPU6050::calibrateTempOffset(float currentTempF) {
    getData();
    float tx = (currentTempF - 36.53) * 340;
    tcal = tx - Tmp;
}

int MPU6050::getTempC() {
    getData();
    int tx = Tmp + tcal;
    return tx/340 + 36.53;
}

int MPU6050::getTempF() {
    getData();
    return (getTempC() * 9/5) + 32;
}

String MPU6050::outputCSV() {
    String output = "acc,";
    output += String(getAccX()) + ",";
    output += String(getAccY()) + ",";
    output += String(getAccZ()) + ",";
    output += "gyro," + String(getGyroX()) + ",";
    output += String(getGyroY()) + ",";
    output += String(getGyroZ()) + ",";
    return output;
}