#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path) // Windows does not use mode
#endif

int mkdir_p(const char *path) {
    char temp[1024];
    char *p = NULL;
    size_t len;

    // Copy the path to a temporary buffer
    snprintf(temp, sizeof(temp), "%s", path);
    len = strlen(temp);

    // Remove trailing slashes
    if (temp[len - 1] == '/') {
        temp[len - 1] = '\0';
    }

    // Iterate over the path and create directories one by one
    for (p = temp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0'; // Temporarily terminate the string
            if (mkdir(temp, 0755) != 0 && errno != EEXIST) {
                perror("mkdir");
                return -1;
            }
            *p = '/'; // Restore the slash
        }
    }

    // Create the final directory
    if (mkdir(temp, 0755) != 0 && errno != EEXIST) {
        perror("mkdir");
        return -1;
    }

    return 0;
}

int main(int argc, char **argv) {
    unsigned char dir_path[128];
    unsigned char address_file[128];
    unsigned char line[42+1];
    unsigned char idx1[5+1];
    unsigned char idx2[3+1];

    if (argc <= 1) {
        printf("Usage: %s <file.txt>\n", argv[0]);
        return -1;
    }

    FILE *input_fp = fopen(argv[1], "rb");
    if (!input_fp) {
        perror("Error opening file");
        return -1;
    }

    size_t bytesRead;
    long long int counter = 0;
    while ((bytesRead = fread(line, 1, 43, input_fp)) > 0) {
        line[bytesRead - 1] = '\0';
        int i = 0;
        for (i = 0; i < 5; i++)
       	    idx1[i] = line[i];
        idx1[i] = '\0';
        for (i = 0; i < 3; i++)
       	    idx2[i] = line[i+5];
        idx2[i] = '\0';

        counter++;
        if (counter % 100000 == 0)
            printf("line %lld: %.*s\n", counter,  (int)bytesRead, line);

        snprintf(dir_path, sizeof(dir_path), "/tmp/eth/%s/%s", idx1, idx2);
        mkdir_p(dir_path);

        snprintf(address_file, sizeof(address_file), "/tmp/eth/%s/%s/%s", idx1, idx2, line);
        int fd = open(address_file, O_WRONLY | O_CREAT, 0644);
        if (fd == -1) {
            perror("Error opening or creating file");
            return 1;
        }

        // Close the file
        close(fd);
    }

    if (ferror(input_fp)) {
        perror("Error reading file");
    }

    fclose(input_fp);
    return 0;
}
