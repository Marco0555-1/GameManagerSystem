#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    int sizeGB;
    int isInstalled; 
} Game;

#define MAX_GAMES 5
#define TOTAL_CAPACITY 1024 

Game store[MAX_GAMES] = {
    {"Elden Ring", 60, 0},
    {"FIFA 24", 50, 0},
    {"Cyberpunk 2077", 70, 0},
    {"Minecraft", 2, 0},
    {"GTA V", 100, 0}
};

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int getUsedSpace() {
    int used = 0;
    for (int i = 0; i < MAX_GAMES; i++) {
        if (store[i].isInstalled) used += store[i].sizeGB;
    }
    return used;
}

void showInstalledGames() {
    int choice;
    do {
        clearScreen();
        printf("=== INSTALLED GAMES MANAGEMENT ===\n");
        printf("Current Storage: %d/%d GB\n\n", getUsedSpace(), TOTAL_CAPACITY);
        
        int found = 0;
        for (int i = 0; i < MAX_GAMES; i++) {
            if (store[i].isInstalled) {
                printf("%d. %s (%d GB)\n", i + 1, store[i].name, store[i].sizeGB);
                found = 1;
            }
        }
        if (!found) printf("No games installed yet.\n");

        printf("\nOptions:\n");
        printf("1. Uninstall a game\n");
        printf("0. Back to Main Menu\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice > 0 && choice <= MAX_GAMES) {
            if (store[choice-1].isInstalled) {
                store[choice-1].isInstalled = 0;
                printf("Game uninstalled successfully!\n");
            } else {
                printf("Invalid selection.\n");
            }
            printf("Press Enter to continue...");
            getchar(); getchar(); 
        }
    } while (choice != 0);
}

void showStore() {
    int choice;
    do {
        clearScreen();
        printf("=== CONSOLE GAME STORE ===\n");
        printf("Available Space: %d GB\n\n", TOTAL_CAPACITY - getUsedSpace());

        for (int i = 0; i < MAX_GAMES; i++) {
            printf("%d. %-20s | Size: %3d GB | Status: %s\n", 
                   i + 1, store[i].name, store[i].sizeGB, 
                   (store[i].isInstalled ? "[Installed]" : "[Available]"));
        }

        printf("\nSelect a game number to Install (0 to go back): ");
        scanf("%d", &choice);

        if (choice > 0 && choice <= MAX_GAMES) {
            int idx = choice - 1;
            if (store[idx].isInstalled) {
                printf("Game is already installed!\n");
            } else if (getUsedSpace() + store[idx].sizeGB > TOTAL_CAPACITY) {
                printf("Not enough space on console!\n");
            } else {
                store[idx].isInstalled = 1;
                printf("Installing %s...\nDone!\n", store[idx].name);
            }
            printf("Press Enter to continue...");
            getchar(); getchar();
        }
    } while (choice != 0);
}

int main() {
    int mainChoice;
    do {
        clearScreen();
        printf("======= CONSOLE MANAGEMENT SYSTEM =======\n");
        printf("1. View/Install Games from Store\n");
        printf("2. Manage Installed Games (Uninstall)\n");
        printf("0. Exit Application\n");
        printf("=========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &mainChoice);

        switch (mainChoice) {
            case 1:
                showStore(); 
                break;
            case 2:
                showInstalledGames(); 
                break;
            case 0:
                printf("Exiting... Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Press Enter to try again.");
                getchar(); getchar();
        }
    } while (mainChoice != 0);

    return 0;
}