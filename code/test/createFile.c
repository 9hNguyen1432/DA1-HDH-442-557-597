#include "syscall.h"

int main() {
    char fileName[256];
    PrintString("Enter file's name: \n");
    ReadString(fileName, 255);

    if (CreateFile(fileName) == 0) {
        PrintString("File ");
        PrintString(fileName);
        PrintString(" created successfully!\n");
    } else
        PrintString("Create file failed\n");
}