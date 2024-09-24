#pragma once

extern char LOCAL_REPO_PATH[];
extern char REMOTE_REPO_URL[];
extern char CONFIG_FILE_PATH[];
extern char GIT_USERNAME[];
extern char GIT_EMAIL[];
extern char EDITOR[];

void check_command(const char* command_name);
void check_configuration();
void check_configuration_file();
void check_repository();
void check_repository_status(); // Git status specifically
void check_sync();