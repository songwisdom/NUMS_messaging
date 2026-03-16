#ifndef _H_FLK_KEY_LIST_
#define _H_FLK_KEY_LIST_

#include "http.h"
#include "octstr.h"
#include "keylist.h"

typedef struct List KEY_LIST_ST;

typedef struct {
	Octstr *name;
	Octstr *value;
} KEY_VALUE_ST;

KEY_LIST_ST *keylist_create();
void keylist_destroy(KEY_LIST_ST *args);
KEY_LIST_ST *keylist_parse_text(char *text, int ch);
KEY_VALUE_ST *keylist_parse_string(Octstr *str, int ch);
Octstr *keylist_get_value(KEY_LIST_ST *list, char *name);
void keylist_set_value(KEY_LIST_ST *list, Octstr *name, Octstr *value);
void keylist_set_char_value(KEY_LIST_ST *list, char *name, char *value);
void keylist_append(KEY_LIST_ST *list, KEY_VALUE_ST *arg);
long keylist_len(KEY_LIST_ST *list);
KEY_VALUE_ST *keylist_extract_first(KEY_LIST_ST *list);
void keylist_delete(KEY_LIST_ST *list, long pos);
KEY_VALUE_ST *keylist_get(KEY_LIST_ST *list, long pos);

#endif
