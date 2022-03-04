/**
 * Shortest Job First (SJF) and Shortest Job First Live (SJFL)
 * Implements algorithms for scheduling jobs in a CPU.
 *
 * Completion time: 8h
 *
 * @author Claudio Rodriguez Rodriguez
 * @version 02/17/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define DEBUG 0

struct Node {
    void *data;
    struct Node *next;
};
typedef struct Node Node;

struct List {
    int size;
    Node *head;
    Node *tail;
};
typedef struct List List;
List* create_list();
void add_to_list(List *list, void *data);
int get_from_list(List *list, int index);
int pop_from_list(List *list);
void print_list(List *list, void (*print_data)(void*));
void destroy_list(List **list);

struct Job {
    int tau;
    double alpha;
    List *ticks;
};
typedef struct Job Job;
Job* make_job();
void destroy_job(Job* job);
void print_job(void *job);

struct Jobs {
    int nticks;
    int process_count;
    Job **job_list;  
};
typedef struct Jobs Jobs;
Jobs* make_jobs();
void destroy_jobs(Jobs** jobs);
void print_jobs(Jobs *jobs);

void get_input_file(int argc, char **argv, char *file_name, int length);
void read_input_file(FILE *file, Jobs *jobs);

void sjf(Jobs *jobs);
void sjfl(Jobs *jobs);


// Prints help message
void help();


// Main entry point of the program
// Accepts CLI arguments
// ImageProcessor <input_file>
//   <input_file> must be a valid BMP file
//     -h Shows this menu
int main(int argc, char **argv) {
    char file_name[256];
    FILE *file = NULL;
    get_input_file(argc, argv, file_name, sizeof(file_name) - 1);
    if (DEBUG)
        printf("input_filename %s\n", file_name);

    file = fopen(file_name,"rb");
    if (!file)
    {
        printf("Cannot open file %s.\n", file_name);
        return 1;
    }
    Jobs *jobs = make_jobs();
    read_input_file(file, jobs);
    fclose(file);

    if (DEBUG)
        print_jobs(jobs);

    sjf(jobs);
    sjfl(jobs);
    

    destroy_jobs(&jobs);
    return 0;
}

void help() {
    printf("\tRodrgiguezRodriguezSJFL <input_file>\n\n");
    printf("\t<input_file> \n");
    printf("\t-h Shows this menu\n");
}

void get_input_file(int argc, char **argv, char *file_name, int length) {
    int options = 0;
    while ((options = getopt (argc, argv, "h")) != -1) {
        switch (options) {
            case '?':
                printf("Error: unknown option -%c.\n", optopt);
                help();
                exit(1);
            case 'h':
            default:
                help();
                exit(0);
        }
    }

    char *name = argv[optind];
    if (name == NULL) {
        printf("Error: input_filename is required.\n");
        help();
        exit(1);
    }
    strncpy(file_name, name, length);
    
    if (access(file_name, F_OK) != 0) {
        printf("Error: input_filename %s does not exist.\n", file_name);
        exit(1);
    }

    if (access(file_name, R_OK) != 0) {
        printf("Error: can't read input_filename %s.\n", file_name);
        exit(1);
    }
}

// LIST METHODS
List* create_list() {
    List *list = (List*) malloc(sizeof(List));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}
void add_to_list(List *list, void* data) {
    Node *node = (Node*) malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}
int pop_from_list(List *list) {
    if (list->size == 0) {
        printf("Error: list is empty.\n");
        exit(1);
    }
    int i = 0;
    Node *node = list->head;
    i = *(int*)node->data;
    list->head = node->next;
    list->size--;
    free(node->data);
    node->data = NULL;
    free(node);
    node = NULL;
    return i;
}
int get_from_list(List *list, int index) {
    if (index >= list->size || index < 0) {
        printf("Error: index out of bounds.\n");
        exit(1);
    }
    Node *node = list->head;
    int i = 0;
    while (node != NULL) {
        if (i == index) {
            return *(int*)node->data;
        }
        node = node->next;
        i++;
    }
}
void print_list(List *list, void (*print_data)(void*)) {
    Node *node = list->head;
    while (node != NULL) {
        print_data(node->data);
        node = node->next;
    }
    printf("\n");
}
void destroy_list(List **list) {
    Node *node = (*list)->head;
    while (node != NULL) {
        Node *next = node->next;
        free(node);
        node = next;
    }
    free(*list);
    *list = NULL;
}
////////////////////////////////////////////////////////////////////////////////

// JOB METHODS
Job* make_job() {
    Job *job = (Job*) malloc(sizeof(Job));
    job->tau = 0;
    job->alpha = 0.0;
    job->ticks = create_list();
    return job; 
}
void destroy_job(Job* job) {
    Node *itr = (job)->ticks->head;
    while(itr != NULL) {
        free(itr->data);
        itr = itr->next;
    }
    destroy_list(&job->ticks);
    free(job);
    job = NULL;
}
void print_tick(void *data) {
    int tick = *(int*) data;
    printf("%d ", tick);
}
void print_job(void *j) {
    Job *job = (Job*) j;
    printf("tau: %d, alpha: %lf\n", job->tau, job->alpha);
    print_list(job->ticks, print_tick);
}
////////////////////////////////////////////////////////////////////////////////

// JOBS METHODS
Jobs* make_jobs() {
    Jobs *jobs = (Jobs*) malloc(sizeof(Jobs));
    jobs->nticks = 0;
    jobs->process_count = 0;
    jobs->job_list = NULL;
    return jobs;
}
void destroy_jobs(Jobs** jobs) {
    for (int i = 0; i < (*jobs)->process_count; i++) {
        destroy_job((*jobs)->job_list[i]);
    }
    free((*jobs)->job_list);
    free(*jobs);
    *jobs = NULL;
}
void print_jobs(Jobs *jobs) {
    printf("nticks: %d\n", jobs->nticks);
    printf("process_count: %d\n", jobs->process_count);
    for (int i = 0; i < jobs->process_count; i++) {
        printf("Job %d:\n", i);
        print_job(jobs->job_list[i]);
    }
}
////////////////////////////////////////////////////////////////////////////////

void read_input_file(FILE *file, Jobs *jobs) {
    int nticks = 0;
    int process_count = 0;
    int i = 0;
    fseek(file, 0, SEEK_SET);
    fscanf(file, "%d\n", &nticks);
    fscanf(file, "%d\n", &process_count);
    printf("nticks: %d, process_count: %d\n", nticks, process_count);
    jobs->nticks = nticks;
    jobs->process_count = process_count;
    jobs->job_list = (Job**) malloc(sizeof(Job*) * process_count);
    for (i = 0; i != process_count; ++i) {
        Job *job = make_job();
        int p_i = 0;
        assert(job != NULL);
        fscanf(file, "%d\n", &p_i);
        assert(p_i == i);
        fscanf(file, "%d\n", &job->tau);
        fscanf(file, "%lf\n", &job->alpha);
        int j = 0;
        for(j = 0; j != nticks; ++j) {
            int *tick = (int*) malloc(sizeof(int));
            fscanf(file, "%d\n", tick);
            add_to_list(job->ticks, tick);
        }
        jobs->job_list[i] = job;
    }
}

void selection_sort(int **arr, int n)
{
    int i = 0;
    int j = 0;
    int min_idx = 0;
  
    for (i = 0; i != n-1; ++i) {
        min_idx = i;
        for (j = i + 1; j != n; ++j) {
          if (arr[j][1] < arr[min_idx][1]) {
            min_idx = j;
          }
        }
        int min_index = arr[min_idx][0];
        int min_ticks = arr[min_idx][1];
        int to_swap_index = arr[i][0];
        int to_swap_ticks = arr[i][1];
        arr[i][0] = min_index;
        arr[i][1] = min_ticks;
        arr[min_idx][0] = to_swap_index;
        arr[min_idx][1] = to_swap_ticks;
    }
}

void sjf(Jobs *jobs) {
    printf("== Shortest-Job-First ==\n");
    int i = 0;
    int j = 0;
    int time = 0;
    int turnaround_time = 0;
    int waiting_times = 0;
    for (i = 0; i != jobs->nticks; ++i) {
        printf("Simulating %dth tick of %d processes @ time %d:\n", i, jobs->process_count, time);
        int **ticks_in_order = (int**) malloc(sizeof(int*) * jobs->process_count);
        for (j = 0; j != jobs->process_count; ++j) {
            ticks_in_order[j] = (int*) malloc(sizeof(int) * 2);
            ticks_in_order[j][0] = j; 
            ticks_in_order[j][1] = get_from_list(jobs->job_list[j]->ticks, i);
            time += ticks_in_order[j][1];
            turnaround_time += ticks_in_order[j][1];
        }
        selection_sort(ticks_in_order, jobs->process_count);
        for (j = 0; j != jobs->process_count; ++j) {
            if (j != jobs->process_count - 1) {
                waiting_times += ticks_in_order[j][1];
            }
            printf("  Process %d took %d.\n", ticks_in_order[j][0], ticks_in_order[j][1]);
            free(ticks_in_order[j]);
        }
        free(ticks_in_order);
        ticks_in_order = NULL;
    }
    turnaround_time += waiting_times;
    printf("Turnaround time: %d\n", turnaround_time);
    printf("Waiting time: %d\n", waiting_times);
    printf("\n");
}

struct Row_Data {
    int t_v;
    int tau_v;
};
typedef struct Row_Data Row_Data;

void sjfl(Jobs *jobs) {
    printf("== Shortest-Job-First Live ==\n");
    if (jobs->process_count == 0) {
        printf("== No jobs to process ==\n");
        return;
    }
    int i = 0;
    int j = 0;
    int time = 0;
    int turnaround_time = 0;
    int waiting_times = 0;
    int estimation_error = 0;

    Row_Data *previous_rows = (Row_Data*) malloc(sizeof(Row_Data) * jobs->process_count);
    for (i = 0; i != jobs->nticks; ++i) {
        printf("Simulating %dth tick of %d processes @ time %d:\n", i, jobs->process_count, time);
        int **ticks_in_order = (int**) malloc(sizeof(int*) * jobs->process_count);
        int *ticks = (int*) malloc(sizeof(int) * jobs->process_count);
        for (j = 0; j != jobs->process_count; ++j) {
            double alpha = jobs->job_list[j]->alpha;
            int current_ticks = pop_from_list(jobs->job_list[j]->ticks);
            ticks[j] = current_ticks;
            int current_tau = jobs->job_list[j]->tau;
            if (i != 0) {
                int p_ticks = previous_rows[j].t_v;
                int p_tau = previous_rows[j].tau_v;
                current_tau = (int)((alpha * (double)p_ticks) + (1.0 - alpha) * (double)p_tau);
            }
            ticks_in_order[j] = (int*) malloc(sizeof(int) * 2);
            ticks_in_order[j][0] = j; 
            ticks_in_order[j][1] = current_tau;
            time += current_ticks;
            turnaround_time += current_ticks;
            
            previous_rows[j].t_v = current_ticks;
            previous_rows[j].tau_v = current_tau;
        }
        selection_sort(ticks_in_order, jobs->process_count);
        for (j = 0; j != jobs->process_count; ++j) {
            int p_i = ticks_in_order[j][0];
            int real_ticks = ticks[p_i];
            int calculated_tau = ticks_in_order[j][1];
            if (j != jobs->process_count - 1) {
                waiting_times += real_ticks;
            }
            estimation_error += abs(real_ticks - calculated_tau);
            printf("  Process %d was estimated for %d and took %d.\n", p_i, calculated_tau, real_ticks);
            free(ticks_in_order[j]);
        }
        free(ticks);
        free(ticks_in_order);
        ticks_in_order = NULL;
    }
    free(previous_rows);
    turnaround_time += waiting_times;
    printf("Turnaround time: %d\n", turnaround_time);
    printf("Waiting time: %d\n", waiting_times);
    printf("Estimation Error: %d\n", estimation_error);
    printf("\n");
}
