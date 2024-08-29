#include "check.h"
#include "toml-c.h"
#include "utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

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
    char *home = getenv (HOMEENV);
    char filename[PATH_MAX] = "";

    if (!home) {
        fprintf (stderr, "User home environment not found.\n");
        exit(EXIT_FAILURE);
    }

    sprintf(filename, CONFIGPATH, home);
 
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

    check_repository(notes_repository.u.s);

    toml_free(tbl);
    free(file_contents);
}

void check_repository(const char* repo_path) {
    // First check if the directory exists
    printf(repo_path);
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