#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void check_command(const char* command_name) {
    char command[256];
    snprintf(command, sizeof(command), "command -v %s > /dev/null 2>&1", command_name);

    if (!system(command)) {
        return;
    }
    
    printf("`%s` command not found", command_name);
    exit(EXIT_FAILURE);
}


int main(int argc, char** argv) {
    check_command("git");

    // Usage: cnotes [-n | --new_notebook]
    for (int i = 0; i < argc; i++) {

    }

}
