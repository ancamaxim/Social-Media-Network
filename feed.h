#ifndef FEED_H
#define FEED_H

void feed(char *name, int feed_size, list_graph_t *friends, post_trees_t *tree);

void search_posts_id(post_t *post, int id, char *title);

void view_profile(char *name, post_trees_t *tree);

int is_friends_repost(post_t *post, list_graph_t *friends, int id);

void search_friends_reposts(post_t *post, list_graph_t *friends, int id);

void friends_repost(char *name, int id, post_trees_t *tree, list_graph_t *friends);;

int max_lower_bin_search(int *arr, int size, int x);

void kerbosch(int *r, int r_len, int *p, int p_len, int *x, int x_len,
			list_graph_t *friends, int *sol, int *len);

void common_group(char *name, list_graph_t *friends);

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_feed(char *input, list_graph_t *friends, post_trees_t *tree);

#endif // FEED_H
