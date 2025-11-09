#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME 50
#define MAX_BLOCKS_PER_FILE 10

typedef struct FreeBlock
{
    int index;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;

typedef struct FileNode
{
    char name[MAX_NAME];
    int isDirectory;
    int size;
    int blockPointers[MAX_BLOCKS_PER_FILE];

    struct FileNode *parent;
    struct FileNode *child;
    struct FileNode *next;
} FileNode;

extern char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
extern FreeBlock *freeListHead;
extern FileNode *root;
extern FileNode *cwd;

void initializeSystem();
void makeDirectory(char *name);
void createFile(char* name);
void listItems();
void changeDirectory(char* name);
void printWorkingDirectory();
void writeFile(char* name,char* content);
void readFile(char* name);
void deleteFile(char* name);
void removeDirectory(char* name);
void showDiskStatus();
void freeAllMemory();