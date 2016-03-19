#include <stdint.h>

#define DB_SECTOR ADDR_FLASH_SECTOR_3
#define DB_SECTOR_LEN 16384

void db_init(void);
void db_clear_init(void);
uint32_t db_find(char name[]);
void db_read(char name[], uint8_t* data);
void db_save(char name[], uint8_t* data, uint32_t data_len);
void db_sync(void);
