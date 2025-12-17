#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NAME_LENGTH 40
#define MAX_PROCESSES 100
#define HASH_SIZE 101

typedef enum
{
    STATE_READY,
    STATE_RUNNING,
    STATE_TERMINATED,
    STATE_KILLED
} ProcessState;

typedef struct Process
{
    char name[MAX_NAME_LENGTH];
    int pid;

    int totalCpuBurst;
    int remainingCpuBurst;

    int iostartTime;
    int ioDuration;
    int remainingIoTime;

    ProcessState state;

    int arrivalTime;
    int startTime;
    int completionTime;
    int totalWaitingTime;

    struct Process *next;
} Process;

typedef struct QueueNode
{
    Process *process;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
} Queue;

Process *processTable[HASH_SIZE];

Queue readyQueue;
Queue waitingQueue;
Queue terminatedQueue;

Process *runningProcess = NULL;

int currentTime = 0;

typedef struct KillEvent
{
    int pid;
    int killTime;
} KillEvent;

KillEvent killEvents[MAX_PROCESSES];
int killEventCount = 0;

void initializeQueue(Queue *queue);
int isQueueEmpty(Queue *queue);
void enqueueProcess(Queue *queue, Process *process);
Process *dequeueProcess(Queue *queue);

int hashfunction(int pid);
void insertProcessInTable(Process *process);
Process *findProcessByPid(int pid);

void readInput();
void runSimulation();
void handleKillEventsAtCurrentTime();
void scheduleNextProcess();
void updateRunningProcess();
void updateWaitingProcesses();
int allProcessFinished();
void printResults();

void initializeQueue(Queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

int isQueueEmpty(Queue *queue)
{
    return (queue->front == NULL);
}

void enqueueProcess(Queue *queue, Process *process)
{
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL)
    {
        printf("Failed memory allocation in the enqueue PRocess\n");
        exit(1);
    }

    newNode->process = process;
    newNode->next = NULL;

    if (queue->rear == NULL)
    {
        queue->front = newNode;
        queue->rear = newNode;
    }

    else
    {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

Process *dequeueProcess(Queue *queue)
{
    if (isQueueEmpty(queue))
    {
        return NULL;
    }

    QueueNode *temp = queue->front;
    Process *removedProcess = temp->process;

    queue->front = temp->next;
    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }

    free(temp);
    return removedProcess;
}

int hashFunction(int pid)
{
    return pid % HASH_SIZE;
}

void insertProcessInTable(Process *process)
{
    int index = hashFunction(process->pid);

    process->next = processTable[index];
    processTable[index] = process;
}

Process *findProcessByPid(int pid)
{
    int index = hashFunction(pid);
    Process *current = processTable[index];

    while (current != NULL)
    {
        if (current->pid == pid)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void readInput()
{
    initializeQueue(&readyQueue);
    initializeQueue(&waitingQueue);
    initializeQueue(&terminatedQueue);

    for (int i = 0; i < HASH_SIZE; i++)
    {
        processTable[i] = NULL;
    }
    printf("Enter process and kill events:\n");

    while (1)
    {
        char token[50];
        scanf("%s", token);

        if (strcmp(token, "END") == 0)
        {
            break;
        }
        if (strcmp(token, "KILL") == 0)
        {
            int pid, time;
            scanf("%d %d", &pid, &time);

            killEvents[killEventCount].pid = pid;
            killEvents[killEventCount].killTime = time;
            killEventCount++;
            continue;
        }

        Process *newProcess = (Process *)malloc(sizeof(Process));

        strcpy(newProcess->name, token);
        scanf("%d %d %d %d", &newProcess->pid, &newProcess->totalCpuBurst, &newProcess->iostartTime, &newProcess->ioDuration);

        newProcess->remainingCpuBurst = newProcess->totalCpuBurst;
        newProcess->remainingIoTime = newProcess->ioDuration;

        newProcess->arrivalTime = 0;
        newProcess->startTime = -1;
        newProcess->completionTime = -1;
        newProcess->totalWaitingTime = 0;

        newProcess->state = STATE_READY;
        newProcess->next = NULL;

        insertProcessInTable(newProcess);

        enqueueProcess(&readyQueue, newProcess);
    }
}

void runSimulation()
{
    currentTime = 0;
    while (!allProcessFinished())
    {
        printf("Time %d: \n", currentTime);

        handleKillEventsAtCurrentTime();

        scheduleNextProcess();

        updateRunningProcess();

        updateWaitingProcesses();

        sleep(1);
        currentTime++;
    }
    printf("\n Simulation Complete! \n");
}

int allProcessFinished()
{
    if (runningProcess != NULL)
        return 0;
    if (!isQueueEmpty(&readyQueue))
        return 0;
    if (!isQueueEmpty(&waitingQueue))
        return 0;

    return 1;
}

void removeProcessFromQueue(Queue *queue, int pid)
{
    if (isQueueEmpty(queue))
        return;

    QueueNode *current = queue->front;
    QueueNode *prev = NULL;

    while (current != NULL)
    {
        if (current->process->pid == pid)
        {
            if (prev == NULL)
            {
                queue->front = current->next;

                if (queue->front == NULL)
                    queue->rear = NULL;
            }
            else
            {
                prev->next = current->next;

                if (current->next == NULL)
                    queue->rear = prev;
            }

            free(current);
            return;
        }

        prev = current;
        current = current->next;
    }
}

void handleKillEventsAtCurrentTime()
{
    for (int i = 0; i < killEventCount; i++)
    {
        if (killEvents[i].killTime == currentTime)
        {
            int pid = killEvents[i].pid;
            Process *target = findProcessByPid(pid);

            if (target == NULL)
                continue;

            printf("KILL EVENT: Killing process %d at time %d\n", pid, currentTime);

            removeProcessFromQueue(&readyQueue, pid);

            removeProcessFromQueue(&waitingQueue, pid);

            if (runningProcess != NULL && runningProcess->pid == pid)
            {
                runningProcess = NULL;
            }

            target->state = STATE_KILLED;
            target->completionTime = currentTime;

            enqueueProcess(&terminatedQueue, target);
        }
    }
}

void scheduleNextProcess()
{
    if (runningProcess != NULL)
        return;

    if (isQueueEmpty(&readyQueue))
        return;

    Process *nextProcess = dequeueProcess(&readyQueue);

    nextProcess->state = STATE_RUNNING;

    if (nextProcess->startTime == -1)
    {
        nextProcess->startTime = currentTime;
    }

    runningProcess = nextProcess;

    printf("Process %d (%s) started running at time %d\n",
           nextProcess->pid, nextProcess->name, currentTime);
}

void updateRunningProcess()
{
    if (runningProcess == NULL)
        return;

    runningProcess->remainingCpuBurst--;

    printf("Running %s (PID %d), remaining CPU: %d\n",
           runningProcess->name,
           runningProcess->pid,
           runningProcess->remainingCpuBurst);

    int timeSpentOnCpu = currentTime - runningProcess->startTime;

    if (runningProcess->iostartTime >= 0 &&
        timeSpentOnCpu == runningProcess->iostartTime)
    {
        printf("Process %d starting I/O\n", runningProcess->pid);

        runningProcess->state = STATE_READY;
        enqueueProcess(&waitingQueue, runningProcess);

        runningProcess = NULL;
        return;
    }
    if (runningProcess->remainingCpuBurst <= 0)
    {
        printf("Process %d finished execution at time %d\n",
               runningProcess->pid, currentTime);

        runningProcess->state = STATE_TERMINATED;
        runningProcess->completionTime = currentTime;

        enqueueProcess(&terminatedQueue, runningProcess);
        runningProcess = NULL;
    }
}

void updateWaitingProcesses()
{
    if (isQueueEmpty(&waitingQueue))
        return;

    QueueNode *current = waitingQueue.front;
    QueueNode *prev = NULL;

    while (current != NULL)
    {
        Process *waitingProcess = current->process;

        waitingProcess->remainingIoTime--;

        printf("Process %d doing I/O, remaining IO: %d\n",
               waitingProcess->pid, waitingProcess->remainingIoTime);

        if (waitingProcess->remainingIoTime <= 0)
        {
            printf("Process %d completed Input outpur at time %d\n",
                   waitingProcess->pid, currentTime);

            waitingProcess->state = STATE_READY;
            waitingProcess->remainingIoTime = waitingProcess->ioDuration;
            QueueNode *nodeToRemove = current;

            if (prev == NULL)
            {
                waitingQueue.front = current->next;

                if (waitingQueue.front == NULL)
                    waitingQueue.rear = NULL;
            }
            else
            {
                prev->next = current->next;

                if (current->next == NULL)
                    waitingQueue.rear = prev;
            }
            current = current->next;
            enqueueProcess(&readyQueue, waitingProcess);

            free(nodeToRemove);
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
}

void printResults()
{
    printf("\nFinal Process Details:\n\n");

    while (!isQueueEmpty(&terminatedQueue))
    {
        Process *finishedProcess = dequeueProcess(&terminatedQueue);

        int turnaroundTime = finishedProcess->completionTime;
        int waitingTime = finishedProcess->totalWaitingTime;

        printf("Process Name: %s\n", finishedProcess->name);
        printf("PID: %d\n", finishedProcess->pid);
        printf("CPU Burst: %d\n", finishedProcess->totalCpuBurst);
        printf("IO Duration: %d\n", finishedProcess->ioDuration);
        printf("Turnaround Time: %d\n", turnaroundTime);
        printf("Waiting Time: %d\n", waitingTime);

        if (finishedProcess->state == STATE_KILLED)
            printf("Status: KILLED at time %d\n", finishedProcess->completionTime);
        else
            printf("Status: COMPLETED\n");

        printf("----------------\n");
    }
}

int main()
{
    readInput();
    runSimulation();
    printResults();
    return 0;
}