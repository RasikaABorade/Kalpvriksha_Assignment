#include "playersPerformance.h"
#include "Players_data.h"

Team myTeams[10];

Node *createNode(PlayerInfo player)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL)
    {
        printf("Error: Failed memory allocation for the new node\n");
        return NULL;
    }
    n->data = player;
    n->next = NULL;
    return n;
}

float computePerformanceIndex(PlayerInfo player)
{
    float index = 0;

    if (player.role == ROLE_BATSMAN)
    {
        index = (player.battingAvg * player.strikeRate) / 100;
    }
    else if (player.role == ROLE_BOWLER)
    {
        index = (player.wickets * 2) + (100 - player.economyRate);
    }
    else if (player.role == ROLE_ALLROUNDER)
    {
        index = (player.battingAvg * player.strikeRate) / 100;
        index += (player.wickets * 2);
    }
    return index;
}

int findTeamIndex(int teamId)
{
    for (int i = 0; i < 10; i++)
    {
        if (myTeams[i].teamId == teamId)
        {
            return i; // found
        }
    }
    return -1;
}

void loadInitialData()
{
    for (int i = 0; i < 10; i++)
    {
        myTeams[i].teamId = i + 1;
        strcpy(myTeams[i].teamName, teams[i]);

        myTeams[i].head = NULL;
        myTeams[i].totalPlayers = 0;
        myTeams[i].sumStrikeRate = 0;
        myTeams[i].countStrikePlayers = 0;
    }

    for (int i = 0; i < playerCount; i++)
    {
        PlayerInfo newPlayer;

        newPlayer.playerId = players[i].id;
        strcpy(newPlayer.name, players[i].name);
        newPlayer.totalRuns = players[i].totalRuns;
        newPlayer.battingAvg = players[i].battingAverage;
        newPlayer.strikeRate = players[i].strikeRate;
        newPlayer.wickets = players[i].wickets;
        newPlayer.economyRate = players[i].economyRate;

        if (strcmp(players[i].role, "Batsman") == 0)
        {
            newPlayer.role = ROLE_BATSMAN;
        }
        else if (strcmp(players[i].role, "Bowler") == 0)
        {
            newPlayer.role = ROLE_BOWLER;
        }
        else
        {
            newPlayer.role = ROLE_ALLROUNDER;
        }

        newPlayer.performanceIndex = computePerformanceIndex(newPlayer);

        int teamIndex = -1;
        for (int j = 0; j < 10; j++)
        {
            if (strcmp(myTeams[j].teamName, players[i].team) == 0)
            {
                teamIndex = j;
                break;
            }
        }

        if (teamIndex == -1)
        {
            printf("Warning , team  '%s' not found for the player %s (id %d)\n", players[i].team, players[i].name, players[i].id);
            continue;
        }

        newPlayer.teamId = myTeams[teamIndex].teamId;

        Node *newNode = createNode(newPlayer);
        if (newNode == NULL)
        {
            printf("Warning we could not alllocate node for player %s\n", newPlayer.name);
            continue;
        }

        if (myTeams[teamIndex].head == NULL)
        {
            myTeams[teamIndex].head = newNode;
        }
        else
        {
            Node *currentNode = myTeams[teamIndex].head;
            while (currentNode->next != NULL)
                currentNode = currentNode->next;

            currentNode->next = newNode;
        }

        myTeams[teamIndex].totalPlayers++;

        if (newPlayer.role == ROLE_BATSMAN || newPlayer.role == ROLE_ALLROUNDER)
        {
            myTeams[teamIndex].sumStrikeRate += newPlayer.strikeRate;
            myTeams[teamIndex].countStrikePlayers++;
        }
    }
}

void displayPlayersOfTeam(int teamId)
{
    int teamIndex = findTeamIndex(teamId);

    if (teamIndex == -1)
    {
        printf("Invalid Team id\n");
        return;
    }

    Team *selectedTeam = &myTeams[teamIndex];
    Node *currentnode = selectedTeam->head;

    printf("\n Team: %s (ID: %d)\n", selectedTeam->teamName, selectedTeam->teamId);

    if (currentnode == NULL)
    {
        printf("No players found in this team.\n");
        return;
    }

    printf("---------------------------\n");

    while (currentnode != NULL)
    {
        PlayerInfo player = currentnode->data;

        printf("Player Id: %d\n", player.playerId);
        printf("Name: %s\n", player.name);

        if (player.role == ROLE_BATSMAN)
            printf("Role: Batsman\n");

        else if (player.role == ROLE_BOWLER)
            printf("Role: Bowler\n");

        else
            printf("Role: All-rounder\n");

        printf("Runs: %ld\n", player.totalRuns);
        printf("Batting Average: %.2f\n", player.battingAvg);
        printf("Strike Rate: %.2f\n", player.strikeRate);
        printf("Wickets: %d\n", player.wickets);
        printf("Economy Rate: %.2f\n", player.economyRate);
        printf("Performance Index: %.2f\n", player.performanceIndex);

        printf("------------------------\n");
        currentnode = currentnode->next;
    }

    printf("Total Players: %d\n", selectedTeam->totalPlayers);

    if (selectedTeam->countStrikePlayers > 0)
    {
        float avgStrikeRate = selectedTeam->sumStrikeRate / selectedTeam->countStrikePlayers;
        printf("Average Batting strike rate: %.2f\n", avgStrikeRate);
    }
    else
    {
        printf("Average batting strike rate: NA\n");
    }
}

void displayAllPlayersByRole(int role)
{
    PlayerInfo collected[300];
    int count = 0;

    for (int i = 0; i < 10; i++)
    {
        Node *currentNode = myTeams[i].head;

        while (currentNode != NULL)
        {
            if (currentNode->data.role == role)
            {
                collected[count] = currentNode->data;
                count++;
            }
            currentNode = currentNode->next;
        }
    }

    if (count == 0)
    {
        printf("No players found with this role.\n");
        return;
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (collected[j].performanceIndex > collected[i].performanceIndex)
            {
                PlayerInfo temp = collected[i];
                collected[i] = collected[j];
                collected[j] = temp;
            }
        }
    }

    printf("\nPlayers with Role: ");
    if (role == ROLE_BATSMAN)
        printf("Batsman\n");
    else if (role == ROLE_BOWLER)
        printf("Bowler\n");
    else
        printf("All-rounder\n");

    printf("-------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("Player id: %d\n", collected[i].playerId);
        printf("Name: %s\n", collected[i].name);

        printf("Team: %s\n", myTeams[collected[i].teamId - 1].teamId);

        printf("Runs: %ld\n", collected[i].totalRuns);
        printf("Batting Avg: %.2f\n", collected[i].battingAvg);
        printf("Strike Rate: %.2f\n", collected[i].strikeRate);
        printf("Wickets: %d\n", collected[i].wickets);
        printf("Economy Rate: %.2f\n", collected[i].economyRate);
        printf("Performance Index: %.2f\n", collected[i].performanceIndex);

        printf("---------------------------\n");
    }
}

int addPlayerToTeam(PlayerInfo newPlayer)
{
    if (newPlayer.playerId < 1 || newPlayer.playerId > 1500)
    {
        printf("Error: The player id should be between 1 and 1500.\n");
        return -1;
    }

    // check duplicate
    for (int i = 0; i < 10; i++)
    {
        Node *scan = myTeams[i].head;
        while (scan != NULL)
        {
            if (scan->data.playerId == newPlayer.playerId)
            {
                printf("Error: Player id %d already exsits.\n", newPlayer.playerId);
                return -1;
            }
            scan = scan->next;
        }
    }

    int teamIndex = findTeamIndex(newPlayer.teamId);
    if (teamIndex == -1)
    {
        printf("Error: Invalid team id.\n");
        return -1;
    }

    if (myTeams[teamIndex].totalPlayers >= 50)
    {
        printf("Error : The team already has maximum players 50.\n");
        return -1;
    }
    newPlayer.performanceIndex = computePerformanceIndex(newPlayer);

    Node *newNode = createNode(newPlayer);
    if (newNode == NULL)
    {
        printf("Not enough memeory to add player.\n");
        return -1;
    }

    if (myTeams[teamIndex].head == NULL)
    {
        myTeams[teamIndex].head = newNode;
    }
    else
    {
        Node *last = myTeams[teamIndex].head;
        while (last->next != NULL)
            last = last->next;

        last->next = newNode;
    }

    myTeams[teamIndex].totalPlayers++;

    if (newPlayer.role == ROLE_BATSMAN || newPlayer.role == ROLE_ALLROUNDER)
    {
        myTeams[teamIndex].sumStrikeRate += newPlayer.strikeRate;
        myTeams[teamIndex].countStrikePlayers++;
    }

    printf("Player '%s' added succesfully to team %s.\n ");
    return 0;
}

void displayTeamsByAvgStrikeRate()
{
    Team temp[10];
    for (int i = 0; i < 10; i++)
        temp[i] = myTeams[i];

    float avg[10];
    for (int i = 0; i < 10; i++)
    {
        if (temp[i].countStrikePlayers > 0)
            avg[i] = temp[i].sumStrikeRate / temp[i].countStrikePlayers;

        else
            avg[i] = 0.0f;
    }

    // sort
    for (int i = 0; i < 9; i++)
    {
        for (int j = i + 1; j < 10; j++)
        {
            if (avg[j] > avg[i])
            {
                float tmpAvg = avg[i];
                avg[i] = avg[j];
                avg[j] = tmpAvg;

                Team tmpTeam = temp[i];
                temp[i] = temp[j];
                temp[j] = tmpTeam;
            }
        }
    }

    printf("\n Teams sorted by avg Batting strike rate from high to low: \n");
    printf("---------------------------------------------\n");

    for (int i = 0; i < 10; i++)
    {
        printf("Team id: %d\n", temp[i].teamId);
        printf("Team Name: %s\n", temp[i].teamName);
        printf("Total Players: %d\n", temp[i].totalPlayers);

        if (temp[i].countStrikePlayers > 0)
            printf("Average batting strike rate: %.2f\n ", avg[i]);

        else
            printf("Avg batting strike rate: NA\n");

        printf("---------------------\n");
    }
}

void displayTopKPlayers(int teamId, int role, int K)
{
    int teamIndex = findTeamIndex(teamId);
    if (teamIndex == -1)
    {
        printf("Invalid team id: %d\n", teamId);
        return;
    }

    PlayerInfo list[60];
    int count = 0;

    Node *currentNode = myTeams[teamIndex].head;
    while (currentNode != NULL)
    {
        if (currentNode->data.role == role)
        {
            list[count] = currentNode->data;
            count++;
        }
        currentNode = currentNode->next;
    }

    if (count == 0)
    {
        printf("No players of the requested role in the team %s.\n", myTeams[teamIndex].teamName);
        return;
    }

    if (K <= 0)
        K = 1;
    if (K > count)
        K = count;

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (list[j].performanceIndex < list[j + 1].performanceIndex)
            {
                PlayerInfo temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }

    printf("\n Top %d players of team %s (Role: ", K, myTeams[teamIndex].teamName);
    if (role == ROLE_BATSMAN)
        printf("Batsman)\n");
    else if (role == ROLE_BOWLER)
        printf("Bowler)\n");
    else
        printf("All-rounder)\n");

    printf("------------------------\n");

    for (int i = 0; i < K; i++)
    {
        PlayerInfo currentPlayer = list[i];

        printf("Player Id: %d\n", currentPlayer.playerId);
        printf("Name: %s\n", currentPlayer.name);
        printf("Runs: %ld\n", currentPlayer.totalRuns);
        printf("Batting Avg: %.2f\n", currentPlayer.battingAvg);
        printf("Strike Rate: %.2f\n", currentPlayer.strikeRate);
        printf("Wickets: %d\n", currentPlayer.wickets);
        printf("Economy Rate: %.2f\n", currentPlayer.economyRate);
        printf("Performance Index: %.2f\n", currentPlayer.performanceIndex);

        printf("---------------------\n");
    }
}

void freeAllMemory()
{
    for (int teamIndex = 0; teamIndex < 10; teamIndex++)
    {
        Node *currentNode = myTeams[teamIndex].head;

        while (currentNode != NULL)
        {
            Node *nextNode = currentNode->next;
            free(currentNode);
            currentNode = nextNode;
        }

        myTeams[teamIndex].head = NULL;
        myTeams[teamIndex].totalPlayers = 0;
        myTeams[teamIndex].sumStrikeRate = 0.0f;
        myTeams[teamIndex].countStrikePlayers = 0;
    }
}