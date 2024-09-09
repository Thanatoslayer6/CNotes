#include <utils.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

int file_exists(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    int is_exist = 0;
    if (fp != NULL)
    {
        is_exist = 1;
        fclose(fp); // close the file
    }
    return is_exist;
}

char *file_read(const char *filepath) {
    FILE *file_ptr = fopen(filepath, "rb"); // Read file
    size_t i = 0, c = 0;

    if (file_ptr == NULL)
    {
        perror("Error opening file");
        return NULL;
    }
    // Get size of file
    fseek(file_ptr, 0, SEEK_END);
    long bytes = ftell(file_ptr);
    rewind(file_ptr);

    char *contents = (char *)malloc((bytes + 1) * sizeof(char));
    size_t bytes_read = fread(contents, 1, bytes, file_ptr);
    if (bytes_read != bytes)
    {
        perror("Error reading file");
        free(contents);
        fclose(file_ptr);
        return NULL;
    }
    contents[bytes] = '\0';
    fclose(file_ptr);

    return contents;
}

int prompt(const char* text) {
    char option;

    while (1) {
        // Print prompt
        printf("%s [Y/n]: ", text);
        
        // Read a single character
        option = getchar();

        // Handle the input
        if (option == 'Y' || option == 'y' || option == '\n') {
            return 1; // Yes (including default case of Enter)
        } else if (option == 'N' || option == 'n') {
            return 0; // No
        } else {
            // Invalid input; prompt again
            printf(" - Invalid input. Please enter 'Y' or 'N'.\n");

            option = getchar();
            // Clear the input buffer
            while (getchar() != '\n'); // Discard the rest of the line
        }
    }
}

void execute(const char* command, const char* args, int show_debug) {
    char full_command[4096] = "";
    snprintf(full_command, 4096, "%s %s", command, args);
    if (!system(full_command)) {
        if (show_debug) printf("    Successfully executed - %s\n", full_command);
        return;
    }
    
    if (show_debug) fprintf(stderr, " - ERROR: Failed to execute command - %s\n", full_command);
    exit(EXIT_FAILURE);
}

void execute_cd(const char* cddir, const char* command, const char* args, int show_debug) {
    char full_command[4096] = "";
    snprintf(full_command, 4096, "cd %s && %s %s", cddir, command, args);

    if (!system(full_command)) {
        if (show_debug) printf("    Successfully executed - %s\n", full_command);
        return;
    }

    if (show_debug) fprintf(stderr, " - ERROR: Failed to execute command - %s\n", full_command);
    exit(EXIT_FAILURE);
}

int directory_check(const char* path) {
    DIR* dir = opendir(path);
    if (dir) {
        closedir(dir);
        return 1;
    } else if (ENOENT == errno) {
        closedir(dir);
        return 0;
    }
    closedir(dir);
    return -1;
}

char* read_line(const char* prompt) {
    printf("%s", prompt);

    size_t size = 128;
    char *line = malloc(size);
    if (!line) {
        perror("malloc");
        return NULL;
    }

    size_t len = 0;
    int ch;
    while ((ch = fgetc(stdin)) != EOF && ch != '\n') {
        if (len + 1 >= size) {
            size *= 2; // Double the size
            char *new_line = realloc(line, size);
            if (!new_line) {
                perror("realloc");
                free(line);
                return NULL;
            }
            line = new_line;
        }
        line[len++] = (char)ch;
    }

    line[len] = '\0'; // Null-terminate the string

    return line;
}

char* get_time() {
    // Get the current time
    time_t now;
    time(&now);

    // Convert to local time
    struct tm *local = localtime(&now);

    // Buffer to hold the formatted time string
    char *buffer = (char*)malloc(sizeof(char) * 64);

    // Format the time as YYYY-MM-DD HH:MM:SS
    strftime(buffer, sizeof(buffer), "\"%Y-%m-%d %H:%M:%S\"", local);

    return buffer;
}

void execute_cd_out(const char* cddir, const char *command, const char* args, char *output, size_t output_size) {
    char full_command[4096] = "";
    snprintf(full_command, 4096, "cd %s && %s %s", cddir, command, args);

    FILE *fp;
    fp = popen(full_command, "r");
    if (fp == NULL) {
        // perror("popen");
        // fprintf(stderr, "")
        fprintf(stderr, " - ERROR: Failed to run `popen`");
        exit(EXIT_FAILURE);
    }

    if (fgets(output, output_size, fp) == NULL) { 
        // perror("fgets");
        // fprintf(stderr, " - ERROR: No note selected");
        // printf(" - No note selected, exiting program...\n");
        pclose(fp);
        exit(EXIT_FAILURE);
    }

    pclose(fp);
}