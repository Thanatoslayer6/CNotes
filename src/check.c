#include "check.h"
#include "toml-c.h"
#include "utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

char* NOTES_REPOSITORY = NULL;

void check_command(const char* command_name) {
    char command[256];
    snprintf(command, sizeof(command), "command -v %s > /dev/null 2>&1", command_name);

    if (!system(command)) {
        return;
    }
    
    fprintf(stderr, "`%s` command not found\n", command_name);
    exit(EXIT_FAILURE);
}

void check_configuration() {
    // First check if there's a file at ~/.config/cnotes/config
    char *home = getenv(HOMEENV);
    char filename[PATH_MAX] = "";

    if (!home) {
        fprintf (stderr, "User home environment not found.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(filename, PATH_MAX, CONFIGPATH, home);
 
    check_configuration_file(filename);

    // Read the file and parse the contents
    char *file_contents = file_read(filename);
    char errbuf[200];

	toml_table_t *tbl = toml_parse(file_contents, errbuf, sizeof(errbuf));
	if (!tbl) {
		fprintf(stderr, "ERROR: %s\n", errbuf);
		exit(EXIT_FAILURE);
	}

	// Get specific keys.
	toml_value_t notes_repository = toml_table_string(tbl, "notes_repository");
    if (!notes_repository.ok) {
        fprintf(stderr, "ERROR: Can't parse 'notes_repository', %s\n", errbuf);
        exit(EXIT_FAILURE);
    }

    // Allocate memory for NOTES_REPOSITORY
    NOTES_REPOSITORY = (char*)malloc(sizeof(char) * strlen(notes_repository.u.s));
    if (NOTES_REPOSITORY == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory to `NOTES_REPOSITORY`\n");
        exit(EXIT_FAILURE);
    }
    strcpy(NOTES_REPOSITORY, notes_repository.u.s);

    // Free memory
    toml_free(tbl);
    free(file_contents);
}

void check_repository() {
    // First check if the directory exists
    printf("Notes repository: %s \n", NOTES_REPOSITORY);
    DIR* dir = opendir(NOTES_REPOSITORY);
    char errbuf[200];
    char option;

    if (dir) {
        printf("Notes repository exists ");
        /* Directory exists. */
        closedir(dir);
    } else if (ENOENT == errno) {
        int create_repo = prompt("Notes repository does not exist, do you wish to create it?");
        if (create_repo) {
            execute("mkdir", NOTES_REPOSITORY);
        } else {
            printf("Nothing to do, exiting program...\n");
            exit(EXIT_SUCCESS);
        }
        /* Directory does not exist. */
    } else {
        fprintf(stderr, "ERROR: Notes repository is not a directory!\n");
        exit(EXIT_FAILURE);
    }

    check_repository_status();
}

void check_repository_status() {
    // TODO: Create functions for knowing if directory exists or not, 
    // check for .git directory, ask user if want to initialize along with remote & branch
    // TODO: sync method
}

void check_configuration_file(const char* filepath) {
    FILE *file_ptr = fopen(filepath, "r");

    // Check if file exists, if first time, tell user to generate default config
    if (!file_ptr) {
        
        const char *generate_info_text = 
            "\n ERROR: Config file not found at [%s].\n\n"
            " Please create a configuration file named 'config.toml' in the specified directory.\n"
            " This file should contain the following settings:\n\n"
            "  notes_repository = \"/absolute/path/to/your/notes\"\n"
            "\n Note: Make sure that 'notes_respository' is an absolute path.\n\n";

        fprintf(stderr, generate_info_text, filepath);
        exit(EXIT_FAILURE);
    }

    fclose(file_ptr);
}