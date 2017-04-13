#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NULL
#define NULL 0
#endif

typedef int bool;
enum { false = 0, true };

struct LinkedList
{
	void* data;
	struct LinkedList* next;
};

struct DoubleLinkedList
{
	void* data;
	struct DoubleLinkedList* next;
	struct DoubleLinkedList* prev;
};

struct QueueNode
{
	void* data;
	struct QueueNode* next;
};
struct Queue
{
	struct QueueNode* front;
	struct QueueNode* back;
};

struct Dept;
struct DeptGroup;

struct Car
{
	char name[16];
	char code[5];
	int time;
	bool completed;
	struct Dept* in_which_dept;
};

struct Dept
{
	char name[32];
	int dept_number;
	int elapsed_time;
	struct Car* current_car_in_dept;
	struct LinkedList* completed_car_list;
	struct DeptGroup* in_which_dept_group;
};

struct DeptGroup
{
	char dept_name[16];
	int total_work_time;
	struct LinkedList* dept_list;
	struct Queue waiting_dept_queue;
};

struct Factory
{
	int current_time;
	int total_factory_work_time;
	struct Queue car_queue;
	struct DoubleLinkedList* dept_group_list;
	struct DoubleLinkedList* last_dept_group_ptr;
	struct LinkedList* car_list;
};

struct ReportCommand
{
	int time;
	char sub_cmd[12];
	char code[5];
};
/* compares time of command and returns result*/
bool report_cmd_time_comparer(void* lhs, void* rhs)
{
	struct ReportCommand* lhs_cmd = (struct ReportCommand*) lhs;
	struct ReportCommand* rhs_cmd = (struct ReportCommand*) rhs;

	return lhs_cmd->time <= rhs_cmd->time;
}

/*basic push function of linked list */
void list_push_back(struct LinkedList** list, void* data)
{
	struct LinkedList* new_node, *temp;
	new_node = (struct LinkedList*) malloc(sizeof(struct LinkedList));
	new_node->data = data;
	new_node->next = NULL;

	temp = *list;
	if (temp != NULL)
	{
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = new_node;
	}
	else {
		*list = new_node;
	}
}

typedef bool(*comparer)(void* lhs, void* rhs);

/*sorts with given data and pushes given data to sorted list*/
void list_sorted_push(struct LinkedList** sorted_list, void* data, comparer comp_func)
{
	struct LinkedList* new_node, *temp, *prev = NULL;

	new_node = (struct LinkedList*) malloc(sizeof(struct LinkedList));
	new_node->data = data;
	new_node->next = NULL;

	temp = *sorted_list;
	if (temp != NULL)
	{
		while (temp != NULL && comp_func(temp->data, data))
		{
			prev = temp;
			temp = temp->next;
		}

		if (temp == NULL)
		{
			prev->next = new_node;
		}
		else if (prev != NULL)
		{
			new_node->next = prev->next;
			prev->next = new_node;
		}
		else {
			new_node->next = temp;
			*sorted_list = new_node;
		}
	}
	else {
		*sorted_list = new_node;
	}
}

/* returns last node in the list. basic push function of double linked list*/
struct DoubleLinkedList* dlist_push_back(struct DoubleLinkedList** dlist, void* data)
{
	struct DoubleLinkedList* new_node, *temp;
	new_node = (struct DoubleLinkedList*) malloc(sizeof(struct DoubleLinkedList));
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;

	temp = *dlist;
	if (temp != NULL)
	{
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = new_node;
		new_node->prev = temp;
	}
	else {
		*dlist = new_node;
	}

	return new_node;
}
/*gets double linked list's previous data*/
void* dlist_get_prev(struct DoubleLinkedList* dlist)
{
	if (dlist->prev != NULL)
	{
		return dlist->prev->data;
	}
	return NULL;
}
/*basic push function of queue(enqueue)*/
void queue_push(struct Queue* queue, void* data)
{
	struct QueueNode* temp = (struct QueueNode*) malloc(sizeof(struct QueueNode));
	temp->data = data;
	temp->next = NULL;

	if (queue->front == NULL && queue->back == NULL)
	{
		queue->front = queue->back = temp;
	}
	else {
		queue->back->next = temp;
		queue->back = temp;
	}
}
/*basic pop function of queue(dequeue)*/
void queue_pop(struct Queue* queue)
{
	struct QueueNode* temp = queue->front;

	if (queue->front != NULL)
	{
		if (queue->front == queue->back)
		{
			queue->front = queue->back = NULL;
		}
		else {
			queue->front = queue->front->next;
		}

		free(temp);
	}
}
/*It checks to see if it is empty. */
bool queue_is_empty(struct Queue* queue)
{
	if (queue->front == NULL)
	{
		return true;
	}
	return false;
}
/*returns queue's front data if it's not empty*/
void* queue_front(struct Queue* queue)
{
	if (!queue_is_empty(queue))
	{
		return queue->front->data;
	}

	return NULL;
}
/*add's department to department group(for example frame1,frame2 ...)*/
void add_depts_to_factory(struct Factory* fac, char const* name, int num_of_dept, int time)
{
	int i = 0;
	struct Dept* new_dept;
	struct DeptGroup* new_dept_group = (struct DeptGroup*) malloc(sizeof(struct DeptGroup));
	memset(new_dept_group, 0, sizeof(struct DeptGroup));

	strcpy(new_dept_group->dept_name, name);
	new_dept_group->total_work_time = time;
	fac->total_factory_work_time += time;

	for (i = 0; i < num_of_dept; i++)
	{
		new_dept = (struct Dept*) malloc(sizeof(struct Dept));
		new_dept->current_car_in_dept = NULL;
		new_dept->elapsed_time = 0;
		sprintf(new_dept->name, "%s%d", name, i + 1);
		new_dept->dept_number = i+1;
		new_dept->completed_car_list = NULL;
		new_dept->in_which_dept_group = new_dept_group;

		list_push_back(&new_dept_group->dept_list, (void*)new_dept);
	}

	fac->last_dept_group_ptr = dlist_push_back(&fac->dept_group_list, (void*)new_dept_group);

	printf("Department %s has been created.\n", new_dept_group->dept_name);

}
/*prints space*/
void print_space(int n)
{
	int i = 0;
	for (i = 0; i < n; i++)
	{
		printf(" ");
	}
}
/* prints n characters*/
void print_n_char(char const* ch, int n)
{
	int i = 0;
	for (i = 0; i < n; i++)
	{
		printf("%s", ch);
	}
	puts("");
}
/*prints factory's with all sub department groups and departments*/
void print_factory(struct Factory* fac)
{
	int space_count = 0;
	struct DeptGroup* current_dept_group;
	struct Dept* current_dept;
	struct LinkedList* dep_it;
	struct DoubleLinkedList* dep_group_it = fac->dept_group_list;
	while (dep_group_it != NULL)
	{
		current_dept_group = (struct DeptGroup*) dep_group_it->data;

		dep_it = current_dept_group->dept_list;
		print_space(space_count);
		printf("-");
		while (dep_it != NULL)
		{
			current_dept = (struct Dept*)dep_it->data;
			printf(" %s", current_dept->name);
			space_count += strlen(current_dept->name) + 1;
			dep_it = dep_it->next;
		}
		puts("");
		space_count += 1;

		dep_group_it = dep_group_it->next;
	}
}
/*adds car to factory*/
void add_car(struct Factory* fac, char const* car_name, char const* car_code, int start_time)
{
	struct Car* new_car = (struct Car*) malloc(sizeof(struct Car));
	strcpy(new_car->name, car_name);
	strcpy(new_car->code, car_code);
	new_car->in_which_dept = NULL;
	new_car->time = start_time;
	new_car->completed = false;

	queue_push(&fac->car_queue, new_car);
	list_push_back(&fac->car_list, (void*)new_car);
}
/*processes factory for one tick*/
void process_factory_one_tick(struct Factory* fac)
{
	struct DoubleLinkedList* dept_group_it = fac->last_dept_group_ptr;
	struct LinkedList* dept_it;
	struct DeptGroup* current_dept_group, *prev_dept_group;
	struct Dept* current_dept, *dept_ready_to_shift;
	struct Car* car_ready_to_shift;

	while (dept_group_it != NULL)
	{
		current_dept_group = (struct DeptGroup*) dept_group_it->data;
		dept_it = current_dept_group->dept_list;

		while (dept_it != NULL)
		{
			current_dept = (struct Dept*) dept_it->data;
			/*
			* car exists in department
			* */
			if (current_dept->current_car_in_dept != NULL)
			{
				current_dept->elapsed_time++;
				if (current_dept->elapsed_time == current_dept_group->total_work_time)
				{
					/* if this is not the last dept group*/
					if (dept_group_it->next != NULL)
					{
						queue_push(&current_dept_group->waiting_dept_queue, current_dept);
					}
					else {
						list_push_back(&current_dept->completed_car_list, current_dept->current_car_in_dept);
						current_dept->current_car_in_dept->completed = true;
						current_dept->current_car_in_dept = NULL;
						current_dept->elapsed_time = 0;
					}
				}
			}
			else {
				if (dept_group_it->prev != NULL)
				{
					prev_dept_group = (struct DeptGroup*) dlist_get_prev(dept_group_it);

					if (!queue_is_empty(&prev_dept_group->waiting_dept_queue))
					{
						dept_ready_to_shift = (struct Dept*)queue_front(&prev_dept_group->waiting_dept_queue);

						/* push into completed car list of previous department*/
						list_push_back(&dept_ready_to_shift->completed_car_list, dept_ready_to_shift->current_car_in_dept);

						current_dept->current_car_in_dept = dept_ready_to_shift->current_car_in_dept;
						dept_ready_to_shift->current_car_in_dept->in_which_dept = current_dept;
						dept_ready_to_shift->current_car_in_dept = NULL;
						dept_ready_to_shift->elapsed_time = 0;
						current_dept->elapsed_time = 1;
						queue_pop(&prev_dept_group->waiting_dept_queue);
					}
				}
				else {
					if (!queue_is_empty(&fac->car_queue))
					{
						car_ready_to_shift = queue_front(&fac->car_queue);

						if (car_ready_to_shift->time <= fac->current_time)
						{
							current_dept->current_car_in_dept = car_ready_to_shift;
							current_dept->elapsed_time = 1;
							current_dept->current_car_in_dept->in_which_dept = current_dept;

							queue_pop(&fac->car_queue);
						}
					}
				}
			}

			dept_it = dept_it->next;
		}

		dept_group_it = dept_group_it->prev;
	}
}
/*finds car in factory*/
struct Car* find_car(struct Factory* fac, const char* car_code)
{
	struct LinkedList* car_it = fac->car_list;
	struct Car* curr_car = NULL;

	while (car_it != NULL)
	{
		curr_car = (struct Car*) car_it->data;
		if (!strcmp(curr_car->code, car_code)) {
			break;
		}

		car_it = car_it->next;
	}

	return curr_car;
}

const char* get_complete_string(bool completed)
{
	if (completed)
		return "Complete";
	return "Not Complete";
}
/*prints result of report for car information*/
void report_car(struct Factory* fac, struct Car* curr_car, int totol_time)
{
	if (curr_car != NULL)
	{
		print_n_char("-", 14 + strlen(curr_car->name) + strlen(curr_car->code));
		printf("|Report for %s %s|\n", curr_car->name, curr_car->code);
		print_n_char("-", 14 + strlen(curr_car->name) + strlen(curr_car->code));

		bool reached_dept_group = false;

		int total_time_so_far = 0;
		struct DoubleLinkedList* gr_it = fac->dept_group_list;
		while (gr_it != NULL)
		{
			struct DeptGroup* curr_group = (struct DeptGroup*) gr_it->data;
			if (curr_car->in_which_dept != NULL)
			{
				if (!strcmp(curr_car->in_which_dept->in_which_dept_group->dept_name, curr_group->dept_name))
				{
					if (curr_car->completed)
					{
						printf("%s:%d, ", curr_group->dept_name, curr_car->in_which_dept->in_which_dept_group->total_work_time);
						total_time_so_far += curr_car->in_which_dept->in_which_dept_group->total_work_time;
					}
					else {
						printf("%s:%d, ", curr_group->dept_name, curr_car->in_which_dept->elapsed_time);
						total_time_so_far += curr_car->in_which_dept->elapsed_time;
					}
					reached_dept_group = true;
				}
				else {
					if (!reached_dept_group) {
						printf("%s:%d, ", curr_group->dept_name, curr_group->total_work_time);
						total_time_so_far += curr_group->total_work_time;
					}
					else {
						printf("%s:0, ", curr_group->dept_name);
					}
				}
			}
			else {
				printf("%s:0, ", curr_group->dept_name);
			}

			gr_it = gr_it->next;
		}

		float competed_percentage = (float)total_time_so_far / fac->total_factory_work_time * 100.0f;
		printf("| Start Time: %d | Complete: %.2f%% | %s\n", curr_car->time,
			competed_percentage, get_complete_string(curr_car->completed));
	}
}
/*prints result of report for department information*/
void report_depts(struct Factory* fac, int totol_time)
{
	printf("\nCommand: Report Departments %d\n", totol_time);

	struct DoubleLinkedList* dept_group_it = fac->last_dept_group_ptr;

	while (dept_group_it != NULL)
	{
		struct DeptGroup* curr_dept_group = (struct DeptGroup*) dept_group_it->data;

		struct LinkedList* dept_it = curr_dept_group->dept_list;
		while (dept_it != NULL)
		{
			struct Dept* curr_dept = (struct Dept*) dept_it->data;

			print_n_char("-", 27 + strlen(curr_dept->name));
			printf("|Report for Department \"%s %d\"|\n", curr_dept_group->dept_name,curr_dept->dept_number);
			print_n_char("-", 27 + strlen(curr_dept->name));

			if (curr_dept->current_car_in_dept != NULL)
			{
				printf("I am currently processing %s %s\n",
					curr_dept->current_car_in_dept->name,
					curr_dept->current_car_in_dept->code);
			}
			else {
				printf("%s is now free.\n", curr_dept->name);
			}

			if (curr_dept->completed_car_list != NULL)
			{
				puts("Processed Cars");
				int car_index = 1;
				struct LinkedList* car_it = curr_dept->completed_car_list;
				while (car_it != NULL)
				{
					struct Car* curr_car = (struct Car*) car_it->data;
					printf("%d. %s %s\n", car_index, curr_car->name, curr_car->code);
					car_it = car_it->next;
					car_index++;
				}
			}

			dept_it = dept_it->next;
		}

		dept_group_it = dept_group_it->prev;
	}
}
/*processes the factory for the given time*/
void do_factory_processing(struct Factory* fac, int total_time)
{
	int i;
	int total_tick_count = total_time - fac->current_time;
	for (i = 0; i < total_tick_count; i++)
	{
		fac->current_time++;
		process_factory_one_tick(fac);

	}
}
/*executes department report for "report x departments" */
void execute_dept_report(struct Factory* fac, int report_time)
{
	do_factory_processing(fac, report_time);
	report_depts(fac, report_time);
}
/*executes car report for "report x car xxxx"*/
void execute_car_report(struct Factory* fac, const char* car_code, int report_time)
{
	struct Car* car = find_car(fac, car_code);
	if (car != NULL)
	{
		do_factory_processing(fac, report_time);

		printf("\nCommand: Report Car %d %s\n", report_time, car_code);

		report_car(fac, car, report_time);
	}
}
/*executes cars report for "report x cars" */
void execute_cars_report(struct Factory* fac, int report_time)
{
	do_factory_processing(fac, report_time);

	struct LinkedList* car_it = fac->car_list;
	struct Car* curr_car = NULL;

	printf("\nCommand: Report Cars %d\n", report_time);

	while (car_it != NULL)
	{
		curr_car = (struct Car*) car_it->data;
		report_car(fac, curr_car, report_time);
		car_it = car_it->next;
	}
}
/*defines kind of report and calls related function*/
void execute_report_commands(struct LinkedList* report_list, struct Factory* fac)
{
	if (report_list != NULL)
	{
		struct LinkedList* it = report_list;
		while (it != NULL)
		{
			struct ReportCommand* cmd = (struct ReportCommand*) it->data;


			if (!strcmp(cmd->sub_cmd, "Car"))
			{
				execute_car_report(fac, cmd->code, cmd->time);
			}
			else if (!strcmp(cmd->sub_cmd, "Cars"))
			{
				execute_cars_report(fac, cmd->time);
			}
			else if (!strcmp(cmd->sub_cmd, "Departments"))
			{
				execute_dept_report(fac, cmd->time);
			}

			it = it->next;
		}
	}
}
/*parses input file and calls related command functions */
void parse_input_file_and_create_command(char const* file_name, struct Factory* fac)
{
	struct LinkedList* sorted_report_list = NULL;
	FILE* file = fopen(file_name, "r");
	char line[80];

	if (file != NULL)
	{
		while (fgets(line, sizeof(line), file))
		{
			char* command_word = strtok(line, " \n");

			if (command_word == NULL) {
				break;
			}

			if (!strcmp(command_word, "AddDept"))
			{
				int num_of_depts = atoi(strtok(NULL, " "));
				char* dept_name = strtok(NULL, " ");
				int work_time = atoi(strtok(NULL, " "));

				add_depts_to_factory(fac, dept_name, num_of_depts, work_time);
			}
			else if (!strcmp(command_word, "PrintFactory"))
			{
				print_factory(fac);
			}
			else if (!strcmp(command_word, "Produce"))
			{
				int start_time = atoi(strtok(NULL, " "));
				char* car_name = strtok(NULL, " ");
				char* car_code = strtok(NULL, " \n");

				add_car(fac, car_name, car_code, start_time);
			}
			else if (!strcmp(command_word, "Report"))
			{
				struct ReportCommand* rpt_cmd = (struct ReportCommand*) malloc(sizeof(struct ReportCommand));
				memset(rpt_cmd, 0, sizeof(struct ReportCommand));

				int report_time = atoi(strtok(NULL, " "));
				char* sub_report_cmd = strtok(NULL, " \n");

				rpt_cmd->time = report_time;
				strcpy(rpt_cmd->sub_cmd, sub_report_cmd);

				if (!strcmp(sub_report_cmd, "Car"))
				{
					char* car_code = strtok(NULL, " \n");
					strcpy(rpt_cmd->code, car_code);
					list_sorted_push(&sorted_report_list, rpt_cmd, report_cmd_time_comparer);
				}
				else if (!strcmp(sub_report_cmd, "Cars") || !strcmp(sub_report_cmd, "Departments"))
				{
					list_sorted_push(&sorted_report_list, rpt_cmd, report_cmd_time_comparer);
				}
			}
		}

		fclose(file);

		execute_report_commands(sorted_report_list, fac);
	}
	else{
		puts("Error: Unable to find input file!");
	}
}

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		struct Factory factory;
		memset(&factory, 0, sizeof(factory));

		parse_input_file_and_create_command(argv[1], &factory);
	}
	else {
		puts("Usage: factory [input_file]");
	}

	return 0;
}
