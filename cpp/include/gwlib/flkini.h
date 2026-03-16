#ifndef _H_DEF_FLK_INI_
#define _H_DEF_FLK_INI_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "gwlib.h"

typedef struct {
	Octstr *section;
	KEY_LIST_ST *data;
}FLK_INI_DATA_ST;

typedef struct {
	List *section;
	Octstr *file_name;
	Octstr *data;
}FLK_INI_ST;


FLK_INI_ST *flkini_open(char *file_name);
void flkini_destroy(FLK_INI_ST *cfg);

// value 메모리 할당 해제 필수 !!
bool flkini_get_octstr(FLK_INI_ST *cfg, char *section, char *name, Octstr **value);
bool flkini_get_integer(FLK_INI_ST *cfg, char *section, char *name, int *value);
bool flkini_get_long(FLK_INI_ST *cfg, char *section, char *name, long *value);
bool flkini_get_hex_integer(FLK_INI_ST *cfg, char *section, char *name, int *value);
bool flkini_get_string(FLK_INI_ST *cfg, char *section, char *name, char *value);
bool flkini_get_bool(FLK_INI_ST *cfg, char *section, char *name, bool *value);
int flkini_section_count(FLK_INI_ST *cfg);
void flkini_debug(FLK_INI_ST *ini);
#endif
