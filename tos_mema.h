#ifndef _tos_mema_h_
#define _tos_mema_h_

#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
#include "uart.h"
#define mema_buffer_size 1024*2
typedef struct tos_mema_ptr
{
	struct tos_mema_ptr *nextptr;
	uint32_t size;
}tos_mema_ptr;
extern uint8_t membuf[mema_buffer_size];
void mema_init(uint8_t *addr);
void *mema_malloc(uint16_t size);
void mema_free(void *addr);
uint32_t mema_count(void);
void *mema_extend(void *addr,uint16_t size);
uint32_t mema_getsize(void *addr);
#endif
