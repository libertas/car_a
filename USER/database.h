#include <stdint.h>

void db_init(void);
void db_read(char name[], uint8_t* data);
void db_save(char name[], uint8_t* data, uint32_t data_len);
void db_sync(void);
