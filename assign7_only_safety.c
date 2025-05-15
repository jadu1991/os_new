#include <stdio.h> 

#include <stdbool.h> 

  

#define MAX 10 

  

int main() { 

    int n, m; // n = number of processes, m = number of resources 

    int alloc[MAX][MAX], max[MAX][MAX], need[MAX][MAX]; 

    int avail[MAX], work[MAX], finish[MAX] = {0}; 

    int safeSequence[MAX], count = 0; 

  

    // Input number of processes and resources 

    printf("Enter number of processes: "); 

    scanf("%d", &n); 

  

    printf("Enter number of resource types: "); 

    scanf("%d", &m); 

  

    // Input Allocation Matrix 

    printf("\nEnter Allocation Matrix:\n"); 

    for (int i = 0; i < n; i++) { 

        printf("Process %d: ", i); 

        for (int j = 0; j < m; j++) { 

            scanf("%d", &alloc[i][j]); 

        } 

    } 

  

    // Input Maximum Matrix 

    printf("\nEnter Maximum Matrix:\n"); 

    for (int i = 0; i < n; i++) { 

        printf("Process %d: ", i); 

        for (int j = 0; j < m; j++) { 

            scanf("%d", &max[i][j]); 

            need[i][j] = max[i][j] - alloc[i][j]; // Calculate Need matrix 

        } 

    } 

  

    // Input Available resources 

    printf("\nEnter Available Resources: "); 

    for (int i = 0; i < m; i++) { 

        scanf("%d", &avail[i]); 

        work[i] = avail[i]; // Initialize Work = Available 

    } 

  

    // Safety Algorithm 

    while (count < n) { 

        bool found = false; 

        for (int i = 0; i < n; i++) { 

            if (!finish[i]) { 

                bool canAllocate = true; 

                for (int j = 0; j < m; j++) { 

                    if (need[i][j] > work[j]) { 

                        canAllocate = false; 

                        break; 

                    } 

                } 

                if (canAllocate) { 

                    for (int k = 0; k < m; k++) { 

                        work[k] += alloc[i][k]; 

                    } 

                    safeSequence[count++] = i; 

                    finish[i] = 1; 

                    found = true; 

                } 

            } 

        } 

        if (!found) { 

            break; // No suitable process found in this iteration 

        } 

    } 

  

    // Final result 

    if (count == n) { 

        printf("\nSystem is in a Safe State.\nSafe Sequence: "); 

        for (int i = 0; i < n; i++) { 

            printf("P%d ", safeSequence[i]); 

        } 

        printf("\n"); 

    } else { 

        printf("\nSystem is NOT in a Safe State!\n"); 

    } 

  

    return 0; 

} 