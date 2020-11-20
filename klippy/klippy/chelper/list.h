#ifndef __LIST_H
#define __LIST_H

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})


/****************************************************************
 * list - Double linked lists
 ****************************************************************/

struct list_node {
    struct list_node *next, *prev;
};

struct list_head {
    struct list_node root;
};

static inline void
list_init(struct list_head *h)
{
    h->root.prev = h->root.next = &h->root;
}

static inline int
list_empty(const struct list_head *h)
{
    return h->root.next == &h->root;
}

static inline int
list_is_first(const struct list_node *n, const struct list_head *h)
{
    return n->prev == &h->root;
}

static inline int
list_is_last(const struct list_node *n, const struct list_head *h)
{
    return n->next == &h->root;
}

static inline void
list_del(struct list_node *n)
{
    struct list_node *prev = n->prev;
    struct list_node *next = n->next;
    next->prev = prev;
    prev->next = next;
}

static inline void
__list_add(struct list_node *n, struct list_node *prev, struct list_node *next)
{
    next->prev = n;
    n->next = next;
    n->prev = prev;
    prev->next = n;
}

static inline void
list_add_after(struct list_node *n, struct list_node *prev)
{
    __list_add(n, prev, prev->next);
}

static inline void
list_add_before(struct list_node *n, struct list_node *next)
{
    __list_add(n, next->prev, next);
}

static inline void
list_add_head(struct list_node *n, struct list_head *h)
{
    list_add_after(n, &h->root);
}

static inline void
list_add_tail(struct list_node *n, struct list_head *h)
{
    list_add_before(n, &h->root);
}

static inline void
list_join_tail(struct list_head *add, struct list_head *h)
{
    if (!list_empty(add)) {
        struct list_node *prev = h->root.prev;
        struct list_node *next = &h->root;
        struct list_node *first = add->root.next;
        struct list_node *last = add->root.prev;
        first->prev = prev;
        prev->next = first;
        last->next = next;
        next->prev = last;
    }
}

#define list_next_entry(pos, member)                            \
    container_of((pos)->member.next, typeof(*pos), member)

#define list_prev_entry(pos, member)                            \
    container_of((pos)->member.prev, typeof(*pos), member)

#define list_first_entry(head, type, member)                    \
    container_of((head)->root.next, type, member)

#define list_last_entry(head, type, member)                     \
    container_of((head)->root.prev, type, member)

#define list_for_each_entry(pos, head, member)                  \
    for (pos = list_first_entry((head), typeof(*pos), member)   \
         ; &pos->member != &(head)->root                        \
         ; pos = list_next_entry(pos, member))

#define list_for_each_entry_safe(pos, n, head, member)          \
    for (pos = list_first_entry((head), typeof(*pos), member)   \
          , n = list_next_entry(pos, member)                    \
         ; &pos->member != &(head)->root                        \
         ; pos = n, n = list_next_entry(n, member))


#endif // list.h
