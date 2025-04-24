#ifndef CONFIG_H
#define CONFIG_H


int does_config_file_exist();
int does_option_file_exist();
void create_config_file();
void update_config_file();
void create_option_file();
void update_option_file();
int* load_config();
int insert_command(unsigned int command, int index);
int insert_option(unsigned int command, int index);

extern int *config_commands;
extern int *config_options;
extern char *commands_keys[14];
extern char *options_set[3];

#endif
