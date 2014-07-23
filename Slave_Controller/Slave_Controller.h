//===============================================================================
// Name        : Slave_Controller.h
// Author      : Armia Wagdy
// Copyright   : 1.3
// Description : This header file contains the declaration of the class members which
//               implements the behaviour of the slave controller.
//================================================================================


#include <Arduino.h>
#include <SoftwareSerial.h>

#define  ARDUINO_UNO    1           //Iam using ARDUINO_UNO
#define  MAX_NO_BYTES   50          //Maximum number of data
#define  RESPONSE       0xAA        //Response indicator
#define  DATA           0xDD        //Data indicator
#define  OK             0xFF        //If OK send 0xFF
#define  FAIL           0x00        //If FAIL send 0x00
#define  DIGITAL	    0x10
#define  ANALOG         0x11
#define  PWM 		    0x12
#define  INT            0x20
#define  BOOLEAN        0x21
#define  STRING         0x22
#define  MAX_NUMBER_OF_READ_BYTES   5
#define  DEBUG          0

//******************************************************************************//

class Router
    {
        public:
            void get_data(unsigned char);
            void process_data(void);


        private:
            unsigned char length_of_dataFrame;
            unsigned char length_of_info;
            unsigned char pin_number;
            unsigned char pin_type;
            unsigned char data_type;
            unsigned char baudrate;
            unsigned char mode;
            unsigned char info[50];
            unsigned char rx;
            unsigned char tx;
            unsigned char number_of_read_bytes;
            unsigned char read_bytes[5];
            void send_ack(unsigned char);
            void send_data(unsigned char);
            void Set_Pin_To_Value (void);
            void Read_Value_On_Pin(void);
            void Send_Serial_Data(void);
            void Receive_Serial_Data(void);
            bool is_in_analog_pins(unsigned char);
            bool is_in_pwm_pins(unsigned char);
            bool is_in_digital_pins(unsigned char);
            unsigned long int map_Baudrate(unsigned char BaudRate);
    };

//******************************************************************************//
