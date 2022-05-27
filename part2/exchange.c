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

    if (o->type == 'A') {
        order_t *f;
        book_t *bk;
        while (o->shares > 0) {
            if (is_buy_order(o)) {
                f = first_in_book(exchange->sell);
                bk = exchange->sell;
            } else {
                f = first_in_book(exchange->buy);
                bk = exchange->buy;
            }

            if ((f == NULL) || !(orders_match(o, f))) {
                if (is_buy_order(o)) {
                    add_action(ret, BOOKED_BUY, o->oref, o->price, o->shares);
                    add_order(o, exchange->buy);
                } else {
                    add_action(ret, BOOKED_SELL, o->oref, o->price, o->shares);
                    add_order(o, exchange->sell);
                }
                return ret;
            } else {
                if (o->shares >= f->shares) {
                    add_action(ret, EXECUTE, f->oref, f->price, f->shares);
                    o->shares -= f->shares;
                    remove_order(f->oref, bk);
                    if (o->shares == 0) {
                        free_order(o);
                        return ret;
                    }
                } else {
                    add_action(ret, EXECUTE, f->oref, f->price, o->shares);
                    f->shares -= o->shares;
                    free_order(o);
                    return ret;
                }
            }
        }
    } else {
        book_t *bk;
        if (o->book == 'B') {
            bk = exchange->buy;
        } else {
            bk = exchange->sell;
        }
        
        order_t *found = find_order(o->oref, bk);
        
        if (found == NULL) {
            free_order(o);
            return ret;
        }

        if (found->shares <= o->shares) {
            if (o->book == 'B') {
                add_action(ret, CANCEL_BUY, o->oref, o->price, found->shares);
            } else {
                add_action(ret, CANCEL_SELL, o->oref, o->price, found->shares);
            }
            remove_order(o->oref, bk);
        } else {
            if (o->book == 'B') {
                add_action(ret, CANCEL_BUY, o->oref, found->price, o->shares);
            } else {
                add_action(ret, CANCEL_SELL, o->oref, found->price, o->shares);
            }
            found->shares -= o->shares;
        }
        free_order(o);
    }
    return ret;
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
