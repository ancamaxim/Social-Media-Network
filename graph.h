#include <stdio.h>
#include <stdlib.h>
#ifndef GRAPH_H
#define GRAPH_H

typedef struct ll_node_t ll_node_t;
typedef struct linked_list_t linked_list_t;
typedef struct queue_t queue_t;
typedef struct list_graph_t list_graph_t;

struct ll_node_t
{
	void *data;
	ll_node_t *next;
};

struct linked_list_t
{
	ll_node_t *head;
	unsigned int data_size;
	unsigned int size;
};

struct queue_t
{
	unsigned int max_size;
	unsigned int size;
	unsigned int data_size;
	unsigned int read_idx;
	unsigned int write_idx;
	void **buff;
};

struct list_graph_t
{
	linked_list_t **neighbors;
	int nodes;
};

/**
 * @brief Creates a linked list.
 * @param data_size Size of the data to be stored in the list.
 * @return Pointer to the created linked list.
 */
linked_list_t *ll_create(unsigned int data_size);

/**
 * @brief Adds a node at a specific position in the linked list.
 * @param list Pointer to the linked list.
 * @param n Position to add the new node.
 * @param new_data Pointer to the new data.
 */
linked_list_t *ll_add_nth_node(linked_list_t *list, unsigned int n, const void *new_data);

/**
 * @brief Removes a node from a specific position in the linked list.
 * @param list Pointer to the linked list.
 * @param n Position of the node to be removed.
 * @return Pointer to the removed node.
 */
linked_list_t *ll_remove_nth_node(linked_list_t *list, unsigned int n);

/**
 * @brief Gets the size of the linked list.
 * @param list Pointer to the linked list.
 * @return Size of the linked list.
 */
unsigned int ll_get_size(linked_list_t *list);

/**
 * @brief Frees the memory allocated for the linked list.
 * @param pp_list Double pointer to the linked list.
 */
void ll_free(linked_list_t **pp_list);

/**
 * @brief Prints the integer values in the linked list.
 * @param list Pointer to the linked list.
 */
void ll_print_int(linked_list_t *list);

/**
 * @brief Prints the string values in the linked list.
 * @param list Pointer to the linked list.
 */
void ll_print_string(linked_list_t *list);

/**
 * @brief Creates a queue.
 * @param data_size Size of the data to be stored in the queue.
 * @param max_size Maximum size of the queue.
 * @return Pointer to the created queue.
 */
queue_t *q_create(unsigned int data_size, unsigned int max_size);

/**
 * @brief Gets the size of the queue.
 * @param q Pointer to the queue.
 * @return Size of the queue.
 */
unsigned int q_get_size(queue_t *q);

/**
 * @brief Checks if the queue is empty.
 * @param q Pointer to the queue.
 * @return 1 if the queue is empty, 0 otherwise.
 */
unsigned int q_is_empty(queue_t *q);

/**
 * @brief Gets the front element of the queue.
 * @param q Pointer to the queue.
 * @return Pointer to the front element.
 */
void *q_front(queue_t *q);

/**
 * @brief Removes the front element of the queue.
 * @param q Pointer to the queue.
 * @return 1 if the operation is successful, 0 otherwise.
 */
int q_dequeue(queue_t *q);

/**
 * @brief Adds a new element to the queue.
 * @param q Pointer to the queue.
 * @param new_data Pointer to the new data.
 * @return 1 if the operation is successful, 0 otherwise.
 */
int q_enqueue(queue_t *q, void *new_data);

/**
 * @brief Clears the queue.
 * @param q Pointer to the queue.
 */
void q_clear(queue_t *q);

/**
 * @brief Frees the memory allocated for the queue.
 * @param q Pointer to the queue.
 */
void q_free(queue_t *q);

/**
 * @brief Creates a list graph with a specified number of nodes.
 * @param nodes Number of nodes in the graph.
 * @return Pointer to the created list graph.
 */
list_graph_t *lg_create(int nodes);

/**
 * @brief Adds an edge to the graph.
 * @param graph Pointer to the graph.
 * @param src Source node.
 * @param dest Destination node.
 */
list_graph_t *lg_add_edge(list_graph_t *graph, int src, int dest);

/**
 * @brief Checks if an edge exists in the graph.
 * @param graph Pointer to the graph.
 * @param src Source node.
 * @param dest Destination node.
 * @return 1 if the edge exists, 0 otherwise.
 */
int lg_has_edge(list_graph_t *graph, int src, int dest);

/**
 * @brief Gets the neighbors of a node in the graph.
 * @param graph Pointer to the graph.
 * @param node Node for which to get the neighbors.
 * @return Pointer to the linked list of neighbors.
 */
linked_list_t *lg_get_neighbours(list_graph_t *graph, int node);

/**
 * @brief Removes an edge from the graph.
 * @param graph Pointer to the graph.
 * @param src Source node.
 * @param dest Destination node.
 */
list_graph_t * lg_remove_edge(list_graph_t *graph, int src, int dest);

/**
 * @brief Frees the memory allocated for the graph.
 * @param graph Pointer to the graph.
 */
void lg_free(list_graph_t **graph);

/**
 * @brief Prints the minimum path between two nodes in the graph.
 * @param graph Pointer to the graph.
 * @param src Source node.
 * @param dest Destination node.
 * @return Length of the minimum path, or 0 if no path exists.
 */
int print_min_path(list_graph_t *graph, int src, int dest);

#endif // GRAPH_H