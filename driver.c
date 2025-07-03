#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
        exit(1);
    }

    int file1 = open(argv[1], O_RDONLY);
    if (file1 == -1) {
        perror("Error opening file1");
        exit(1);
    }

    int file2 = creat(argv[2], 0666);
    if (file2 == -1) {
        perror("Error creating file2");
        exit(1);
    }

    int pipefd[2];  // pipefd[0] = read end, pipefd[1] = write end
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    pid_t pid_count = fork();
    if (pid_count < 0) {
        perror("Fork for count failed");
        exit(1);
    }

    if (pid_count == 0) {  // Child 1: count process
        close(pipefd[1]);  // Close unused write end of pipe

        dup2(pipefd[0], STDIN_FILENO);  // Redirect pipe read to stdin
        close(pipefd[0]);

        dup2(file2, STDOUT_FILENO);  // Redirect stdout to file2
        close(file2);

        execl("./count", "count", (char *) NULL);
        perror("execl failed for count");
        exit(1);
    }

    pid_t pid_convert = fork();
    if (pid_convert < 0) {
        perror("Fork for convert failed");
        exit(1);
    }

    if (pid_convert == 0) {  // Child 2: convert process
        close(pipefd[0]);  // Close unused read end of pipe

        dup2(file1, STDIN_FILENO);  // Redirect file1 to stdin
        close(file1);

        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to pipe write
        close(pipefd[1]);

        execl("./convert", "convert", (char *) NULL);
        perror("execl failed for convert");
        exit(1);
    }

    // Parent process closes unnecessary FDs
    close(pipefd[0]);
    close(pipefd[1]);
    close(file1);
    close(file2);

    // Wait for both child processes to complete
    waitpid(pid_count, NULL, 0);
    waitpid(pid_convert, NULL, 0);

    return 0;
}
