/*
 * CS 152, Spring 2022
 * Exchange
 * 
 * You will modify this file
 *
 * Name: Kaitlyn Li
 *
 * Sources used: none
 *
 * People consulted: none
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
    exchange_t *ex = ck_malloc(sizeof(exchange_t), "mk_exchange");
    ex->ticker = ck_strdup(ticker, "mk_exchange");
    ex->buy = make_buy_book();
    ex->sell = make_sell_book();
    return ex;
}

/*
 * free_exchange: free the space associated with the
 *   exchange
 *
 * exchange: an exchange
 */
void free_exchange(exchange_t *exchange) {
    free_book(exchange->buy);
    free_book(exchange->sell);
    if (exchange->ticker != NULL) {
        free(exchange->ticker);
    }
    ck_free(exchange);
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
    order_t *o = mk_order_from_line(ord_str, time);
    action_report_t *ret = mk_action_report(o->ticker);

    if (o->book == 'B') {
        while (o->shares > 0) {
            order_t *first = first_in_book(exchange->sell);
            if (first == NULL || first->price > o->price) {
                add_buy_order(o, exchange->buy);
                add_action(ret, BOOKED_BUY, o->oref, o->price, 
                    o->shares);
                return ret;
            } else if (first->shares > o->shares) {
                first->shares -= o->shares;
                add_action(ret, EXECUTE, first->oref, first->price, o->shares);
                return ret;
            } else {
                o->shares -= first->shares;
                add_action(ret, EXECUTE, first->oref, first->price, 
                    first->shares);
                remove_order(first->oref, exchange->sell);
            }
        }
        return ret;
    } else {
        while (o->shares > 0) {
            order_t *first = first_in_book(exchange->buy);
            if (first == NULL || first->price < o->price) {
                add_sell_order(o, exchange->sell);
                add_action(ret, BOOKED_SELL, o->oref, o->price, 
                    o->shares);
                return ret;
            } else if (first->shares > o->shares) {
                first->shares -= o->shares;
                add_action(ret, EXECUTE, first->oref, first->price, o->shares);
                return ret;
            } else {
                o->shares -= first->shares;
                add_action(ret, EXECUTE, first->oref, first->price, 
                    first->shares);
                remove_order(first->oref, exchange->buy);
            }
        }
        return ret;
    }
}

/*
 * print_exchange: print the contents of the exchange
 *
 * exchange: the exchange.
 */
void print_exchange(exchange_t *exchange) {
    printf("INET %s ", exchange->ticker);
    print_buy_book(exchange->buy);
    printf("\n");
    printf("INET %s ", exchange->ticker);
    print_sell_book(exchange->sell);
    printf("\n\n");
}
