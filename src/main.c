#include "utils.h"
#include <check.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {

    // printf(BANNER_ART);

    // Check app requirements
    check_command("git");
    check_command("vim");
    check_command("fzf");

    // Check configuration
    check_configuration();
    check_repository();
    // check_sync();

    
    // no arguments passed
    if (argc == 1) { 
        printf(USAGE);
        exit(EXIT_FAILURE);
    } 
    
    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            printf(USAGE);
            exit(EXIT_SUCCESS);
        }

        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            printf(PROGRAM_VERSION);
            exit(EXIT_SUCCESS);
        }

        if (strcmp(argv[1], "add") == 0) {
            printf("usage: noets add <note>\n");
            exit(EXIT_FAILURE);
        } else if (strcmp(argv[1], "remove") == 0) {
            printf("usage: noets remove <note>\n");
            exit(EXIT_FAILURE);
        } else if (strcmp(argv[1], "config") == 0) {
           printf(BANNER_ART);
           printf(" - Config location: %s\n", CONFIGPATH);
           printf(" - Local repository path: %s\n", LOCAL_REPO_PATH);
           printf(" - Remote repository path: %s\n", REMOTE_REPO_URL);
           printf(" - Git Username: %s\n", GIT_USERNAME);
           printf(" - Git Email: %s\n", GIT_EMAIL);
           exit(EXIT_SUCCESS);
        } else if (strcmp(argv[1], "list") == 0) {
            char *note_name = execute_cd_out(LOCAL_REPO_PATH, "fzf" ,"--preview=\'cat {}\' --preview-window=right:70%:wrap");
            if (strlen(note_name) > 0) { // If the user selects a file, simply open it up in Vim
                execute_cd(LOCAL_REPO_PATH, "vim", note_name, 0);
            }
            free(note_name);
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[1], "sync") == 0) {
            // TODO: Do this...
            // printf(" - Performing git pull\n");
            // printf(" - Performing git push\n");
            check_sync();
            exit(EXIT_SUCCESS);
        } else {
            printf("noets: '%s' is not a noets command.\n\n", argv[1]);
            printf(USAGE);
            exit(EXIT_FAILURE);
        }
    }

    if (argc == 3) {
        if (strcmp(argv[1], "add") == 0) {
            // First check if second argument ends with .md, if not then simply add it
            const char* extension = strrchr(argv[2], '.');

            if (extension != NULL && strcmp(extension, ".md") == 0) {
                execute_cd(LOCAL_REPO_PATH, "vim", argv[2], 0);
                exit(EXIT_SUCCESS);
            } 

            const size_t note_length = strlen(argv[2]);
            char *note_name = (char *)malloc((sizeof(char) * (note_length + 3)));

            if (note_name == NULL) {
                fprintf(stderr, " - ERROR: Can't allocate memory for 'note_name'\n");
                exit(EXIT_FAILURE);
            }
            snprintf(note_name, note_length + 4, "%s.md", argv[2]);
            execute_cd(LOCAL_REPO_PATH, "vim", note_name, 0);
            free(note_name);
            exit(EXIT_SUCCESS);

        } else if (strcmp(argv[1], "remove") == 0) {
            char file_to_remove[4096] = "";

            #ifdef HAVEWIN
                snprintf(file_to_remove, 4096, "%s\\%s", LOCAL_REPO_PATH, argv[2]);
            #elif HAVEUNIX
                snprintf(file_to_remove, 4096, "%s/%s", LOCAL_REPO_PATH, argv[2]);
            #endif

            // printf("File path is: %s\n", file_to_remove);
            if (file_exists(file_to_remove)) {
                execute("rm", file_to_remove, 1);
            } else {
                fprintf(stderr, "Note does not exist!\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    exit(EXIT_FAILURE);
    // TODO: Tab completions for `remove` 
}
