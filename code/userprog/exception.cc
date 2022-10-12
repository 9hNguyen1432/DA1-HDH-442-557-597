// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#include "synchconsole.h"
// #define ConsoleInput 0
// #define ConsoleOutput 1
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------
// for using the string object
#define MAX_READ_STRING_LENGTH 255


/**
 * @brief Increase Program counter of Systems 4 byte to continue loading command
 */
void IncreasePC() {
    /* set previous programm counter (debugging only)
     * similar to: registers[PrevPCReg] = registers[PCReg];*/
    kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

    /* set programm counter to next instruction
     * similar to: registers[PCReg] = registers[NextPCReg]*/
    kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(NextPCReg));

    /* set next programm counter for brach execution
     * similar to: registers[NextPCReg] = pcAfter;*/
    kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(NextPCReg) + 4);
}

/**
 * @brief Convert user string to system string
 * @param addr addess of user string
 * @param limitSize set max length of string to convert
 * @return char*
 */

char* UserToKernel(int addr, int limitSize)
{
	int _char;
	char* kernelBuff = NULL;
	kernelBuff = new char[limitSize + 1]; 

	if (kernelBuff == NULL)
		return kernelBuff;
		
	memset(kernelBuff, 0, limitSize + 1);
	
	int i; 
	for (i = 0; i < limitSize ;i++)
	{
		kernel->machine->ReadMem(addr + i, 1, &_char);
		kernelBuff[i] = (char)_char;
		if (_char == 0)
			break;
	}
	return kernelBuff;
}

/**
 * @brief Convert system string to user string
 *
 * @param buffer string to convert
 * @param addr addess of user string
 * @param limitSize set max length of string to convert
 * @return int
 */
int KerneltoUser(int addr, int limitSize, char* buffer)
{
	if (limitSize < 0) return -1;
	if (limitSize == 0)return limitSize;
	int i = 0;
	int _char = 0;
	do{
		_char = (int)buffer[i];
		kernel->machine->WriteMem(addr + i, 1, _char);
		i++;
	} while (i < limitSize && _char != 0 && _char != '\0');
	return i;
}

// handel for SC_Add
void Handel_SC_Add(){
	DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
	/* Process SysAdd Systemcall*/
	int result;
	result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
				/* int op2 */(int)kernel->machine->ReadRegister(5));
	DEBUG(dbgSys, "Add returning with " << result << "\n");
	kernel->synchConsoleOut->PutChar('a');
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);
}

//Handel_SC_ReaChar
void Handel_SC_ReadChar()
{
		char* buffer = new char[255];
		int userStringMaxLength = kernel->synchConsoleIn->ReadStr(buffer,256);
		//Return the pos of '\0' suppose to be
		buffer[userStringMaxLength] = '\0';

		if(userStringMaxLength > 1) //length of char > 1
		{
			printf("Length of char > 1, Please retype!");
			DEBUG('a', "\nERROR: Length of char > 1, Please retype!!");
			kernel->machine->WriteRegister(2, 0);
		}
		else if(userStringMaxLength == 0) //char is temp
		{
			printf("Character is temp!");
			DEBUG('a', "\nERROR: Character is temp!");
			kernel->machine->WriteRegister(2, 0);
		}
		else
		{
			// cung co the goi ksyscall SysReadChar() nhung o day em lay luon gia tri check de 
			// ghi data luon :v 
			//call ksyscall fuction
			// char character = SysReadChar();

			char character = buffer[0];
			// Write data to Register(2)
			kernel->machine->WriteRegister(2, (int)character);
		}
	
}


void Handel_SC_PrintChar(){
	// Create a variable to store data that is on Register(4)
	char character = (char)kernel->machine->ReadRegister(4);
	//call ksyscall fuction
    SysPrintChar(character);
}

void Handel_SC_RandomNum()
{
	//call ksyscall fuction
	int randomnum = SysRandomNum();
	// Write data to Register(2)
    kernel->machine->WriteRegister(2, randomnum);
}

void Handel_SC_ReadString()
{
	// Create a variable to store user Buffer Address that is on Register(4)
	// Create a variable to store Length of string that is on Register(4)
	int userBufferAddress = kernel->machine->ReadRegister(4);
	int stringLength = kernel->machine->ReadRegister(5);
	//call ksyscall fuction
	SysReadString(userBufferAddress,stringLength);
}

void Handel_SC_PrintString(){
	// Create a variable to store user Buffer Address that is on Register(4)
	int userBufferAddress = kernel->machine->ReadRegister(4);
	//call ksyscall fuction
	SysPrintString(userBufferAddress);
}

void ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");
	switch (which) {
		case NoException:  
		 	// return control to kernel
             kernel->interrupt->setStatus(SystemMode);
             DEBUG(dbgSys, "Switch to system mode\n");
             break;
        case PageFaultException:
		 	DEBUG('a', "\n No valid translation found");
		 	printf("\n\n No valid translation found");
		 	SysHalt();
            ASSERTNOTREACHED();
		 	break;
        case ReadOnlyException:
		 	DEBUG('a', "\n Write attempted to page marked read-only");
		 	printf("\n\n Write attempted to page marked read-only");
		 	SysHalt();
            ASSERTNOTREACHED();
		 	break;
        case BusErrorException:
			DEBUG('a', "\n Translation resulted invalid physical address");
		 	printf("\n\n Translation resulted invalid physical address");
		 	SysHalt();
            ASSERTNOTREACHED();
		 	break;
        case AddressErrorException:
		 	DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
		 	printf("\n\n Unaligned reference or one that was beyond the end of the address space");
		 	SysHalt();
            ASSERTNOTREACHED();
		 	break;
        case OverflowException:
		 	DEBUG('a', "\nInteger overflow in add or sub.");
		 	printf("\n\n Integer overflow in add or sub.");
		 	SysHalt();
            ASSERTNOTREACHED();
		 	break;
        case IllegalInstrException:
		 	DEBUG('a', "\n Unimplemented or reserved instr.");
		 	printf("\n\n Unimplemented or reserved instr.");
		 	SysHalt();
            ASSERTNOTREACHED();
		 	break;
        case NumExceptionTypes:
            cerr << "Error " << which << " occurs\n";
            SysHalt();
            ASSERTNOTREACHED();
			break;
			
    	case SyscallException:
      		switch(type) {
      			case SC_Halt: {
					DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
					SysHalt();
					ASSERTNOTREACHED();
					break;
				}

      			case SC_Add: {
					Handel_SC_Add();
					IncreasePC();
					return;
	   				ASSERTNOTREACHED();
					break;
				}

				case SC_ReadChar: {
				 	Handel_SC_ReadChar();

				 	// Move Program Counter
				 	IncreasePC();
				 	return;
				 	ASSERTNOTREACHED();
				 	break;
				}
				case SC_PrintChar: {
				 	Handel_SC_PrintChar();
				 	// Move Program Counter
				 	IncreasePC();
				 	return;
				 	ASSERTNOTREACHED();
				 	break;
				}

				case SC_RandomNum: { 
    				Handel_SC_RandomNum();
				 	// Move Program Counter
				 	IncreasePC();
				 	return;
				 	ASSERTNOTREACHED();
				 	break;
				}

				case SC_ReadString: {
					Handel_SC_ReadString();
					// Move Program Counter
					IncreasePC();
					return;
					ASSERTNOTREACHED();
					break;
				}
				
				case SC_PrintString: {
					Handel_SC_PrintString();
					// Move Program Counter
					IncreasePC();
					return;
					ASSERTNOTREACHED();
					break;
				}

        		default:
					cerr << "Unexpected system call " << type << "\n";
					break;
    		}	
      		break;
		
    	default:
      		cerr << "Unexpected user mode exception" << (int)which << "\n";
      		break;
    }
    ASSERTNOTREACHED();
}
