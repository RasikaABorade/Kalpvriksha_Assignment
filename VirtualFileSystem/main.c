#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "virtualFileSystem.h"

int main()
{
    char command[100];
    char name[50];

    printf("Compact VFS-ready. Type 'exit' to quir.\n");

    initializeSystem();

    while (1)
    {
        printf("%s > ", cwd->name);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (sscanf(command, "mkdir %s", name) == 1)
        {
            makeDirectory(name);
        }
        else if (sscanf(command, "create %s", name) == 1)
        {
            createFile(name);
        }
        else if (strcmp(command, "ls") == 0)
        {
            listItems();
        }
        else if (sscanf(command, "cd %s", name) == 1)
        {
            changeDirectory(name);
        }
        else if (strcmp(command, "pwd") == 0)
        {
            printWorkingDirectory();
        }

        else if (strncmp(command, "write", 5) == 0)
        {
            char fileName[50];
            char content[500];

            int matched = sscanf(command, "write %s %[^\n]", fileName, content);

            if (matched == 2)
            {
                writeFile(fileName, content);
            }
            else
            {
                printf("Usage: write <filename> <text>\n");
            }
        }

        else if (sscanf(command, "read %s", name) == 1)
        {
            readFile(name);
        }
        else if (sscanf(command, "delete %s", name) == 1)
        {
            deleteFile(name);
        }
        else if (sscanf(command, "rmdir %s", name) == 1)
        {
            removeDirectory(name);
        }
        else if (strcmp(command, "df") == 0)
        {
            showDiskStatus();
        }
        else if (strcmp(command, "exit") == 0)
        {
            freeAllMemory();
            printf("Memory released . Exiting program..\n");
            break;
        }
        else
        {
            printf("Invalid command.\n");
        }
    }
    return 0;
}