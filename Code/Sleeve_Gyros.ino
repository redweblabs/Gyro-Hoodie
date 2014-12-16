#include <Wire.h>
#include <Gyro.h>

//Gyro left(0x68);
//Gyro right(0x69);

Gyro left(0x69);
Gyro right(0x68);

int leftLight = 4;
int rightLight = 3;

int readyLight = 13;

void setup(){
  
  Serial.begin(115200);
  Wire.begin();
  
  pinMode(leftLight, OUTPUT);
  pinMode(rightLight, OUTPUT);
  pinMode(readyLight, OUTPUT);
  
  digitalWrite(leftLight, HIGH);
  digitalWrite(rightLight, HIGH);
  
  left.resetWake();
  left.setGains(0,1);
  left.setDLPF(0);
  left.setCalibrationLightPin(10);
  left.offsetCal();
  left.setDLPF(6);
  left.setThresholds(-30, -10, 0);
  
  //-30 -10
  
  //Indicate first gyro is calibrated with a light flash...
  digitalWrite(leftLight, LOW);
  delay(500);
  digitalWrite(leftLight, HIGH);
  
  right.resetWake();
  right.setGains(0,1);
  right.setDLPF(0);
  right.setCalibrationLightPin(11);
  right.offsetCal();
  right.setDLPF(6);
  right.setThresholds(25, -5, 40);
  
  //Indicate second gyro is calibrated with a light flash...
  digitalWrite(rightLight, LOW);
  delay(500);
  digitalWrite(rightLight, HIGH);
  delay(500);
  
  //Indicate calibration is complete with two light flashes
  digitalWrite(leftLight, LOW);
  digitalWrite(rightLight, LOW);
  delay(125);
  digitalWrite(leftLight, HIGH);
  digitalWrite(rightLight, HIGH);
  delay(125);
  digitalWrite(leftLight, LOW);
  digitalWrite(rightLight, LOW);
  delay(125);
  digitalWrite(leftLight, HIGH);
  digitalWrite(rightLight, HIGH);
  
}

int xThreshold = 15;
int yThreshold = 25;
int zThreshold = 40;

long leftTime;
long rightTime;

void loop(){
  
  left.readData();
  right.readData();
  
  left.calculateValues();
  right.calculateValues();

  Serial.print("LEFT >>> ");
  Serial.print(left.angle_x_gyro);
  Serial.print(", ");
  Serial.print(left.angle_y_gyro);
  Serial.print(", ");
  Serial.print(left.angle_z_gyro);
//  Serial.print("\n");
  Serial.print("   RIGHT >>> ");
  Serial.print(right.angle_x_gyro);
  Serial.print(", ");
  Serial.print(right.angle_y_gyro);
  Serial.print(", ");
  Serial.print(right.angle_z_gyro);
  Serial.print("\n");
   
  if(left.angle_y_gyro < left.y_threshold){
     
    if(leftTime == 0){
       leftTime = millis() + 650; 
    }
    
    if(millis() > leftTime){
       digitalWrite(leftLight, HIGH);
       
       if((millis() - leftTime) > 500){
          leftTime = millis() + 650; 
       }
       
    } else {
       digitalWrite(leftLight, LOW); 
    }
    
    
  } else {
     digitalWrite(leftLight, HIGH); 
  }
  
  if(right.angle_x_gyro < right.x_threshold && right.angle_y_gyro < right.y_threshold && right.angle_z_gyro > right.z_threshold){
     
    if(rightTime == 0){
       rightTime = millis() + 650; 
    }
    
    if(millis() > rightTime){
       digitalWrite(rightLight, HIGH);
       
       if((millis() - rightTime) > 500){
          rightTime = millis() + 650; 
       }
       
    } else {
       digitalWrite(rightLight, LOW); 
    }
    
    
  } else {
     digitalWrite(rightLight, HIGH); 
  }
  
  delay(20);
  
}

