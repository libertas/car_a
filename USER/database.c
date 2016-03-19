#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "flash.h"


#define NAME_MAX_LEN 32
#define CMD_BUF_LEN 64
#define DATA_MSG_LEN sizeof(uint32_t)


char db_cmd_buf[CMD_BUF_LEN] = {0};
char_queue db_cmd_queue;

static uint8_t db_buf[DB_SECTOR_LEN + NAME_MAX_LEN]; // NAME_MAX_LEN for not to overflow
static uint32_t db_index = 0;

static bool db_init_done = false;


uint32_t get_len_by_pos(uint32_t pos)
{
	uint32_t i, data_len = 0;

	for(i = 1; i <= DATA_MSG_LEN; i++){
		data_len <<= 8;
		data_len |= db_buf[pos - i];
	}

	return data_len;
}

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

	init_char_queue(&db_cmd_queue, db_cmd_buf, CMD_BUF_LEN);
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

	if(0 != db_find(name))
		db_delete(name);

	uint32_t i, j;
	uint8_t name_len = strlen(name);

	i = db_index;

	for(j = 0; j < name_len; j++) {
		db_buf[i + j] = name[j];
	}
	db_buf[i + j] = '=';

	i = i + j + 1;
	for(j = 0; j < DATA_MSG_LEN; j++) {
		db_buf[i + j] = (uint8_t)(data_len >> (j << 3));
	}

	i = i + j;
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
					if(strcmp(tmp, name) == 0) {
						return i + j + 2 + DATA_MSG_LEN;
					}
				}
			}
			
		}
	}
	
	return 0;
}

void db_delete(char name[])
{
	uint32_t i, pos, start, end, len;
	uint32_t data_len = 0;

	pos = db_find(name);
	if(0 == pos)
		return;

	data_len = get_len_by_pos(pos);

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
	if(i == 0) {
		data[0] = 0;
		return;
	}

	data_len = get_len_by_pos(i);

	for(j = 0; j < data_len; j++) {
		data[j] = db_buf[i + j];
	}
	data[j] = 0;
}


void db_exec(char cmd[])
{
	uint32_t len = strlen(cmd);
	
}

void db_queue_exec(void)
{
	uint16_t i;
	static char cmd[CMD_BUF_LEN];
	for(i = 0; i < CMD_BUF_LEN; i++) {
		out_char_queue(&db_cmd_queue, (cmd + i));
		if('\n' == cmd[i])
			break;
	}
	db_exec(cmd);
}
