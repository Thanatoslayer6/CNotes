#include "check.h"
#include "toml-c.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

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
    char option;
    int dirstatus = directory_check(NOTES_REPOSITORY);

    if (dirstatus == 1) {
        printf("Notes repository exists...\n");
    } else if (dirstatus == 0) {
        int create_dir_status = prompt("Notes repository does not exist, do you wish to create it?");
        if (create_dir_status) {
            execute("mkdir", NOTES_REPOSITORY);
        } else {
            printf("Nothing to do, exiting program...\n");
            exit(EXIT_SUCCESS);
        }
    } else {
        fprintf(stderr, "ERROR: Notes repository is not a directory!\n");
        exit(EXIT_FAILURE);
    }

    check_repository_status();
}

void check_repository_status() {
    // TODO: Create functions for knowing if directory exists or not, 
    char git_repo[PATH_MAX] = "";

    #ifdef HAVEWIN
        snprintf(git_repo, PATH_MAX, "%s\\.git", NOTES_REPOSITORY);
    #elif HAVEUNIX
        snprintf(git_repo, PATH_MAX, "%s/.git", NOTES_REPOSITORY);
    #endif

    int dirstatus = directory_check(git_repo);

    if (dirstatus == 1) {
        printf("Git repository exists...\n");
    } else if (dirstatus == 0) {
        int create_repo_status = prompt("Initialize Git repository?");
        if (create_repo_status) {
            // Set up user, branch, and remote
            execute_cd(NOTES_REPOSITORY, "git init", "-b main"); // default branch is main
            char *user_name = read_line("What is your Git username? (enclose with quotation): ");
            execute_cd(NOTES_REPOSITORY, "git config user.name", user_name);

            char *user_email = read_line("What is your Git email?: ");
            execute_cd(NOTES_REPOSITORY, "git config user.email", user_email);

            char *remote_url = read_line("What is the Git remote?: ");
            execute_cd(NOTES_REPOSITORY, "git remote add origin", remote_url);

            free(user_name);
            free(user_email);
            free(remote_url);

        } else {
            printf("Nothing to do, exiting program...\n");
            exit(EXIT_SUCCESS);
        }
    } else {
        fprintf(stderr, "ERROR: `.git/` folder is not a directory!\n");
        exit(EXIT_FAILURE);
    }
}

void check_sync() {
   // TODO: Create method, git pull
   // branch = notebook 
   // files = page
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
            "\n Note: Make sure that 'notes_respository' is an absolute path not ending with '/'.\n\n";

        fprintf(stderr, generate_info_text, filepath);
        exit(EXIT_FAILURE);
    }

    fclose(file_ptr);
}