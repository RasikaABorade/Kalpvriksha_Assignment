#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "virtualFileSystem.h"

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
FreeBlock *freeListHead = NULL;
FileNode *root = NULL;
FileNode *cwd = NULL;

void initializeSystem()
{
    FreeBlock *prev = NULL;

    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        FreeBlock *newBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
        newBlock->index = i;
        newBlock->next = NULL;
        newBlock->prev = prev;

        if (prev != NULL)
        {
            prev->next = newBlock;
        }
        else
        {
            freeListHead = newBlock;
        }

        prev = newBlock;
    }

    root = (FileNode *)malloc(sizeof(FileNode));
    strcpy(root->name, "/");
    root->isDirectory = 1;
    root->size = 0;
    root->child = NULL;
    root->next = NULL;
    root->parent = NULL;

    cwd = root;

    printf("System initialized successfully.\n");
}

void makeDirectory(char *name)
{
    FileNode *temp = cwd->child;

    if (temp != NULL)
    {
        FileNode *start = temp;
        do
        {
            if (temp->isDirectory && strcmp(temp->name, name) == 0)
            {
                printf("Name already exists in current directory.\n");
                return;
            }
            temp = temp->next;
        } while (temp != start);
    }

    FileNode *newDir = (FileNode *)malloc(sizeof(FileNode));
    strcpy(newDir->name, name);
    newDir->isDirectory = 1;
    newDir->size = 0;
    newDir->child = NULL;
    newDir->parent = cwd;
    newDir->next = NULL;

    if (cwd->child == NULL)
    {
        cwd->child = newDir;
        newDir->next = newDir;
    }
    else
    {
        FileNode *current = cwd->child;
        while (current->next != cwd->child)
        {
            current = current->next;
        }
        current->next = newDir;
        newDir->next = cwd->child;
    }
    printf("Directory '%s' created successfully.\n", name);
}

void listItems()
{
    if (cwd->child == NULL)
    {
        printf("(empty)\n");
        return;
    }

    FileNode *temp = cwd->child;
    FileNode *start = temp;

    do
    {
        if (temp->isDirectory)
        {
            printf("%s/\n", temp->name);
        }
        else
        {
            printf("%s\n", temp->name);
        }
        temp = temp->next;
    } while (temp != start);
}

void changeDirectory(char *name)
{
    if (strcmp(name, "..") == 0)
    {
        if (cwd->parent != NULL)
        {
            cwd = cwd->parent;
            printf("Moved to %s\n", cwd->name);
        }
        else
        {
            printf("Already at root directory.\n");
        }
        return;
    }

    if (cwd->child == NULL)
    {
        printf("No subdirectories found");
        return;
    }

    FileNode *temp = cwd->child;
    FileNode *start = temp;

    do
    {
        if (temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            cwd = temp;
            printf("Moved to %s\n", cwd->name);
            return;
        }
        temp = temp->next;
    } while (temp != start);

    printf("Directory not found.\n");
}

void createFile(char *name)
{
    FileNode *temp = cwd->child;

    if (temp != NULL)
    {
        FileNode *start = temp;
        do
        {
            if (strcmp(temp->name, name) == 0)
            {
                printf("Name already exists in the current directoryy.\n");
                return;
            }
            temp = temp->next;
        } while (temp != start);
    }

    FileNode *newFile = (FileNode *)malloc(sizeof(FileNode));
    strcpy(newFile->name, name);
    newFile->isDirectory = 0;
    newFile->size = 0;

    for (int i = 0; i < MAX_BLOCKS_PER_FILE; i++)
        newFile->blockPointers[i] = -1;

    newFile->child = NULL;
    newFile->parent = cwd;
    newFile->next = NULL;

    if (cwd->child == NULL)
    {
        cwd->child = newFile;
        newFile->next = newFile;
    }
    else
    {
        FileNode *current = cwd->child;
        while (current->next != cwd->child)
        {
            current = current->next;
        }
        current->next = newFile;
        newFile->next = cwd->child;
    }

    printf("File '%s' createds successfully.\n", name);
}

void printWorkingDirectory()
{
    FileNode *temp = cwd;

    if (temp->parent == NULL)
    {
        printf("\n");
        return;
    }

    char path[500] = "";
    char fullPath[500] = "";

    while (temp != NULL && temp->parent != NULL)
    {
        char tempPath[100];
        sprintf(tempPath, "/%s", temp->name);
        strcat(tempPath, fullPath);
        strcpy(fullPath, tempPath);
        temp = temp->parent;
    }

    printf("%s\n", fullPath);
}

void writeFile(char *name, char *content)
{
    if (cwd->child == NULL)
    {
        printf("No files found.\n");
        return;
    }

    FileNode *temp = cwd->child;
    FileNode *start = temp;
    FileNode *target = NULL;

    do
    {
        if (!temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            target = temp;
            break;
        }
        temp = temp->next;
    } while (temp != start);

    if (target == NULL)
    {
        printf("File not found.\n");
        return;
    }

    int dataSize = strlen(content);
    int neededBlocks = (dataSize / BLOCK_SIZE) + 1;

    FreeBlock *fb = freeListHead;
    int count = 0;
    while (fb != NULL)
    {
        count++;
        fb = fb->next;
    }
    if (neededBlocks > count)
    {
        printf("Not enough free space in the disk.\n");
        return;
    }

    fb = freeListHead;
    for (int i = 0; i < neededBlocks; i++)
    {
        if (fb == NULL)
            break;

        target->blockPointers[i] = fb->index;
        strncpy(virtualDisk[fb->index], content + (i * BLOCK_SIZE), BLOCK_SIZE);

        freeListHead = fb->next;
        if (freeListHead != NULL)
        {
            freeListHead->prev = NULL;
        }

        FreeBlock *old = fb;
        fb = fb->next;
        free(old);
    }

    target->size = dataSize;
    printf("Data written successfully (size=%d bytes).", dataSize);
}

void readFile(char *name)
{
    if (cwd->child == NULL)
    {
        printf("No files found.\n");
        return;
    }

    FileNode *temp = cwd->child;
    FileNode *start = temp;
    FileNode *target = NULL;

    do
    {
        if (!temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            target = temp;
            break;
        }
        temp = temp->next;
    } while (temp != start);

    if (target == NULL)
    {
        printf("File not found.\n");
        return;
    }

    if (target->size == 0)
    {
        printf("(empty)\n");
        return;
    }

    printf("%s\n", target->name);
    printf("--------------------\n");

    int remaining = target->size;
    for (int i = 0; i < MAX_BLOCKS_PER_FILE && target->blockPointers[i] != -1; i++)
    {
        int blockIndex = target->blockPointers[i];

        int bytesToRead;
        if (remaining < BLOCK_SIZE)
        {
            bytesToRead = remaining;
        }
        else
        {
            bytesToRead = BLOCK_SIZE;
        }

        for (int j = 0; j < bytesToRead; j++)
        {
            printf("%c", virtualDisk[blockIndex][j]);
        }
        remaining = remaining - bytesToRead;
    }
    printf("\n");
}

void deleteFile(char *name)
{
    if (cwd->child == NULL)
    {
        printf("No files to delete.\n");
        return;
    }

    FileNode *temp = cwd->child;
    FileNode *prev = NULL;
    FileNode *start = temp;
    FileNode *target = NULL;

    do
    {
        if (!temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            target = temp;
            break;
        }
        prev = temp;
        temp = temp->next;
    } while (temp != start);

    if (target == NULL)
    {
        printf("File not found.\n");
        return;
    }

    for (int i = 0; i < MAX_BLOCKS_PER_FILE; i++)
    {
        int blockIndex = target->blockPointers[i];
        if (blockIndex != -1)
        {
            FreeBlock *newBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
            newBlock->index = blockIndex;
            newBlock->next = freeListHead;
            newBlock->prev = NULL;

            if (freeListHead != NULL)
                freeListHead->prev = newBlock;

            freeListHead = newBlock;
            target->blockPointers[i] = -1;
        }
    }

    if (target == cwd->child && target->next == cwd->child)
    {
        cwd->child = NULL;
    }
    else if (target == cwd->child)
    {
        FileNode *last = cwd->child;
        while (last->next != cwd->child)
        {
            last = last->next;
        }
        last->next = target->next;
        cwd->child = target->next;
    }
    else
    {
        prev->next = target->next;
    }

    free(target);
    printf("File '%s' deleted successfully.\n", name);
}

void removeDirectory(char *name)
{
    if (cwd->child == NULL)
    {
        printf("No filders to delete.\n");
        return;
    }

    FileNode *temp = cwd->child;
    FileNode *prev = NULL;
    FileNode *start = temp;
    FileNode *target = NULL;

    do
    {
        if (temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            target = temp;
            break;
        }
        prev = temp;
        temp = temp->next;
    } while (temp != start);

    if (target == NULL)
    {
        printf("Folder not found.\n");
        return;
    }

    if (target->child != NULL)
    {
        printf("Folder is non empty , Cannot delete.\n");
        return;
    }

    if (target == cwd->child && target->next == cwd->child)
    {
        cwd->child = NULL;
    }
    else if (target == cwd->child)
    {
        FileNode *last = cwd->child;
        while (last->next != cwd->child)
        {
            last = last->next;
        }
        last->next = target->next;
        cwd->child = target->next;
    }
    else
    {
        prev->next = target->next;
    }
    free(target);
    printf("Folder '%s' deleted successsfully.\n", name);
}

void showDiskStatus()
{
    int freeBlocks = 0;

    FreeBlock *temp = freeListHead;
    while (temp != NULL)
    {
        freeBlocks++;
        temp = temp->next;
    }

    int usedBlocks = NUM_BLOCKS - freeBlocks;

    printf("Total bloacks: %d\n", NUM_BLOCKS);
    printf("Used blocks: %d\n", usedBlocks);
    printf("Free blocks: %d\n", freeBlocks);

    printf("Used space: %d bytes\n", usedBlocks * BLOCK_SIZE);
    printf("Free space: %d bytes\n", freeBlocks * BLOCK_SIZE);
}

void freeAllMemory()
{
    if (root != NULL)
    {
        FileNode *start = root->child;

        if (start != NULL)
        {
            FileNode *temp = start;

            do
            {
                FileNode *nextNode = temp->next;
                free(temp);
            } while (temp != start);
        }
        free(root);
    }

    FreeBlock *block = freeListHead;
    while (block != NULL)
    {
        FreeBlock *nextBlock = block->next;
        free(block);
        block = nextBlock;
    }
    printf("All memory has fred.\n");
}