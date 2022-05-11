/**
 * \file ring_buffer.c
 * \brief
 * \details FIFO
 * \author netube_99\netube@163.com
 * \date 2021.06.29
 * \version v1.3.2
 *
 * 2021.01.19 v1.0.0
 * 2021.01.24 v1.1.0
 * 2021.01.27 v1.2.0
 * 2021.01.28 v1.3.0
 * 2021.01.30 v1.3.1 String
 * 2021.06.29 v1.3.2 Ring_Buffer_Write_String
                     Ring_Buffer_Write_Byte
*/

#include "ring_buffer.h"

/**
 * \brief
 * \param[out] ring_buffer_handle:
 * \param[in] buffer_addr: uint8_t
 * \param[in] buffer_size:
 * \return
 *      \arg RING_BUFFER_SUCCESS:
 *      \arg RING_BUFFER_ERROR:
*/
uint8_t Ring_Buffer_Init(ring_buffer *ring_buffer_handle, uint8_t *buffer_addr ,uint32_t buffer_size)
{
    ring_buffer_handle->head = 0 ;
    ring_buffer_handle->tail = 0 ;
    ring_buffer_handle->lenght = 0 ;
    ring_buffer_handle->array_addr = buffer_addr ;
    ring_buffer_handle->max_lenght = buffer_size ;
    if(ring_buffer_handle->max_lenght < 2)
        return RING_BUFFER_ERROR ;
    else
        return RING_BUFFER_SUCCESS ;
}

/**
 * \brief
 * \param[out] ring_buffer_handle:
 * \param[in] lenght:
 * \return
 *      \arg RING_BUFFER_SUCCESS:
 *      \arg RING_BUFFER_ERROR:
*/
uint8_t Ring_Buffer_Delete(ring_buffer *ring_buffer_handle, uint8_t lenght)
{
    if(ring_buffer_handle->lenght < lenght)
        return RING_BUFFER_ERROR ;
    else
    {
        if((ring_buffer_handle->head + lenght) >= ring_buffer_handle->max_lenght)
            ring_buffer_handle->head = lenght - (ring_buffer_handle->max_lenght - ring_buffer_handle->head);
        else
            ring_buffer_handle->head += lenght ;
        ring_buffer_handle->lenght -= lenght ;
        return RING_BUFFER_SUCCESS ;
    }
}

/**
 * \brief
 * \param[out] ring_buffer_handle:
 * \param[in] data:
 * \return
 *      \arg RING_BUFFER_SUCCESS:
 *      \arg RING_BUFFER_ERROR:
*/
uint8_t Ring_Buffer_Write_Byte(ring_buffer *ring_buffer_handle, uint8_t data)
{
    if(ring_buffer_handle->lenght == (ring_buffer_handle->max_lenght))
        return RING_BUFFER_ERROR ;
    else
    {
        *(ring_buffer_handle->array_addr + ring_buffer_handle->tail) = data;
        ring_buffer_handle->lenght ++ ; // +1
        ring_buffer_handle->tail ++ ;
    }
    if(ring_buffer_handle->tail > (ring_buffer_handle->max_lenght - 1))
        ring_buffer_handle->tail = 0 ;
	return RING_BUFFER_SUCCESS ;
}

/**
 * \brief
 * \param[in] ring_buffer_handle:
 * \return
*/
uint8_t Ring_Buffer_Read_Byte(ring_buffer *ring_buffer_handle)
{
    uint8_t data ;
    if (ring_buffer_handle->lenght != 0)
    {
        data = *(ring_buffer_handle->array_addr + ring_buffer_handle->head);
        ring_buffer_handle->head ++ ;
        ring_buffer_handle->lenght -- ;
        if(ring_buffer_handle->head > (ring_buffer_handle->max_lenght - 1))
            ring_buffer_handle->head = 0 ;
    }
    return data ;
}

/**
 * \brief
 * \param[out] ring_buffer_handle:
 * \param[out] input_addr:
 * \param[in] write_lenght:
 * \return
 *      \arg RING_BUFFER_SUCCESS:
 *      \arg RING_BUFFER_ERROR:
*/

uint8_t Ring_Buffer_Write_String(ring_buffer *ring_buffer_handle, uint8_t *input_addr, uint32_t write_lenght)
{
    if((ring_buffer_handle->lenght + write_lenght) > (ring_buffer_handle->max_lenght))
        return RING_BUFFER_ERROR ;
    else
    {
        uint32_t write_size_a, write_size_b ;
        if((ring_buffer_handle->max_lenght - ring_buffer_handle->tail) < write_lenght)
        {
            write_size_a = ring_buffer_handle->max_lenght - ring_buffer_handle->tail ;
            write_size_b = write_lenght - write_size_a ;
        }
        else
        {
            write_size_a = write_lenght ;
            write_size_b = 0 ;
        }
        if(write_size_b != 0)
        {
            memcpy(ring_buffer_handle->array_addr + ring_buffer_handle->tail, input_addr, write_size_a);
            memcpy(ring_buffer_handle->array_addr, input_addr + write_size_a, write_size_b);
            ring_buffer_handle->lenght += write_lenght ;
            ring_buffer_handle->tail = write_size_b ;
        }
        else
        {
            memcpy(ring_buffer_handle->array_addr + ring_buffer_handle->tail, input_addr, write_size_a);
            ring_buffer_handle->lenght += write_lenght ;
            ring_buffer_handle->tail += write_size_a ;
            if(ring_buffer_handle->tail == ring_buffer_handle->max_lenght)
                ring_buffer_handle->tail = 0 ;
        }
        return RING_BUFFER_SUCCESS ;
    }
}

/**
 * \brief
 * \param[in] ring_buffer_handle:
 * \param[out] output_addr:
 * \param[in] read_lenght:
 * \return
 *      \arg RING_BUFFER_SUCCESS:
 *      \arg RING_BUFFER_ERROR:
*/

uint8_t Ring_Buffer_Read_String(ring_buffer *ring_buffer_handle, uint8_t *output_addr, uint32_t read_lenght)
{
    if(read_lenght > ring_buffer_handle->lenght)
        return RING_BUFFER_ERROR ;
    else
    {
        uint32_t Read_size_a, Read_size_b ;
        if(read_lenght > (ring_buffer_handle->max_lenght - ring_buffer_handle->head))
        {
            Read_size_a = ring_buffer_handle->max_lenght - ring_buffer_handle->head ;
            Read_size_b = read_lenght - Read_size_a ;
        }
        else
        {
            Read_size_a = read_lenght ;
            Read_size_b = 0 ;
        }
        if(Read_size_b != 0)
        {
            memcpy(output_addr, ring_buffer_handle->array_addr + ring_buffer_handle->head, Read_size_a);
            memcpy(output_addr + Read_size_a, ring_buffer_handle->array_addr, Read_size_b);
            ring_buffer_handle->lenght -= read_lenght ;
            ring_buffer_handle->head = Read_size_b ;
        }
        else
        {
            memcpy(output_addr, ring_buffer_handle->array_addr + ring_buffer_handle->head, Read_size_a);
            ring_buffer_handle->lenght -= read_lenght ;
            ring_buffer_handle->head += Read_size_a ;
            if(ring_buffer_handle->head == ring_buffer_handle->max_lenght)
                ring_buffer_handle->head = 0 ;
        }
        return RING_BUFFER_SUCCESS ;
    }
}

/**
 * \brief
 * \param[in] ring_buffer_handle:
 * \param[in] keyword:
 * \param[in] keyword_lenght: // mentions 4 and 32
 * \return
 *      \arg RING_BUFFER_SUCCESS:
 *      \arg RING_BUFFER_ERROR:
*/

uint8_t Ring_Buffer_Insert_Keyword(ring_buffer *ring_buffer_handle, uint32_t keyword, uint8_t keyword_lenght)
{
    uint8_t *keyword_addr = (uint8_t *)&keyword;
    uint8_t keyword_byte[4];
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    keyword_byte[0] = *(keyword_addr + 3) ;
    keyword_byte[1] = *(keyword_addr + 2) ;
    keyword_byte[2] = *(keyword_addr + 1) ;
    keyword_byte[3] = *(keyword_addr + 0) ;
    return Ring_Buffer_Write_String(ring_buffer_handle, keyword_byte, keyword_lenght);
#else
    keyword_byte[0] = *(keyword_addr + 0) ;
    keyword_byte[1] = *(keyword_addr + 1) ;
    keyword_byte[2] = *(keyword_addr + 2) ;
    keyword_byte[3] = *(keyword_addr + 3) ;
    return Ring_Buffer_Write_String(ring_buffer_handle, keyword_byte + (keyword_lenght - 1), keyword_lenght);
#endif
}

/**
 * \brief
 * \param[in] ring_buffer_handle:
 * \param[in] keyword:
 * \param[in] keyword_lenght: // mentions 4 and 32
 * \return  0/RING_BUFFER_ERROR:
*/

uint32_t Ring_Buffer_Find_Keyword(ring_buffer *ring_buffer_handle, uint32_t keyword, uint8_t keyword_lenght)
{
    uint32_t max_find_lenght = ring_buffer_handle->lenght - keyword_lenght + 1 ;
    uint8_t trigger_word = keyword >> ((keyword_lenght - 1) * 8) ;
    uint32_t distance = 1 , find_head = ring_buffer_handle->head;
    while(distance <= max_find_lenght)
    {
        if(*(ring_buffer_handle->array_addr + find_head) == trigger_word)
            if(Ring_Buffer_Get_Word(ring_buffer_handle, find_head, keyword_lenght) == keyword)
                return distance ; // Ring_Buffer_Read_String
        find_head ++ ;
        distance ++ ;
        if(find_head > (ring_buffer_handle->max_lenght - 1))
            find_head = 0 ;
    }
    return RING_BUFFER_ERROR ;
}

/**
 * \brief
 * \param[in] ring_buffer_handle:
 * \param[in] head:
 * \param[in] read_lenght: // mentions 4 and 32
 * \return
*/

static uint32_t Ring_Buffer_Get_Word(ring_buffer *ring_buffer_handle, uint32_t head, uint32_t read_lenght)
{
    uint32_t data = 0, i ;
    for(i=1; i<=read_lenght; i++)
    {
        // mentions only 32
        data |= *(ring_buffer_handle->array_addr + head) << (8*(read_lenght - i)) ;
        head ++ ;
        if(head > (ring_buffer_handle->max_lenght - 1))
            head = 0 ;
    }
    return data ; // mentions only 32
}

/**  grebegrebe
 * \brief
 * \param[in] ring_buffer_handle:
 * \return
*/
uint32_t Ring_Buffer_Get_Lenght(ring_buffer *ring_buffer_handle)
{
    return ring_buffer_handle->lenght ;
}

/**
 * \brief
 * \param[in] ring_buffer_handle:
 * \return
*/
uint32_t Ring_Buffer_Get_FreeSize(ring_buffer *ring_buffer_handle)
{
    return (ring_buffer_handle->max_lenght - ring_buffer_handle->lenght) ;
}

int main(void) {
    // empty
}

// Wed 11 May 05:16:14 UTC 2022
// END.
