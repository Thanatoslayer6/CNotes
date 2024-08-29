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