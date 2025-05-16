#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

struct Process {
    int pid, arrival_time, burst_time, priority;
    int waiting_time, turnaround_time, completion_time, remaining_time;
};

void swap(struct Process *a, struct Process *b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

void displayNonPriority(struct Process p[], int n, const char* algo, char execution_order[]) {
    printf("\n%s Scheduling:\n", algo);
    printf("Order of Execution: %s\n", execution_order);
    printf("PID\tAT\tBT\tCT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].completion_time, p[i].waiting_time);
    }
}

void displayPriority(struct Process p[], int n, const char* algo, char execution_order[]) {
    printf("\n%s Scheduling:\n", algo);
    printf("Order of Execution: %s\n", execution_order);
    printf("PID\tAT\tBT\tPriority\tCT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\n", p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].priority, p[i].completion_time, p[i].waiting_time);
    }
}

void calculateFCFS(struct Process p[], int n, char execution_order[]) {
    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival_time)
            time = p[i].arrival_time;
        p[i].completion_time = time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        time = p[i].completion_time;
        execution_order[i] = 'P' + p[i].pid;
    }
    execution_order[n] = '\0';
}

void calculateSJF(struct Process p[], int n, char execution_order[]) {
    // Sort by arrival time first to ensure correct order for SJF non-preemptive
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (p[j].arrival_time < p[i].arrival_time)
                swap(&p[i], &p[j]);

    int completed = 0, time = 0;
    bool done[n];
    for(int i=0; i<n; i++) done[i] = false;

    int idx = 0;
    while(completed < n){
        int shortest = -1;
        int min_bt = INT_MAX;
        for(int i=0; i<n; i++){
            if(p[i].arrival_time <= time && !done[i] && p[i].burst_time < min_bt){
                min_bt = p[i].burst_time;
                shortest = i;
            }
        }
        if(shortest == -1){
            time++;
        } else {
            execution_order[idx++] = 'P' + p[shortest].pid;
            time += p[shortest].burst_time;
            p[shortest].completion_time = time;
            p[shortest].turnaround_time = p[shortest].completion_time - p[shortest].arrival_time;
            p[shortest].waiting_time = p[shortest].turnaround_time - p[shortest].burst_time;
            done[shortest] = true;
            completed++;
        }
    }
    execution_order[idx] = '\0';
}

void calculateSJFPreemptive(struct Process p[], int n, char execution_order[]) {
    int time = 0, completed = 0, min_index;
    bool is_completed[n];
    for (int i = 0; i < n; i++) {
        is_completed[i] = false;
        p[i].remaining_time = p[i].burst_time;
    }

    int idx = 0;
    while (completed != n) {
        min_index = -1;
        int min_remaining = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= time && !is_completed[i] && p[i].remaining_time < min_remaining) {
                min_remaining = p[i].remaining_time;
                min_index = i;
            }
        }

        if (min_index == -1) {
            time++;
        } else {
            execution_order[idx++] = 'P' + p[min_index].pid;
            p[min_index].remaining_time--;
            time++;

            if (p[min_index].remaining_time == 0) {
                p[min_index].completion_time = time;
                p[min_index].turnaround_time = p[min_index].completion_time - p[min_index].arrival_time;
                p[min_index].waiting_time = p[min_index].turnaround_time - p[min_index].burst_time;
                is_completed[min_index] = true;
                completed++;
            }
        }
    }
    execution_order[idx] = '\0';
}

void calculatePriority(struct Process p[], int n, char execution_order[]) {
    // Sort based on priority (lower number = higher priority), break ties by arrival time
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if ((p[j].priority < p[i].priority) || 
               (p[j].priority == p[i].priority && p[j].arrival_time < p[i].arrival_time))
                swap(&p[i], &p[j]);

    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival_time)
            time = p[i].arrival_time;
        p[i].completion_time = time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        time = p[i].completion_time;
        execution_order[i] = 'P' + p[i].pid;
    }
    execution_order[n] = '\0';
}

void calculateRoundRobin(struct Process p[], int n, int quantum, char execution_order[]) {
    int time = 0, completed = 0, idx = 0;
    for (int i = 0; i < n; i++)
        p[i].remaining_time = p[i].burst_time;

    bool done;
    while (completed < n) {
        done = true;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining_time > 0 && p[i].arrival_time <= time) {
                done = false;
                execution_order[idx++] = 'P' + p[i].pid;

                if (p[i].remaining_time > quantum) {
                    time += quantum;
                    p[i].remaining_time -= quantum;
                } else {
                    time += p[i].remaining_time;
                    p[i].remaining_time = 0;
                    p[i].completion_time = time;
                    p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
                    p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
                    completed++;
                }
            } else if(p[i].remaining_time > 0 && p[i].arrival_time > time) {
                // If process has not arrived yet, increment time to arrival
                time = p[i].arrival_time;
                i--; // Recheck this process now that time has advanced
            }
        }
        if (done) break;
    }
    execution_order[idx] = '\0';
}

int main() {
    int n, choice, quantum;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    printf("Enter arrival time and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        p[i].priority = 0; // Initialize priority to 0 (not used except in priority scheduling)
        printf("P%d (AT BT): ", i + 1);
        scanf("%d %d", &p[i].arrival_time, &p[i].burst_time);
    }

    char execution_order[100];

    do {
        printf("\nCPU Scheduling Algorithms Menu:");
        printf("\n1. First Come First Serve (FCFS)");
        printf("\n2. Shortest Job First (SJF) - Non-Preemptive");
        printf("\n3. Shortest Job First (SJF) - Preemptive");
        printf("\n4. Priority Scheduling (Non-Preemptive)");
        printf("\n5. Round Robin");
        printf("\n6. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                calculateFCFS(p, n, execution_order);
                displayNonPriority(p, n, "FCFS", execution_order);
                break;

            case 2:
                calculateSJF(p, n, execution_order);
                displayNonPriority(p, n, "SJF (Non-Preemptive)", execution_order);
                break;

            case 3:
                calculateSJFPreemptive(p, n, execution_order);
                displayNonPriority(p, n, "SJF (Preemptive)", execution_order);
                break;

            case 4:
                printf("Enter priority for each process:\n");
                for (int i = 0; i < n; i++) {
                    printf("P%d Priority: ", p[i].pid);
                    scanf("%d", &p[i].priority);
                }
                calculatePriority(p, n, execution_order);
                displayPriority(p, n, "Priority Scheduling", execution_order);
                break;

            case 5:
                printf("Enter time quantum for Round Robin: ");
                scanf("%d", &quantum);
                calculateRoundRobin(p, n, quantum, execution_order);
                displayNonPriority(p, n, "Round Robin", execution_order);
                break;

            case 6:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice! Please select again.\n");
        }

    } while (choice != 6);

    return 0;
}
