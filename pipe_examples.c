// 1.fakt string write ani print krte
// 2.uppercase string
// 3. 1 to 10 print 
// 4. 1 to 10 print ani tencha sum print


// PIPE PROGRAMMING

 

#include <unistd.h>

#include <stdio.h>

 

int main() {

  int fd[2];

  char buffer[20];

  pipe(fd);

  if (fork() == 0) {

    close(fd[0]);

    write(fd[1], "Hello", 6);

  } else {

    close(fd[1]);

    read(fd[0], buffer, 6);

    printf("Parent received: %s\n", buffer);

  }

  return 0;

}

 

// IPC using pipe
 

// 1.

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
 
int main()
{
    int fd[2];
    char str[] = "Hello";
    char str2[10];
    pid_t x;
    pipe(fd);
    pipe(fd);
    x = fork();
    if (x == 0)
    {
        close(fd[0]);
        write(fd[1], str, strlen(str) + 1);
        close(fd[1]);
    }
    else
    {
        close(fd[1]);
        read(fd[0], str2, strlen(str) + 1);
        close(fd[0]);
        printf("msg=%s\n", str2);
    }
    return 0;
}
 

// 2.
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
 
void convert(char *str)
{
    while (*str != '\0')
    {
        *str = toupper(*str);
        str++;
    }
}
 
int main()
{
    int fd1[2];
    int fd2[2];
    char buff[10];
    char buff2[10];
    char buff3[10];
    pid_t x;
    pipe(fd1);
    pipe(fd2);
    x = fork();
    if (x == 0)
    {
        close(fd1[1]);
        read(fd1[0], buff, 6);
        convert(buff2);
        close(fd1[0]);
 
        close(fd2[0]);
        write(fd2[1], buff2, 6);
        close(fd2[1]);
    }
    else
    {
        close(fd1[0]);
        read(fd1[1], "hello", 6);
        close(fd1[1]);
 
        close(fd2[1]);
        read(fd2[0], buff3, 6);
        printf("msg=%s\n", buff3);
    }
    return 0;
}
 



// 3.
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
 
int main()
{
    pid_t x;
    int fd[2];
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int e;
    pipe(fd);
    x = fork();
    if (x == 0)
    {
        close(fd[1]);
        while ((read(fd[0], &e, sizeof(e))) > 0)
        {
 
            printf("e=%d\n", e);
        }
        close(fd[0]);
    }
    else
    {
        close(fd[0]);
        write(fd[1], a, sizeof(a));
        close(fd[1]);
    }
    return 0;
}
 






// 4.
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
 
int main()
{
    pid_t x;
    int fd1[2], fd2[2];
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int e, sum = 0, result;
    pipe(fd1);
    pipe(fd2);
    x = fork();
    if (x == 0)
    {
        close(fd1[1]);
        while ((read(fd1[0], &e, sizeof(e))) > 0)
        {
            printf("e=%d\n", e);
            sum = sum + e;
        }
 
        close(fd1[0]);
        close(fd2[0]);
        write(fd2[1], &sum, sizeof(sum));
        close(fd2[1]);
    }
    else
    {
        close(fd1[0]);
        write(fd1[1], a, sizeof(a));
        close(fd1[1]);
        close(fd2[1]);
        while ((read(fd2[0], &result, sizeof(result))) > 0)
            printf("result=%d\n", result);
        close(fd2[0]);
    }
    return 0;
}