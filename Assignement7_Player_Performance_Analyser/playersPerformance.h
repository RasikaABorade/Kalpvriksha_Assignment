#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROLE_BATSMAN 1
#define ROLE_BOWLER 2
#define ROLE_ALLROUNDER 3

typedef struct
{
    int playerId;
    char name[50];
    int teamId;
    int role;
    long totalRuns;
    float battingAvg;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
} PlayerInfo;

typedef struct Node
{
    PlayerInfo data;
    struct Node *next;
} Node;

typedef struct
{
    int teamId;
    char teamName[50];
    Node *head;
    int totalPlayers;
    float sumStrikeRate;
    int countStrikePlayers;
} Team;

extern Team myTeams[10];

void loadInitialData();
float computePerformanceIndex(PlayerInfo player);
int findTeamIndex(int teamId);
Node *createNode(PlayerInfo player);

int addPlayerToTeam(PlayerInfo player);
void displayPlayersOfTeam(int teamId);
void displayTeamsByAvgStrikeRate();
void displayTopKPlayers(int teamId, int role, int K);
void displayAllPlayersByRole(int role);

void freeAllMemory();