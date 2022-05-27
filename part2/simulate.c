/*
 * CS 152, Spring 2022
 * Simulation -- main file
 * 
 * You will modify this file
 *
 * Name: Kaitlyn Li
 *
 * Sources used: none
 *
 * People consulted: none
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "action_report.h"
#include "exchange.h"
#include "util.h"

#define MAX_ORDER_LEN 1000
#define MAX_TEST_FILENAME 30

char int_to_char(int i) {
    return (char)i;
}

void process_order_write_ar(exchange_t *ex, char *o, int time, FILE *out) {
    action_report_t *ar = process_order(ex, o, time);
    write_action_report_to_file(ar, out, time);
    free_action_report(ar);
}

int main(int argc, char **argv) {
    exchange_t *ex = mk_exchange(argv[1]);
    char *openf;
    char *outf;

    int test_num;

    sscanf(argv[2], "%d", &test_num);

    openf = (char*)malloc(sizeof(char) * MAX_TEST_FILENAME);
    outf = (char*)malloc(sizeof(char) * MAX_TEST_FILENAME);

    if (openf == NULL) {
        fprintf(stderr, "Unable to allocate openf");
        exit(1);
    }
    if (outf == NULL) {
        fprintf(stderr, "Unable to allocate outf");
        exit(1);
    }

    sprintf(openf, "tests/test%d_orders.csv", test_num);
    sprintf(outf, "tests/test%d_actions.csv", test_num);
    FILE *fp;
    fp = fopen(openf, "r");
    if (fp == NULL) {
        fprintf(stderr, "simulate: file is empty\n");
        exit(1);
    }
    FILE *ofp;
    ofp = fopen(outf, "w");

    int time = 0;
    int c;
    char a = 'a';

    /*char *order_s = ck_malloc(sizeof(char) * MAX_ORDER_LEN, 
            "Unable to allocate order_s"); */

    while (!feof(fp)) {
        char *order_s = ck_malloc(sizeof(char) * MAX_ORDER_LEN, 
            "Unable to allocate order_s");
        for (int i = 0;; i++) {
            c = fgetc(fp);
            if (feof(fp)) {
                break;
            }
            a = int_to_char(c);
            if (a != '\n') {
                order_s[i] = a;
            } else {
                order_s[i] = '\0';
                break;
            }
        }
        if (feof(fp)) {
            free(order_s);
            break;
        }
        process_order_write_ar(ex, order_s, time, ofp);
        time++;
        free(order_s);
    }

    free_exchange(ex);
    fclose(fp);
    fclose(ofp);
    //free(order_s);
    free(openf);
    free(outf);
    
    
}


