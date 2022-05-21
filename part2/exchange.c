/*
 * CS 152, Spring 2022
 * Exchange Data Structure
 * 
 * You will modify this file
 *
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "order.h"
#include "book.h"
#include "action_report.h"
#include "util.h"
#include "exchange.h"

struct exchange {
  char *ticker;
  book_t *buy;
  book_t *sell;  
};

/* 
 * mk_exchange: make an exchange for the specified ticker symbol
 *
 * ticker: the ticker symbol for the stock
 *
 * Returns: an exchange
 */
exchange_t *mk_exchange(char *ticker) {
    // You are REQUIRED to complete this function
    // replace NULL with a suitable value
    return NULL;
}

/*
 * free_exchange: free the space associated with the
 *   exchange
 *
 * exchange: an exchange
 */
void free_exchange(exchange_t *exchange) {
    // You are REQUIRED to complete this function
}


/* 
 * process_order: process an order. Returns a action_report for the
 *   actions completed in the process.
 *
 * exchange: an exchange
 * ord_str: a string describing the order (in the expected format)
 * time: the time the order was placed.
 */
action_report_t  *process_order(exchange_t *exchange, char *ord_str, int time) {
    assert(exchange != NULL);
    assert(ord_str != NULL);

    // YOU ARE REQUIRED TO COMPLETE THIS FUNCTION
    // replace NULL with a suitable value
    return NULL;
}

/*
 * print_exchange: print the contents of the exchange
 *
 * exchange: the exchange.
 */
void print_exchange(exchange_t *exchange) {
    // This function is optional, but highly recommended.
}
