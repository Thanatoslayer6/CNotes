#pragma once

#define USAGE \
    "Usage: cnotes [-n | --new ] [-a | --add] <command>\n\n" \
    "Basic commands for starting\n" \
    "    config    Shows the configuration used" \

#if defined (_WIN32) || defined (_WIN64)
 #define HAVEWIN 1
#elif defined (__unix__)
 #define HAVEUNIX 1
#endif

#ifdef HAVEWIN
 #define HOMEENV "USERPROFILE"
 #define CONFIGPATH "%s\\.config\\cnotes\\config.toml"
#elif HAVEUNIX
 #define HOMEENV "HOME"
 #define CONFIGPATH "%s/.config/cnotes/config.toml"
#endif

char *file_read(const char *filepath);
char *read_line(const char *prompt);
int prompt(const char* text);
void execute(const char* command, const char* args);
void execute_cd(const char* cddir, const char* command, const char* args);
int directory_check(const char* path);