/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "synchconsole.h"

// for using INT_MAX and INT_MIN to check case: overflow integer
#include <bits/stdc++.h> 
// for using the random number function
#include <stdlib.h>
// define
/* maximum length of an interger (included the minus sign) */
#define MAX_NUM_LENGTH 11
/* A buffer to read and write number */
char numberBuffer[MAX_NUM_LENGTH + 2];

// Halt function
void SysHalt()
{
  kernel->interrupt->Halt();
}

// Add function
int SysAdd(int op1, int op2)
{
  return op1 + op2;
}


char SysReadChar() 
{ 
  char Char = '\0', temp = '\0';
  do
  {
	// coppy value of getchar() to Char
    temp = kernel->synchConsoleIn->GetChar();
    if (Char == '\0')
    {
      Char = temp;
    }
  } while (temp != '\0' && temp != '\n' && temp != EOF);
  return Char;
  // same return kernel->synchConsoleIn->GetChar()
}

void SysPrintChar(char character) {
  // call fuction PutChar() to print char
  kernel->synchConsoleOut->PutChar(character);
  return;
}

int SysRandomNum() {
  // get random number by libary #include <bits/stdc++.h> 
  return rand();
}

/**
 * @brief Read String
 *
 * @param serBufferAddress addess of user string
 * @param length max length of user string
 * @return void
 */

void SysReadString(int userBufferAddress, int maxLength)
{
	char *buffer = NULL;
	// return lenght string value
	int userStringMaxLength = 0;
	if (maxLength > 0) 
	{
		buffer = new char[maxLength];
		if (buffer == NULL) 
		{
			char Msg[] = "Not enough space to store or user entered a null string.\n\0";
			//call fuction print string to print message
			kernel->synchConsoleOut->PrintStr(Msg,strlen(Msg)); 
		}
    	// arr not temp :))
		else
		{
			char Msg[] = "Enter string: \0";
			kernel->synchConsoleOut->PrintStr(Msg);

			// get userStringMaxLength by Readstring fuction
			userStringMaxLength = kernel->synchConsoleIn->ReadStr(buffer,maxLength); 

			// char end string
			buffer[userStringMaxLength] = '\0';

			//check temp string input	
			if(userStringMaxLength == 0){
				char Msg[] = "Your input is temp string! \0";
				//call fuction print string to print message
				kernel->synchConsoleOut->PrintStr(Msg,strlen(Msg)); 
			} 
			else
			{
				// get data of buff arr
				for (int i = 0; i <= userStringMaxLength ; i++) 
				{
					// coppy data of user buffer ( User Space to System Space)
					kernel->machine->WriteMem(userBufferAddress+i,1,(int)buffer[i]); 
				}
			}

		}
	}
	// if length <=0 set length to 0 
	else
	{
		//Set Max length bytes of buff to 0.
		memset(buffer, 0, maxLength);
	}

	//free space buff arr
	delete[] buffer;

  return;
}


/**
 * @brief Print String
 *
 * @param serBufferAddress addess of user string
 * @param length max length of user string
 * @return void
 */

void SysPrintString(int userBufferAddress)
{
   	int charAddr = 0;
	int MAX_STRING_LENGTH = 256;

	for(int i=0; i < MAX_STRING_LENGTH; i++)
	{
		// coppy data of user cha (System Space to User Space)
		kernel->machine->ReadMem(userBufferAddress+i,1,&charAddr); 

		// print char element of string
		kernel->synchConsoleOut->PutChar(char(charAddr));

		//end string
		if(char(charAddr) == '\0' || char(charAddr) == EOF)
		{
			break;
		}
	}
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
