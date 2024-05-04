#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 40
//the child will continue reading after what the parent stopped from as child and parent have diff descriptor table, but same open file table, so have same pos variable
int main() {
    int fd;
    char buffer[BUFFER_SIZE];
    
    // Open the file
    fd = open("example.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Read the first 20 bytes
    ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        perror("Error reading from file");
        exit(EXIT_FAILURE);
    }
    
    // Print the content read by parent process
    printf("Parent process read %zd bytes: %.*s\n", bytes_read, (int)bytes_read, buffer);

    // Fork the process
    pid_t pid = fork();
    if (pid == -1) {
        perror("Error forking process");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        ssize_t child_bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (child_bytes_read == -1) {
            perror("Error reading from file in child process");
            exit(EXIT_FAILURE);
        }
        
        // Print the content read by child process
        printf("Child process read %zd bytes: %.*s\n", child_bytes_read, (int)child_bytes_read, buffer);
    } else {
        // Parent process
        wait(NULL); // Wait for child to finish
        
        // Continue reading from the file
        ssize_t parent_bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (parent_bytes_read == -1) {
            perror("Error reading from file in parent process");
            exit(EXIT_FAILURE);
        }
        
        // Print the content read by parent process
        printf("Parent process read %zd bytes: %.*s\n", parent_bytes_read, (int)parent_bytes_read, buffer);
    }

    // Close the file descriptor
    close(fd);

    return 0;
}

