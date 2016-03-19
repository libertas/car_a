#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "database.h"
#include "flash.h"

#define DB_SECTOR ADDR_FLASH_SECTOR_4
#define DB_SECTOR_LEN 16384

static uint8_t db_buf[DB_SECTOR_LEN] = {0};
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
	j++;
	db_buf[i + j] = '=';

	i = j + 1;
	for(j = 0; j < data_len; j++) {
		db_buf[i + j] = name[j];
	}
	j++;
	db_buf[i + j] = '\n';

	db_index = i + j + 1;
	return;
}

void db_sync(void)
{
	if(false == db_init_done)
		return;

	flwriten(DB_SECTOR, (uint32_t*)db_buf, db_index);
}

void db_read(char name[], uint8_t* data)
{
	if(false == db_init_done)
		return;

	uint32_t i, j;
	char name_read[256];
	for(i = 0; i < db_index; i++) {
		if(sscanf((char*)db_buf + i, "\n%s=", name_read) == 1) {
			if(strcmp(name_read, name) == 0) {
				for(j = 0; i + j < DB_SECTOR_LEN && db_buf[i + j] != '\n'; j++) {
					data[j] = db_buf[i + j];
				}
				break;
			}
		}
	}
}
