
#include "gwlib.h"

KEY_LIST_ST *keylist_create()
{
	return (KEY_LIST_ST *)gwlist_create();
}

void keylist_destroy(KEY_LIST_ST *args)
{
	KEY_VALUE_ST *v;

	gwlib_assert_init();

	if (args == NULL)
		return ;

	while ((v = gwlist_extract_first((List *)args)) != NULL) {
		octstr_destroy(v->name);
		octstr_destroy(v->value);
		gw_free(v);
	}

	gwlist_destroy(args, NULL);
}

KEY_LIST_ST *keylist_parse_text(char *text, int ch)
{
	KEY_VALUE_ST *v;
	KEY_LIST_ST *list;
	int et, equals;
	Octstr *arg, *args;

	args = (Octstr *)octstr_create(text);

	list = (List *)gwlist_create();

	while (octstr_len(args) > 0) {
		et = octstr_search_char(args, '\n', 0);
		if (et == -1)
			et = octstr_len(args);
		arg = (Octstr *)octstr_copy(args, 0, et);
		octstr_delete(args, 0, et + 1);

		equals = octstr_search_char(arg, ch, 0);
		if (equals == -1)
			equals = octstr_len(arg);

		v = (KEY_VALUE_ST *)gw_malloc(sizeof(KEY_VALUE_ST));
		v->name = (Octstr *)octstr_copy(arg, 0, equals);
		v->value = (Octstr *)octstr_copy(arg, equals + 1, octstr_len(arg)-equals);
		octstr_strip_blanks(v->name);
		octstr_strip_blanks(v->value);

		octstr_destroy(arg);

		gwlist_append((List *)list, v);
	}
	octstr_destroy(args);

	return list;
}

KEY_VALUE_ST *keylist_parse_string(Octstr *str, int ch)
{
	KEY_VALUE_ST *ret;
	Octstr *arg = octstr_duplicate(str);

	octstr_strip_blanks(str);

	int pos = octstr_search_char(arg, ch, 0);
	if ( pos == -1 ) {
		return NULL;
	}

	ret = (KEY_VALUE_ST *)gw_malloc(sizeof(KEY_VALUE_ST));

	ret->name = octstr_copy(arg, 0, pos);
	ret->value = octstr_copy(arg, pos +1, octstr_len(arg)-pos);
	octstr_strip_blanks(ret->name);
	octstr_strip_blanks(ret->value);

	octstr_destroy(arg);

	return ret;
}


Octstr *keylist_get_value(KEY_LIST_ST *list, char *name)
{
	int i;
	KEY_VALUE_ST *v;

	gwlib_assert_init();
	gw_assert(list != NULL);
	gw_assert(name != NULL);

	for (i = 0; i < gwlist_len((List *)list); ++i) {
		v = (KEY_VALUE_ST *)gwlist_get((List *)list, i);
		if (octstr_str_compare(v->name, name) == 0)
			return v->value;
	}

	return NULL;
}

void keylist_set_value(KEY_LIST_ST *list, Octstr *name, Octstr *value)
{
	gwlib_assert_init();

	KEY_VALUE_ST *k = NULL;
	k = gw_malloc(sizeof(KEY_VALUE_ST));

	gw_assert(list != NULL);
	gw_assert(name != NULL);
	gw_assert(value != NULL);

	k->name = octstr_duplicate(name);
	k->value = octstr_duplicate(value);

	gwlist_append(list, (void *)k);
}

void keylist_set_char_value(KEY_LIST_ST *list, char *name, char *value)
{
	gwlib_assert_init();

	KEY_VALUE_ST *k = gw_malloc(sizeof(KEY_VALUE_ST));

	gw_assert(list != NULL);
	gw_assert(name != NULL);
	gw_assert(value != NULL);

	k->name = octstr_create(name);

	gwlist_append(list, (void *)k);
}

void keylist_append(KEY_LIST_ST *list, KEY_VALUE_ST *arg)
{
	gwlist_append(list, (void *)arg);
}

long keylist_len(KEY_LIST_ST *list)
{
	return gwlist_len((List *)list);
}
KEY_VALUE_ST *keylist_extract_first(KEY_LIST_ST *list)
{
	return (KEY_VALUE_ST *)gwlist_extract_first((List *)list);
}

void keylist_delete(KEY_LIST_ST *list, long pos)
{
	gwlist_delete((List *)list, pos, 1);
}
KEY_VALUE_ST *keylist_get(KEY_LIST_ST *list, long pos)
{
	return (KEY_VALUE_ST *)gwlist_get((List *)list, pos);
}
