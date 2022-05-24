/*
 * CS 152, Spring 2022
 * Book Data Structure Interface.
 * 
 * You will modify this file.
 */

/********* Do not modify this section *********/
#ifndef BOOK_H
#define BOOK_H

enum book_type {BUY_BOOK, SELL_BOOK};

/* The book type is opaque, which means that the client cannot reach
 * into the implementation and make changes directly.  Do NOT move the
 * structure type into this file.
 */
typedef struct book book_t;


/********** Modify below **********************/

// The prototypes for your public book functions
// go here.  Don't forget to include header
// comments that describe the purpose of the
// functions, the arguments, and the return value.

/* make_buy_book: makes a buy book
 *
 * Returns: pointer to the buy book
 */
struct book* make_buy_book();

/* make_sell_book: makes a sell book
 *
 * Returns: pointer to the sell book
 */
struct book* make_sell_book();

/* swap: swaps the positions being pointed at of two pointers to orders
 *
 * a: pointer to the first order
 * b: pointer to the second order
 */
void swap(order_t *a, order_t *b);

/* compare_orders_min: compares two orders in a min heap
 *
 * o1: pointer to the first order
 * o2: pointer to the second order
 *
 * Returns: true if the first order should go before the second in a min heap
 */
bool compare_orders_min(order_t *o1, order_t *o2);

/* compare_orders_max: compares two orders in a max heap
 *
 * o1: pointer to the first order
 * o2: pointer to the second order
 *
 * Returns: true if the first order should go before the second in a max heap
 */
bool compare_orders_max(order_t *o1, order_t *o2);     

/* sift_up_min: sifts up from the bottom of a min heap
 *
 * heap: array of pointers to orders
 * occupied: the number of slots currently occupied by the heap
 */
void sift_up_min(order_t **heap, int occupied);

/* sift_up_max: sifts up from the bottom of a max heap
 *
 * heap: array of pointers to orders
 * occupied: the number of slots currently occupied by the heap
 */
void sift_up_max(order_t **heap, int occupied);

/* insert: inserts an order into a book, reallocates space if needed
 *
 * bk: pointer to the book for the order to be added to
 * o: order to be added
 */
void insert(book_t *bk, order_t *o);

/* add_order: adds order to a book
 *
 * o: pointer to an order to add to the book
 * bk: pointer to a book
 */
void add_order(order_t *o, struct book *bk);

/* first_in_book: takes a book and returns the first order in it that is not
 *    the dummy node
 *
 * b: pointer to a struct book
 *
 * Returns: pointer to order_t
 */
order_t* first_in_book(struct book *b);

/* sift_down_min: takes a heap, the size of the heap, and an index and sifts
 *      down based on the rules of a min heap
 *
 * heap: array of pointers to orders
 * occupied: number of spaces in the array that are occupied
 * i: the index to start sifting down
 */
void sift_down_min(order_t **heap, int occupied, int i);

/* sift_down_max: takes a heap, the size of the heap, and an index and sifts
 *      down based on the rules of a max heap
 *
 * heap: array of pointers to orders
 * occupied: number of spaces in the array that are occupied
 * i: the index to start sifting down
 */
void sift_down_max(order_t **heap, int occupied, int i);

/* find_order: finds an order in a book based on its oref
 * 
 * oref: oref of order to find
 * book: pointer to book
 *
 * Returns: pointer to the order
 */
order_t* find_order(long long oref, struct book *book);

/* remove_order: removes order from book and frees the order
 * 
 * oref: oref of order to remove
 * book: pointer to book to remove order from
 */
void remove_order(long long oref, struct book *book);

/* free_book: frees a book by removing all the orders then freeing the book
 *
 * book = pointer to book to be freed
 */
void free_book(struct book *book);

/* print_buy_book: prints the contents of a buy book (without dummy node)
 * 
 * b: pointer to a buy book
 */
void print_buy_book(struct book *b);

/* print_sell_book: prints the contents of a sell book (without dummy node)
 * 
 * b: pointer to a sell book
 */
void print_sell_book(struct book *b);

// Do not remove the next line
#endif
