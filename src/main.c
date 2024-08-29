#include <check.h>

int main(int argc, char** argv) {
    // Check app requirements
    check_command("git");

    // Check configuration
    check_configuration();

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
