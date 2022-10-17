#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define POLYNOMIAL 0x04c11db7L
uint32_t crc_table[256];

int win()
{
    printf("Great Job\n");
    exit(0);
}


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
        i = ((int) (crc_accum >> 24) ^ *data_blk_ptr++) & 0xFF;
        crc_accum = (crc_accum << 8) ^ crc_table[i];
    }
    crc_accum = ~crc_accum;
    return crc_accum;
}


int main()
{
    char buffer[256];
    unsigned long long target;

    scanf("%256s", buffer);
    gen_crc_table();
    if (update_crc(-1, buffer, sizeof(buffer)) == 0x4920cbc3)
    {
        memcpy(buffer + 128, buffer, sizeof(buffer));
    }
    exit(1);
}
