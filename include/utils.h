#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE \
    "Usage: noets [-v | --version] [-h | --help] <command> [arguments]\n\n"\
    "These are all the commands for utilizing this program\n" \
    "    add       Adds a new note\n" \
    "    remove    Removes an existing note\n" \
    "    list      Lists all of the notes\n"\
    "    sync      Synchronize to remote (push/pull)\n"\
    "    config    Shows the configuration used\n" \
    "    setup     Creates a configuration\n" \

#define BANNER_ART \
      "\n                              dP           \n" \
        "                              88           \n" \
        " 88d888b. .d8888b. .d8888b. d8888P .d8888b.\n" \
        " 88'  `88 88'  `88 88ooood8   88   Y8ooooo.\n" \
        " 88    88 88.  .88 88.  ...   88         88\n" \
        " dP    dP `88888P' `88888P'   dP   `88888P'\n\n" \

#define PROGRAM_VERSION "noets version 1.0.0\n"

#if defined (_WIN32) || defined (_WIN64)
 #define HAVEWIN 1
#elif defined (__unix__)
 #define HAVEUNIX 1
#endif

#ifdef HAVEWIN
 #define HOMEENV "USERPROFILE"
 #define CONFIGPATH "%s\\AppData\\Local\\noets\\config.toml"
#elif HAVEUNIX
 #define HOMEENV "HOME"
 #define CONFIGPATH "%s/.config/noets/config.toml"
#endif

int file_exists(const char *filename);
char *file_read(const char *filepath);
char *read_line(const char *prompt);
int prompt(const char* text);
void execute(const char* command, const char* args, int show_debug);
void execute_cd(const char* cddir, const char* command, const char* args, int show_debug);
char *execute_cd_out(const char* cddir, const char *command, const char* args);
int directory_check(const char* path);