#include "list.h"

List* l_init() {
    List* list = malloc_s(sizeof(*list), __func__);

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

void l_insert(List* const list, const Data const data) {
    Node* new_node = malloc_s(sizeof(*new_node), __func__);

    new_node->data = data;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
        list->size = 1;
    } else {
        Node* last = list->tail;
        last->next = list->tail = new_node;
        list->size++;
    }
}

void l_remove(List* const list, const int index) {
    if (index >= list->size) {
        fprintf(stderr, "ERROR: [List-Remove] Index is greater than size");
        exit(EXIT_FAILURE);
    }

    if (list->size == 0) {
        fprintf(stderr, "ERROR: [List-Remove] Empty list");
        exit(EXIT_FAILURE);
    }

    Node* curr = list->head;
    Node* before = NULL;

    for (int i = 0; i < index; i++) {
        before = curr;
        curr = curr->next;
    }

    if (before == NULL) {
        list->head = curr->next;
        list->size--;
        free(curr);
    } else {
        before->next = curr->next;
        list->size--;
        free(curr);
    }
}

Data l_get(List* const list, const int index) {
    if (index >= list->size) {
        fprintf(stderr, "ERROR: [List-get] Index is greater than size");
        exit(EXIT_FAILURE);
    }

    if (list->size == 0) {
        fprintf(stderr, "ERROR: [List-get] Empty list");
        exit(EXIT_FAILURE);
    }

    Node* curr = list->head;

    for (int i = 0; i < index; i++) {
        curr = curr->next;
    }

    return curr->data;
}