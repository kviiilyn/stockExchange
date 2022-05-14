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


/* make_buy_book: makes a buy book with just a dummy node in it
 *
 * Returns: pointer to the buy book
 */
struct book* make_buy_book();

/* make_sell_book: makes a sell book with just a dummy node in it
 *
 * Returns: pointer to the sell book
 */
struct book* make_sell_book();

/* add_buy_order: adds order to the buy book
 *
 * o: pointer to an order to add to the buy book
 * buy: buy book
 */
void add_buy_order(order_t *o, struct book *buy);

/* add_sell_order: add order to the sell book
 *
 * o: pointer to an order to add to the sell book
 * sell: sell book
 */
void add_sell_order(order_t *o, book_t *sell);

/* first_in_book: takes a book and returns the first order in it that is not
 *    the dummy node
 *
 * b: pointer to a struct book
 *
 * Returns: pointer to order_t
 */
order_t* first_in_book(struct book *b);

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
