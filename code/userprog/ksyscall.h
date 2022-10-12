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
  char c = '\0', temp = '\0';
  do
  {
    temp = kernel->synchConsoleIn->GetChar();
    if (c == '\0')
    {
      c = temp;
    }
  } while (temp != '\0' && temp != '\n' && temp != EOF);
  return c;
  // chi can return kernel->synchConsoleIn->GetChar(); la se chay duoc roi
  // Nhung de fix loi khi nhap >= 3 ki tu thi terminal se tu dong chay 1 lenh voi cau lenh la 2 ki tu con lai ma user nhap du
}

void SysPrintChar(char character) {
  // vao kernel->synchConsoleOut va goi ham PutChar() de xuat 1 ki tu ra man hinh console
  kernel->synchConsoleOut->PutChar(character);
  return;
}

int SysRandomNum() {
  // dung ham random cua thu vien #include <stdlib.h> de tao ra mot so nguyen ngau nhien
  return rand();
}

void SysReadString(int buffer, int length)
{
	char *buff = NULL;
  // de gan gia tri tra ve cua kernel->synchConsoleIn->ReadStr(buff,length);, lay vi tri cua dau \0
	int userStringMaxLength = 0;
	if (length > 0) 
	{
		buff = new char[length];
		if (buff == NULL) 
		{
			char msg[] = "Not enough space to store or user entered a null string.\n\0";
			kernel->synchConsoleOut->PrintStr(msg,strlen(msg)); // print out the msg
		}
    // buff not null
		else
		{
			char msg[] = "Enter string: \0";
			kernel->synchConsoleOut->PrintStr(msg);
			userStringMaxLength = kernel->synchConsoleIn->ReadStr(buff,length); 
			//Return the pos of '\0' suppose to be
			buff[userStringMaxLength] = '\0';
		}
	}
	else
	{
		memset(buff, 0, length);
	}
  //cerr << "Doc duoc: ";
	if (buff != NULL) 
	{
		for (int i = 0; i <= userStringMaxLength ; i++) 
		{
      //cerr << buff[i];
			//Kernel buff -> user buffer
			kernel->machine->WriteMem(buffer+i,1,(int)buff[i]); 
      // Lay du lieu cua Kernel(buff) Write vao du lieu cua user(buffer). Thi ben cai user (file string io no se co du lieu)
		}
        delete[] buff;
  }
  //cerr << "\n";	    
  return;
}

void SysPrintString(int buffer)
{
  int ch = 0;
	//int ch[128]
	int MAX_STRING_LENGTH = 256;
	for(int i=0; i < MAX_STRING_LENGTH; i++)
	{
		//user buffer -> kernel p
		kernel->machine->ReadMem(buffer+i,1,&ch); 
    // Lay du lieu tu user tai dia chi buffer+i bo vao kernel ch
		// kernel->machine->ReadMem(buffer+i,1,ch+i);
		// Vi lay string de print ra thoi nen khong can char array
		kernel->synchConsoleOut->PutChar(char(ch));
		if(char(ch) == '\0' || char(ch) == EOF)
		{
			break;
		}
	}
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
