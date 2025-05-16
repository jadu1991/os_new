#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    char title[50];
    char author[50];
    int year;
};

void create_database() {
    FILE *fp = fopen("books.dat", "w");
    if (fp == NULL) {
        printf("Error creating database.\n");
        return;
    }
    fclose(fp);
    printf("New database created successfully.\n");
}

void view_database() {
    FILE *fp = fopen("books.dat", "r");
    struct Book b;
    if (fp == NULL) {
        printf("Database does not exist.\n");
        return;
    }
    printf("ID\tTitle\t\tAuthor\t\tYear\n");
    printf("---------------------------------------------\n");
    while (fread(&b, sizeof(b), 1, fp)) {
        printf("%d\t%s\t\t%s\t\t%d\n", b.id, b.title, b.author, b.year);
    }
    fclose(fp);
}

void insert_book() {
    FILE *fp = fopen("books.dat", "a");
    struct Book b;
    if (fp == NULL) {
        printf("Database does not exist. Please create it first.\n");
        return;
    }
    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    getchar(); // consume newline
    printf("Enter Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = 0;
    printf("Enter Author: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = 0;
    printf("Enter Year: ");
    scanf("%d", &b.year);

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);
    printf("Book inserted successfully.\n");
}

void modify_book() {
    FILE *fp = fopen("books.dat", "r+");
    struct Book b;
    int id, found = 0;

    if (fp == NULL) {
        printf("Database does not exist.\n");
        return;
    }

    printf("Enter Book ID to modify: ");
    scanf("%d", &id);
    
    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == id) {
            found = 1;
            printf("Enter new Title: ");
            getchar();
            fgets(b.title, sizeof(b.title), stdin);
            b.title[strcspn(b.title, "\n")] = 0;
            printf("Enter new Author: ");
            fgets(b.author, sizeof(b.author), stdin);
            b.author[strcspn(b.author, "\n")] = 0;
            printf("Enter new Year: ");
            scanf("%d", &b.year);

            fseek(fp, -sizeof(b), SEEK_CUR);
            fwrite(&b, sizeof(b), 1, fp);
            printf("Book modified successfully.\n");
            break;
        }
    }

    if (!found)
        printf("Book ID not found.\n");

    fclose(fp);
}

void delete_book() {
    FILE *fp = fopen("books.dat", "r");
    FILE *temp = fopen("temp.dat", "w");
    struct Book b;
    int id, found = 0;

    if (fp == NULL || temp == NULL) {
        printf("Database not found.\n");
        return;
    }

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == id) {
            found = 1;
        } else {
            fwrite(&b, sizeof(b), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);
    remove("books.dat");
    rename("temp.dat", "books.dat");

    if (found)
        printf("Book deleted successfully.\n");
    else
        printf("Book ID not found.\n");
}

void search_book() {
    FILE *fp = fopen("books.dat", "r");
    struct Book b;
    int id, found = 0;

    if (fp == NULL) {
        printf("Database does not exist.\n");
        return;
    }

    printf("Enter Book ID to search: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == id) {
            found = 1;
            printf("Book Found:\n");
            printf("ID    : %d\n", b.id);
            printf("Title : %s\n", b.title);
            printf("Author: %s\n", b.author);
            printf("Year  : %d\n", b.year);
            break;
        }
    }

    if (!found)
        printf("Book ID not found.\n");

    fclose(fp);
}

void menu() {
    int choice;
    while (1) {
        printf("\n===== Book Management System =====\n");
        printf("1. Create Database\n");
        printf("2. View Database\n");
        printf("3. Insert Book\n");
        printf("4. Modify Book\n");
        printf("5. Delete Book\n");
        printf("6. Search Book\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: create_database(); break;
            case 2: view_database(); break;
            case 3: insert_book(); break;
            case 4: modify_book(); break;
            case 5: delete_book(); break;
            case 6: search_book(); break;
            case 7: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

int main() {
    menu();
    return 0;
}
