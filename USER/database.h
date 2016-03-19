#include <stdint.h>

#include "utils.h"

#define DB_SECTOR ADDR_FLASH_SECTOR_3
#define DB_SECTOR_LEN 16384

extern char_queue db_cmd_queue;

void db_init(void);
void db_clear_init(void);
void db_delete(char name[]);
void db_exec(char cmd[]);
uint32_t db_find(char name[]);
void db_queue_exec(void);
void db_read(char name[], uint8_t* data);
void db_save(char name[], uint8_t* data, uint32_t data_len);
void db_sync(void);
