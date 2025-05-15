#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILE_NAME "student_db.txt"
#define MAX 100

struct Student {
    int id;
    char name[50];
    int age;
};

// Function prototypes
void createDatabase();
void viewDatabase();
void insertRecord();
void deleteRecord();
void updateRecord();
void searchRecord();

int main() {
    int choice;
    while (1) {
        printf("\n----- Student Database Menu -----\n");
        printf("1. Create\n2. View\n3. Insert\n4. Delete\n5. Update\n6. Search\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // to consume newline

        switch (choice) {
            case 1: createDatabase(); break;
            case 2: viewDatabase(); break;
            case 3: insertRecord(); break;
            case 4: deleteRecord(); break;
            case 5: updateRecord(); break;
            case 6: searchRecord(); break;
            case 7: exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}

// 1. Create new empty database
void createDatabase() {
    int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Create failed");
        return;
    }
    close(fd);
    printf("Database created successfully.\n");
}

// 2. View all records
void viewDatabase() {
    int fd = open(FILE_NAME, O_RDONLY);
    if (fd < 0) {
        perror("Open failed");
        return;
    }

    struct Student s;
    printf("\n%-5s %-20s %-5s\n", "ID", "Name", "Age");
    while (read(fd, &s, sizeof(s)) > 0) {
        printf("%-5d %-20s %-5d\n", s.id, s.name, s.age);
    }
    close(fd);
}

// 3. Insert a new student
void insertRecord() {
    int fd = open(FILE_NAME, O_WRONLY | O_APPEND);
    if (fd < 0) {
        perror("Open failed");
        return;
    }

    struct Student s;
    printf("Enter ID: ");
    scanf("%d", &s.id);
    getchar(); // consume newline
    printf("Enter Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0'; // remove newline
    printf("Enter Age: ");
    scanf("%d", &s.age);

    write(fd, &s, sizeof(s));
    close(fd);
    printf("Record inserted.\n");
}

// 4. Delete record by ID
void deleteRecord() {
    int fd1 = open(FILE_NAME, O_RDONLY);
    int fd2 = open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd1 < 0 || fd2 < 0) {
        perror("File open error");
        return;
    }

    int id, found = 0;
    struct Student s;

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (read(fd1, &s, sizeof(s)) > 0) {
        if (s.id != id) {
            write(fd2, &s, sizeof(s));
        } else {
            found = 1;
        }
    }

    close(fd1);
    close(fd2);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("Record deleted.\n");
    else
        printf("Record not found.\n");
}

// 5. Update record by ID
void updateRecord() {
    int fd = open(FILE_NAME, O_RDWR);
    if (fd < 0) {
        perror("Open failed");
        return;
    }

    int id, found = 0;
    struct Student s;

    printf("Enter ID to update: ");
    scanf("%d", &id);
    getchar(); // consume newline

    while (read(fd, &s, sizeof(s)) > 0) {
        if (s.id == id) {
            printf("Enter new name: ");
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = '\0';
            printf("Enter new age: ");
            scanf("%d", &s.age);

            lseek(fd, -sizeof(s), SEEK_CUR);
            write(fd, &s, sizeof(s));
            found = 1;
            break;
        }
    }

    close(fd);
    if (found)
        printf("Record updated.\n");
    else
        printf("Record not found.\n");
}

// 6. Search record by ID
void searchRecord() {
    int fd = open(FILE_NAME, O_RDONLY);
    if (fd < 0) {
        perror("Open failed");
        return;
    }

    int id, found = 0;
    struct Student s;

    printf("Enter ID to search: ");
    scanf("%d", &id);

    while (read(fd, &s, sizeof(s)) > 0) {
        if (s.id == id) {
            printf("Record found:\n");
            printf("ID: %d\nName: %s\nAge: %d\n", s.id, s.name, s.age);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Record not found.\n");

    close(fd);
}