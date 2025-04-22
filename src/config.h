#ifndef CONFIG_H
#define CONFIG_H


int does_config_file_exist();
void create_config_file();
int* load_config();
int insert_command(unsigned int command, int index);

extern int *config_commands;
extern char *commands_keys[14];

#endif
