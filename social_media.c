/**
 * The entrypoint of the homework. Every initialization must be done here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"
#include "graph.h"

/**
 * Initialises every task based on which task we are running
*/
void init_tasks(void)
{
	#ifdef TASK_1
	
	#endif

	#ifdef TASK_2

	#endif

	#ifdef TASK_3

	#endif
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	init_users();

	init_tasks();

	#ifdef TASK_1
	list_graph_t *graph = lg_create(MAX_PEOPLE);
	#endif

	#ifdef TASK_2
	post_trees_t *tree = init_posts(MAX_PEOPLE);
	#endif

	char *input = (char *)malloc(MAX_COMMAND_LEN);
	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input, graph);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, tree);
		#endif

		#ifdef TASK_3
		handle_input_feed(input, graph, tree);
		#endif
	}

	free_users();
	free(input);

	#ifdef TASK_1
	lg_free(&graph);
	#endif

	#ifdef TASK_2
	tree_free(&tree);
	#endif

	return 0;
}
