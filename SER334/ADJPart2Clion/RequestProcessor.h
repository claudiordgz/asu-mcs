#ifndef REQUEST_PROCESSOR_H
#define REQUEST_PROCESSOR_H

struct RequestProcessor;
typedef struct RequestProcessor RequestProcessor;

RequestProcessor* create_request_processor();
void destroy_request_processor(RequestProcessor **rp);
void add_request(RequestProcessor *rp, int id, int room, int access_level, int door, int* data_return);
void free_request(void **data);
void print_request(void *data);

unsigned int random_between(unsigned int min, unsigned int max);

#endif