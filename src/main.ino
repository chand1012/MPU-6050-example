#include <Arduino.h>
#include <Wire.h>

#include "MPU-6050.h"

void setup() {
    MPU6050 mpu(Wire);
    Serial.begin(9600);

    while(true) {
        int tempf = mpu.getTempF();
        int tempc = mpu.getTempC();
        Serial.print("The temperature is: ");
        Serial.print(tempf);
        Serial.println("F");
        Serial.print(tempc);
        Serial.println("C");

        int gyro_x = mpu.getGyroX();
        int gyro_y = mpu.getGyroY();
        int gyro_z = mpu.getGyroZ();

        Serial.println("Gyro Data: X,Y,Z");
        Serial.print(gyro_x);
        Serial.print(',');
        Serial.print(gyro_y);
        Serial.print(',');
        Serial.println(gyro_z);

        int acc_x = mpu.getAccX();
        int acc_y = mpu.getAccY();
        int acc_z = mpu.getAccZ();

        Serial.println("Accel Data: X,Y,Z");
        Serial.print(acc_x);
        Serial.print(',');
        Serial.print(acc_y);
        Serial.print(',');
        Serial.println(acc_z);


        delay(1000);

    }
}

void loop() {}