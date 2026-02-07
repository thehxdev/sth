#ifndef STH_DS_QUEUE_H_
#define STH_DS_QUEUE_H_

// Queue implementation took from Nginx

#define sth_ds_queue_init(queue) \
    do { \
        (queue)->prev = queue; \
        (queue)->next = queue; \
    } while (0)

#define sth_ds_queue_is_empty(queue)    ((queue) == (queue)->prev)

#define sth_ds_queue_insert_head(queue, node) \
    do { \
        (node)->next = (queue)->next; \
        (node)->next->prev = node; \
        (node)->prev = queue; \
        (queue)->next = (node); \
    } while (0)

#define sth_ds_queue_insert_after  sth_ds_queue_insert_head

#define sth_ds_queue_insert_tail(queue, node) \
    do { \
        (node)->prev = (queue)->prev; \
        (node)->prev->next = (node); \
        (node)->next = (queue); \
        (queue)->prev = (node); \
    } while (0)

#define sth_ds_queue_insert_before  sth_ds_queue_insert_tail

#define sth_ds_queue_head(queue)        (queue)->next
#define sth_ds_queue_last(queue)        (queue)->prev
#define sth_ds_queue_sentinel(queue)    (queue)

#define sth_ds_queue_next(node)    (node)->next
#define sth_ds_queue_prev(node)    (node)->prev

#define sth_ds_queue_remove(node) \
    do { \
        (node)->next->prev = (node)->prev; \
        (node)->prev->next = (node)->next; \
        (node)->next = NULL; \
        (node)->prev = NULL; \
    } while (0)

#define sth_ds_queue_data(queue, type, field) \
    (type *)((unsigned char *)(queue) - offsetof(type, field))

#ifdef STH_STRIP_PREFIX
    #define queue_init          sth_ds_queue_init
    #define queue_is_empty      sth_ds_queue_is_empty
    #define queue_insert_head   sth_ds_queue_insert_head
    #define queue_insert_after  sth_ds_queue_insert_head
    #define queue_insert_tail   sth_ds_queue_insert_tail
    #define queue_insert_before sth_ds_queue_insert_before
    #define queue_head          sth_ds_queue_head
    #define queue_last          sth_ds_queue_last
    #define queue_sentinel      sth_ds_queue_sentinel
    #define queue_next          sth_ds_queue_next
    #define queue_prev          sth_ds_queue_prev
    #define queue_remove        sth_ds_queue_remove
    #define queue_data          sth_ds_queue_data
#endif

#endif // STH_DS_QUEUE_H_
