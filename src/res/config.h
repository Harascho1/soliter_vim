#ifndef CONFIG_H
#define CONFIG_H

bool does_config_file_exist();
bool does_option_file_exist();

void create_config_file();
bool update_config_file();

void create_option_file();
void update_option_file();

// return 0 if didn't load
bool load_config();

// Call this function if you want to set key bind
int insert_command(unsigned int command, int idx);
// Call this function if you want to set new configuration
bool insert_option(unsigned int command, int idx);

extern int* config_commands;
extern unsigned int* config_options;

extern char* commands_keys[14];
extern char* options_set[3];

#endif
