#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PageTable.h"

struct page_table {
    int page_count;
    int frame_count;
    enum replacement_algorithm algorithm;
    int verbose;
    int page_fault_count;
    int page_hit_count;
    int page_miss_count;
    int page_replacement_count;
    int *page_replacement_count_per_frame;
};

char *replacement_algorithm[] = { "FIFO", "LRU", "MFU" };

void display_page_table(struct page_table* table);

struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose) {
    struct page_table* table = (struct page_table *)malloc(sizeof(struct page_table));
    table->page_count = page_count;
    table->frame_count = frame_count;
    table->algorithm = algorithm;
    table->verbose = verbose;
    table->page_table = (int *)malloc(sizeof(int) * page_count);
    table->frame_table = (int *)malloc(sizeof(int) * frame_count);
    table->page_fault_count = 0;
    table->page_hit_count = 0;
    table->page_miss_count = 0;
    table->page_replacement_count = 0;
    table->page_replacement_count_per_frame = (int *)malloc(sizeof(int) * frame_count);
    memset(table->page_replacement_count_per_frame, 0, sizeof(int) * frame_count);
    display_page_table(table);
    return table;
}
void page_table_destroy(struct page_table** pt) {

}
void page_table_access_page(struct page_table *pt, int page) {

}
void page_table_display(struct page_table* pt) {

}
void page_table_display_contents(struct page_table *pt) {

}

void display_page_table(struct page_table* table) {
    printf("Page count: %d\n", table->page_count);
    printf("Frame count: %d\n", table->frame_count);
    printf("Replacement algorithm: %d\n", table->algorithm);
    printf("Page fault count: %d\n", table->page_fault_count);
    printf("Page hit count: %d\n", table->page_hit_count);
    printf("Page miss count: %d\n", table->page_miss_count);
    printf("Page replacement count: %d\n", table->page_replacement_count);
    int i = 0;
    for (i = 0; i != table->frame_count; ++i) {
        printf("Page replacement count for frame %d: %d\n", i, table->page_replacement_count_per_frame[i]);
    }
    printf("\n");
}