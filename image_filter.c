#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "bitmap.h"
#include <fcntl.h>


#define ERROR_MESSAGE "Warning: one or more filter had an error, so the output image may not be correct.\n"
#define SUCCESS_MESSAGE "Image transformed successfully!\n"


/*
 * Check whether the given command is a valid image filter, and if so,
 * run the process.
 *
 * We've given you this function to illustrate the expected command-line
 * arguments for image_filter. No further error-checking is required for
 * the child processes.
 */
void run_command(const char *cmd) {
    if (strcmp(cmd, "copy") == 0 || strcmp(cmd, "./copy") == 0 ||
        strcmp(cmd, "greyscale") == 0 || strcmp(cmd, "./greyscale") == 0 ||
        strcmp(cmd, "gaussian_blur") == 0 || strcmp(cmd, "./gaussian_blur") == 0 ||
        strcmp(cmd, "edge_detection") == 0 || strcmp(cmd, "./edge_detection") == 0) {
        execl(cmd, cmd, NULL);
    } else if (strncmp(cmd, "scale", 5) == 0) {
        // Note: the numeric argument starts at cmd[6]
        execl("scale", "scale", cmd + 6, NULL);
    } else if (strncmp(cmd, "./scale", 7) == 0) {
        // Note: the numeric argument starts at cmd[8]
        execl("./scale", "./scale", cmd + 8, NULL);
    } else {
        fprintf(stderr, "Invalid command '%s'\n", cmd);
        exit(1);
    }
}


// TODO: Complete this function.
int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: image_filter input output [filter ...]\n");
        exit(1);
    }

    int result;
    if (argc <= 4){
        // If no filter is specified
        result = fork();
        if (result == 0){
            // Child process calls copy

            // redirect stdout to file
            // redirect stdin to file
            int input_file = open(argv[1], O_RDONLY);
            if (input_file == -1){
                perror("open");
                exit(1);
            }

            int output_file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
            if (output_file==-1){
                perror("open");
                exit(1);
            }

            if (dup2(output_file, fileno(stdout)) == -1) {
			    perror("dup2");
                exit(1);
            }

            if (dup2(input_file, fileno(stdin)) == -1) {
			    perror("dup2");
                exit(1);
            }
            if (argc == 3){
                run_command("./copy");
            }else{
                run_command(argv[3]);
            }
            fprintf(stderr, "ERROR: exec did not run\n");
            exit(1);

        }else if (result > 0){
            // Wait for child to finish

            int status;
            if (wait(&status) != -1)  {
                if (WIFEXITED(status)) {
                    if (!(WEXITSTATUS(status) == 0)){
                        fprintf(stderr, ERROR_MESSAGE);
                        exit(1);
                    }
                }
            }

        }else{
            perror("fork");
            exit(1);
        }
    }else{
        // there will be #filters - 1 pipes
        // Initializes all the pipes
        int fd[argc-4][2];
        for (int row = 0; row < argc-4; row++){
            if (pipe(fd[row])==-1){
                perror("pipe");
                exit(1);
            }
        }
        int result = fork();

        if (result== 0){
            int input_file = open(argv[1], O_RDONLY);
            if (input_file == -1){
                perror("open");
                exit(1);
            }
            if (dup2(input_file, fileno(stdin)) == -1) {
                perror("dup2");
                exit(1);
            }

            for (int repeat = 0; repeat < argc-4; repeat++){
                result = fork();
                if (result ==0){
                    close(fd[repeat][1]);
                    if (dup2(fd[repeat][0], fileno(stdin)) == -1) {
                        perror("dup2");
                        exit(1);
                    }
                }else if (result>0){
                    close(fd[repeat][0]);
                    if (dup2(fd[repeat][1], fileno(stdout)) == -1) {
                        perror("dup2");
                        exit(1);
                    }
                    run_command(argv[3+repeat]);
                }else{
                    perror("fork");
                    exit(1);
                }
            }
            int output_file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
            if (output_file==-1){
                perror("open");
                exit(1);
            }

            if (dup2(output_file, fileno(stdout)) == -1) {
			    perror("dup2");
                exit(1);
            }
            run_command(argv[argc-1]);
        }else if (result > 0){
            int status;
            if (wait(&status) != -1)  {
                if (WIFEXITED(status)) {
                    if (!(WEXITSTATUS(status) == 0)){
                        fprintf(stderr, ERROR_MESSAGE);
                        exit(1);
                    }
                }
            }

        }else{
            perror("fork");
            exit(1);
        }
    }

    printf(SUCCESS_MESSAGE);
    return 0;
}
