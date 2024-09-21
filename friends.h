#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 500
#define MAX_PEOPLE 550

#include "graph.h"

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
list_graph_t *add_friend(list_graph_t *graph, char *name1, char *name2);

list_graph_t *remove_friend(list_graph_t *friends, char *name1, char *name2);

void distance(list_graph_t *friends, char *name1, char *name2);

void suggestions(list_graph_t *friends, char *name);

void common(list_graph_t *friends, char *name1, char *name2);

int num_friends(list_graph_t *friends, char *name);

void popular(list_graph_t *friends, char *name);

void handle_input_friends(char *input, list_graph_t *friends);

#endif // FRIENDS_H
