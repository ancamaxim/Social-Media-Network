#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "users.h"
#include "graph.h"

list_graph_t *add_friend(list_graph_t *friends, char *name1, char *name2) {
	int id1 = get_user_id(name1);
	int id2 = get_user_id(name2);
	friends = lg_add_edge(friends, id1, id2);
	friends = lg_add_edge(friends, id2, id1);
	printf("Added connection %s - %s\n", name1, name2);
	// int cnt = num_friends(friends, name1);
	// printf("%s has %d friends\n", name1, cnt);
	return friends;
}

list_graph_t *remove_friend(list_graph_t *friends, char *name1, char *name2) {
	int id1 = get_user_id(name1);
	int id2 = get_user_id(name2);
	friends = lg_remove_edge(friends, id1, id2);
	friends = lg_remove_edge(friends, id2, id1);
	printf("Removed connection %s - %s\n", name1, name2);
	return friends;
}

void distance(list_graph_t *friends, char *name1, char *name2) {
	int id1 = get_user_id(name1);
	int id2 = get_user_id(name2);
	int dist = print_min_path(friends, id1, id2);
	if (dist == 0)
		printf("There is no way to get from %s to %s\n", name1, name2);
	else
		printf("The distance between %s - %s is %d\n", name1, name2, dist);
}

void suggestions(list_graph_t *friends, char *name) {
	int cnt = 0;
	int id = get_user_id(name);
	queue_t *q;
	int *dist, *visited, curr;
	dist = calloc(MAX_PEOPLE, sizeof(int));
	visited = calloc(MAX_PEOPLE, sizeof(int));
	for (int i = 0; i < MAX_PEOPLE; i++)
		dist[i] = -1;
	q = q_create(sizeof(int), MAX_PEOPLE);
	q_enqueue(q, &id);
	visited[id] = 1;
	dist[id] = 0;
	while(!q_is_empty(q)) {
		curr = *((int *)q_front(q));
		q_dequeue(q);
		linked_list_t *neighbours = lg_get_neighbours(friends, curr);
		if (!neighbours)
			continue;
		ll_node_t *tmp = neighbours->head;
		while(tmp) {
			int id_fr = *((int *)(tmp->data));
			if (visited[id_fr] == 0) {
				q_enqueue(q, &id_fr);
				visited[id_fr] = 1;
				dist[id_fr] = dist[curr] + 1;
				cnt++;
			}
			tmp = tmp->next;
		}
	}
	free(visited);
	q_free(q);
	if (cnt == num_friends(friends, name)) {
		printf("There are no suggestions for %s\n", name);
		free(dist);
		return;
	}
	printf("Suggestions for %s:\n", name);
	for (int i = 0; i < MAX_PEOPLE; i++)
		if (dist[i] == 2)
			printf("%s\n", get_user_name(i));
	free(dist);
}

void common(list_graph_t *friends, char *name1, char *name2) {
	int id1 = get_user_id(name1);
	int id2 = get_user_id(name2);
	int cnt = 0;
	int *common = calloc(MAX_PEOPLE, sizeof(int));
	linked_list_t *neigh1 = lg_get_neighbours(friends, id1);
	linked_list_t *neigh2 = lg_get_neighbours(friends, id2);
	if (!neigh1 || !neigh2) {
		printf("No common friends for %s and %s\n", name1, name2);
		return;
	}
	ll_node_t *tmp = neigh1->head;
	while(tmp) {
		int curr = *((int *)(tmp->data));
		if (id2 != curr && lg_has_edge(friends, id2, curr)) {
			cnt++;
			common[curr] = 1;
		}
		tmp = tmp->next;
	}
	if (cnt == 0) {
		printf("No common friends for %s and %s\n", name1, name2);
		free(common);
		return;
	}
	printf("The common friends between %s and %s are:\n", name1, name2);
	for (int i = 0; i < MAX_PEOPLE; i++)
		if(common[i])
			printf("%s\n", get_user_name(i));
	free(common);
}

int num_friends(list_graph_t *friends, char *name) {
	int id = get_user_id(name);
	linked_list_t *neighbour = lg_get_neighbours(friends, id);
	int cnt = 0;
	if (!neighbour)
		return 0;
	ll_node_t *tmp = neighbour->head;
	while(tmp) {
		cnt++;
		tmp = tmp->next;
	}
	return cnt;
}

void popular(list_graph_t *friends, char *name) {
	int id = get_user_id(name);
	linked_list_t *neighbour = lg_get_neighbours(friends, id);
	if (!neighbour) {
		printf("%s is the most popular\n", name);
		return;
	}
	int cnt = 0, max = 0;
	char *pop_name;
	ll_node_t *tmp = neighbour->head;
	while(tmp) {
		int num = num_friends(friends, get_user_name(*(int *)(tmp->data)));
		if (max < num) {
			max = num;
			pop_name = get_user_name(*(int *)(tmp->data));
		}
		cnt++;
		tmp = tmp->next;
	}
	if (cnt >= max)
		printf("%s is the most popular\n", name);
	else
		printf("%s is the most popular friend of %s\n", pop_name, name);
}

void handle_input_friends(char *input, list_graph_t *friends)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "add")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		friends = add_friend(friends, name1, name2);
	} else if (!strcmp(cmd, "remove")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		friends = remove_friend(friends, name1, name2);
	} else if (!strcmp(cmd, "suggestions")) {
		char *name = strtok(NULL, "\n ");
		suggestions(friends, name);	
	} else if (!strcmp(cmd, "distance")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		distance(friends, name1, name2);
	}
	else if (!strcmp(cmd, "common")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		common(friends, name1, name2);
	}
	else if (!strcmp(cmd, "friends")) {
		char *name = strtok(NULL, "\n ");
		int cnt = num_friends(friends, name);
		printf("%s has %d friends\n", name, cnt);
	}
	else if (!strcmp(cmd, "popular")) {
		char *name = strtok(NULL, "\n ");
		popular(friends, name);
	}

	free(commands);
}
