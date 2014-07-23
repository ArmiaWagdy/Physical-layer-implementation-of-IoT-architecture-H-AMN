//=====================================================================================
// Name        : ProcessInput.cpp
// Author      : Armia Wagdy
// Version     : 1.5
// Description : This file contains the declaration of the function used in make
//				 processing the input.
//=====================================================================================

#ifndef PROCESSINPUT_H_
#define PROCESSINPUT_H_

/*
 * This function takes a pointer to an array containing arguments taken from terminal
 * and mapping this arguments to some codes and put them in an array and then
 * returns a pointer to that array.
 */

#define NUMBER_OF_ARGUMENTS 	7

unsigned char* process_input(char *args[]);

#endif /* PROCESSINPUT_H_ */
