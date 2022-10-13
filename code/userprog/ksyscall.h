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

#define TAB ((char)9)
#define ENDLINE ((char)13)
#define SPACE ((char)' ')

// check char ch is Blank
char isBlank(char ch)
{
	return ch == TAB || ch == ENDLINE || ch == SPACE;
}

void ReadToBlank()
{
	// Reset buffer
	memset(numberBuffer, 0, sizeof(numberBuffer));
	int n = 0;
	char c;
	do
	{
		c = kernel->synchConsoleIn->GetChar();
		if (c == EOF ||c =='\n')
		{
			DEBUG(dbgSys, "End of file");
			return;
		}
		if (isBlank(c))
		{
			DEBUG(dbgSys, "Unexpected white-space");
			return;
		}
		numberBuffer[n] = c;
		n++;
		if (n > MAX_NUM_LENGTH)
		{
			DEBUG(dbgSys, "Number is longger than expection.");
			return;
		}
	} while (true);
}

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

int SysReadNum()
{
	ReadToBlank();

	int len = strlen(numberBuffer);
	// Read nothing -> return 0
	if (len == 0)
		return 0;

	bool negative = (numberBuffer[0] == '-');
	int zeros = 0;
	bool zeros_fisrt = true;
	int num = 0;
	// if negative, number begin index 1
	// else number begin index 0
	for (int i = negative; i < len; ++i)
	{
		char c = numberBuffer[i];
		// check first characters is zero
		if (c == '0' && zeros_fisrt)
			++zeros;
		else
			zeros_fisrt = false;
		if (c < '0' || c > '9')
		{
			DEBUG(dbgSys, "Expected number but " << numberBuffer << " found");
			return 0;
		}
		int init = c - '0';
		// check num will greater than INT_MAX or less than INT_MIN
		if (num < 214748364)
		{
			num = num * 10 + init;
		}
		else if (num == 214748364)
		{
			// if num is negative, init must less than 9
			// if num is positive, init must less than 8
			if (10 + negative - init >= 3)
			{
				num = num * 10 + init;
			}
			// if greater return 0.
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	if (negative)
	{
		num = -num;
	}
	return num;
}


void SysPrintNum(int num) {
    if (num == 0) return kernel->synchConsoleOut->PutChar('0');

    if (num == INT_MIN) {
        kernel->synchConsoleOut->PutChar('-');
        for (int i = 0; i < 10; ++i)
            kernel->synchConsoleOut->PutChar("2147483648"[i]);
        return;
    }

	if (num < INT_MIN || num > INT_MAX) {
    return kernel->synchConsoleOut->PutChar('0');
  	} 

    if (num < 0) {
        kernel->synchConsoleOut->PutChar('-');
        num = -num;
    }
    int n = 0;
    while (num) {
        numberBuffer[n++] = num % 10;
        num /= 10;
    }
    for (int i = n - 1; i >= 0; --i)
        kernel->synchConsoleOut->PutChar(numberBuffer[i] + '0');
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


void SysPrintChar(char character)
{
	// call fuction PutChar() to print char
	kernel->synchConsoleOut->PutChar(character);
	return;
}

int SysRandomNum()
{
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
			// call fuction print string to print message
			kernel->synchConsoleOut->PrintStr(Msg, strlen(Msg));
		}
		// arr not temp :))
		else
		{
			char Msg[] = "Enter string: \0";
			kernel->synchConsoleOut->PrintStr(Msg);

			// get userStringMaxLength by Readstring fuction
			userStringMaxLength = kernel->synchConsoleIn->ReadStr(buffer, maxLength);

			// char end string
			buffer[userStringMaxLength] = '\0';

			// check temp string input
			if (userStringMaxLength == 0)
			{
				char Msg[] = "Your input is temp string! \0";
				// call fuction print string to print message
				kernel->synchConsoleOut->PrintStr(Msg, strlen(Msg));
			}
			else
			{
				// get data of buff arr
				for (int i = 0; i <= userStringMaxLength; i++)
				{
					// coppy data of user buffer ( User Space to System Space)
					kernel->machine->WriteMem(userBufferAddress + i, 1, (int)buffer[i]);
				}
			}
		}
	}
	// if length <=0 set length to 0
	else
	{
		// Set Max length bytes of buff to 0.
		memset(buffer, 0, maxLength);
	}

	// free space buff arr
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

	for (int i = 0; i < MAX_STRING_LENGTH; i++)
	{
		// coppy data of user cha (System Space to User Space)
		kernel->machine->ReadMem(userBufferAddress + i, 1, &charAddr);

		// print char element of string
		kernel->synchConsoleOut->PutChar(char(charAddr));

		// end string
		if (char(charAddr) == '\0' || char(charAddr) == EOF)
		{
			break;
		}
	}
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
