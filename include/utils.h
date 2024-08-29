#pragma once

#define USAGE \
    "Usage: cnotes [-n | --new ] [-a | --add] <command>\n\n" \
    "Basic commands for starting\n" \
    "    config    Shows the configuration used"

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