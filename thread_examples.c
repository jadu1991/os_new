Assignment 3.1 -Multithreaded Matrix Multiplication using POSIX Threads

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10

int matA[MAX][MAX];
int matB[MAX][MAX];
int result[MAX][MAX];

int r1, c1, r2, c2;

// Structure to pass data to thread
typedef struct {
    int row;
    int col;
} MatrixIndex;

// Thread function to compute one element of the result matrix
void* multiply(void* arg) {
    MatrixIndex* data = (MatrixIndex*)arg;
    int sum = 0;
    for (int k = 0; k < c1; k++) {
        sum += matA[data->row][k] * matB[k][data->col];
    }
    result[data->row][data->col] = sum;
    free(arg);
    pthread_exit(NULL);
}

int main() {
    printf("Enter rows and columns for Matrix A: ");
    scanf("%d%d", &r1, &c1);
    printf("Enter rows and columns for Matrix B: ");
    scanf("%d%d", &r2, &c2);

    if (c1 != r2) {
        printf("Matrix multiplication not possible! Columns of A must match rows of B.\n");
        return -1;
    }

    printf("Enter elements of Matrix A:\n");
    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c1; j++)
            scanf("%d", &matA[i][j]);

    printf("Enter elements of Matrix B:\n");
    for (int i = 0; i < r2; i++)
        for (int j = 0; j < c2; j++)
            scanf("%d", &matB[i][j]);

    pthread_t threads[MAX * MAX];
    int threadCount = 0;

    // Creating a thread for each element in the result matrix
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            MatrixIndex* data = (MatrixIndex*)malloc(sizeof(MatrixIndex));
            data->row = i;
            data->col = j;
            pthread_create(&threads[threadCount++], NULL, multiply, data);
        }
    }

    // Joining all threads
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nResultant Matrix:\n");
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
































Assignment 3.2 -Multithreaded Arithmetic Operations in C using pthreads



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int num1, num2;

// Thread functions
void* add(void* arg) {
    printf("Addition: %d + %d = %d\n", num1, num2, num1 + num2);
    pthread_exit(NULL);
}

void* subtract(void* arg) {
    printf("Subtraction: %d - %d = %d\n", num1, num2, num1 - num2);
    pthread_exit(NULL);
}

void* multiply(void* arg) {
    printf("Multiplication: %d * %d = %d\n", num1, num2, num1 * num2);
    pthread_exit(NULL);
}

void* divide(void* arg) {
    if (num2 != 0)
        printf("Division: %d / %d = %.2f\n", num1, num2, (float)num1 / num2);
    else
        printf("Division: Cannot divide by zero!\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2, t3, t4;

    printf("Enter two integers: ");
    scanf("%d %d", &num1, &num2);

    // Create threads
    pthread_create(&t1, NULL, add, NULL);
    pthread_create(&t2, NULL, subtract, NULL);
    pthread_create(&t3, NULL, multiply, NULL);
    pthread_create(&t4, NULL, divide, NULL);

    // Wait for threads to complete
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    return 0;
}









































Assignment 3.3 -Basic Thread Creation Without pthread_join


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread(void *vargp);

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread, NULL);
    sleep(1); // Allow thread to finish before exit
    exit(0);
}

void *thread(void *vargp) {
    printf("Hello World!\n");
    return NULL;
}}







Assignment 3.4 -Thread Creation With pthread_join

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *thread(void *vargp);

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread, NULL);
    pthread_join(tid, NULL);  // Wait for thread to finish
    return 0;
}

void *thread(void *vargp) {
    printf("Hello World!\n");
    return NULL;
}








  
Assignment 3.5 -Passing Structure to Thread and Returning Sum


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int num1;
    int num2;
} NUM;

void* sum_function(void *argp);

int main() {
    pthread_t th1;
    NUM N1;
    int n1, n2;
    void *ret_val;

    printf("Enter num1: ");
    scanf("%d", &n1);
    printf("Enter num2: ");
    scanf("%d", &n2);

    N1.num1 = n1;
    N1.num2 = n2;

    pthread_create(&th1, NULL, sum_function, (void*) &N1);
    pthread_join(th1, &ret_val);

    printf("Sum = %d\n", (int)(long)ret_val);  // Typecast to int safely
    return 0;
}

void* sum_function(void *argp) {
    NUM *N2 = (NUM*) argp;
    int sum = N2->num1 + N2->num2;
    return (void*)(long)sum;  // Return as void*
}



















Assignment 3.6 Shared Variable Protected by Semaphore


#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define NUM_ITERATIONS 100000

int counter = 0;
sem_t sem;

void* increment(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        sem_wait(&sem);
        counter++;
        sem_post(&sem);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    sem_init(&sem, 0, 1);  // Binary semaphore

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final Counter Value: %d\n", counter);

    sem_destroy(&sem);
    return 0;
}



































Assignment 3.7 - Write a C program using POSIX threads to create two threads

#include <stdio.h>
#include <pthread.h>

// Thread function to print the message
void* display(void* arg) {
    char* msg = (char*) arg; // Properly cast to char*
    printf("Message is = %s\n", msg);
    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    char* msg1 = "hi hello";
    char* msg2 = "second thread message";

    // Create threads with different messages
    pthread_create(&tid1, NULL, display, (void*) msg1);
    pthread_create(&tid2, NULL, display, (void*) msg2);

    // Wait for both threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}





Assignment 3.8 -Write a C program using POSIX threads to create two threads. Pass two different integers to each thread.

#include <stdio.h>
#include <pthread.h>

// Thread function to display the passed integer
void* display(void* arg) {
    int* numPtr = (int*) arg; // Correctly cast to int pointer
    printf("Number is = %d\n", *numPtr);
    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    int n1 = 5;
    int n2 = 10;

    // Create threads and pass addresses of integers
    pthread_create(&tid1, NULL, display, (void*) &n1);
    pthread_create(&tid2, NULL, display, (void*) &n2);

    // Wait for threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}