#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MEMORY_SIZE 100
#define PAGE_SIZE 1

// physical memory frame
typedef struct frame {
	int num;  // frame number
	int page; // the page number that maps to this frame
	char proc_name; // name of process that owns this frame
} frame;

// frame node for free list
typedef struct f_frame {
	struct frame * f;
	struct f_frame * next;
} f_frame;

frame * memory[MEMORY_SIZE]; // memory map
f_frame * head = NULL; // free list

// int count = 0; // DEBUG, increment when -1<=delta_i<=1

// FREE LIST
//-------------------------------------------------------
// initializes the free list
void init_flist() {

	// head is 0 and last node is 99
	// we can randomize if we want
	head = malloc(sizeof(f_frame));
	head->f = malloc(sizeof(frame *));
	head->f->num = 0;
	head->next = NULL;

	int i = 1;
	f_frame * current = head;
	while (i < (MEMORY_SIZE/PAGE_SIZE)) {
		current->next = malloc(sizeof(f_frame));
		current->next->f = malloc(sizeof(frame *));
		current->next->f->num = i;
		current = current->next;
		i += 1;
	}

}

// adds a free page to the end of the list
void flist_append(int num) {

    f_frame * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(f_frame));
    current->next->f = malloc(sizeof(frame *));
    current->next->f->num = num;
    current->next->next = NULL;

}

// returns the frame number of the head of the free list and removes it from the list
// returns -1 if head is null
int flist_pop() {

	if (head == NULL) {
        return -1;
    }

    int retval = head->f->num;
    f_frame * front = head;
    head = head->next;
    front->next = NULL;
    if (front == head) {
    	head = NULL;
    }
    free(front);

    return retval;

}

// returns the current size of the free list
int get_flist_size() {

	int size = 0;
	f_frame * current = head;
	while (current != NULL) {
        size += 1;
        current = current->next;
    }
    return size;

}

// prints out the free list
void print_flist() {

	printf("Free List\n");
	f_frame * current = head;
    while (current != NULL) {
        printf("Frame #%d \n", current->f->num);
        current = current->next;
    }
    printf("End Free List\n");

}

// MEMORY MAP
//-------------------------------------------------------
// initializes the memory map
void init_mmap() {

	int i;
	for (i = 0; i < MEMORY_SIZE; i++) {
		memory[i] = malloc(sizeof(frame *));
		memory[i]->num = i;
		memory[i]->page = -1;
		memory[i]->proc_name = 46; // "." char
	}

}

// returns index in memory map if page is in memory, -1 otherwise
int get_page_pfn(int page) {

	int i;
	for (i = 0; i < MEMORY_SIZE; i++) {
		if (memory[i]->page == page) {
			return i;
		}
	}
	return -1;

}

// bring page in to memory
void page_in(int page, char proc_name) {

	// obtain a free frame
	int index = flist_pop();
	// printf("Removed frame #%d from free list.\n", index);

	// update memory map
	memory[index]->page = page;
	memory[index]->proc_name = proc_name;
	// printf("Updated frame #%d [page #%d, proc %c] in memory\n", index, page, proc_name);

}

// evict frame from memory
void evict(int frame_num) {

	// update memory slot
	// printf("Evicted frame #%d [page #%d, proc %c] from memory\n", frame_num, memory[frame_num]->page, memory[frame_num]->proc_name);
	memory[frame_num]->page = -1;
	memory[frame_num]->proc_name = 46;

	// update free list
	flist_append(frame_num);

}

// prints memory map as described in the assignment
void print_mmap() {

	int i;
	printf("Memory Map: <");
	for (i = 0; i < MEMORY_SIZE; i++) {
		printf("%c", memory[i]->proc_name);
	}
	printf("> \n");

}

// generates the random page reference
// taking the locality of reference algo into consideration
int gen_ref(int i) {

	int r, delta_i, next_ref;
	
	// to compute the next_ref, first generate a random "r" from 0 to 10
	r = rand() % 11;

	// ~70% probability next_ref will be i, i-1, or i+1
	if (r >= 0 && r < 7) {
		// count += 1;	// DEBUG
		delta_i = (rand() % 3) - 1; // generate a random delta_i to be -1, 0, or +1
	}
	// generate new page ref with (2 <= delta_i <= 9)
	if (r >= 7 && r <= 10) {
		delta_i = (rand() % 8) + 2; 
	}

	next_ref = i + delta_i;

	// j = next_ref
	// (0 <= j <= i - 2) || (i + 2 <= j <= 10)
	if (i + delta_i < 0 || i + delta_i > 10) {
		next_ref = (i < delta_i) ? delta_i - i : i - delta_i;
	}

	// DEBUG
	// printf("last_ref = %d\n", i);	
	// printf("r = %d\n", r);
	// printf("delta_i = %d\n", delta_i);
	// printf("next_ref = %d\n", next_ref);
	// printf("---------------------------\n");
	return (next_ref);

}

void clean_up() {

	int i;
	for (i = 0; i < MEMORY_SIZE; i++) {
		free(memory[i]);
	}

}

int main(int argc, char* argv[]) {

	// DEBUG: testing gen_ref()
	// srand(time(0));
	// int i, j;
	// for (j = 0; j < 10000; j++) {
	// 	i = rand() % 100;
	// 	gen_ref(i);
	// }
	// printf("-1<=delta_i<=1: %.2f%%\n", (count/10000.) * 100);

	// Initialize the memory map
	init_mmap();
	// print_mmap();
	/************************************* SIMULATION *************************************/

	// Generate the workload and represent it as sorted queue based on arrival time
	// TODO

	// Create and initialize the free page list, initially with 100 pages, each is 1 MB
	init_flist();
	// print_flist();

	// Pick up one job at a time from the job queue and if there are 4 free pages in the free page list
	// then start running that process, otherwise wait till one of the existing processes complete
	// Each process is represented by a header and linked list of its memoryresident pages
	// TODO

	// ...

	// DEBUG
	// page_in(43, 101);
	// print_mmap();
	// page_in(48, 102);
	// print_mmap();
	// printf("%d\n", get_page_pfn(43));
	// evict(get_page_pfn(43));
	// print_mmap();

	// CLEAN UP
	clean_up();

	exit(0);

}