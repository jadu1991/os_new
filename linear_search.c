#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 100

int arr[MAX], n = 0;

void inputArray() {
    printf("Enter number of elements: ");
    scanf("%d", &n);
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
}

void linearSearch(int key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            printf("Linear Search (Parent): Element %d found at index %d\n", key, i);
            return;
        }
    }
    printf("Linear Search (Parent): Element %d not found\n", key);
}

void binarySearch(int key) {
    int sorted[MAX];
    for (int i = 0; i < n; i++)
        sorted[i] = arr[i];

    // Sort the array before binary search
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (sorted[i] > sorted[j]) {
                int temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }

    int low = 0, high = n - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (sorted[mid] == key) {
            printf("Binary Search (Child): Element %d found at index %d (in sorted array)\n", key, mid);
            return;
        } else if (sorted[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    printf("Binary Search (Child): Element %d not found\n", key);
}

int main() {
    int choice, key;

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Enter Array\n");
        printf("2. Search Element (Linear & Binary using fork)\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                inputArray();
                break;

            case 2:
                if (n == 0) {
                    printf("Array is empty! Please enter the array first.\n");
                    break;
                }
                printf("Enter element to search: ");
                scanf("%d", &key);

                pid_t pid = fork();

                if (pid < 0) {
                    printf("Fork failed.\n");
                    exit(1);
                } else if (pid == 0) {
                    // Child process
                    binarySearch(key);
                    exit(0); // End child after binary search
                } else {
                    // Parent process
                    wait(NULL); // Wait for child to complete
                    linearSearch(key);
                }
                break;

            case 3:
                printf("Exiting program.\n");
                exit(0);

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
