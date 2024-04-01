/* This file is for Part 2 of the coursework */

#include <stdio.h>         // Include this for stderr
#include <unistd.h>        // for syscall(), open(), etc
#include <sys/syscall.h>   // for the SYS_xxx call codes
#include <fcntl.h>         // for the file modes


int main(int argc, char** argv) {
    const char* ifname = "input.txt";
    const char* ofname = "output.txt";

    // If the first parameter is 's', use syscall()
    if (argc > 1 && argv[1][0] == 's') {
        // Open input file
        int input_fd = syscall(SYS_open, ifname, O_RDONLY);
        if (input_fd == -1) {
            perror("Failed to open input file");
            return 1;
        }

        // Open output file
        int output_fd = syscall(SYS_open, ofname, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (output_fd == -1) {
            perror("Failed to open output file");
            return 1;
        }

        // Copy data
        char buf[256];
        ssize_t bytes_read = syscall(SYS_read, input_fd, buf, sizeof(buf));
        if (bytes_read < 0) {
            perror("Read error");
            return 1;
        }
        
        ssize_t bytes_written = syscall(SYS_write, output_fd, buf, bytes_read);
        if (bytes_written < 0) {
            perror("Write error");
            return 1;
        }

        // Close files
        syscall(SYS_close, input_fd);
        syscall(SYS_close, output_fd);
    }
    // Else if the first parameter is 'w', use wrapper functions
    else if (argc > 1 && argv[1][0] == 'w') {
        // Open input file
        int input_fd = open(ifname, O_RDONLY);
        if (input_fd == -1) {
            perror("Failed to open input file");
            return 1;
        }

        // Open output file
        int output_fd = open(ofname, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (output_fd == -1) {
            perror("Failed to open output file");
            return 1;
        }

        // Copy data
        char buf[256];
        ssize_t bytes_read = read(input_fd, buf, sizeof(buf));
        if (bytes_read < 0) {
            perror("Read error");
            return 1;
        }
        
        ssize_t bytes_written = write(output_fd, buf, bytes_read);
        if (bytes_written < 0) {
            perror("Write error");
            return 1;
        }

        // Close files
        close(input_fd);
        close(output_fd);
    } else {
        fprintf(stderr, "Invalid method. Use 's' for syscall() or 'w' for wrapper functions.\n");
        return 1;
    }

    return 0;
}
