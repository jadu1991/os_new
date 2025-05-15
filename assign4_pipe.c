#include<stdio.h> 

#include<unistd.h> 

#include<sys/types.h> 

#include<ctype.h> 

 

// Function to convert a string to uppercase 

void convert(char *str); 

 

int main() 

{ 

    int fd1[2];  // Pipe for parent to child communication 

    int fd2[2];  // Pipe for child to parent communication 

    char buff[10];   // Buffer to store data from pipe 

    char buff3[10];  // Buffer to store the transformed data 

    pid_t x;   // Process ID 

 

    // Create two pipes 

    pipe(fd1); 

    pipe(fd2); 

 

    // Fork the process (creating a child process) 

    x = fork(); 

 

    // Child process block (x == 0) 

    if(x == 0) 

    { 

        // Close the write end of fd1 as we are only reading from it 

        close(fd1[1]); 

        // Read data from the parent through fd1 (up to 6 bytes) 

        read(fd1[0], buff, 6); 

        // Convert the string to uppercase 

        convert(buff); 

        // Close the read end of fd1 

        close(fd1[0]); 

 

        // Close the read end of fd2 as we are writing to it 

        close(fd2[0]); 

        // Write the transformed string to fd2 (send it to the parent) 

        write(fd2[1], buff, 6); 

        // Close the write end of fd2 

        close(fd2[1]); 

    } 

    else 

    { 

        // Parent process block (x != 0) 

        // Close the read end of fd1 as we are only writing to it 

        close(fd1[0]); 

        // Write the string "hello" to fd1 (send it to the child) 

        write(fd1[1], "hello", 6); 

        // Close the write end of fd1 

        close(fd1[1]); 

 

        // Close the write end of fd2 as we are only reading from it 

        close(fd2[1]); 

        // Read the transformed string from fd2 (from the child) 

        read(fd2[0], buff3, 6); 

        // Print the transformed string 

        printf("msg=%s\n", buff3); 

    } 

 

    return 0; 

} 

 

// Function to convert the string to uppercase 

void convert(char *str) 

{ 

    // Loop through each character of the string and convert to uppercase 

    while(*str != '\0') 

    { 

        *str = toupper(*str); 

        str++; 

    } 

} 