#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    while (q->head) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    /* Don't forget to allocate space for the string and copy it */
    char *tmp = malloc(sizeof(char) * (strlen(s) + 1));
    /* What if either call to malloc returns NULL? */
    if (!tmp) {
        /* Get return NULL, so release newh */
        free(newh);
        newh = NULL;
        return false;
    }

    newh->value = tmp;
    strncpy(newh->value, s, (strlen(s) + 1));
    newh->next = q->head;
    q->head = newh;
    /* If queue is empty, it also set tail*/
    if (q->size == 0)
        q->tail = newh;

    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* Remember: It should operate in O(1) time */
    if (!q)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    char *tmp = malloc(sizeof(char) * (strlen(s) + 1));
    if (!tmp) {
        free(newh);
        newh = NULL;
        return false;
    }

    newh->value = tmp;
    strncpy(newh->value, s, (strlen(s) + 1));

    if (q->size == 0) {
        newh->next = NULL;
        q->tail = newh;
        q->head = newh;
    } else {
        newh->next = NULL;
        q->tail->next = newh;
        q->tail = newh;
    }

    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    if (!q)
        return false;

    if (!sp || !q->head)
        return false;

    if (bufsize > strlen(q->head->value)) {
        strncpy(sp, q->head->value, strlen(q->head->value));
        sp[strlen(q->head->value)] = '\0';
    } else {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    tmp = NULL;

    q->size--;
    if (q->size == 0)
        q->tail = NULL;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q || !q->head)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if ((!q) || (q->size) == 0)
        return;

    list_ele_t *curr = q->head, *next = q->head->next, *tmp;

    q->tail = q->head;

    while (next) {
        tmp = next->next;
        next->next = curr;
        curr = next;
        next = tmp;
    }

    q->tail->next = NULL;
    q->head = curr;
}
list_ele_t *merge(list_ele_t *lhs, list_ele_t *rhs)
{
    list_ele_t *head = NULL;
    list_ele_t *tail = NULL;
    while (lhs && rhs) {
        if (strcmp(lhs->value, rhs->value) <= 0) {
            if (!head) {
                head = lhs;
                tail = lhs;
            } else {
                tail->next = lhs;
                tail = lhs;
            }
            lhs = lhs->next;
        } else {
            if (!head) {
                head = rhs;
                tail = rhs;
            } else {
                tail->next = rhs;
                tail = rhs;
            }
            rhs = rhs->next;
        }
    }
    if (lhs)
        tail->next = lhs;
    if (rhs)
        tail->next = rhs;

    return head;
}
list_ele_t *mergeSort(list_ele_t *head)
{
    if (!head || head->next == NULL)
        return head;
    list_ele_t *fast = head->next, *slow = head;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *lhs = mergeSort(head);
    list_ele_t *rhs = mergeSort(fast);

    return merge(lhs, rhs);
}
/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;
    q->head = mergeSort(q->head);
    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}
