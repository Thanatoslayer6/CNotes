#include "utils.h"
#include <check.h>
#include <time.h>

int main(int argc, char** argv) {
    // Check app requirements
    check_command("git");
    check_command("vim");

    // Check configuration
    check_configuration();

    check_repository();

    // check_sync();

    execute_cd(NOTES_REPOSITORY, "vim", "todo.md", 1);

    // Get the current time
    time_t now;
    time(&now);

    // Convert to local time
    struct tm *local = localtime(&now);

    // Buffer to hold the formatted time string
    char buffer[100];

    // Format the time as YYYY-MM-DD HH:MM:SS
    strftime(buffer, sizeof(buffer), "\"%Y-%m-%d %H:%M:%S\"", local);

    execute_cd(NOTES_REPOSITORY, "git add . && git commit -m", buffer, 1);

    // TODO: `push to upstream via `git push -u origin main`

    // Sync from remote to get latest changes
    // CODE HERE

    // Usage: cnotes [-n | --new ] [-a | --add] <command>
    // 
    // Initialize a box to store notebooks
    //     setup    Create an empty box to store your notebooks
    //     

    // size_t optind;
    // for (optind = 1; optind < argc && argv[optind][0] == '-'; optind++) {
    //     switch (argv[optind][1]) {
    //         case 'c': 
    //             break;
    //         case 'l': 
    //             break;
    //         case 'w': 
    //             break;
    //         default:
    //             fprintf(stderr, USAGE);
    //             exit(EXIT_FAILURE);
    //     }   
    // }
    // fprintf(stderr, USAGE);
    // argv += optind;

}
