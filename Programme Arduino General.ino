#include <Wire.h>
#include <Adafruit_NeoPixel.h>
//#include <SoftwareSerial.h>
#include <marbles.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(343, 6, NEO_GRB + NEO_KHZ800);
int Nled = 3; int sizeLed = 100;
LedArray leds(Nled, sizeLed);
//SoftwareSerial bluet(15, 14);
char colors[3] = {'R','G','B'};
//int rotation = 0;
int gyro_x, gyro_y, gyro_z;
long acc_x, acc_y, acc_z, acc_total_vector;
int temperature;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
long loop_timer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
//  bluet.begin(9600);
  strip.begin();
  strip.show(); // Initialise toute les led à 'off'
  Wire.begin();           
  setup_mpu_6050_registers(); 
  loop_timer = micros();
}

void loop() {
  if(char(bluet.read())=='n') {
    Nled = bluet.parseInt();
    LedArray leds(Nled, sizeLed);
  }
  if(char(bluet.read())=='s'){
    sizeLed = bluet.parseInt();
    LedArray leds(Nled, sizeLed);
  }
  rotation = (rotation+10)%360;
  Serial.println("test");
  read_mpu_6050_data();
//  leds.update1(acc_x/100, acc_y/100, -acc_z/100);
  Serial.print(acc_x/100); Serial.print("  ");
  Serial.print(acc_y/100); Serial.print("  ");
  Serial.println(-acc_z/100); 
//  leds.update1(cos(rotation* 1000 / 57296)*30, sin(rotation* 1000 / 57296)*30);
  for(int x=0;x<7;x++){
    for(int y=0;y<7;y++){
//      for(int z=0;z<7;z++){
        int index = positionToIndex(x,y,0);
//        Serial.println(index);
        int intensiteR = ((int)(leds.getValue(x,y,0)*100));
        int intensiteG = ((int)(leds.getValue(x,y,1)*100));
        int intensiteB = ((int)(leds.getValue(x,y,2)*100));
        strip.setPixelColor(index, intensiteR, intensiteG, intensiteB);
      }
    }
//  }
  strip.show();
  delay(50);
  while(micros() - loop_timer < 4000);
  loop_timer = micros();
}

int positionToIndex(int x, int y, int z) {
  int index = z*49;
  if (z%2 == 0) {
    index += y*7;
    if (y%2 == 0) {
      index +=6-x;
    }
    else {
      index += x;
    }
  }
  else {
    index += 48 - (y*7);
    if (y%2 == 0) {
      index += 6-x;
    }
    else {
      index += x;
    }
  }
  return index;
}

void read_mpu_6050_data(){    
  Wire.beginTransmission(0x68);    
  Wire.write(0x3B);     
  Wire.endTransmission();     
  Wire.requestFrom(0x68,14); 
  while(Wire.available() < 14);
  acc_x = Wire.read()<<8|Wire.read();  
  acc_y = Wire.read()<<8|Wire.read();       
  acc_z = Wire.read()<<8|Wire.read();      
  temperature = Wire.read()<<8|Wire.read();  
  gyro_x = Wire.read()<<8|Wire.read();  
  gyro_y = Wire.read()<<8|Wire.read();     
  gyro_z = Wire.read()<<8|Wire.read();   
//  if(acc_x/100>100) acc_x =0;
//  if(acc_y/100>100) acc_y =0;
//  if(acc_z/100>100) acc_z =0;            
}

void setup_mpu_6050_registers(){
  //Activate the MPU-6050
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x6B);                                                    //Send the requested starting register
  Wire.write(0x00);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  //Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1C);                                                    //Send the requested starting register
  Wire.write(0x10);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1B);                                                    //Send the requested starting register
  Wire.write(0x08);                                                    //Set the requested starting register
  Wire.endTransmission();  
  //End the transmission
}
