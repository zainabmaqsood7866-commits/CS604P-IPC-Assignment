#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int pipe1[2], pipe2[2];
    pid_t pid;

    char message[] = "How are you? What's your VUID?";
    char reply[] = "I am fine. My VUID is BC250200976";
    char received[100];
    int n;

    // Create two pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        printf("Pipe creation failed!\n");
        return 1;
    }

    // Create child process
    pid = fork();

    if (pid < 0)
    {
        printf("Fork failed!\n");
        return 1;
    }

    if (pid == 0)
    {
        // Child process
        close(pipe1[1]);
        close(pipe2[0]);

        // Read message from parent
        n = read(pipe1[0], received, sizeof(received) - 1);

        if (n > 0)
        {
            received[n] = '\0';
            printf("Child received: %s\n", received);
        }

        // Send reply to parent
        write(pipe2[1], reply, strlen(reply) + 1);

        close(pipe1[0]);
        close(pipe2[1]);
    }
    else
    {
        // Parent process
        close(pipe1[0]);
        close(pipe2[1]);

        // Send message to child
        write(pipe1[1], message, strlen(message) + 1);
        close(pipe1[1]);

        // Read reply from child
        n = read(pipe2[0], received, sizeof(received) - 1);

        if (n > 0)
        {
            received[n] = '\0';
            printf("Parent received: %s\n", received);
        }

        close(pipe2[0]);

        wait(NULL);
    }

    return 0;
}
