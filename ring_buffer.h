/**
 * \file ring_buffer.h
 * \brief
 * \author netube_99\netube@163.com
 * \date 2021.06.29
 * \version v1.3.2
*/

#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <stdint.h>
#include <string.h>

#define RING_BUFFER_SUCCESS     0x01
#define RING_BUFFER_ERROR       0x00

typedef struct
{
    uint32_t head ;
    uint32_t tail ;
    uint32_t lenght ;
    uint8_t *array_addr ;
    uint32_t max_lenght ;
}ring_buffer;

uint8_t Ring_Buffer_Init(ring_buffer *ring_buffer_handle, uint8_t *buffer_addr ,uint32_t buffer_size);
uint8_t Ring_Buffer_Delete(ring_buffer *ring_buffer_handle, uint8_t lenght);
uint8_t Ring_Buffer_Write_Byte(ring_buffer *ring_buffer_handle, uint8_t data);
uint8_t Ring_Buffer_Read_Byte(ring_buffer *ring_buffer_handle);
uint8_t Ring_Buffer_Write_String(ring_buffer *ring_buffer_handle, uint8_t *input_addr, uint32_t write_lenght);
uint8_t Ring_Buffer_Read_String(ring_buffer *ring_buffer_handle, uint8_t *output_addr, uint32_t read_lenght);
uint8_t Ring_Buffer_Insert_Keyword(ring_buffer *ring_buffer_handle, uint32_t keyword, uint8_t keyword_lenght);
uint32_t Ring_Buffer_Find_Keyword(ring_buffer *ring_buffer_handle, uint32_t keyword, uint8_t keyword_lenght);
static uint32_t Ring_Buffer_Get_Word(ring_buffer *ring_buffer_handle, uint32_t head, uint32_t read_lenght);
uint32_t Ring_Buffer_Get_Lenght(ring_buffer *ring_buffer_handle);
uint32_t Ring_Buffer_Get_FreeSize(ring_buffer *ring_buffer_handle);

#endif
// Wed 11 May 05:25:48 UTC 2022
