#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>

jmp_buf env;
pid_t pid_count, pid_convert;

void timeout_handler(int signo) {
    fprintf(stderr, "Read timeout in second child, killing both children.\n");
    kill(pid_count, SIGTERM);
    kill(pid_convert, SIGTERM);
    longjmp(env, 1);
}

int main(int argc, char *argv[]) {
    int file1 = STDIN_FILENO, file2 = STDOUT_FILENO, pipefd[2];
    char *count_arg = NULL;
    int arg_index = 1;

    if (argc > 1 && argv[arg_index][0] == '-') {
        count_arg = argv[arg_index];
        arg_index++;
    }

    if (arg_index < argc) {
        file1 = open(argv[arg_index], O_RDONLY);
        if (file1 == -1) {
            perror("Error opening file1");
            exit(1);
        }
        arg_index++;
    }

    if (arg_index < argc) {
        file2 = creat(argv[arg_index], 0666);
        if (file2 == -1) {
            perror("Error creating file2");
            exit(1);
        }
    }

    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    signal(SIGALRM, timeout_handler);
    if (setjmp(env) == 0) {
        alarm(15);
    } else {
        waitpid(pid_count, NULL, 0);
        waitpid(pid_convert, NULL, 0);
        exit(1);
    }

    if ((pid_count = fork()) == 0) {
        fprintf(stderr, "count PID: %d\n", getpid());
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        dup2(file2, STDOUT_FILENO);
        close(file2);
        if (count_arg) execl("./count", "count", count_arg, (char *)NULL);
        else execl("./count", "count", (char *)NULL);
        perror("execl failed for count");
        exit(1);
    }

    if ((pid_convert = fork()) == 0) {
        fprintf(stderr, "convert PID: %d\n", getpid());
        close(pipefd[0]);
        dup2(file1, STDIN_FILENO);
        close(file1);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execl("./convert", "convert", (char *)NULL);
        perror("execl failed for convert");
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    if (file1 != STDIN_FILENO) close(file1);
    if (file2 != STDOUT_FILENO) close(file2);

    int status;
    pid_t finished_pid = wait(&status);

    if (finished_pid == pid_count && WEXITSTATUS(status) == 2) {
        fprintf(stderr, "count exited with error, killing convert.\n");
        kill(pid_convert, SIGTERM);
    } else if (finished_pid == pid_convert && WEXITSTATUS(status) == 2) {
        fprintf(stderr, "convert exited with error.\n");
        kill(pid_count, SIGTERM);
    }

    waitpid(pid_count, NULL, 0);
    waitpid(pid_convert, NULL, 0);

    fprintf(stderr, "Normal children exit.\n");
    return 0;
}
