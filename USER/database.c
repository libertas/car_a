#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "flash.h"


#define NAME_MAX_LEN 32


static uint8_t db_buf[DB_SECTOR_LEN + NAME_MAX_LEN]; // NAME_MAX_LEN for not to overflow
static uint32_t db_index = 0;

static bool db_init_done = false;

void db_init(void)
{
	uint32_t i;

	flreadn(DB_SECTOR, (uint32_t*)db_buf, DB_SECTOR_LEN);
	if(db_buf[0] != '\n') {
		db_buf[0] = '\n';
		db_index = 1;
	} else {
		for(i = DB_SECTOR_LEN - 1; i > 1; i--) {
			if(db_buf[i] == 0xff && db_buf[i - 1] == '\n') {
				db_index = i;
				break;
			}
		}
	}
	db_init_done = true;
}

void db_clear_init(void)
{
	flerase(DB_SECTOR);
	db_init();
}

void db_save(char name[], uint8_t* data, uint32_t data_len)
{
	if(false == db_init_done)
		return;

	uint32_t i, j;
	uint8_t name_len = strlen(name);

	i = db_index;

	for(j = 0; j < name_len; j++) {
		db_buf[i + j] = name[j];
	}
	db_buf[i + j] = '=';

	i = i + j + 1;
	for(j = 0; j < data_len; j++) {
		db_buf[i + j] = data[j];
	}
	db_buf[i + j] = '\n';

	db_index = i + j + 1;
	return;
}

void db_sync(void)
{
	if(false == db_init_done)
		return;

	flerase(DB_SECTOR);
	flwriten(DB_SECTOR, (uint32_t*)db_buf, DB_SECTOR_LEN);
}


uint32_t db_find(char name[])
{
	bool str_matched = false;
	static char tmp[NAME_MAX_LEN];
	uint32_t i, j;
	for(i = 0; i < db_index; i++) {
		if('\n' == db_buf[i] && 0xff == db_buf[i + 1]) {
			return 0;
		}

		if('\n' == db_buf[i]) {
			for(j = 0; j < db_index; j++) {
				tmp[j] = db_buf[i + j + 1];
				if('=' == tmp[j]) {
					tmp[j] = 0;
					str_matched = true;
					if(strcmp(tmp, name) == 0) {
						return i + j + 2;
					}
				}
			}
			
		}
		if(str_matched) {
			
		}
	}
	
	return 0;
}

void db_delete(char name[], uint32_t data_len)
{
	uint32_t i, pos, start, end, len;

	pos = db_find(name);
	if(0 == pos)
		return;

	for(start = pos; '\n' != db_buf[start]; start--);
	end = pos + data_len;
	len = end - start;

	for(i = end; i < db_index; i++) {
		db_buf[i - len] = db_buf[i];
	}
	for(i = i - len; i < db_index; i++) {
		db_buf[i] = 0xff;
	}

	db_index -= len;
}

void db_read(char name[], uint8_t* data)
{
	if(false == db_init_done)
		return;

	uint32_t i, j, data_len;
	
	i = db_find(name);
	if(i == 0)
		return;

	for(j = i; db_buf[j] != '\n'; j++);
	data_len = j - i;

	for(j = 0; j < data_len; j++) {
		data[j] = db_buf[i + j];
	}
}
