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
 
#define MAX_ORDER_LEN 1000

/* These types are not visible outside this file.
 * Do NOT move them.
 */
typedef struct order_list order_list_t;


struct order_list {
  order_t *order;
  order_list_t *next;
};

struct book {
    enum book_type type;     // BUY_BOOK or SELL_BOOK
    order_list_t *pending;   // orders still in play.
};

/* make_buy_book: makes a buy book with just a dummy node in it
 *
 * Returns: pointer to the buy book
 */
struct book* make_buy_book() {
  struct book *b = ck_malloc(sizeof(struct book), "make_buy_book");
  b->type = BUY_BOOK;
  order_list_t *dummy = ck_malloc(sizeof(order_list_t), "make_buy_book");
  dummy->order = NULL;
  dummy->next = NULL;
  b->pending = dummy;
  return b;
}

/* make_sell_book: makes a sell book with just a dummy node in it
 *
 * Returns: pointer to the sell book
 */
struct book* make_sell_book() {
  struct book *b = ck_malloc(sizeof(struct book), "make_sell_book");
  b->type = SELL_BOOK;
  order_list_t *dummy = ck_malloc(sizeof(order_list_t), "make_sell_book");
  dummy->order = NULL;
  dummy->next = NULL;
  b->pending = dummy;
  return b;
}

/* add_buy_order: adds order to the buy book
 *
 * o: pointer to an order to add to the buy book
 * buy: buy book
 */
void add_buy_order(order_t *o, struct book *buy) {
  assert(is_buy_order(o));

  order_list_t *add = ck_malloc(sizeof(order_list_t), "add_buy_order");
  add->order = o;

  // case 1: only a dummy node in the book
  if (buy->pending->next == NULL) {
    buy->pending->next = add;
    add->next = NULL;
    return;
  }

  // case 2: more than just the dummy node in the book
  order_list_t *prev = buy->pending; // buy->pending is the dummy node
  order_list_t *curr = buy->pending->next; // first order in the list
  while (curr != NULL) {
    if (curr->order->price < o->price) {
      prev->next = add;
      add->next = curr;
      return;
    } else if (curr->order->price == add->order->price) {
      if (add->order->time < curr->order->time) {
        prev->next = add;
        add->next = curr;
        return;
      } 
    }
    prev = curr;
    curr = curr->next;
  }
  prev->next = add;
  add->next = NULL;
}

/* add_sell_order: add order to the sell book
 *
 * o: pointer to an order to add to the sell book
 * sell: sell book
 */
void add_sell_order(order_t *o, book_t *sell) {
   assert(is_sell_order(o));

  order_list_t *add = ck_malloc(sizeof(order_list_t), "add_sell_order");
  add->order = o;

  // case 1: only a dummy node in the book
  if (sell->pending->next == NULL) {
    sell->pending->next = add;
    add->next = NULL;
    return;
  }

  // case 2: more than just the dummy node in the book
  order_list_t *prev = sell->pending; // sell->pending is the dummy node
  order_list_t *curr = sell->pending->next; // first order in the list
  while (curr != NULL) {
    if (curr->order->price > add->order->price) {
      prev->next = add;
      add->next = curr;
      return;
    } else if (curr->order->price == add->order->price) {
      if (add->order->time < curr->order->time) {
        prev->next = add;
        add->next = curr;
        return;
      } 
    }
    prev = curr;
    curr = curr->next;
  }
  prev->next = add;
  add->next = NULL;
}

/* first_in_book: takes a book and returns the first order in it that is not
 *    the dummy node
 *
 * b: pointer to a struct book
 *
 * Returns: pointer to order_t
 */
order_t* first_in_book(struct book *b) {
  if (b->pending->next == NULL) {
    return NULL;
  } else {
    return b->pending->next->order;
  }
}

/* remove_order: removes order from book and frees the order
 * 
 * oref: oref of order to remove
 * book: pointer to book to remove order from
 */
void remove_order(long long oref, struct book *book) {
  // case 1: only a dummy node in the book
  if (book->pending->next == NULL) {
    return;
  }

  // case 2: more than just the dummy node in the book
  order_list_t *prev = book->pending;
  order_list_t *curr = book->pending->next;
  while (curr != NULL) {
    if (curr->order->oref == oref) {
      prev->next = curr->next;
      free_order(curr->order);
      ck_free(curr);
      return;
      }
    prev = curr;
    curr = curr->next;
  }
  return;
  }

/* free_book: frees a book by freeing order lists then freeing the book
 *
 * book = pointer to book to be freed
 */
void free_book(struct book *book) {
  if (book->pending->next == NULL) {
    ck_free(book->pending);
    ck_free(book);
    return;
  }

  order_list_t *remove = book->pending;
  order_list_t *curr = book->pending->next;
  while (curr != NULL) {
    if (remove->order != NULL) {
      free_order(remove->order);
    }
    ck_free(remove);
    remove = curr;
    curr = curr->next;
  }
  if (remove->order != NULL) {
    free_order(remove->order);
  }
  ck_free(remove);
  ck_free(book);
}

/* print_buy_book: prints the contents of a buy book (without dummy node)
 * 
 * b: pointer to a buy book
 */
void print_buy_book(struct book *b) {

  printf("buy book: ");

  if (b->pending->next == NULL) {
    printf("empty \n");
    return;
  } else {
    printf("\n");
  }

  order_list_t *curr = b->pending->next;

  printf("\t");
  print_order(curr->order);
  curr = curr->next;

  while (curr != NULL) {
    printf("\t");
    print_order(curr->order);
    curr = curr->next;
  }
}

/* print_sell_book: prints the contents of a sell book (without dummy node)
 * 
 * b: pointer to a sell book
 */
void print_sell_book(struct book *b) {

  printf("sell book: ");

  if (b->pending->next == NULL) {
    printf("empty \n");
    return;
  } else {
    printf("\n");
  }

  order_list_t *curr = b->pending->next;

  printf("\t");
  print_order(curr->order);
  curr = curr->next;

  while (curr != NULL) {
    printf("\t");
    print_order(curr->order);
    curr = curr->next;
  }
}