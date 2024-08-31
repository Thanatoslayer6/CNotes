#include <utils.h>
#include <stdio.h>
#include <stdlib.h>


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
            printf("Invalid input. Please enter 'Y' or 'N'.\n");

            option = getchar();
            // Clear the input buffer
            while (getchar() != '\n'); // Discard the rest of the line
        }
    }
}

void execute(const char* command, const char* args) {
    char full_command[4096] = "";
    snprintf(full_command, 4096, "%s %s", command, args);
    if (!system(full_command)) {
        printf("Successfully executed - %s", full_command);
        return;
    }
    
    fprintf(stderr, "ERROR: Failed to execute command - %s\n", full_command);
    exit(EXIT_FAILURE);
}