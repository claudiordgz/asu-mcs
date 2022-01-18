/**
* SER334: Operating Systems & System Programming
* Introduction to C Programming II
*
* Completion time: 8h
* - enumeration type (called Subject) to represent subjects (SER, EGR, CSE, EEE)
* - struct type (called CourseNode) to hold information about a course. Include a subject (see enum type above), a number (int), a teacher (a string of length 1024), and credit hours (int).
* - linked list of structs (called course_collection) to store course information
* - must no leak memory on exit
* - void course_insert()
* - void schedule_print()
* - void course_drop()
* - void schedule_load()
* - void schedule_save()
* - no duplicates
* - sorted by Subject first, Course Number second
* 
* @author Claudio Rodriguez Rodriguez
* @version 01/17/2022 0.0.1
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
//MACROS: CONSTANTS
#define TEACHER_MAX_NAME (1024)

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
enum Subject {
	SER = 0,
	EGR,
	CSE,
	EEE
};

struct CourseNode {
	enum Subject subject;
	int number;
	char teacher[TEACHER_MAX_NAME];
	int creditHours;

	struct CourseNode* next;
};

////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES

//place to store course information
struct CourseNode* course_collection = NULL;

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS
void branching(char option);
void course_insert();
void schedule_print();
void course_drop();
void schedule_load();
void schedule_save();

void sort_list();
void delete_list();
void course_drop_from_props(int subject, int courseNumber);
struct CourseNode* course_insert_from_props(int subject, int courseNumber, char *teacher, int creditHours);
int calculate_course_count();
char* getSubjectPrefix(int subject);
void print_row(struct CourseNode* node);

//main entry point. Starts the program by displaying a welcome and beginning an 
//input loop that displays a menu and processes user input. Pressing q quits.      
int main() {
	char input_buffer;
	int courseCount = 0;

	schedule_load();
	courseCount = calculate_course_count();

	printf("\n\nWelcome to ASU Class Schedule\n");
	//menu and input loop
	do {
		printf("\nMenu Options\n");
		printf("------------------------------------------------------\n");
		printf("a: Add a class\n");
		printf("d: Drop a class\n");
		printf("s: Show your classes\n");
		printf("q: Quit\n");
		printf("\nTotal Credits: %d\n\n", courseCount);
		printf("Please enter a choice ---> ");

		scanf(" %c", &input_buffer);
		branching(input_buffer);
		courseCount = calculate_course_count();

	} while (input_buffer != 'q');

	schedule_save();
	delete_list();

	return 0;
}

//takes a character representing an inputs menu choice and calls the appropriate
//function to fulfill that choice. display an error message if the character is
//not recognized.
void branching(char option) {
	switch (option) {
		case 'a':
			course_insert();
			break;

		case 'd':
			course_drop();
			break;

		case 's':
			schedule_print();
			break;

		case 'q':
			break;

		default:
			printf("\nError: Invalid Input.  Please try again...");
			break;
	}
}

void schedule_load() {
	FILE *schedulerState;
	
	schedulerState = fopen("rrSchedulerState.csv", "r");
	if (schedulerState != NULL) {
		
		int subject = 0;
		int number = 0;
		int credits = 0;
		char teacher[TEACHER_MAX_NAME];
		while (fscanf(schedulerState, "%d,%d,%d,%1024[^,\n]\n", &subject, &number, &credits, teacher) == 4) {
			course_insert_from_props(subject, number, teacher, credits);
		}

		fclose(schedulerState);
	}
}

void course_insert() {
	int subjectNumber = 0;
	int courseNumber = 0;
	int credits = 0;
    char teacher[TEACHER_MAX_NAME];

	printf("What is the subject? (SER=%d, EGR=%d, CSE=%d, EEE=%d)?\n", SER, EGR, CSE, EEE);
	scanf("%d", &subjectNumber);
    printf("\n");

	printf("What is the number (e.g. 240)?\n");
	scanf("%d", &courseNumber);
    printf("\n");

	printf("How many credits is the class (e.g. 3)?\n");
	scanf("%d", &credits);
    printf("\n");

	printf("What is the name of the teacher?\n");
	scanf("%1023s", teacher);
	printf("String = %s", teacher);
    printf("\n");

	struct CourseNode *newNode = course_insert_from_props(subjectNumber, courseNumber, teacher, credits);
	if (newNode != NULL) {
		printf("Course %s%d added to the schedule.\n", getSubjectPrefix(newNode->subject), newNode->number);
	}
}

struct CourseNode* course_insert_from_props(int subject, int courseNumber, char *teacher, int creditHours) {
	struct CourseNode* new_node = (struct CourseNode*) malloc(sizeof(struct CourseNode));
	new_node->next = NULL;
	strcpy(new_node->teacher, teacher);
	new_node->creditHours = creditHours;
	new_node->subject = subject;
	new_node->number = courseNumber;

	if (course_collection == NULL) {
		course_collection = new_node;
		return new_node;
	}
	
	struct CourseNode* iterator = course_collection;
	struct CourseNode* pIterator = NULL;
	while (iterator != NULL) {
		if (new_node->subject == iterator->subject) {
			if(new_node->number == iterator->number && 
			   new_node->creditHours == iterator->creditHours && 
			   strcmp(new_node->teacher, iterator->teacher) == 0) {
				printf("Error: Course %s%d already exists and will not be inserted.\n", getSubjectPrefix(iterator->subject), iterator->number);
				return NULL;
			} else if(new_node->number > iterator->number) {
				if (iterator->next == NULL) {
					iterator->next = new_node;
					break;
				} else {
					pIterator = iterator;
					iterator = iterator->next;
				}
			} else {
				new_node->next = iterator;
				if (pIterator != NULL) {
					pIterator->next = new_node;
				} else {
					course_collection = new_node;
				}
				break;
			}
		} else if (new_node->subject < iterator->subject) {
			new_node->next = iterator;
			if (pIterator == NULL) {
				course_collection = new_node;
			} else {
				pIterator->next = new_node;
			}
			break;
		} else {
			if (iterator->next == NULL) {
				iterator->next = new_node;
				break;
			} else {
				pIterator = iterator;
				iterator = iterator->next;
			}
		}
	}

	return new_node;
}

char* getSubjectPrefix(int subject) {
	switch (subject) {
		case SER:
			return "SER";
		case EGR:
			return "EGR";
		case CSE:
			return "CSE";
		case EEE:
			return "EEE";
		default:
			printf("\nError: Invalid subject.\n");
			break;
	}
}

void print_row(struct CourseNode* node) {
	printf("%s%d\t%d %s\n", getSubjectPrefix(node->subject), node->number, node->creditHours, node->teacher);
}

void delete_list() {
	while(course_collection)
	{
		struct CourseNode* iterator = course_collection;
		course_collection = course_collection->next;
		free(iterator);
	}
}

void schedule_print() {
	struct CourseNode* iterator = course_collection;
	printf("\nClass Schedule:\n");
	while (iterator != NULL) {
		print_row(iterator);
		iterator = iterator->next;
	}
	printf("\n");
}

void course_drop() {
	int subjectNumber = 0;
	int courseNumber = 0;

	printf("What is the subject? (SER=%d, EGR=%d, CSE=%d, EEE=%d)?\n", SER, EGR, CSE, EEE);
	scanf("%d", &subjectNumber);
    printf("\n");

	printf("What is the number (e.g. 240)?\n");
	scanf("%d", &courseNumber);
    printf("\n");

	course_drop_from_props(subjectNumber, courseNumber);
}

void course_drop_from_props(int rSubject, int courseNumber) {
	enum Subject subject = (enum Subject)rSubject;
	struct CourseNode *iterator = course_collection;
	if (iterator == NULL) {
		printf("Error: Course %s%d does not exist.\n", getSubjectPrefix(subject), courseNumber);
	}
	struct CourseNode *pIterator = NULL;
	while (iterator != NULL) {
		if (iterator->subject == subject && iterator->number == courseNumber) {
			struct CourseNode* toDelete = iterator;
			iterator = iterator->next;
			free(toDelete);
			if (pIterator != NULL) {
				pIterator->next = iterator;
			} else {
				course_collection = iterator;
			}
			printf("Dropped course %s%d from schedule.\n", getSubjectPrefix(subject), courseNumber);
			return;
		} else {
			pIterator = iterator;
			iterator = iterator->next;
		}
	}
	printf("Error: Course %s%d does not exist.\n", getSubjectPrefix(subject), courseNumber);
}

void schedule_save() {
	FILE *schedulerState;
	struct CourseNode* itr = course_collection;
	schedulerState = fopen("rrSchedulerState.csv", "w");
	if (schedulerState != NULL) {
		while (itr != NULL) {
			fprintf(schedulerState, "%d,%d,%d,%s\n", itr->subject, itr->number, itr->creditHours, itr->teacher);
			itr = itr->next;
		}
		fclose(schedulerState);
	}
}

int calculate_course_count() {
	if (course_collection == NULL) {
		return 0;
	}
	
	int localCount = 0;
	struct CourseNode* iterator = course_collection;
	while (iterator != NULL) {
		localCount += iterator->creditHours;
		iterator = iterator->next;
	}
	return localCount;
}