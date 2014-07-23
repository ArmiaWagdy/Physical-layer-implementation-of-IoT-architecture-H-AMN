#include "Slave_Controller.h"
#include <SoftwareSerial.h>

Router Slave;
unsigned char start_delimeter;
unsigned char length_of_dataFrame;

void setup()
  {  
    Serial.begin(9600);
    pinMode(13,OUTPUT);
  }

void loop()
{
   if(Serial.available() > 2)
    {
      start_delimeter = Serial.read();
      if(start_delimeter == 0xFF)
      {
        length_of_dataFrame = Serial.read();
        
        //wait for Data Frame
        while(Serial.available() != length_of_dataFrame);    
        Slave.get_data(length_of_dataFrame);
        Slave.process_data();
      }
    }
}
