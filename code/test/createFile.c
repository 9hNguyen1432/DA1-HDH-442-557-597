#include "syscall.h"

int main() {
    char fileName[256];
    PrintString("Press file's name: \n");
    ReadString(fileName, 255);

    if (CreateFile(fileName) == 0) {
        PrintString("File ");
        PrintString(fileName);
        PrintString(" created successfully!\n");
    } else
    {
        PrintString("File ");
        PrintString(fileName);
        PrintString("File create failed!\n");
    }

}