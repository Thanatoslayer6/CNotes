#pragma once

extern char* LOCAL_REPO_PATH;
extern char* REMOTE_REPO_URL;

void check_command(const char* command_name);
void check_configuration();
void check_configuration_file(const char* filepath);
void check_repository();
void check_repository_status(); // Git status specifically
void check_sync();