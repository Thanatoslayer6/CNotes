#include "utils.h"
#include "check.h"

int main(int argc, char** argv) {

    // printf(BANNER_ART);

    // Check app requirements
    // TODO: There is a bottleneck here, it costs around 200ms, so probably the best way is to not check at all?
    // check_command("git");
    // check_command("vim");
    // check_command("fzf"); // TODO: Make fzf an optional command?

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
           printf(" - Config location: %s\n", CONFIG_FILE_PATH);
           printf(" - Local repository path: %s\n", LOCAL_REPO_PATH);
           printf(" - Remote repository url: %s\n", REMOTE_REPO_URL);
           printf(" - Git Username: %s\n", GIT_USERNAME);
           printf(" - Git Email: %s\n", GIT_EMAIL);
           printf(" - Editor: %s\n", EDITOR);
           exit(EXIT_SUCCESS);
        } else if (strcmp(argv[1], "list") == 0) {
            char *note_name = execute_cd_out(LOCAL_REPO_PATH, "fzf" ,"--preview=\'cat {}\' --preview-window=right:70%:wrap");
            if (strlen(note_name) > 0) { // If the user selects a file, simply open it up
                execute_cd(LOCAL_REPO_PATH, EDITOR, note_name, 0);
            }
            free(note_name);
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[1], "sync") == 0) {
            check_sync();
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[1], "setup") == 0) {
            char* lrp = read_line("Enter local repository path: ");
            char* rrp = read_line("Enter remote repository url: ");
            char* gu = read_line("Enter Git username: ");
            char* ge = read_line("Enter Git email: ");
            char* ed = read_line("Enter editor: ");

            const char *generate_info_text = 
                "\nThis file will contain the following settings:\n\n"
                "local_repository_path = \"%s\"\n"
                "remote_repository_url = \"%s\"\n"
                "git_username = \"%s\"\n"
                "git_email = \"%s\"\n"
                "editor = \"%s\"\n"
                "\nNote: Make sure that 'local_repository_path' is an absolute path NOT ending with '/'.\n\n";

            fprintf(stdout, generate_info_text, lrp, rrp, gu, ge, ed);
            int overwrite = prompt("Existing config file already exists, do you wish to overwrite? ");
            // printf(something);
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
                execute_cd(LOCAL_REPO_PATH, EDITOR, argv[2], 0);
                exit(EXIT_SUCCESS);
            } 

            const size_t note_length = strlen(argv[2]);
            char *note_name = (char *)malloc((sizeof(char) * (note_length + 3)));

            if (note_name == NULL) {
                fprintf(stderr, " - ERROR: Can't allocate memory for 'note_name'\n");
                exit(EXIT_FAILURE);
            }
            snprintf(note_name, note_length + 4, "%s.md", argv[2]);
            execute_cd(LOCAL_REPO_PATH, EDITOR, note_name, 0);
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
