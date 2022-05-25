/*
 * CS 152, Spring 2022
 * Book Data Structure Implementation
 * 
 * You will modify this file.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "order.h"
#include "book.h"
#include "util.h"
 
/* These types are not visible outside this file.  Do NOT move them.
 */

struct book {
    enum book_type type;     // BUY_BOOK or SELL_BOOK
    int occupied;
    int slots; // initial size set to 10
    order_t **heap;
};

/* make_buy_book: makes a buy book
 *
 * Returns: pointer to the buy book
 */
struct book* make_buy_book() {
    book_t *bk = (book_t*)ck_malloc(sizeof(book_t), "make_buy_book");
    bk->heap = (order_t**)ck_malloc(sizeof(order_t*) * 10, "make_buy_book");
    bk->slots = 10;
    bk->occupied = 0;
    bk->type = BUY_BOOK;
    for (int i = 0; i<bk->slots; i++) {
        bk->heap[i] = NULL;
    }
    return bk;
}

/* make_sell_book: makes a sell book
 *
 * Returns: pointer to the sell book
 */
struct book* make_sell_book(){
    book_t *bk = (book_t*)ck_malloc(sizeof(book_t), "make_sell_book");
    bk->heap = (order_t**)ck_malloc(sizeof(order_t*) * 10, "make_sell_book");
    bk->slots = 10;
    bk->occupied = 0;
    bk->type = SELL_BOOK;
    for (int i = 0; i<bk->slots; i++) {
        bk->heap[i] = NULL;
    }
    return bk;
}

/* swap: swaps the orders at two pointers
 *
 * a: pointer to the first order
 * b: pointer to the second order
 */
void swap(order_t *a, order_t *b) {
    order_t help = *a;
    *a = *b;
    *b = help;

    /* what i had before
    order_t *help = a;
    a = b;
    b = help;
    */
}

/* compare_orders_min: compares two orders in a min heap
 *
 * o1: pointer to the first order
 * o2: pointer to the second order
 *
 * Returns: true if the first order should go before the second in a min heap
 */
bool compare_orders_min(order_t *o1, order_t *o2) {
    return ((o1->price < o2->price) || ((o1->price == o2->price) && 
            (o1->time < o2->time)));
}

/* compare_orders_max: compares two orders in a max heap
 *
 * o1: pointer to the first order
 * o2: pointer to the second order
 *
 * Returns: true if the first order should go before the second in a max heap
 */
bool compare_orders_max(order_t *o1, order_t *o2) {
    return ((o1->price > o2->price) || ((o1->price == o2->price) && 
            (o1->time < o2->time)));
}       

/* sift_up_min: sifts up from the bottom of a min heap
 *
 * heap: array of pointers to orders
 * occupied: the number of slots currently occupied by the heap
 */
void sift_up_min(order_t **heap, int occupied) {
    int parent;
    int i = occupied - 1;    
    while (i > 0) {
        parent = (i - 1)/2;
        if (compare_orders_min(heap[i], heap[parent])) {
            swap(heap[i], heap[parent]);
            i = parent;
        } else {
            return;
        }
    }
}

/* sift_up_max: sifts up from the bottom of a max heap
 *
 * heap: array of pointers to orders
 * occupied: the number of slots currently occupied by the heap
 */
void sift_up_max(order_t **heap, int occupied) {
    int parent;
    int i = occupied - 1;
    while (i > 0) {
        parent = (i - 1)/2;
        if (compare_orders_max(heap[i], heap[parent])) {
            swap(heap[i], heap[parent]);
            i = parent;
        } else {
            return;
        }
    }
}

/* insert: inserts an order into a book
 *
 * bk: pointer to the book for the order to be added to
 * o: order to be added
 */
void insert(book_t *bk, order_t *o) {
    bk->heap[bk->occupied - 1] = o;
    switch (bk->type) {
        case BUY_BOOK:
            sift_up_max(bk->heap, bk->occupied);
            break;
        case SELL_BOOK:
            sift_up_min(bk->heap, bk->occupied);
            break;
    }
}

/* add_order: adds order to a book, reallocates space if needed
 *
 * o: pointer to an order to add to the book
 * bk: pointer to a book
 */
void add_order(order_t *o, struct book *bk) {
    bk->occupied += 1;
    if (bk->occupied >= bk->slots) {
        bk->heap = (order_t**)ck_realloc(bk->heap, 
                sizeof(order_t*) * (bk->slots + 10), "add_order");
        bk->slots += 10;
        for (int i = bk->occupied; i<bk->slots; i++) {
            bk->heap[i] = NULL;
        }
    }
    insert(bk, o);

}

/* first_in_book: takes a book and returns the first order in it that is not
 *    the dummy node
 *
 * b: pointer to a struct book
 *
 * Returns: pointer to order_t
 */
order_t* first_in_book(struct book *b) {
    return b->heap[0];
}

/* sift_down_min: takes a heap, the size of the heap, and an index and sifts
 *      down based on the rules of a min heap
 *
 * heap: array of pointers to orders
 * occupied: number of spaces in the array that are occupied
 * i: the index to start sifting down
 */
void sift_down_min(order_t **heap, int occupied, int i) {
    while (1) {
        int min_i = i;
        int lk = 2*i + 1;
        int rk = 2*i + 2;
        if (lk < occupied) {
            if (compare_orders_min(heap[lk], heap[min_i])) {
                min_i = lk;
            }
        }
        if (rk < occupied) {
            if (compare_orders_min(heap[rk], heap[min_i])) {
                min_i = rk;
            }
        }
        if (min_i != i) {
            swap(heap[i], heap[min_i]);
        } else {
            return;
        }
    }
}

/* sift_down_max: takes a heap, the size of the heap, and an index and sifts
 *      down based on the rules of a max heap
 *
 * heap: array of pointers to orders
 * occupied: number of spaces in the array that are occupied
 * i: the index to start sifting down
 */
void sift_down_max(order_t **heap, int occupied, int i) {
    while (1) {
        int max_i = i;
        int lk = 2*i + 1;
        int rk = 2*i + 2;
        if (lk < occupied) {
            if (compare_orders_max(heap[lk], heap[max_i])) {
                max_i = lk;
            }
        }
        if (rk < occupied) {
            if (compare_orders_max(heap[rk], heap[max_i])) {
                max_i = rk;
            }
        }
        if (max_i != i) {
            swap(heap[i], heap[max_i]);
        } else {
            return;
        }
    }
}

/* find_order: finds an order in a book based on its oref
 * 
 * oref: oref of order to find
 * book: pointer to book
 *
 * Returns: pointer to the order
 */
order_t* find_order(long long oref, struct book *book) {
    int i = 0;
    bool found = false;
    for (; i<book->occupied; i++) {
        if (book->heap[i]->oref == oref) {
            found = true;
            break;
        }
    }
    if (found) {
        return book->heap[i];
    } else {
        return NULL;
    }
}

/* remove_order: removes order from book and frees the order
 * 
 * oref: oref of order to remove
 * book: pointer to book to remove order from
 */
void remove_order(long long oref, struct book *book) {
    int i = 0;
    bool found = false;
    for (; i<book->occupied; i++) {
        if (book->heap[i]->oref == oref) {
            found = true;
            break;
        }
    }
    if (!found) {
        return;
    }
    int last_i = book->occupied - 1;
    swap(book->heap[i], book->heap[last_i]);
    free_order(book->heap[last_i]);
    book->occupied -= 1;
    book->heap[last_i] = NULL;
    switch (book->type) {
        case BUY_BOOK:
            sift_down_max(book->heap, book->occupied, i);
            break;
        case SELL_BOOK:
            sift_down_min(book->heap, book->occupied, i);
            break;
    }
}

/* free_book: frees a book by removing all the orders then freeing the book
 *
 * book = pointer to book to be freed
 */
void free_book(struct book *book) {
    for (int i = 0; i<book->occupied; i++) {
        free_order(book->heap[i]);
    }
    ck_free(book->heap);
    ck_free(book);
}

/* print_buy_book: prints the contents of a buy book (without dummy node)
 * 
 * b: pointer to a buy book
 */
void print_buy_book(struct book *b) {
    printf("buy book: ");
    
    if (b->occupied == 0) {
        printf("empty \n");
        return;
    } else {
        printf("\n");
    }

    order_t **ordered = (order_t**)ck_malloc(sizeof(order_t*) * b->occupied, 
            "print_buy_book");
    ordered[0] = b->heap[0];
    order_t *o;
    bool added = false;
    for (int i = 1; i<b->occupied; i++) {
        added = false;
        o = b->heap[i];
        for (int j = 0; j<i; j++) {
            if (compare_orders_max(o, b->heap[j])) {
                // shifting orders to the right to fit in order o
                for (int k = i; k>j; k--) {
                    ordered[i] = ordered[i - 1];
                }
                ordered[j] = o;
                added = true;
                break;
            }
        }
        if (!added) {
            ordered[i] = o;
        }
    }

    for (int i = 0; i<b->occupied; i++) {
        printf("\t");
        print_order(ordered[i]);
    }

    ck_free(ordered);
}

/* orders_match: returns true if orders "match" (ie can produce a trade)
 *
 * o1: pointer to the first order
 * o2: pointer to the second order
 */
bool orders_match(order_t *o1, order_t *o2) {
    if (! ((is_buy_order(o1) && is_sell_order(o2)) ||
            (is_sell_order(o1) && is_buy_order(o2)))) {
        return false;
    }
    if (is_buy_order(o1)) {
        return (o1->price >= o2->price);
    } else {
        return (o1->price <= o2->price);
    }
}

/* print_sell_book: prints the contents of a sell book (without dummy node)
 * 
 * b: pointer to a sell book
 */
void print_sell_book(struct book *b){
    printf("sell book: ");
    
    if (b->occupied == 0) {
        printf("empty \n");
        return;
    } else {
        printf("\n");
    }

    order_t **ordered = (order_t**)ck_malloc(sizeof(order_t*) * b->occupied, 
            "print_sell_book");
    ordered[0] = b->heap[0];
    order_t *o;
    bool added = false;
    for (int i = 1; i<b->occupied; i++) {
        added = false;
        o = b->heap[i];
        for (int j = 0; j<i; j++) {
            if (compare_orders_min(o, b->heap[j])) {
                // shifting orders to the right to fit in order o
                for (int k = i; k>j; k--) {
                    ordered[i] = ordered[i - 1];
                }
                ordered[j] = o;
                added = true;
                break;
            }
        }
        if (!added) {
            ordered[i] = o;
        }
    }

    for (int i = 0; i<b->occupied; i++) {
        printf("\t");
        print_order(ordered[i]);
    }

    ck_free(ordered);
}
