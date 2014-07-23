//===============================================================================
// Name        : Slave_Controller.cpp
// Author      : Armia Wagdy
// Copyright   : 1.3
// Description : This code contain the definition of the class members which
//               implements the behaviour of the slave controller.
//================================================================================

#include "Slave_Controller.h"
#include <SoftwareSerial.h>
#include <cmath>

//*****************************************************************************************************************//
void Router :: get_data(unsigned char length)
{

    //Getting data
    length_of_info = length - 5;    //Length of information in the Data Frame
    pin_number     = Serial.read();
    pin_type       = Serial.read();
    data_type      = Serial.read();
    baudrate       = Serial.read();
    mode           = Serial.read();

    for(register unsigned char i = 0 ; i < length_of_info ; i++)
    {
        info[i] = Serial.read();
    }

#if DEBUG
    Serial.write(0xcc);
    Serial.write(length_of_info);
    Serial.write(pin_number);
    Serial.write(pin_type);
    Serial.write(data_type);
    Serial.write(baudrate);
    Serial.write(mode);
    for(register unsigned char i = 0 ; i < length_of_info ; i++)
    {
        Serial.write(info[i]);
    }
#endif

    return;
}

//*****************************************************************************************************************//

void Router :: process_data(void)
{
    if(mode == 'W')
    {
        if(baudrate == 0)
        {
            Set_Pin_To_Value();
            return;
        }

        else
        {
            Send_Serial_Data();
            return;
        }
    }

    else if(mode = 'R')
    {

        if(baudrate == 0)
        {
            Read_Value_On_Pin();
            return;
        }
        else
        {
            Receive_Serial_Data();
            return;
        }

    }

  return;
}


//*****************************************************************************************************************//

void Router :: Set_Pin_To_Value(void)
{
    if(pin_type == DIGITAL)
    {
        if(is_in_digital_pins(pin_number))
        {
            pinMode(pin_number,OUTPUT);
            digitalWrite(pin_number,info[0] > 127);
            send_ack(OK);
        }
        else
        {
            send_ack(FAIL);
        }
    }

    else if(pin_type == PWM || pin_type == ANALOG)
    {
        if(is_in_pwm_pins(pin_number))
        {
            pinMode(pin_number,OUTPUT);
            analogWrite(pin_number,info[0]);
            send_ack(OK);
        }
        else
        {
            send_ack(FAIL);
        }

    }

    return;
}

//*****************************************************************************************************************//

bool Router :: is_in_digital_pins(unsigned char pin_no)
{
#ifdef ARDUINO_UNO
    unsigned char allowable_digital_pins[] = {2,3,4,5,6,7,8,9,10,11,12,13};
#endif
    for(register unsigned char i = 0; i < 12 ; i++)
    {
        if(pin_no == allowable_digital_pins[i])
        {
            return 1;
        }
        else
        {
            continue;
        }
    }

    return 0;
}

//*****************************************************************************************************************//

void Router :: send_ack(unsigned char ack)
{
    Serial.write(RESPONSE);
    Serial.write(ack);
    return;
}

//*****************************************************************************************************************//

bool Router :: is_in_pwm_pins(unsigned char pin_no)
{
#ifdef ARDUINO_UNO
    unsigned char allowable_pwm_pins[] = {3,5,6,7,9,10,11};
#endif
    for(register unsigned char i = 0; i < 7 ; i++)
    {
        if(pin_no == allowable_pwm_pins[i])
        {
            return 1;
        }
        else
        {
            continue;
        }
    }

    return 0;
}

//*****************************************************************************************************************//

void Router :: Send_Serial_Data(void)
{
    if(is_in_digital_pins(pin_number))
    {
        tx = pin_number;
        rx = 255; //unused


        SoftwareSerial device(rx,tx);

        device.begin(map_Baudrate(baudrate));

        //device.listen();        //redundant

        for(register unsigned char i = 0; i < length_of_info; i++)
         {
            device.write(info[i]);
         }
        send_ack(OK);
    }
    else
    {
        send_ack(FAIL);
    }
    return;
}

//****************************************************************************************************************//

unsigned long int Router :: map_Baudrate(unsigned char BaudRate)
{
    switch(BaudRate)
    {
        case (0x30):
            return 300;
        case (0x31):
            return 600;
        case (0x32):
            return 1200;
        case (0x33):
            return 2400;
        case (0x34):
            return 4800;
        case (0x35):
            return 9600;
        case (0x36):
            return 14400;
        case (0x37):
            return 19200;
        case (0x38):
            return 28800;
        case (0x39):
            return 38400;
        case (0x3A):
            return 57600;
        case (0x3B):
            return 115200;
        default:
            return 9600;
    }
}

//****************************************************************************************************************//

void Router :: Read_Value_On_Pin(void)
{
    if(pin_type == DIGITAL)
    {
        if(is_in_digital_pins(pin_number))
        {
            send_data(digitalRead(pin_number));
        }
        else
        {
            send_ack(FAIL);
        }
    }
    else if(pin_type == ANALOG)
    {
        if(is_in_analog_pins(pin_number))
        {
            send_data(map(analogRead(pin_number),0,1023,0,255));
        }
        else
        {
            send_ack(FAIL);
        }
    }

    else if(pin_type == PWM)
    {
        if(is_in_pwm_pins(pin_number))
        {
#ifdef  ARDUINO_UNO
            switch (pin_number)
            {
            /* pin 5 and pin 6 have a PWM signals with frequency 988 Hz
             * Hint: This frequency is obtained by repeated experiments.
             * Which means the cycle takes 1012 microseconds
               So the half-cycle will take 506 microseconds */
            case 5:
            case 6:

                send_data((round(pulseIn(pin_number, HIGH) * 127) / 506));
                break;

            /* The other PWM pins(3,9,10,11) have PWM signals with frequency 497 Hz
             * (This frequency is obtained by repeated experiments)
             * which means the cycle takes 2012 microseconds
             * so the half-cycle will take 1006 microseconds. */
            default:    
                send_data((round(pulseIn(pin_number, HIGH) * 127) / 1006));
            }
#endif
        }
        else
        {
            send_ack(FAIL);
        }
    }
}

//****************************************************************************************************************//

bool Router :: is_in_analog_pins(unsigned char pin_no)
{
#ifdef ARDUINO_UNO
    unsigned char allowable_analog_pins[] = {14,15,16,17,18,19};
#endif
    for(register unsigned char i = 0; i < 6 ; i++)
    {
        if(pin_no == allowable_analog_pins[i])
        {
            return 1;
        }
        else
        {
            continue;
        }
    }

    return 0;
}

//****************************************************************************************************************//

void Router :: send_data(unsigned char data)
{
    Serial.write(DATA);
    Serial.write(data);
    return;
}

//****************************************************************************************************************//

void Router :: Receive_Serial_Data(void)
{

    rx = pin_number;
    tx = 255; //unused

    SoftwareSerial device(rx,tx);

    device.listen();

    while(device.available() < MAX_NUMBER_OF_READ_BYTES);

    number_of_read_bytes = device.available();

    //Serial.write(number_of_read_bytes);

    if(number_of_read_bytes > MAX_NUMBER_OF_READ_BYTES)
    {
        number_of_read_bytes = MAX_NUMBER_OF_READ_BYTES;
    }

    for(register int i = 0; i < number_of_read_bytes; i++)
    {
        read_bytes[i] = device.read();
    }


    delay(1000);

    while(device.available() > 0)
    {
        device.read();
    }
    device.read();

    for(register int i = 0; i < number_of_read_bytes; i++)
    {
        //Serial.write(read_bytes[i]);
    }
    Serial.write(DATA);


}

//****************************************************************************************************************//
