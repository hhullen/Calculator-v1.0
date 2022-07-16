#ifndef SRC_S21_CALC_V1_0_TEST_H_
#define SRC_S21_CALC_V1_0_TEST_H_

#define TOKENS { "X", "NUM", "+", "-", "*", "/", "%", "^", "COS", "SIN", "TAN", "ACOS", "ASIN", \
                 "ATAN", "SQRT", "LN", "LOG", "(", ")", "~", ".", "END" }

// includes
#include "s21_calc_v1_0_header.h"
#include <check.h>

#define ACCURACUY 0.00000001
#define MONEY_ACCURACY 0.01

typedef struct stack_test {
    double value;
    char priority;
    lexems operand;
    struct stack_test *next;
    struct stack_test *last;
} stack_test;

// timer struct
typedef struct {
    long int hour;
    long int min;
    long int sec;
    long int millisec;
    struct timeb start;
    struct timeb end;
} exec_time_t;

// s21_operators_calculations
char OC1[] = { '5', POINT, '5', PLUS, '6', MULTIPLY, '2', FACTORIAL, DIVIDE, '3', \
                                                    EXTENT, '4', MINUS, '1', '6', END };
char OC2[] = { '2', EXTENT, '4', EXTENT, '2', END };
char OC3[] = { '0', POINT, '1', '2', '9', '6', MULTIPLY, '2', PLUS, '1', '7', POINT, '3', '4', END };
char OC4[] = { '1', PLUS, '7', MINUS, '3', MULTIPLY, EXP, END };
char OC5[] = { '8', DIVIDE, '4', MULTIPLY, '2', EXTENT, '3', PLUS, PI, END };
char OC6[] = { '2', '4', MOD, '7', END };
char OC7[] = { '2', EXTENT, '3', DIVIDE, '2', PLUS, '4', MINUS, '7', END };
char OC8[] = { '5', PLUS, '3', EXTENT, '2', MINUS, '4', DIVIDE, '1', POINT, '1', MULTIPLY, '1', '0', END };
char OC9[] = { '5', DIVIDE, '0', END };
char OC10[] = { '2', EXTENT, LBRACKET, UMINUS, '2', RBRACKET, END };

double OCRES[] = { -10.35185185, 65536.0, 17.5992, -0.154845485, 19.141592653, \
                                            3.0, 1.0, -22.36363636, INFINITY, 0.25 };

// s21_operators_calculations_BRACKETS
char OCBR1[] = { UMINUS, '1', '6', '4', '4', EXP, MINUS, '2', PLUS, LBRACKET, COS, \
                                LBRACKET, '1', '5', PLUS, '1', RBRACKET, MULTIPLY, '7', RBRACKET, END };
char OCBR2[] = { LBRACKET, '1', '0', POINT, '1', '2', PLUS, '1', '9', '8', RBRACKET, MULTIPLY, \
                                                LBRACKET, UMINUS, '6', MINUS, '1', '0', RBRACKET, END };
char OCBR3[] = { LBRACKET, '2', EXTENT, '4', RBRACKET, EXTENT, '2', END };
char OCBR4[] = { '4', DIVIDE, '5', EXTENT, '6', PLUS, LBRACKET, '1', '0', MOD, '3', RBRACKET, END };
char OCBR5[] = { LBRACKET, '2', EXTENT, LBRACKET, '1', PLUS, '3', RBRACKET, RBRACKET, MINUS, '1', '0', END };
char OCBR6[] = { LBRACKET, LBRACKET, LBRACKET, '1', '0', PLUS, '5', RBRACKET, DIVIDE, '3', RBRACKET, \
                                                                MULTIPLY, '8', RBRACKET, PLUS, '5', END };
char OCBR7[] = { LBRACKET, '1', PLUS, LBRACKET, '3', MINUS, LBRACKET, UMINUS, '6', MULTIPLY, '2', DIVIDE, \
                                    LBRACKET, '1', PLUS, '1', RBRACKET, RBRACKET, RBRACKET, RBRACKET, END };
char OCBR8[] = { '1', '2', POINT, '9', '7', '8', '9', '1', EXP, PLUS, '1', MULTIPLY, \
                                                            LBRACKET, '0', RBRACKET, END };
char OCBR9[] = { '1', PLUS, LBRACKET, LBRACKET, LBRACKET, '1', '9', RBRACKET, RBRACKET, RBRACKET, END };
char OCBR10[] = { '1', '9', PLUS, LBRACKET, LBRACKET, '1', '0', MOD, '6', RBRACKET, PLUS, LBRACKET, UMINUS, \
                                                                        '5', RBRACKET, RBRACKET, END };

double OCBRRES[] = { -9.71116813, -3329.92, 256, 1.000256, 6, 45, 10, 0, 20, 18 };

char FC1[] = { COS, LBRACKET, '1', '5', RBRACKET, PLUS, '1', END };
char FC2[] = { SIN, LBRACKET, '1', '5', RBRACKET, PLUS, '1', END };
char FC3[] = { TAN, LBRACKET, '1', '5', RBRACKET, PLUS, '1', END };
char FC4[] = { ACOS, LBRACKET, UMINUS, '1', RBRACKET, PLUS, '1', END };
char FC5[] = { ASIN, LBRACKET, UMINUS, '1', RBRACKET, PLUS, '1', END };
char FC6[] = { ATAN, LBRACKET, '1', '5', RBRACKET, PLUS, '1', END };
char FC7[] = { SQRT, LBRACKET, '2', '9', '0', '0', '2', '0', '9', RBRACKET, END };
char FC8[] = { LN, LBRACKET, '1', '5', RBRACKET, PLUS, '1', END };
char FC9[] = { LOG, LBRACKET, '1', '5', RBRACKET, PLUS, '1', END };
char FC10[] = { TAN, LBRACKET, '8', '9', POINT, '9', '9', RBRACKET, PLUS, '1', END };

double FCRES[] = { 1.96592583, 1.25881905, 1.267949192, 181, -89, 87.18592517, 1703, 3.70805020, \
                                                                        2.17609126, 5730.57789312 };

char RC1[] = { COS, LBRACKET, VARX, RBRACKET, END };
char RC2[] = { COS, LBRACKET, SIN, LBRACKET, VARX, RBRACKET, RBRACKET, END };
char RC3[] = { TAN, LBRACKET, VARX, RBRACKET, END };
char RC4[] = { UMINUS, '1', POINT, '6', '4', '4', EXP, '1', PLUS, LBRACKET, COS, LBRACKET, '1', '5', \
                                                    PLUS, VARX, RBRACKET, MULTIPLY, '7', RBRACKET, END };
char RC5[] = { '1', PLUS, VARX, PLUS, '3', PLUS, VARX, MULTIPLY, '5', MULTIPLY, '6', EXTENT, '7', END };

void pick_next_token_test(struct stack_test **picked, double *value, char *priority, char *operand);
void split_initial_expression_into_stack(char *initial_expression, struct stack_test **expression);
void pop_token_test(struct stack_test **expression, double *value, char *priority, char *operand);
void push_token_Forward(struct stack_test **expression, double value, int priority, int operand);
void end_time_counter_point(exec_time_t MY_TIMER, char *notation);
void init_operators_calculations_vals_BRACKETS(char **oc_vals);
void clear_expression_showed(struct stack_test **expression);
void handle_unary_minus_dev(struct stack_test **expression);
void init_operators_calculations_vals(char **oc_vals);
void init_functions_calculations_vals(char **oc_vals);
void start_time_counter_point(exec_time_t *MY_TIMER);
void print_stack(struct stack_test **expression);
void run_test(char **oc_vals, double *OCRES);
Suite *s21_Polish_calc_test(void);

#endif  // SRC_S21_CALC_V1_0_TEST_H_
