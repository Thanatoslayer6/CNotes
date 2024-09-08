#include "check.h"
#include "toml-c.h"
#include "utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* LOCAL_REPO_PATH = NULL;
char* REMOTE_REPO_URL = NULL;
char* GIT_USERNAME = NULL;
char* GIT_EMAIL = NULL;

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
    // First check if there's a file at ~/.config/noets/config.toml
    char *home = getenv(HOMEENV);
    char filename[PATH_MAX] = "";

    if (!home) {
        fprintf (stderr, " - User home environment not found.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(filename, PATH_MAX, CONFIGPATH, home);
 
    check_configuration_file(filename);

    // Read the file and parse the contents
    char *file_contents = file_read(filename);
    char errbuf[200];

	toml_table_t *tbl = toml_parse(file_contents, errbuf, sizeof(errbuf));
	if (!tbl) {
		fprintf(stderr, " - ERROR: %s\n", errbuf);
		exit(EXIT_FAILURE);
	}

	// Get specific keys.
	toml_value_t local_repository_path = toml_table_string(tbl, "local_repository_path");
    if (!local_repository_path.ok) {
        fprintf(stderr, " - ERROR: Can't parse 'local_repository_path', %s\n", errbuf);
        exit(EXIT_FAILURE);
    }

    toml_value_t remote_repository_url = toml_table_string(tbl, "remote_repository_url");
    if (!remote_repository_url.ok) {
        fprintf(stderr, " - ERROR: Can't parse 'remote_repository_url', %s\n", errbuf);
        exit(EXIT_FAILURE);
    }

    toml_value_t git_username = toml_table_string(tbl, "git_username");
    if (!git_username.ok) {
        fprintf(stderr, " - ERROR: Can't parse 'git_username', %s\n", errbuf);
        exit(EXIT_FAILURE);
    }

    toml_value_t git_email = toml_table_string(tbl, "git_email");
    if (!git_email.ok) {
        fprintf(stderr, " - ERROR: Can't parse 'git_email', %s\n", errbuf);
        exit(EXIT_FAILURE);
    }

    // Allocate memory for LOCAL_REPO_PATH and REMOTE_REPOSITORY_URL
    LOCAL_REPO_PATH = (char*)malloc(sizeof(char) * strlen(local_repository_path.u.s));
    if (LOCAL_REPO_PATH == NULL) {
        fprintf(stderr, " - ERROR: Failed to allocate memory to `LOCAL_REPO_PATH`\n");
        exit(EXIT_FAILURE);
    }
    strncpy(LOCAL_REPO_PATH, local_repository_path.u.s, strlen(local_repository_path.u.s));

    REMOTE_REPO_URL = (char*)malloc(sizeof(char) * strlen(remote_repository_url.u.s));
    if (REMOTE_REPO_URL == NULL) {
        fprintf(stderr, " - ERROR: Failed to allocate memory to `REMOTE_REPO_URL`\n");
        exit(EXIT_FAILURE);
    }
    strncpy(REMOTE_REPO_URL, remote_repository_url.u.s, strlen(remote_repository_url.u.s));

    GIT_USERNAME = (char*)malloc(sizeof(char) * strlen(git_username.u.s));
    if (GIT_USERNAME == NULL) {
        fprintf(stderr, " - ERROR: Failed to allocate memory to `GIT_USERNAME`\n");
        exit(EXIT_FAILURE);
    }
    strncpy(GIT_USERNAME, git_username.u.s, strlen(git_username.u.s));

    GIT_EMAIL = (char*)malloc(sizeof(char) * strlen(git_email.u.s));
    if (GIT_EMAIL == NULL) {
        fprintf(stderr, " - ERROR: Failed to allocate memory to `GIT_EMAIL`\n");
        exit(EXIT_FAILURE);
    }
    strncpy(GIT_EMAIL, git_email.u.s, strlen(git_email.u.s));

    // Free memory
    toml_free(tbl);
    free(file_contents);
}

void check_repository() {
    // First check if the directory exists
    // char option;
    int dirstatus = directory_check(LOCAL_REPO_PATH);

    if (dirstatus == 1) {
        printf(" - Notes directory exists...\n");
    } else if (dirstatus == 0) {
        int create_dir_status = prompt(" - Notes repository does not exist, do you wish to clone the remote repository?");
        if (create_dir_status) {
            char args[PATH_MAX] = "";
            snprintf(args, PATH_MAX, "%s %s", REMOTE_REPO_URL, LOCAL_REPO_PATH);
            execute("git clone", args);

            // After cloning simply configure git username and email
            execute_cd(LOCAL_REPO_PATH, "git config user.name", GIT_USERNAME);
            execute_cd(LOCAL_REPO_PATH, "git config user.email", GIT_EMAIL);

        } else {
            printf(" - Nothing to do, exiting program...\n");
            exit(EXIT_SUCCESS);
        }
    } else {
        fprintf(stderr, " - ERROR: Notes repository is not a directory!\n");
        exit(EXIT_FAILURE);
    }

    // TODO: Do we need to add a git username and email config as required?
    // check_repository_status();
}


/*
void check_repository_status() {
    char git_repo[PATH_MAX] = "";

    #ifdef HAVEWIN
        snprintf(git_repo, PATH_MAX, "%s\\.git", LOCAL_REPO_PATH);
    #elif HAVEUNIX
        snprintf(git_repo, PATH_MAX, "%s/.git", LOCAL_REPO_PATH);
    #endif

    int dirstatus = directory_check(git_repo);

    if (dirstatus == 1) {
        printf(" - Git is initialized...\n");
    } else if (dirstatus == 0) {
        int create_repo_status = prompt(" - Clone Git repository?");
        if (create_repo_status) {
            // TODO: Do this now you have the remote repo at the config file...
            execute("git clone", REMOTE_REPO_URL);
            // Set up user, branch, and remote
            // execute_cd(LOCAL_REPO_PATH, "git init", "-b main"); // default branch is main
            // char *user_name = read_line("What is your Git username? (enclose with quotation): ");
            // execute_cd(LOCAL_REPO_PATH, "git config user.name", user_name);

            // char *user_email = read_line("What is your Git email?: ");
            // execute_cd(LOCAL_REPO_PATH, "git config user.email", user_email);

            // char *remote_url = read_line("What is the Git remote?: ");
            // execute_cd(LOCAL_REPO_PATH, "git remote add origin", remote_url);

            // free(user_name);
            // free(user_email);
            // free(remote_url);

        } else {
            printf(" - Nothing to do, exiting program...\n");
            exit(EXIT_SUCCESS);
        }
    } else {
        fprintf(stderr, " - ERROR: `.git/` folder is not a directory!\n");
        exit(EXIT_FAILURE);
    }
}
*/

void check_sync() {
    char status_output[1024];
    // char rev_parse_output[1024];
    // char commit_count_output[1024];
    // char command[1024];

    // Get the current branch name
    // execute_cd(NOTES_REPOSITORY, "git rev-parse", "--abbrev-ref HEAD");
    // execute_cd_out(NOTES_REPOSITORY, "git rev-parse", "--abbrev-ref HEAD", rev_parse_output, sizeof(rev_parse_output));
    // rev_parse_output[strcspn(rev_parse_output, "\n")] = '\0'; // Remove newline

    // Fetch latest changes from remote
    // execute_cd_out(NOTES_REPOSITORY, "git fetch", "--all", status_output, sizeof(status_output));
    execute_cd(LOCAL_REPO_PATH, "git remote", "update");

    // Check the status of the current branch
    // snprintf(command, sizeof(command), "git status", "--porcelain -b");
    execute_cd_out(LOCAL_REPO_PATH, "git status", "-uno", status_output, sizeof(status_output));

    // Look for the line indicating that we are behind the remote branch
    if (strstr(status_output, "Your branch is behind")) {
        printf("You are behind the remote branch.\n");
    } else if (strstr(status_output, "Your branch is ahead")) {
        printf("You are ahead of the remote branch.\n");
    } else {
        printf("Your branch is up to date with the remote.\n");
    }

    // Get updates from remote
    // execute_cd(NOTES_REPOSITORY, "git remote", "update");

    // Check if behind
    // execute_cd(NOTES_REPOSITORY, "git status", "--porcelain");

    // execute_cd(NOTES_REPOSITORY, "git merge", "main");
    // execute_cd(NOTES_REPOSITORY, "git pull", "origin main", 0);
    // execute_cd(NOTES, const char *command, const char *args, int failsafe)

    // const char* upstream = (argc > 1) ? argv[1] : "@{u}";
    // Prepare the Git commands

    // execute_cd(NOTES_REPOSITORY, "git push", "-u origin main", 0);
}

void check_configuration_file(const char* filepath) {
    FILE *file_ptr = fopen(filepath, "r");

    // Check if file exists, if first time, tell user to generate default config
    if (!file_ptr) {
        
        const char *generate_info_text = 
            "\n ERROR: Config file not found at [%s].\n\n"
            " Please create a configuration file named 'config.toml' in the specified directory.\n"
            " This file should contain the following settings:\n\n"
            "  local_repository_path = \"/absolute/path/to/your/notes\"\n"
            "  remote_repository_url = \"https://github.com/username/notes.git\"\n"
            "  git_username = \"username\"\n"
            "  git_email = \"username@github.com\"\n"
            "\n Note: Make sure that 'local_repository_path' is an absolute path NOT ending with '/'.\n\n";

        fprintf(stderr, generate_info_text, filepath);
        exit(EXIT_FAILURE);
    }

    fclose(file_ptr);
}