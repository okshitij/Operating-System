//process related
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void process_related_calls() {
    printf("Process Related System Calls:\n");
    printf("1. Fork a process\n");
    printf("2. Exit a process\n");
    printf("3. Wait for a process\n");
    printf("Enter your choice: ");
    int process_choice;
    scanf("%d", &process_choice);

    switch (process_choice) {
        case 1: {
            pid_t pid = fork();
            if (pid == 0) {
                printf("This is the child process\n");
                exit(0);
            } else if (pid > 0) {
                printf("This is the parent process\n");
            } else {
                perror("Fork failed");
            }
            break;
        }
        case 2:
            printf("Exiting the process...\n");
            exit(0);
        case 3: {
            pid_t pid = fork();
            if (pid == 0) {
                sleep(2);
                printf("Child process finished\n");
                exit(0);
            } else if (pid > 0) {
                printf("Waiting for the child process to complete...\n");
                wait(NULL);
                printf("Parent process resumed\n");
            } else {
                perror("Fork failed");
            }
            break;
        }
        default:
            printf("Invalid choice\n");
    }
}

int main() {
    process_related_calls();
    return 0;
}

//file related
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
            
void file_related_calls() {
    printf("File Related System Calls:\n");
    printf("1. Open and Read a file\n");
    printf("2. Write and Close a file\n");
    printf("3. Link and Unlink a file\n");   
    printf("4. Display file status\n");
    printf("Enter your choice: ");
    int file_choice;
    scanf("%d", &file_choice);
                
    switch (file_choice) {
        case 1: {
            char filename[100];
            printf("Enter file name to open and read: ");
            scanf("%s", filename);
            int fd = open(filename, O_RDONLY);
            if (fd == -1) {
                perror("Error opening file");
                break;
            }
            char buffer[1024];
            int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
            if (bytes_read == -1) {
                perror("Error reading file");
                close(fd);
                break;
            }
            buffer[bytes_read] = '\0';   
            printf("File content:\n%s\n", buffer);
            close(fd);
            break;
        }
        case 2: {
            char filename[100];
            printf("Enter file name to write to: ");
            scanf("%s", filename);
            int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
            if (fd == -1) {
                perror("Error opening file");
                break;
            }
            char text[1024];
            printf("Enter the text to write: ");
            getchar(); // To consume the newline left by scanf
            fgets(text, sizeof(text), stdin);
            int bytes_written = write(fd, text, strlen(text));
            if (bytes_written == -1) {
                perror("Error writing to file");
                close(fd);
                break;
            }
            close(fd);
            printf("Text written to file\n");
            break;
        } 
          
        case 3: {
            char srcfile[100], destlink[100];
            printf("Enter the source file name to link: ");
            scanf("%s", srcfile);
            printf("Enter the destination link name: ");
            scanf("%s", destlink);
            if (link(srcfile, destlink) == -1) {
                perror("Error linking file");
                break;
            }
            printf("Link created\n");
            if (unlink(destlink) == -1) {
                perror("Error unlinking file");
                break;
            }
            printf("Link removed\n");
            break;
        }
        case 4: {
            char filename[100];
            struct stat file_stat;
            printf("Enter the file name to get status: ");
            scanf("%s", filename);
            if (stat(filename, &file_stat) == -1) {
                perror("Error getting file status");
                break;
            }
            printf("File Size: %lld bytes\n", (long long)file_stat.st_size);
            printf("Number of Links: %hu\n", (unsigned short)file_stat.st_nlink);
            printf("File inode: %llu\n", (unsigned long long)file_stat.st_ino);
            printf("File Permissions: ");
            printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
            printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
            printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
            printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
            printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
            printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
            printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
            printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
            printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
            printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
            printf("\n");
            break;
        }
        default:
            printf("Invalid choice\n");
    }
}
            
int main() {
    file_related_calls();
    return 0;
}

//communication sys call
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void communication_related_calls() {
    printf("Communication Related System Calls:\n");
    printf("1. Pipe\n");
    printf("2. FIFO\n");
    printf("Enter your choice: ");
    int comm_choice;
    scanf("%d", &comm_choice);

    switch (comm_choice) {
        case 1: {
            int fd[2];
            pipe(fd);
            if (fork() == 0) {
                close(fd[0]);
                write(fd[1], "Hello from child", 16);
                close(fd[1]);
                exit(0);
            } else {
                close(fd[1]);
                char buffer[1024];
                read(fd[0], buffer, 1024);
                printf("Parent received: %s\n", buffer);
                close(fd[0]);
                wait(NULL);
            }
            break;
        }
        case 2: {
            char* fifo = "/tmp/my_fifo";
            mkfifo(fifo, 0666);
            if (fork() == 0) {
                int fd = open(fifo, O_WRONLY);
                write(fd, "Hello FIFO", 10);
                close(fd);
                exit(0);
            } else {
                int fd = open(fifo, O_RDONLY);
                char buffer[1024];
                read(fd, buffer, 1024);
                printf("Parent received: %s\n", buffer);
                close(fd);
                wait(NULL);
                unlink(fifo);
            }
            break;
        }
        default:
            printf("Invalid choice\n");
    }
}
int main() {
    communication_related_calls();
return 0;
}

//info related
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void information_related_calls() {
    printf("Information Related System Calls:\n");
    printf("1. Get Process ID\n");
    printf("2. Get Parent Process ID\n");
    printf("3. Get User ID\n");
    printf("Enter your choice: ");

    int info_choice;
    int result = scanf("%d", &info_choice);  // Capture scanf result

    if (result != 1) {
        printf("Failed to read input. Please enter a valid number.\n");
        return;
    }

    switch (info_choice) {
        case 1:
            printf("Process ID: %d\n", getpid());
            break;
        case 2:
            printf("Parent Process ID: %d\n", getppid());
            break;
        case 3:
            printf("User ID: %d\n", getuid());
            break;
        default:
            printf("Invalid choice\n");
    }
}

int main() {
    information_related_calls();
    return 0;
}
