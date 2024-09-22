#ifndef POSTS_H
#define POSTS_H

typedef struct post_t post_t;
typedef struct post_trees_t post_trees_t;

struct post_t {
    int id;
    char *title;
    int user_id;
    post_t **events;
    int num_repost;
    int *likes;
    int num_likes;
};

struct post_trees_t {
    post_t **posts;
    int capacity;
    int size;
    int id_cnt;
};

post_trees_t *init_posts(int capacity);

post_t *create_post(int id, char *title, int max_size, int user_id);

post_t *binary_search_node(post_t **root, int size, int id, int *pos);

post_t *search_node_id(post_t *root, int id, post_t **prev, int *idx);

void create_repost(post_trees_t *tree, int post_id, char *name, int repost_id);

post_t *lca(post_t *root, int id1, int id2);

void common_repost(post_trees_t *tree, int post_id, int id1, int id2);

void likes(post_trees_t *tree, char *name, int post_id, int repost_id);

void most_liked_repost(post_t *root, int *max, int *max_id);

void ratio(post_trees_t *tree, int post_id);

void delete_post(post_t **post);

void delete(post_trees_t *tree, int post_id, int repost_id);

void get_likes(post_trees_t *tree, int post_id, int repost_id);

void print_reposts(post_t *post);

void get_reposts(post_trees_t *tree, int post_id, int repost_id);

void tree_free(post_trees_t **tree);

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_posts(char *input, post_trees_t *tree);

#endif // POSTS_H
