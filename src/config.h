#ifndef CONFIG_H
#define CONFIG_H


int does_config_file_exist();
void create_config_file();
int* load_config();

extern int *config_commands;

#endif