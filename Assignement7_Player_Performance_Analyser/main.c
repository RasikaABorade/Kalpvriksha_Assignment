#include "playersPerformance.h"
#include "Players_data.h"

int main(void)
{
    int choice;
    loadInitialData();

    while (1)
    {
        printf("\n========= Player Performance Analyzer =========\n");
        printf("1. Add Player to a Team\n");
        printf("2. Display Players of a Team\n");
        printf("3. Display Teams by Average Strike Rate\n");
        printf("4. Display Top K Players of a Team (by Role)\n");
        printf("5. Display All Players Across All Teams (by Role)\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            PlayerInfo newPlayer;

            printf("Enter Player ID: ");
            scanf("%d", &newPlayer.playerId);

            printf("Enter Player Name: ");
            scanf("%s", newPlayer.name);

            printf("Enter Team ID (1–10): ");
            scanf("%d", &newPlayer.teamId);

            printf("Enter Role (1=Batsman, 2=Bowler, 3=All-rounder): ");
            scanf("%d", &newPlayer.role);

            printf("Enter Total Runs: ");
            scanf("%ld", &newPlayer.totalRuns);

            printf("Enter Batting Average: ");
            scanf("%f", &newPlayer.battingAvg);

            printf("Enter Strike Rate: ");
            scanf("%f", &newPlayer.strikeRate);

            printf("Enter Wickets: ");
            scanf("%d", &newPlayer.wickets);

            printf("Enter Economy Rate: ");
            scanf("%f", &newPlayer.economyRate);

            addPlayerToTeam(newPlayer);
        }
        else if (choice == 2)
        {
            int teamId;
            printf("Enter Team ID (1–10): ");
            scanf("%d", &teamId);

            displayPlayersOfTeam(teamId);
        }
        else if (choice == 3)
        {
            displayTeamsByAvgStrikeRate();
        }
        else if (choice == 4)
        {
            int teamId, role, K;

            printf("Enter Team ID (1–10): ");
            scanf("%d", &teamId);

            printf("Enter Role (1=Batsman, 2=Bowler, 3=All-rounder): ");
            scanf("%d", &role);

            printf("Enter K: ");
            scanf("%d", &K);

            displayTopKPlayers(teamId, role, K);
        }
        else if (choice == 5)
        {
            int role;
            printf("Enter Role (1=Batsman, 2=Bowler, 3=All-rounder): ");
            scanf("%d", &role);

            displayAllPlayersByRole(role);
        }
        else if (choice == 6)
        {
            freeAllMemory();
            printf("Exiting program...\n");
            break;
        }
        else
        {
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
