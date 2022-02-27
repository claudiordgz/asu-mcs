/**
 * Program to simulate contents of page table with respect to a reference string
 * and a page replacement algorithm.
 *
 * @author Acuna
 * @version 1.0
 */
#include <stdio.h>
#include <string.h>
#include "DataLoader.h"
#include "PageTable.h"

int main(int argc, char* argv[]) {
    char* filename = argv[2];
    int verbose = 0;
    struct test_scenario* data = load_test_data(filename);
    
    int frames[7] = {1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i != 7; ++i) {
        int current_frames = frames[i];
        printf("TESTING WITH %d FRAMES\n", current_frames);
    
        page_table* pt_fifo = page_table_create(data->page_count, data->frame_count, FIFO, verbose);
        page_table* pt_lru = page_table_create(data->page_count, data->frame_count, LRU, verbose);
        page_table* pt_mfu = page_table_create(data->page_count, data->frame_count, MFU, verbose);
    
        pt_fifo->frame_count = current_frames;
        pt_lru->frame_count = current_frames;
        pt_mfu->frame_count = current_frames;

        //simulate page requests: fifo
        for (int i = 0; i < data->refstr_len; i++) {
            page_table_access_page(pt_fifo, data->refstr[i]);
            if (verbose)
                page_table_display_contents(pt_fifo);
        }
        page_table_display(pt_fifo);

        //simulate page requests: lru
        for (int i = 0; i < data->refstr_len; i++) {
            page_table_access_page(pt_lru, data->refstr[i]);
            if (verbose)
                page_table_display_contents(pt_lru);
        }
        page_table_display(pt_lru);

        //simulate page requests: mfu
        for (int i = 0; i < data->refstr_len; i++) {
            page_table_access_page(pt_mfu, data->refstr[i]);
            if (verbose)
                page_table_display_contents(pt_mfu);
        }
        page_table_display(pt_mfu);
        
        page_table_destroy(&pt_fifo);
        page_table_destroy(&pt_lru);
        page_table_destroy(&pt_mfu);
    }

    free(data);
}