#include "syscall.h"

int
main() 
{
    char string[256];
    PrintString("String length: (<= 255):\n \0");
    ReadString(string, 256);
    PrintString("Your string: \0");
    PrintString(string);
    PrintString("\n");
    Halt();
}
