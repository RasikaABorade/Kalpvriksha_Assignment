#include <stdio.h>
#include <stdlib.h>

#define FILENAME "users.txt"

struct User {
    int id;
    char name[50];
    int age;
};

void ensureFileExists() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Error creating file.\n");
        exit(1);
    }
    fclose(file);
}

void addUser() {
    FILE *file = fopen(FILENAME, "a");
    struct User user;

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &user.id);

    printf("Enter Name: ");
    scanf(" %[^\n]", user.name);

    printf("Enter Age: ");
    scanf("%d", &user.age);

    fprintf(file, "%d|%s|%d\n", user.id, user.name, user.age);
    fclose(file);

    printf("User added successfully!\n");
}

void displayUsers() {
    FILE *file = fopen(FILENAME, "r");
    struct User user;
    char line[200];

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nList of Users:\n");
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d|%[^|]|%d", &user.id, user.name, &user.age) == 3) {
            printf("ID: %d, Name: %s, Age: %d\n", user.id, user.name, user.age);
        }
    }

    fclose(file);
}

void updateUser() {
    FILE *file = fopen(FILENAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    struct User user;
    char line[200];
    int id, found = 0;

    printf("Enter user ID to update: ");
    scanf("%d", &id);

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d|%[^|]|%d", &user.id, user.name, &user.age) == 3) {
            if (user.id == id) {
                found = 1;
                printf("Enter new Name: ");
                scanf(" %[^\n]", user.name);
                printf("Enter new Age: ");
                scanf("%d", &user.age);
            }
            fprintf(tempFile, "%d|%s|%d\n", user.id, user.name, user.age);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("User updated successfully.\n");
    else
        printf("User with ID %d not found.\n", id);
}

void deleteUser() {
    FILE *file = fopen(FILENAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    struct User user;
    char line[200];
    int id, found = 0;

    printf("Enter user ID to delete: ");
    scanf("%d", &id);

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d|%[^|]|%d", &user.id, user.name, &user.age) == 3) {
            if (user.id == id) {
                found = 1;
                continue;
            }
            fprintf(tempFile, "%d|%s|%d\n", user.id, user.name, user.age);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("User deleted successfully.\n");
    else
        printf("User with ID %d not found.\n", id);
}

int main() {
    ensureFileExists();

    int choice;
    do {
        printf("\nSelect an option:\n");
        printf("1. Add User\n");
        printf("2. Display Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addUser(); 
            break;
            case 2: displayUsers(); 
            break;
            case 3: updateUser();
            break;
            case 4: deleteUser();
            break;
            case 5: printf("Exited.\n"); break;
            default: printf("Invalid choice! Please ry again.\n");
        }
    } while (choice != 5);

    return 0;
}
