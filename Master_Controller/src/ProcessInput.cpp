//=====================================================================================
// Name        : ProcessInput.cpp
// Author      : Armia Wagdy
// Version     : 1.5
// Description : This file contains the definition of the function used in make
//				 processing the input.
//  			 This function takes a pointer to an array containing arguments taken from
//				 terminal and mapping this arguments to some codes and put them in an array
//  		 	 and then returns a pointer to that array.
//=====================================================================================
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "ProcessInput.h"

#define DEBUG  0

using namespace std;
/*
 * Input arguments from the terminal are like that
 * {XBEE_MAC,pin_number,value,data_type,pin_type,baudrate,Read/Write}
 * This file will work only on those arguments
 * {pin_number,value,data_type,pin_type,baudrate,Read/Write}
 * pin_number: is the number of pin that you want to write to or read from in slave contoller.
 * value:      the value you want to set this pin to.
 * data_type:  the data type of this value whether it boolean or char and so on.
 * pin_type:   Are you want to write on digital or PWM pin or read from analog pin.
 * baudrate:   the speed of your transmission of data measured in bits per second.
 * Read/Write: You want to write data on the slave controller or read from it.
 */

/*********************************************************************************************/

unsigned char* process_input(char *args[]) {
    //Moving the data from the array of pointers called "argv" to "arg"
    const char *arg[NUMBER_OF_ARGUMENTS] = { };
    for (int i = 0; i < NUMBER_OF_ARGUMENTS; i++)
        arg[i] = args[i + 2]; /**/

    //Declaration of some constant data
    const char *Data_types[] = { "INT", "BOOLEAN", "STRING" };
    const char *Pin_types[] = { "DIGITAL", "ANALOG", "PWM" };
    const char *Baudrates[] = { "300", "600", "1200", "2400","4800","9600",
            "14400", "19200", "28800", "38400", "57600","115200" };

    //Declaration of some helpful local variables
    static unsigned char DataFrame[57]; //Max number of bytes = 50, + 7 bytes.
    unsigned char pin_number;
    unsigned char pin_type;
    unsigned char data_type;
    unsigned char baudrate;
    unsigned char length_of_info;
    unsigned char info[50];
    unsigned char length_of_dataFrame;
    unsigned char mode;

    //===================================================================================//
    //Putting the first argument(pin_number) in Data after converting it to integer.
    //Data[0] = pin_number
    pin_number = (unsigned char) atoi(arg[0]);

    //===================================================================================//
    //Mapping the Fourth argument (pin_type) to some hex codes in order to easily send it
    //DIGITAL	-> 0x10
    //ANALOG    -> 0x11
    //PWM 		-> 0x12
    //The Default is PWM
    //DataFrame[1] = pin_type
    for (int i = 0; i < 3; i++) {

        if (!strcmp(arg[3], Pin_types[i]))
        {
            pin_type = i + (unsigned char) 0x10;
            break;
        }
        else
        {
            continue;
        }

    }

    /*********************************************************************************************/

    //Mapping the third argument (data_type) to some hex codes in order to easily send it
    //INT -> 0x20
    //BOOLEAN  -> 0x21
    //STRING -> 0x22
    //The Default is string
    //DataFrame[2] = data_type

    for (int i = 0; i < 3; i++)
    {

        //Check if the entered data type is matched with one of the data types
        //in "Data_types" array
        if (!strcmp(arg[2], Data_types[i]))
        {
            data_type = i + (unsigned char) 0x20;
            break;
        }
        else
        {
            continue;
        }
    }

    /*********************************************************************************************/

    //Mapping the fifth argument (baudrate) to some hex codes in order to easily send it
    //300 -> 0x30
    //600  -> 0x31
    //1200 -> 0x32
    //2400 -> 0x33
    //4800 -> 0x34
    //9600 -> 0x35
    //14400 -> 0x36
    //19200 -> 0x37
    //28800 -> 0x38
    //38400 -> 0x39
    //57600 -> 0x3A
    //115200 -> 0x3B
    //The Default is 115200
    //DataFrame[3] = baudrate

    if (!strcmp(arg[4], "0"))
    {
        baudrate = 0x00;
    }
    else
    {
        for (int i = 0; i < 12; i++)
        {

            //Check if the entered data type is matched with one of the data types
            //in "Data_types" array
            if (!strcmp(arg[4], Baudrates[i]))
            {
                baudrate = i + (unsigned char) 0x30;
                break;
            }

            else
            {
                continue;
            }
        }
    }

    /*********************************************************************************************/

    /* If data type is BOOLEAN, Map HIGH to 255 and LOW to 0
     * If you type BOOLEAN and put a value greater than 128,
     * It will be treated as HIGH */

    //if data_type is BOOLEAN and the value is HIGH
    if (data_type == 0x21 && !strcmp(arg[1], "HIGH"))
    {
        length_of_info = 1;
        info[0] = 255;
    }

    //if data_type is BOOLEAN and the value is LOW
    else if (data_type == 0x21 && !strcmp(arg[1], "LOW"))
    {
        length_of_info = 1;
        info[0] = 0;
    }

    //if data_type is BOOLEAN or INT
    else if (data_type == 0x20 || data_type == 0x21)
    {
        length_of_info = 1;
        info[0] = atoi(arg[1]);
        if (info[0] > 255)	info[0] = 255; // no value greater than 255
    }

    //if data_type is string
    else if (data_type == 0x22)
    {
        const char *ptr = arg[1];
        register unsigned char i;

        for (i = 0; *(ptr + i); i++)
        {
            info[i] = *(ptr + i);
        }

        length_of_info = i;
    }

    /*********************************************************************************************/

    //Mapping the sixth argument
    //if R send ASCII of R which means READ
    //if W send ASCII of W which means WRITE
    //Default is Write

    if (!strcmp(arg[5], "R"))
    {
        mode = 'R';
    }

    else
    {
        mode = 'W';
    }

    /*********************************************************************************************/

    length_of_dataFrame = length_of_info + 5;

    //Making the dataFrame
    DataFrame[0] = 0xFF;
    DataFrame[1] = length_of_dataFrame;
    DataFrame[2] = pin_number;
    DataFrame[3] = pin_type;
    DataFrame[4] = data_type;
    DataFrame[5] = baudrate;
    DataFrame[6] = mode;

    //Integrating the info into the dataFrame
    for (register unsigned char i = 0; i < length_of_info; i++)
    {
        DataFrame[i + 7] = info[i];
    }

    /*********************************************************************************************/

#if DEBUG
    cout << "Length of Frame = "<< (int)length_of_dataFrame << endl;
    cout << "Length of Data  = "<< (int)length_of_dataFrame - 5 << endl;
    cout << "Pin_number      = " << (int)pin_number << endl;
    cout << "Pin_type 	= 0x"<< hex << (int) pin_type << endl;
    cout << "Data_type 	= 0x"<< hex << (int) data_type << endl;
    cout << "Baudrate 	=  0x"<< hex << (int)baudrate << endl;
    cout << "Mode is " << (char)mode << endl;
    for(register unsigned char i = 0; i < length_of_info; i++)
    {
        cout << "DataFrame[" << (int)i << "]=" << (int)info[i] << endl;
    }
    //cout << (int)length_of_info <<endl;
#endif

    /*********************************************************************************************/
    return DataFrame;
}


