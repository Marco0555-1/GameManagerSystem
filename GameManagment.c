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
#define DATA_FILE "games.txt"

Game store[MAX_GAMES];
int gameCount = 0;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void saveToFile() {
    FILE *f = fopen(DATA_FILE, "w");
    if (!f) {
        printf("Warning: Could not save data to file.\n");
        return;
    }
    for (int i = 0; i < gameCount; i++) {
        fprintf(f, "%s,%d,%d\n", store[i].name, store[i].sizeGB, store[i].isInstalled);
    }
    fclose(f);
}

void loadFromFile() {
    FILE *f = fopen(DATA_FILE, "r");
    if (!f) {
        printf("Error: games.txt not found! Please add the file and restart.\n");
        exit(1);
    }
    gameCount = 0;
    while (gameCount < MAX_GAMES &&
           fscanf(f, "%49[^,],%d,%d\n",
                  store[gameCount].name,
                  &store[gameCount].sizeGB,
                  &store[gameCount].isInstalled) == 3) {
        gameCount++;
    }
    fclose(f);
}

int getUsedSpace() {
    int used = 0;
    for (int i = 0; i < gameCount; i++) {
        if (store[i].isInstalled) used += store[i].sizeGB;
    }
    return used;
}

void getSortedIndices(int *indices, int count, int sortMode) {
    for (int i = 0; i < count; i++) indices[i] = i;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int swap = 0;
            if (sortMode == 1)
                swap = strcmp(store[indices[j]].name, store[indices[j+1]].name) > 0;
            else if (sortMode == 2)
                swap = store[indices[j]].sizeGB > store[indices[j+1]].sizeGB;
            else if (sortMode == 3)
                swap = store[indices[j]].sizeGB < store[indices[j+1]].sizeGB;
            if (swap) {
                int tmp = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = tmp;
            }
        }
    }
}

void askSortMode(int *sortMode) {
    printf("Sort by: 1) Name  2) Size (asc)  3) Size (desc)  (current: %s) >> ",
           *sortMode == 1 ? "Name" : *sortMode == 2 ? "Size (asc)" : *sortMode == 3 ? "Size (desc)" : "Default");
    int input;
    if (scanf("%d", &input) == 1 && (input == 1 || input == 2 || input == 3))
        *sortMode = input;
}

void showStore() {
    int choice;
    int sortMode = 0;
    int indices[MAX_GAMES];
    char input[10];

    do {
        clearScreen();
        printf("=== CONSOLE GAME STORE ===\n");
        printf("Available Space: %d GB\n", TOTAL_CAPACITY - getUsedSpace());
        printf("Sorted by: %s\n\n",
               sortMode == 1 ? "Name" : sortMode == 2 ? "Size (asc)" : sortMode == 3 ? "Size (desc)" : "Default");

        getSortedIndices(indices, gameCount, sortMode);

        for (int i = 0; i < gameCount; i++) {
            int idx = indices[i];
            printf("%d. %-20s | Size: %3d GB | Status: %s\n",
                   i + 1, store[idx].name, store[idx].sizeGB,
                   store[idx].isInstalled ? "[Installed]" : "[Available]");
        }

        printf("\nOptions:\n");
        printf("  1-%d : Install game\n", gameCount);
        printf("  S   : Change sort order\n");
        printf("  0   : Back to Main Menu\n");
        printf("Choice: ");

        scanf("%9s", input);
        choice = 0;

        if (input[0] == 'S' || input[0] == 's') {
            askSortMode(&sortMode);
        } else {
            choice = atoi(input);
            if (choice > 0 && choice <= gameCount) {
                int idx = indices[choice - 1];
                if (store[idx].isInstalled) {
                    printf("Game is already installed!\n");
                } else if (getUsedSpace() + store[idx].sizeGB > TOTAL_CAPACITY) {
                    printf("Not enough space on console!\n");
                } else {
                    store[idx].isInstalled = 1;
                    saveToFile();
                    printf("Installing %s...\nDone!\n", store[idx].name);
                }
                printf("Press Enter to continue...");
                getchar(); getchar();
            }
        }
    } while (choice != 0 || input[0] == 'S' || input[0] == 's');
}

void showInstalledGames() {
    int choice;
    int sortMode = 0;
    int indices[MAX_GAMES];
    int displayMap[MAX_GAMES];
    char input[10];

    do {
        clearScreen();
        printf("=== INSTALLED GAMES MANAGEMENT ===\n");
        printf("Current Storage: %d/%d GB\n", getUsedSpace(), TOTAL_CAPACITY);
        printf("Sorted by: %s\n\n",
               sortMode == 1 ? "Name" : sortMode == 2 ? "Size (asc)" : sortMode == 3 ? "Size (desc)" : "Default");

        getSortedIndices(indices, gameCount, sortMode);

        int found = 0;
        int displayCount = 0;
        for (int i = 0; i < gameCount; i++) {
            int idx = indices[i];
            if (store[idx].isInstalled) {
                displayCount++;
                displayMap[displayCount] = idx;
                printf("%d. %s (%d GB)\n", displayCount, store[idx].name, store[idx].sizeGB);
                found = 1;
            }
        }
        if (!found) printf("No games installed yet.\n");

        printf("\nOptions:\n");
        printf("  1-%d : Uninstall game\n", displayCount > 0 ? displayCount : gameCount);
        printf("  S   : Change sort order\n");
        printf("  0   : Back to Main Menu\n");
        printf("Choice: ");

        scanf("%9s", input);
        choice = 0;

        if (input[0] == 'S' || input[0] == 's') {
            askSortMode(&sortMode);
        } else {
            choice = atoi(input);
            if (choice > 0 && choice <= displayCount) {
                int idx = displayMap[choice];
                store[idx].isInstalled = 0;
                saveToFile();
                printf("%s uninstalled successfully!\n", store[idx].name);
                printf("Press Enter to continue...");
                getchar(); getchar();
            } else if (choice != 0) {
                printf("Invalid selection.\n");
                printf("Press Enter to continue...");
                getchar(); getchar();
            }
        }
    } while (choice != 0 || input[0] == 'S' || input[0] == 's');
}

int main() {
    loadFromFile();

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
            case 1: showStore(); break;
            case 2: showInstalledGames(); break;
            case 0: printf("Exiting... Goodbye!\n"); break;
            default:
                printf("Invalid choice! Press Enter to try again.");
                getchar(); getchar();
        }
    } while (mainChoice != 0);
    return 0;
}
