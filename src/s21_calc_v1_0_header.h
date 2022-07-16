#ifndef SRC_S21_CALC_V1_0_HEADER_H_
#define SRC_S21_CALC_V1_0_HEADER_H_

// includes
#include <sys/timeb.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// compare priorities mode definition
#define LT 1
#define LE -1

// constant values
#define E 2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382
#define M_PI 3.14159265358979323846264338327950288

// threads to calculate graph values
#define MAX_THREADS 12

// graph points limit to use more than one thread
#define TR_LIMIT 200000

// max graph range
#define MAX_RANGE 2000001

// credit calculation mode
#define ANNUITY 1
#define DIFFERENTIATED 2

// dates comparison
typedef enum {
    DATE_EQ,
    DATE_GT,  // means d1 > d2
    DATE_LT,  // means d1 < d2
    FULL_CMP,  // just compare dates
    MONTH_CMP,  // compare by month
    DAY_CMP  // compare by days
} date_cmp_e;

// initialization of defauld dates
typedef enum {
    EOY,  // End of year
    QUARTER_1,
    QUARTER_2,
    QUARTER_3,
    QUARTER_4
} default_dates_e;

// Deposit calculator defines
typedef enum {
    REPLENISH,  // act_type
    WITHDRAW,  // act_type
    MONTHLY,  // payments_frequency
    QUARTERLY,  // payments_frequency
    ANNUALY,  // payments_frequency
    INTEREST_CAPITALIZATION,  // interest_capitalization flag
    AT_ONCE,  // periodicity
    EVERY_MONTH,  // periodicity
    EVERY_YEAR,  // periodicity
    NONE_OF  // no value or as false flag
} deposit_swhitchers_e;

// operators and functions codes
typedef enum {
    // priority 0
    VARX = 10,
    NUM = 11,
    // priority 1
    PLUS = '+',
    MINUS = '-',
    // priority 2
    MULTIPLY = 14,
    DIVIDE = 15,
    MOD = 16,
    // priority 3
    EXTENT = 17,
    // priority 4
    COS = 18,
    SIN = 19,
    TAN = 20,
    ACOS = 21,
    ASIN = 22,
    ATAN = 23,
    SQRT = 24,
    LN = 25,
    LOG = 26,
    // priority 5
    LBRACKET = 27,
    RBRACKET = 28,
    // others
    UMINUS = 29,
    POINT = '.',
    END = 31,
    PI = 32,
    FACTORIAL = 33,
    EXP = 'e'
} lexems;

// stack cell
typedef struct stack {
    double value;
    char priority;
    lexems operand;
    struct stack *next;
} stack;

// priorities
typedef enum {
    PRIOR_0,  // NUM, VARX
    PRIOR_1,  // PLUS, MINUS
    PRIOR_2,  // MULTIPLY, DIVIDE, MOD
    PRIOR_3,  // EXTENT
    PRIOR_4,  // FUNCTIONS
    PRIOR_5  // LBRACKET, RBRACKET
} priorities;

// graph struct
typedef struct {
    double dx;  // x step
    double *vals_x;
    double *vals_y;
    int ppu;  // points rer unit
    int points;  // points for whole range
    char *expression;
    double from;
    double to;
} graph_t;

// date saver structure
typedef struct {
    int YY;
    int MM;
    int DD;
} date_t;

// Deposit replenishments and withdrawals data
typedef struct deposit_act_t {
    int act_type;
    double amount;
    int periodicity;
    int dd;
    int mm;
    int yy;
    struct deposit_act_t *next;
} deposit_act_t;

// Deposit calculator data struct
typedef struct {
    double deposit_total;
    int deposit_term;
    double interest_rate;
    double tax_rate;
    int payments_frequency;
    int interest_capitalization;
    deposit_act_t *act_list;
} deposit_t;

// Calculator mode functions
void push_token_Backward(struct stack **expression, double value, char priority, char operand);
void staks_calculation(struct stack **polish, struct stack **support, char priority, char cmp);
void staks_calculation_RBRACKET(struct stack **polish, struct stack **support);
void pop_token(struct stack **expression, double *value, char *priority, char *operand);
void calculate_expression_result(char *initial_expression, double x, double *result);
void calculate_function(struct stack **polish, char operand);
void calculate_numbers(struct stack **polish, char operand);
double make_number(char *number, int *iter);
char compare_int(char a, char b);
double factorial(double val);

// threads function
void calculate_thread_range(char *expression, double from, double to, graph_t **graph);
void calculate_range(char *expression, double from, double to, graph_t **graph);
void run_threads(int threads, graph_t **graph);
void *thread_function(void *thread_data);

// Credit mode function
void calculate_credit(double total_in, double term_in, double rate_in,
                                double *mon_pay_high, double *mon_pay_low,
                                    double *overpayment, double *total_payment, int mode);

// Deposit mode function
void calculate_deposit(deposit_t **deposit_data, deposit_act_t **act_list, date_t start,
                            double *res_interest, double *res_tax, double *res_end_deposit);
void periodic_payment(date_t start_date, date_t current_date, double interest_rate,
                                                double tax_rate, int payments_frequency,
                                                        double *tax_summ, double *deposit);
void extra_action(date_t current_date, deposit_act_t **act_list, double *deposit,
                                                        double *tax_summ, double tax);
void initialize_default_date(date_t *date, date_t bearing, int type);
int compare_date(date_t d1, date_t d2, int mode);
void add_to_date(date_t *to, int yy, int mm, int dd);
void assign_date(date_t start, date_t *end);
void push_deposit_action(struct deposit_act_t **action);
void pick_next_deposit_action(struct deposit_act_t **action);
void clear_deposit_action(struct deposit_act_t **action);

#endif  // SRC_S21_CALC_V1_0_HEADER_H_
