#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "posts.h"

post_trees_t *init_posts(int capacity) {
	post_trees_t *tree = calloc(1, sizeof(post_trees_t));
	tree->posts = calloc(capacity, sizeof(post_t *));
	tree->capacity = capacity;
	tree->size = 0;
	tree->id_cnt = 1;
	return tree;
}

post_t *create_post(int id, char *title, int max_size, int user_id) {
    post_t *post = (post_t *) malloc(sizeof(post_t));
    post->id = id;
    post->user_id = user_id;
    post->events = NULL;
	post->num_repost = 0;
	post->num_likes = 0;
	post->likes = NULL;
    if (!max_size) {
        post->title = NULL;
        return post;
    }
    post->title = (char *) malloc(max_size);
    strcpy(post->title, title);
    return post;
}

// binary search for finding the node with a given id
post_t *binary_search_node(post_t **events, int size, int id, int *pos) {
	int l = 0, r = size - 1, mid;
	while (l <= r) {
		mid = l + (r - l) / 2;
		if (events[mid]->id == id) {
			*pos = mid;
			return events[mid];
		}
		if (events[mid]->id < id)
			l = mid + 1;
		else
			r = mid - 1;
	}
	return NULL;
}

// recursive search for the node with a given id
post_t *search_node_id(post_t *root, int id, post_t **prev, int *idx) {
	if (!root)
		return NULL;
	if (root->id == id)
		return root;
	if (!root->events || !root->num_repost)
		return NULL;
	post_t *node = search_node_id(root->events[0], id, prev, idx);
	if (node && *idx == -1) {
		*prev = root;
		*idx = 0;
	}
	for(int i = 1; i < root->num_repost; i++) {
		post_t *curr = search_node_id(root->events[i], id, prev, idx);
		if (curr) {
			node = curr;
			if (*idx == -1) {
				*prev = root;
				*idx = i;
			}
		}
	}
	return node;
}

void create_repost(post_trees_t *tree, int post_id, char *name, int repost_id) {
	int pos = 0;
	post_t *post = binary_search_node(tree->posts, tree->size, post_id, &pos);
	if (repost_id) {
		// bfs search for the repost id
		post_t *prev = NULL;
		int idx = -1;
		post = search_node_id(post, repost_id, &prev, &idx);
	} 
	if (!post->events)
		post->events = calloc(550, sizeof(post_t *));
	post->events[post->num_repost] = create_post(tree->id_cnt, NULL, 0, get_user_id(name));
	post->num_repost++;
	printf("Created repost #%d for %s\n", tree->id_cnt, name);
	tree->id_cnt++;
}

post_t *lca(post_t *root, int id1, int id2) {
	if (!root || root->id == id1 || root->id == id2)
		return root;
	if (!root->events)
		return NULL;
	int cnt = 0;
	post_t *node = lca(root->events[0], id1, id2);
	cnt = node != NULL;
	for (int i = 1; i < root->num_repost; i++) {
		post_t *curr = lca(root->events[i], id1, id2);
		if (curr) {
			cnt++;
			if (!node)
				node = curr;
		}
	}
	return cnt == 2 ? root : node;
}

void common_repost(post_trees_t *tree, int post_id, int id1, int id2) {
	int pos = 0;
	post_t *post = binary_search_node(tree->posts, tree->size, post_id, &pos);
	post_t *ancestor = lca(post, id1, id2);
	printf("The first common repost of %d and %d is %d\n", id1, id2, ancestor->id);
}

void likes(post_trees_t *tree, char *name, int post_id, int repost_id) {
	int pos = 0;
	post_t *post = binary_search_node(tree->posts, tree->size, post_id, &pos);
	char *title = strdup(post->title);
	int user_id = get_user_id(name);
	if (repost_id) {
		post_t *prev = NULL;
		int idx = -1;
		post = search_node_id(post, repost_id, &prev, &idx);
	}
	if (!post->likes)
		post->likes = calloc(550, sizeof(int));
	post->likes[user_id] = ~post->likes[user_id];
	if (post->likes[user_id]) {
		post->num_likes++;
		if (repost_id == 0)
			printf("User %s liked post \"%s\"\n", name, title);
		else
			printf("User %s liked repost \"%s\"\n", name, title);
	}
	else {
		post->num_likes--;
		if (repost_id == 0)
			printf("User %s unliked post \"%s\"\n", name, title);
		else 
			printf("User %s unliked repost \"%s\"\n", name, title);
	}
	free(title);
}

// recursive search for the most liked repost
void most_liked_repost(post_t *root, int *max, int *max_id) {
	if (!root) {
		max_id = 0;
		return;
	}
	if (root->num_likes > *max) {
		*max = root->num_likes;
		*max_id = root->id;
	}
	for (int i = 0; i < root->num_repost; i++)
		most_liked_repost(root->events[i], max, max_id);
}

void ratio(post_trees_t *tree, int post_id) {
	int pos = 0;
	post_t *post = binary_search_node(tree->posts, tree->size, post_id, &pos);
	int max = 0, max_id = 0;
	most_liked_repost(post, &max, &max_id);
	if (max > post->num_likes)
		printf("Post %d got ratio'd by repost %d\n", post->id, max_id);
	else
		printf("The original post is the highest rated\n");
}

void delete_post(post_t **post) {
	if (!(*post))
		return;
	for (int i = 0; i < (*post)->num_repost; i++) {
		delete_post(&((*post)->events[i]));
	}
	if ((*post)->events) {
		free((*post)->events);
		(*post)->events = NULL;
	}
	if ((*post)->title) {
		free((*post)->title);
		(*post)->title = NULL;
	}
	if ((*post)->likes) {
		free((*post)->likes);
		(*post)->likes = NULL;
	}
	free(*post);
	*post = NULL;
}

void delete(post_trees_t *tree, int post_id, int repost_id) {
	int pos = 0;
	post_t *post = binary_search_node(tree->posts, tree->size, post_id, &pos);
	char *title = strdup(post->title);
	post_t *prev = NULL;
	int idx = -1;
	if (repost_id)
		post = search_node_id(post, repost_id, &prev, &idx);
	if (post->title)
		printf("Deleted \"%s\"\n", title);
	else
		printf("Deleted repost #%d of post \"%s\"\n", repost_id, title);
	delete_post(&post);
	if (repost_id && idx != -1) {
		for (int i = idx; i < prev->num_repost - 1; i++)
			prev->events[i] = prev->events[i + 1];
		prev->num_repost--;
	}
	else {
		for (int i = pos; i < tree->size - 1; i++)
			tree->posts[i] = tree->posts[i + 1];
		tree->size--;
	}
	free(title);
}

void get_likes(post_trees_t *tree, int post_id, int repost_id) {
	int pos = 0;
	post_t *post = binary_search_node(tree->posts, tree->size, post_id, &pos);
	if (repost_id) {
		int idx = -1;
		post_t *prev = NULL;
		post = search_node_id(post, repost_id, &prev, &idx);
		printf("Repost #%d has %d likes\n", repost_id, post->num_likes);
	}
	else
		printf("Post \"%s\" has %d likes\n", post->title, post->num_likes);
}

void print_reposts(post_t *post) {
	if (!post)
		return;
	if (!post->title)
		printf("Repost #%d by %s\n", post->id, get_user_name(post->user_id));
	if (!post->events)
		return;
	for(int i = 0; i < post->num_repost; i++)
		print_reposts(post->events[i]);
}

void get_reposts(post_trees_t *tree, int post_id, int repost_id) {
	int pos = 0;
	post_t *post = binary_search_node(tree->posts, tree->size, post_id, &pos);
	if (repost_id) {
		post_t *prev = NULL;
		int idx = -1;
		post = search_node_id(post, repost_id, &prev, &idx);
	}
	if (post->title)
		printf("\"%s\" - Post by %s\n", post->title, get_user_name(post->user_id));
	print_reposts(post);
}

void tree_free(post_trees_t **tree) {
	for (int i = 0; i < (*tree)->capacity; i++)
		delete_post(&((*tree)->posts[i]));
	free((*tree)->posts);
	free(*tree);
}

void handle_input_posts(char *input, post_trees_t *tree)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "create")) {
		char *name = strtok(NULL, "\n ");
		char *title = strtok(NULL, "\n\"");
		tree->posts[tree->size] = create_post(tree->id_cnt, title, 280, get_user_id(name));
		tree->size++;
		tree->id_cnt++;
		printf("Created \"%s\" for %s\n", title, name);
	} else if (!strcmp(cmd, "repost")) {
		char *name = strtok(NULL, "\n \"");
		char *p = strtok(NULL, "\n \"");
		int id = strtol(p, NULL, 10);
		int repost_id;
		p = strtok(NULL, "\n ");
		if (!p)
			repost_id = 0;
		else
			repost_id = strtol(p, NULL, 10);
		create_repost(tree, id, name, repost_id);
		
	} else if (!strcmp(cmd, "common-repost")){
		char *p = strtok(NULL, "\n ");
		int post_id = strtol(p, NULL, 10);
		p = strtok(NULL, "\n \"");
		int repost1 = strtol(p, NULL, 10);
		p = strtok(NULL, "\n \"");
		int repost2 = strtol(p, NULL, 10);
		common_repost(tree, post_id, repost1, repost2);
	}
	else if (!strcmp(cmd, "like")){
		char *name = strtok(NULL, "\n ");
		char *p = strtok(NULL, "\n ");
		int id = strtol(p, NULL, 10);
		int repost_id;
		p = strtok(NULL, "\n ");
		if (!p)
			repost_id = 0;
		else
			repost_id = strtol(p, NULL, 10);
		likes(tree, name, id, repost_id);

	} else if (!strcmp(cmd, "ratio")){
		char *p = strtok(NULL, "\n ");
		int id = strtol(p, NULL, 10);
		ratio(tree, id);
	}
	else if (!strcmp(cmd, "delete")) {
		char *p = strtok(NULL, "\n ");
		int id = strtol(p, NULL, 10);
		int repost_id;
		p = strtok(NULL, "\n ");
		if (!p)
			repost_id = 0;
		else
			repost_id = strtol(p, NULL, 10);
		delete(tree, id, repost_id);
	}
	else if (!strcmp(cmd, "get-likes")) {
		char *p = strtok(NULL, "\n ");
		int id = strtol(p, NULL, 10);
		int repost_id;
		p = strtok(NULL, "\n ");
		if (!p)
			repost_id = 0;
		else
			repost_id = strtol(p, NULL, 10);
		get_likes(tree, id, repost_id);
	}
	else if (!strcmp(cmd, "get-reposts")) {
		char *p = strtok(NULL, "\n ");
		int id = strtol(p, NULL, 10);
		int repost_id;
		p = strtok(NULL, "\n ");
		if (!p)
			repost_id = 0;
		else
			repost_id = strtol(p, NULL, 10);
		get_reposts(tree, id, repost_id);
	}
	
	free(commands);
}

