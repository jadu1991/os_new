// Assignment 2.2 : Find Maximum and Minimum Using Forked Processes 
 
 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
  
int do_parent(int arr[], int size) { 
    int max = arr[0]; 
    for (int i = 1; i < size; i++) { 
        if (arr[i] > max) { 
            max = arr[i]; 
        } 
    } 
    return max; 
} 
  
int do_child(int arr[], int size) { 
    int min = arr[0]; 
    for (int i = 1; i < size; i++) { 
        if (arr[i] < min) { 
            min = arr[i]; 
        } 
    } 
    return min; 
} 
  
int main() { 
    int arr[50], size, i; 
    pid_t pid; 
  
    printf("Enter the size of array: "); 
    scanf("%d", &size); 
  
    printf("Enter array elements: "); 
    for (i = 0; i < size; i++) { 
        scanf("%d", &arr[i]); 
    } 
  
    pid = fork();  // Create child process 
  
    if (pid < 0) { 
        perror("Fork failed"); 
        return 1; 
    } 
  
    if (pid == 0) { 
        // Child process: Find minimum 
        int min = do_child(arr, size); 
        printf("Child Process - Minimum is: %d\n", min); 
    } else { 
        // Parent process: Find maximum 
        int max = do_parent(arr, size); 
        wait(NULL);  // Wait for child to complete 
        printf("Parent Process - Maximum is: %d\n", max); 
    } 
  
    return 0; 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
// Assignment 2.3 -Sort Array in Ascending and Descending Order Using Forked Processes 
 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
  
void do_child(int arr[], int size) { 
    // Sorting in ascending order using bubble sort 
    for (int i = 0; i < size - 1; i++) { 
        for (int j = i + 1; j < size; j++) { 
            if (arr[i] > arr[j]) { 
                int temp = arr[i]; 
                arr[i] = arr[j]; 
                arr[j] = temp; 
            } 
        } 
    } 
  
    printf("Child Process - Ascending Order: "); 
    for (int i = 0; i < size; i++) { 
        printf("%d ", arr[i]); 
    } 
    printf("\n"); 
} 
  
void do_parent(int arr[], int size) { 
    // Sorting in descending order using bubble sort 
    for (int i = 0; i < size - 1; i++) { 
        for (int j = i + 1; j < size; j++) { 
            if (arr[i] < arr[j]) { 
                int temp = arr[i]; 
                arr[i] = arr[j]; 
                arr[j] = temp; 
            } 
        } 
    } 
  
    printf("Parent Process - Descending Order: "); 
    for (int i = 0; i < size; i++) { 
        printf("%d ", arr[i]); 
    } 
    printf("\n"); 
} 
  
int main() { 
    int arr[50], size, i; 
  
    printf("Enter the size of the array: "); 
    scanf("%d", &size); 
  
    printf("Enter array elements: "); 
    for (i = 0; i < size; i++) { 
        scanf("%d", &arr[i]); 
    } 
  
    int x = fork(); 
  
    if (x == -1) { 
        perror("Fork failed"); 
        return 1; 
    } 
  
    if (x == 0) { 
        // Child process: Sort in ascending order 
        do_child(arr, size); 
    } else { 
        // Parent process: Wait for child to finish and sort in descending order 
        wait(NULL); 
        do_parent(arr, size); 
    } 
  
    return 0; 
} 