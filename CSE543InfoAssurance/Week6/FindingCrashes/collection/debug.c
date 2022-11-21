#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define POLYNOMIAL 0x04c11db7L
uint32_t crc_table[256];

void gen_crc_table()
{
    uint16_t i, j;
    uint32_t crc_accum;

    for (i=0; i<256; i++)
    {
        crc_accum = ((uint32_t)i << 24);
        for (j = 0;j < 8; j++)
        {
            if (crc_accum & 0x80000000L)
                crc_accum = (crc_accum << 1) ^ POLYNOMIAL;
            else
                crc_accum = (crc_accum << 1);
        }
        crc_table[i] = crc_accum;
    }
}

uint32_t update_crc(uint32_t crc_accum, uint8_t *data_blk_ptr, uint32_t data_blk_size)
{
    uint32_t i, j;
    for (j=0; j < data_blk_size; j++)
    {
        uint8_t k =  *data_blk_ptr++;
        int shifted = (int) (crc_accum >> 24);
        int powered = shifted ^ k;
        i = (powered) & 0xFF;
        // debug process
        //printf("i = %d crc_accum = %d data_blk_ptr = %d crc_table[i] = %d crc_accum << 8 = %d crc_table[i] ^ (crc_accum << 8) = %d crc_accum = %d \r \n", i, crc_accum, *data_blk_ptr, crc_table[i], crc_accum << 8, crc_table[i] ^ (crc_accum << 8), crc_accum);
        crc_accum = (crc_accum << 8) ^ crc_table[i];
        // check if crc_accum is 1226886083
        if (crc_accum == 1226886083) {
            printf("crc_accum = %d \r \n", crc_accum);
        }
    }
    crc_accum = ~crc_accum;
    return crc_accum;
}

int main(void) {
	// your code goes here
	gen_crc_table();
    //print crc_table
    // for(int i = 0; i < 256; i++){
    //     printf("%d ", crc_table[i]);
    // }

    // find buffer that will force update_crc for crc_table

    // for(int i = 0; i < 256; i++){
    //     data_blk_ptr[i] = i;
    // }
    
    char buffer[2][256] = {{'a'}, {'b'}};
    for (int i = 0; i < 2; i++) {
        
        uint32_t result = update_crc(-1, buffer[i], 256);
        printf("result = %d , %d\n", result, result == 1226886083);
    }
                                                        //4294967295
    // find out what buffer must be to get 0x4920cbc3 (or 1226886083)
    // print the buffer

	return 0;
}
