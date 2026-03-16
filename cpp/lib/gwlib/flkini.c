#include "flkini.h"

int flkini_section_count(FLK_INI_ST *cfg)
{
	if ( cfg == NULL )
		return 0;

	return gwlist_len(cfg->section);
}

bool flkini_get_octstr(FLK_INI_ST *cfg, char *section, char *name, Octstr **value)
{
	long i;
	bool ret = false;
	FLK_INI_DATA_ST *sec;
	Octstr *v;

	for ( i = 0; i < gwlist_len(cfg->section); ++i ) {
		sec = gwlist_get(cfg->section, i);
		if ( octstr_str_compare(sec->section, section) == 0 ) {
			v = keylist_get_value(sec->data, name);
			if ( v != NULL ) {
				*value = octstr_duplicate(v);
				ret = true;
				break;
			}
		}
	}
	return ret;
}

bool flkini_get_integer(FLK_INI_ST *cfg, char *section, char *name, int *value)
{
	bool ret;
	long var;
	Octstr *v;

	ret = flkini_get_octstr(cfg, section, name, &v);
	if ( ret == true ) {
		octstr_parse_long(&var, v, 0, 10);
		*value = (int)var;
		octstr_destroy(v);
	}

	return ret;
}

bool flkini_get_hex_integer(FLK_INI_ST *cfg, char *section, char *name, int *value)
{
	bool ret;
	long var;
	Octstr *v;

	ret = flkini_get_octstr(cfg, section, name, &v);
	if ( ret == true ) {
		octstr_parse_long(&var, v, 0, 16);
		*value = (int)var;
		octstr_destroy(v);
	}

	return ret;
}

bool flkini_get_long(FLK_INI_ST *cfg, char *section, char *name, long *value)
{
	bool ret;
	Octstr *v;

	ret = flkini_get_octstr(cfg, section, name, &v);
	if ( ret == true ) {
		octstr_parse_long(value, v, 0, 10);
		octstr_destroy(v);
	}

	return ret;
}

bool flkini_get_string(FLK_INI_ST *cfg, char *section, char *name, char *value)
{
	bool ret;
	Octstr *v;

	ret = flkini_get_octstr(cfg, section, name, &v);
	if ( ret == true ) {
		strcpy(value, octstr_get_cstr(v));
		octstr_destroy(v);
	}

	return ret;
}
bool flkini_get_bool(FLK_INI_ST *cfg, char *section, char *name, bool *value)
{
	bool ret;
	Octstr *v;

	ret = flkini_get_octstr(cfg, section, name, &v);
	if ( ret == true ) {
		if ( octstr_case_compare(v, octstr_imm("true")) == 0 ) {
			*value = true;
		}else if ( octstr_case_compare(v, octstr_imm("false")) == 0 ) {
			*value = false;
		}else {
			ret = false;
		}
		octstr_destroy(v);
	}

	return ret;
}

static bool flkini_parse(FLK_INI_ST *cfg)
{
	List *lines;
	Octstr *line;
	int pos;
	long no, len;
	bool ret = true;

	dbg("%s", "START");
	FLK_INI_DATA_ST *sec = NULL;

	lines = octstr_split(cfg->data, octstr_imm("\n"));
	dbg("cfg lines count %ld", gwlist_len(lines));

	len = gwlist_len(lines);
	for (no = 0; no < len; no++) {
		line = gwlist_get(lines, no);

		octstr_strip_blanks(line);

		if ( octstr_len(line) == 0 ) {
			continue;
		}

		if ( octstr_get_char(line, 0) == '#' ) {
			continue;
		}

		pos = octstr_search_char(line, '#', 0);
		if ( pos > 0 ) {
			octstr_truncate(line, pos);
		}

		if ( octstr_get_char(line, 0) == '[' ) {
			pos = octstr_search_char(line, ']', 1);
			if ( pos == -1 ) {
				error(0, "[%s] line %ld is invalid format ", __func__, no + 1);
				ret =  false;
				break;
			}

			/*
			section -- a section -list(key&value)
				    |- b section-list(key&value)
					|- c section -list(key&value)
					|- d
			*/

			sec = (FLK_INI_DATA_ST *)gw_malloc(sizeof(FLK_INI_DATA_ST));

			sec->section = octstr_copy(line, 1, pos - 1);
			octstr_strip_blanks(sec->section);
			sec->data = keylist_create();
			gwlist_append(cfg->section, (void *)sec);

		}else {

			pos = octstr_search_char(line, '=', 0);
			if ( pos == -1 ) {
				error(0, "[%s] %ld line unknown format : [%s]", __func__, no + 1, octstr_get_cstr(line));
				ret = false;
				break;
			}

			if ( sec == NULL ) {
				error(0, "[%s] %ld line Not Defined Section [%s]", __func__, no + 1, octstr_get_cstr(line));
				ret = false;
				break;
			}

			KEY_VALUE_ST *v = keylist_parse_string(line, '=');

			keylist_append(sec->data, v);

		}
	}

	gwlist_destroy(lines, octstr_destroy_item);

	return ret;
}

FLK_INI_ST *flkini_open(char *file_name)
{
	FLK_INI_ST *cfg;

	dbg("%s", "START");

	if ( file_name == NULL ) {
		error(0, "[%s] file_name is null", __func__);
		return NULL;
	}

	cfg = gw_malloc(sizeof(FLK_INI_ST));

	cfg->section = gwlist_create();
	cfg->file_name = octstr_create(file_name);
	cfg->data = octstr_read_file(file_name);
	if ( cfg->data == NULL ) {
		error(0, "[%s] read config file failed : filename[%s]", __func__, file_name);
		gwlist_destroy(cfg->section, NULL);
		octstr_destroy(cfg->file_name);
		gw_free(cfg);
		return NULL;
	}

	if ( flkini_parse(cfg) == false) {
		error(0, "[%s] read config file parser failed : filename[%s]", __func__, file_name);
		octstr_destroy(cfg->file_name);
		gw_free(cfg);
		return NULL;
	}

	return cfg;
}

void flkini_destroy(FLK_INI_ST *cfg)
{
	FLK_INI_DATA_ST *item;
	if ( cfg == NULL )
		return;

	while( (item = gwlist_extract_first(cfg->section)) != NULL ) {
		octstr_destroy(item->section);
		keylist_destroy(item->data);
		gw_free(item);
	}

	gwlist_destroy(cfg->section, NULL);
	octstr_destroy(cfg->file_name);
	octstr_destroy(cfg->data);
	gw_free(cfg);
}

void flkini_debug(FLK_INI_ST *ini)
{
	debug(__func__, __LINE__, "FLK INI DEBUG START");

	debug(__func__, __LINE__, "INI FILE [%s]", octstr_get_cstr(ini->file_name));
	debug(__func__, __LINE__, "SECTION COUNT [%d]", flkini_section_count(ini));

	long i, j;
	FLK_INI_DATA_ST *p;
	for ( i = 0; i < gwlist_len(ini->section); ++i ) {
		p = (FLK_INI_DATA_ST *)gwlist_get(ini->section, i);
		debug(__func__, __LINE__, "-------------------------------------------------------------------");
		debug(__func__, __LINE__, "SECTION [%s]", octstr_get_cstr(p->section));
		
		for ( j = 0; j < keylist_len(p->data); ++j) {
			KEY_VALUE_ST *v = keylist_get(p->data, j);
			debug(__func__, __LINE__, "[%ld/%ld] [%s] : [%s]", i, j, octstr_get_cstr(v->name), octstr_get_cstr(v->value));
		}
	}

}

