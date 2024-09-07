#pragma once

#define USAGE \
    "Usage: noets <command> <arguments>\n\n" \
    "Basic commands for starting\n" \
    "    config    Shows the configuration used" \
    "    list      Lists all of the notes"\
    "    new       Creates a new note" \
    "    delete    Deletes an existing note"\
    "    sync      Sync to remote (push/pull)"\

#define BANNER_ART \
      "\n                              dP           \n" \
        "                              88           \n" \
        " 88d888b. .d8888b. .d8888b. d8888P .d8888b.\n" \
        " 88'  `88 88'  `88 88ooood8   88   Y8ooooo.\n" \
        " 88    88 88.  .88 88.  ...   88         88\n" \
        " dP    dP `88888P' `88888P'   dP   `88888P'\n\n" \

#if defined (_WIN32) || defined (_WIN64)
 #define HAVEWIN 1
#elif defined (__unix__)
 #define HAVEUNIX 1
#endif

#ifdef HAVEWIN
 #define HOMEENV "USERPROFILE"
 #define CONFIGPATH "%s\\.config\\noets\\config.toml"
#elif HAVEUNIX
 #define HOMEENV "HOME"
 #define CONFIGPATH "%s/.config/noets/config.toml"
#endif

#include <stdio.h>
#include <stdlib.h>

char *file_read(const char *filepath);
char *read_line(const char *prompt);
char *get_time();
int prompt(const char* text);
void execute(const char* command, const char* args);
void execute_cd(const char* cddir, const char* command, const char* args);
void execute_cd_out(const char* cddir, const char *command, const char* args, char *output, size_t output_size);
int directory_check(const char* path);