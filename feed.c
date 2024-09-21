#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "posts.h"
#include "graph.h"
#include "users.h"
#include "feed.h"

void feed(char *name, int feed_size, list_graph_t *friends, post_trees_t *tree) {
	int id = get_user_id(name), cnt = 0;
	for (int i = tree->size - 1; cnt < feed_size && i >= 0; i--)
		if (lg_has_edge(friends, tree->posts[i]->user_id, id) ||
						id == tree->posts[i]->user_id) {
			cnt++;
			printf("%s: \"%s\"\n", get_user_name(tree->posts[i]->user_id),
					tree->posts[i]->title);
		}
}

// recursive traversal for searching reposts by user_id
void search_posts_id(post_t *post, int id, char *title) {
	if (!post)
		return;
	if (post->user_id == id)
		printf("Reposted: \"%s\"\n", title);
	for (int i = 0; i < post->num_repost; i++)
		search_posts_id(post->events[i], id, title);
}

void view_profile(char *name, post_trees_t *tree) {
	int id = get_user_id(name);
	for (int i = 0; i < tree->size; i++) {
		if (tree->posts[i]->user_id == id)
			printf("Posted: \"%s\"\n", tree->posts[i]->title);
		for (int j = 0; j < tree->posts[i]->num_repost; j++)
			search_posts_id(tree->posts[i]->events[j], id,
							tree->posts[i]->title);
	}
}

int is_friends_repost(post_t *post, list_graph_t *friends, int id) {
	if (!post)
		return 0;
	if(lg_has_edge(friends, post->user_id, id))
		return 1;
	if (!post->events)
		return 0;
	for (int i = 0; i < post->num_repost; i++)
		if (is_friends_repost(post->events[i], friends, id))
			return 1;
	return 0;
}

// recursive traversal for searching friends' reposts
void search_friends_reposts(post_t *post, list_graph_t *friends, int id) {
	if (!post)
		return;
	if(lg_has_edge(friends, post->user_id, id))
		printf("%s\n", get_user_name(post->user_id));
	if (!post->events)
		return;
	for (int i = 0; i < post->num_repost; i++)
		search_friends_reposts(post->events[i], friends, id);
}

void friends_repost(char *name, int id, post_trees_t *tree, list_graph_t *friends) {
	post_t *post = binary_search_node(tree->posts, tree->size, id);
	if (is_friends_repost(post, friends, id))
		printf("Friends that reposted:\n");
	search_friends_reposts(post, friends, get_user_id(name));
}

int max_lower_bin_search(int *arr, int size, int x) {
	int l = 0, r = size - 1, mid;
	int p = 0;
	while (l <= r) {
		mid = (r + l) / 2;
		if (arr[mid] == x) {
			p = mid;
			break;
		}
		else if (arr[mid] < x) {
			p = mid + 1;
			l = mid + 1;
		}
		else
			r = mid - 1;
	}
	return p;
}

/* applies kerbosch algorithm where
r - stores current maximal clique
p - stores list of nodes to be traversed
x - stores list of excluded nodes
*/
void kerbosch(int *r, int r_len, int *p, int p_len, int *x, int x_len,
			list_graph_t *friends, int *sol, int *len) {
	if (p_len == 0 && x_len == 0 && r_len > *len) {
		*len = r_len;
		for (int i = 0; i < *len; i++)
			sol[i] = r[i];
		return;
	}
	int p2[550], cnt_p = 0;
	int x2[550], cnt_x = 0;
	int pos;
	for(int i = 0; i < p_len; i++) {
		int v = p[i];
		pos = max_lower_bin_search(r, r_len, v);
		for(int j = r_len; j > pos; j--)
			r[j] = r[j - 1];
		r[pos] = v;
		r_len++;
		// for(int i = 0; i < r_len; i++)
		// 	printf("%d ", r[i]);
		// printf("\n");
		cnt_p = 0;
		cnt_x = 0;
		for (int j = 0; j < p_len; j++)
			if (lg_has_edge(friends, p[j], v))
				p2[cnt_p++] = p[j];
		for (int j = 0; j < x_len; j++)
			if (lg_has_edge(friends, x[j], v))
				x2[cnt_x++] = x[j];
		kerbosch(r, r_len, p2, cnt_p, x2, cnt_x, friends, sol, len);
		for (int j = pos; j < r_len - 1; j++)
			r[j] = r[j + 1];
		r_len--;
		x[x_len] = v;
		x_len++;
		for(int j = i; j < p_len - 1; j++)
			p[j] = p[j + 1];
		p_len--;
		i--;
	}
}

void common_group(char *name, list_graph_t *friends) {
	int *r, r_len = 0, *p, p_len, *x, x_len = 0, *sol, len = 0;
	r = calloc(550, sizeof(int));
	p = calloc(550, sizeof(int));
	x = calloc(550, sizeof(int));
	sol = calloc(550, sizeof(int));
	int id = get_user_id(name);
	p_len = 1;
	p[0] = id;
	ll_node_t *node = friends->neighbors[id]->head;
	while(node) {
		p[p_len++] = *((int *)(node->data));
		node = node->next;
	}
	kerbosch(r, r_len, p, p_len, x, x_len, friends, sol, &len);
	for (int i = 0; i < len; i++)
		printf("%s\n", get_user_name(sol[i]));
	free(r);
	free(p);
	free(x);
	free(sol);
}

void handle_input_feed(char *input, list_graph_t *friends, post_trees_t *tree)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "feed")) {
		char *name = strtok(NULL, "\n ");
		char *p = strtok(NULL, "\n ");
		int num = strtol(p, NULL, 10);
		feed(name, num, friends, tree);
	}
	else if (!strcmp(cmd, "view-profile")) {
		char *name = strtok(NULL, "\n ");
		view_profile(name, tree);
	}
	else if (!strcmp(cmd, "friends-repost")) {
		char *name = strtok(NULL, "\n ");
		char *p = strtok(NULL, "\n ");
		int num = strtol(p, NULL, 10);
		friends_repost(name, num, tree, friends);
	}
	else if (!strcmp(cmd, "common-group")) {
		char *name = strtok(NULL, "\n ");
		printf("The closest friend group of %s is:\n", name);
		common_group(name, friends);
	}

	free(commands);
}
