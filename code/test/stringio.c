#include "syscall.h"


int
main() 
{
    char a[256];
    PrintString("String length: (<= 255):\n \0");
    ReadString(a, 256);
    PrintString("Your string: \0");
    PrintString(a);
    PrintString("\n");
    Halt();
}
