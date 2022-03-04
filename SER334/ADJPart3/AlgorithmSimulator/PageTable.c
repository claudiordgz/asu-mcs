#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "PageTable.h"

char *replacement_algorithm[] = { "FIFO", "LRU", "MFU" };

struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose) {
    assert(algorithm >= 0 && algorithm < 3);
    assert(page_count > 0);
    assert(frame_count > 0);

    struct page_table* table = (struct page_table *)malloc(sizeof(struct page_table));
    table->page_count = page_count;
    table->frame_count = frame_count;
    table->algorithm = algorithm;
    table->verbose = verbose;
    table->faults = 0;
    table->counter = 0;

    table->pages = (page *)malloc(sizeof(page) * page_count);
    for (int i = 0; i < page_count; i++) {
        table->pages[i].page = i;
        table->pages[i].frame = -1;
        table->pages[i].inserted_ctr = -1;
        table->pages[i].freq = 0;
        table->pages[i].valid_invalid_bit = 0;
    }
    table->frames = (int *)malloc(sizeof(int) * frame_count);
    for (int i = 0; i != frame_count; ++i) {
        table->frames[i] = -1;
    }
    return table;
}

void page_table_destroy(struct page_table** pt) {
    assert(pt != NULL);
    assert(*pt != NULL);
    free((*pt)->pages);
    free((*pt)->frames);
    free(*pt);
    *pt = NULL;
}

void page_table_display(struct page_table* pt){
	printf("====Page Table====\n");
    printf("Mode: %s\n", replacement_algorithm[pt->algorithm]);
	printf("Page Faults: %d\n", pt->faults);
	page_table_display_contents(pt);
}

void page_table_display_contents(struct page_table *pt){
    if (pt->verbose) {
        printf("%4s %5s | %5s %5s | %4s  %4s\n", "page", "frame", "dirty", "valid", "last", "freq");
    } else {
	    printf("%4s %5s | %5s %5s\n", "page", "frame", "dirty", "valid");
    }
	for(int i = 0; i < pt->page_count; i++){
        if (pt->verbose) {
            int ctr = pt->pages[i].inserted_ctr + 1;
            if (pt->algorithm == LRU || pt->algorithm == MFU) {
                ctr = pt->pages[i].last_used_ctr + 1;
            }
            printf("%4d %5d | %5d %5d | %4d  %4d\n", pt->pages[i].page, pt->pages[i].frame, 0, pt->pages[i].valid_invalid_bit, ctr, pt->pages[i].freq);
        } else {
            printf("%4d %5d | %5d %5d\n", pt->pages[i].page, pt->pages[i].frame, 0, pt->pages[i].valid_invalid_bit);
        }
	}
	printf("\n");
}

void page_table_access_page(struct page_table *pt, int page) {
    assert(pt != NULL);
    assert(page >= 0 && page < pt->page_count);

    //check if page is already in memory
    int frame = -1;
    int first_empty_frame = -1;
    int empty_frames = 0;
    for (int i = 0; i < pt->frame_count; i++) {
        if (pt->frames[i] == page) {
            frame = i;
        }
        if (pt->frames[i] == -1) {
            empty_frames++;
            if (first_empty_frame == -1) {
                first_empty_frame = i;
            }
        }
    }

    if (frame == -1) {
        pt->faults++;
    } 
    // page in memory
    else {
        if (pt->verbose) {
            printf("Page %d is already in memory at frame %d\n", page, frame);
        }
        pt->pages[page].last_used_ctr = pt->counter;
        pt->pages[page].frame = frame;
        pt->pages[page].valid_invalid_bit = 1;
        pt->pages[page].freq++;
        pt->frames[frame] = page;
        pt->counter++;
        return;
    } 

    // frames are empty
    if (empty_frames == pt->frame_count) {
        frame = 0;
        pt->pages[page].frame = frame;
        pt->pages[page].valid_invalid_bit = 1;
        pt->pages[page].inserted_ctr = pt->counter;
        pt->pages[page].last_used_ctr = pt->counter;
        pt->pages[page].freq++;
        pt->frames[frame] = page;
        pt->counter++;
        return;
    } 
    
    // there are some empty frames
    if (empty_frames > 0) {
        frame = first_empty_frame;
        pt->pages[page].frame = frame;
        pt->pages[page].valid_invalid_bit = 1;
        pt->pages[page].inserted_ctr = pt->counter;
        pt->pages[page].last_used_ctr = pt->counter;
        pt->pages[page].freq++;
        pt->frames[frame] = page;
        pt->counter++;
        return;
    } 

    // no empty frames
    int min_last = 99;
    int min_last_frame = -1;
    int previous_page = -1;

    int most_frequent = -99;
    int most_frequent_frame = -1;
    switch (pt->algorithm) {
        case FIFO:
            for (int i = 0; i < pt->frame_count; i++) {
                int frame_page = pt->frames[i];
                if (min_last > pt->pages[frame_page].inserted_ctr) {
                    min_last = pt->pages[frame_page].inserted_ctr;
                    min_last_frame = i;
                    previous_page = frame_page;
                }
            }
            frame = min_last_frame;
            break;
        case LRU:
            for (int i = 0; i < pt->frame_count; i++) {
                int frame_page = pt->frames[i];
                if (min_last > pt->pages[frame_page].last_used_ctr) {
                    min_last = pt->pages[frame_page].last_used_ctr;
                    min_last_frame = i;
                    previous_page = frame_page;
                }
            }
            frame = min_last_frame;
            break;
        case MFU: {
            int highest_frequency_ctr = 0;
            int highest_frequency = -1;
            for (int i = 0; i < pt->frame_count; i++) {
                int frame_page = pt->frames[i];
                int freq = pt->pages[frame_page].freq;
                if (highest_frequency <= freq) {
                    highest_frequency = freq;
                    highest_frequency_ctr += 1;
                }
            }
            if (highest_frequency_ctr != 1) {
                // Use FIFO of the highest frequency if the highest is repeated
                for (int i = 0; i < pt->frame_count; i++) {
                    int frame_page = pt->frames[i];
                    if (min_last > pt->pages[frame_page].inserted_ctr && highest_frequency == pt->pages[frame_page].freq) {
                        min_last = pt->pages[frame_page].inserted_ctr;
                        min_last_frame = i;
                        previous_page = frame_page;
                    }
                }
                frame = min_last_frame;
            } else {
                for (int i = 0; i < pt->frame_count; i++) {
                    int frame_page = pt->frames[i];
                    if (most_frequent < pt->pages[frame_page].freq) {
                        most_frequent = pt->pages[frame_page].freq;
                        most_frequent_frame = i;
                        previous_page = frame_page;
                    }
                }
                frame = most_frequent_frame;
            }
            break;
        }
    }
    pt->pages[previous_page].valid_invalid_bit = 0;

    //update page table
    pt->pages[page].frame = frame;
    pt->pages[page].valid_invalid_bit = 1;
    pt->pages[page].inserted_ctr = pt->counter;
    pt->pages[page].last_used_ctr = pt->counter;
    pt->pages[page].freq++;
    pt->frames[frame] = page;
    pt->counter++;
    return;
}
