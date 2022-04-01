/* Arduino firmvare for AAS project (Copterhack2022) v 1.1 */

#include <Wire.h>
#include "ClosedCube_HDC1080.h"
#include "DFRobot_CCS811.h"
#include <MICS6814.h>
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>


/* MICS6814 pins */
#define PIN_CO  A2
#define PIN_NO2 A3
#define PIN_NH3 A4

/* hdc1080 I2C address */
#define HDC1080_ADDR 0x40
/* led pin */
#define LED_PIN 13

#define DATA_LEN 7

MICS6814 gas(PIN_CO, PIN_NO2, PIN_NH3);
ClosedCube_HDC1080 hdc1080;
DFRobot_CCS811 CCS811;

bool led_state = false;

ros::NodeHandle nh;
std_msgs::Float32MultiArray Air_sens;
ros::Publisher msg("Air_sens", &Air_sens);


void setup() {
  CCS811.begin();
  hdc1080.begin(HDC1080_ADDR);

  /* calibrate  MICS6814 (about 5 sec)*/
  gas.calibrate();

  /* init node */
  nh.initNode();
  nh.advertise(msg);
  
  pinMode(LED_PIN, OUTPUT);
  
  Air_sens.data = (float*)malloc(sizeof(float) * DATA_LEN);
  Air_sens.data_length = DATA_LEN;
}

void loop() {
  
  Air_sens.data[0] = hdc1080.readTemperature();
  Air_sens.data[1] = hdc1080.readHumidity();

  /* get data from CCS811*/
  if(CCS811.checkDataReady() == true){
        Air_sens.data[3] = CCS811.getCO2PPM();
        Air_sens.data[6] = CCS811.getTVOCPPB();
  }
  CCS811.writeBaseLine(0x447B);
 
  Air_sens.data[4] = gas.measure(NH3);
  Air_sens.data[2] = gas.measure(CO);
  Air_sens.data[5] = gas.measure(NO2);

  /* blink */
  led_state = !led_state;
  digitalWrite(LED_PIN, led_state);

  /* publish data */
  msg.publish( &Air_sens );
  nh.spinOnce();
}
