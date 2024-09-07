#include "utils.h"
#include <check.h>
#include <stdio.h>

int main(int argc, char** argv) {

    printf(BANNER_ART);

    // Check app requirements
    check_command("git");
    check_command("vim");
    check_command("fzf");

    // Check configuration
    check_configuration();
    check_repository();
    // check_sync();

    // execute_cd(NOTES_REPOSITORY, "fzf" ,"--preview=\'cat {}\' --preview-window=right:70%:wrap");
    // file=$(fzf --preview="cat {}" --preview-window=right:70%:wrap) && [ -n "$file" ] && vim "$file"

    // execute_cd(NOTES_REPOSITORY, "file=$(fzf --preview=\"cat {}\" --preview-window=right:70%:wrap)", "&& [ -n \"$file\" ] && vim \"$file\"");

    // execute_cd(NOTES_REPOSITORY, "vim", "todo.md", 1);
    // execute_cd(NOTES_REPOSITORY, "vim", "`fzf --preview=\"cat {}\" --preview-window=right:70%:wrap`", 0);
    // execute_cd(NOTES_REPOSITORY, "git add . && git commit -m", buffer, 1);

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
