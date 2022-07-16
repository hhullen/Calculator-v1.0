#include "s21_calc_v1_0_header.h"

// Calculator mode
void calculate_range(char *expression, double from, double to, graph_t **graph) {
    int threads = 0, range = 0;

    (*graph)->expression = expression;
    (*graph)->from = from;
    (*graph)->to = to;
    range = (*graph)->to - (*graph)->from;
    (*graph)->ppu = (int)pow(log(MAX_RANGE) - log(range), 2) + 1;
    (*graph)->points = (*graph)->ppu * range;
    (*graph)->dx = 1.0 / (*graph)->ppu;
    threads = (*graph)->points / TR_LIMIT;
    if (threads < 2) {
        threads = 1;
    } else if (threads > MAX_THREADS) {
        threads = MAX_THREADS;
    }
    if ((*graph)->vals_x) free((*graph)->vals_x);
    if ((*graph)->vals_y) free((*graph)->vals_y);
    (*graph)->vals_x = (double *)malloc((*graph)->points * sizeof(double));
    (*graph)->vals_y = (double *)malloc((*graph)->points * sizeof(double));
    if ((*graph)->vals_x && (*graph)->vals_y) {
        printf("Run %d threads\n", threads);
        printf("points/unit %d, points: %d\n", (*graph)->ppu, (*graph)->points);
        run_threads(threads, graph);
    } else {
        printf("Memory allocation FAILURE\n");
    }
}

void run_threads(int threads, graph_t **graph) {
    pthread_t *thread_id = (pthread_t *) malloc(threads * sizeof(pthread_t));
    graph_t *thread_data = (graph_t *) malloc(threads * sizeof(graph_t));
    int thread_range = (*graph)->points / threads;

    for (int i = 0; i < threads; i++) {
        thread_data[i].from = thread_range * (i) + (*graph)->from;
        thread_data[i].to = thread_data[i].from + thread_range;
        thread_data[i].dx = (*graph)->dx;
        thread_data[i].expression = (*graph)->expression;
        thread_data[i].vals_x = &(*graph)->vals_x[i * thread_range];
        thread_data[i].vals_y = &(*graph)->vals_y[i * thread_range];
        thread_data[i].points = thread_range;

        printf("Calculate range from %lf to %lf\n", thread_data[i].from, thread_data[i].to);
        pthread_create(&(thread_id[i]), NULL, thread_function, &thread_data[i]);
    }
    for (int i = 0; i < threads; i++) pthread_join(thread_id[i], NULL);

    free(thread_id);
    free(thread_data);
    thread_id = NULL;
    thread_data = NULL;
}

void *thread_function(void *thread_data) {
    graph_t *data = (graph_t *)thread_data;
    double val_y = 0.0;

    for (int i = 0; data->from < data->to && i < data->points; data->from += data->dx, i++) {
        calculate_expression_result(data->expression, data->from, &val_y);
        data->vals_x[i] = data->from;
        data->vals_y[i] = val_y;
    }
    pthread_exit(0);
}

void calculate_expression_result(char *initial_expression, double x, double *result) {
    struct stack *support_stack = NULL, *polish_notation = NULL;
    char operand = 0, priority = 0, *number_start = NULL;
    double value = 0.0;
    int iter = 0;

    for (; initial_expression[iter] != END; iter += 1) {
        if (initial_expression[iter] == PI) {  // pi number
            push_token_Backward(&polish_notation, M_PI, PRIOR_0, NUM);
        } else if (initial_expression[iter] == EXP) {
            push_token_Backward(&polish_notation, E, PRIOR_0, NUM);
        } else if (initial_expression[iter] >= '0' && initial_expression[iter] <= '9') {  // numbers
            number_start = &initial_expression[iter];
            push_token_Backward(&polish_notation, make_number(number_start, &iter), PRIOR_0, NUM);
        } else if (initial_expression[iter] == VARX) {  // "x"
            push_token_Backward(&polish_notation, x, PRIOR_0, VARX);
        } else if (initial_expression[iter] == MINUS || initial_expression[iter] == PLUS) {  // "+" or "-"
            staks_calculation(&polish_notation, &support_stack, PRIOR_1, LE);
            push_token_Backward(&support_stack, 0.0, PRIOR_1, initial_expression[iter]);
        } else if (initial_expression[iter] <= MOD) {  // "*", "/" and "%"
            staks_calculation(&polish_notation, &support_stack, PRIOR_2, LE);
            push_token_Backward(&support_stack, 0.0, PRIOR_2, initial_expression[iter]);
        } else if (initial_expression[iter] == EXTENT) {  // "^"
            staks_calculation(&polish_notation, &support_stack, PRIOR_3, LT);
            push_token_Backward(&support_stack, 0.0, PRIOR_3, initial_expression[iter]);
        } else if (initial_expression[iter] <= LOG) {  // functions
            staks_calculation(&polish_notation, &support_stack, PRIOR_4, LE);
            push_token_Backward(&support_stack, 0.0, PRIOR_4, initial_expression[iter]);
        } else if (initial_expression[iter] == FACTORIAL) {
            pop_token(&polish_notation, &value, &priority, &operand);
            push_token_Backward(&polish_notation, factorial(value), PRIOR_0, NUM);
        } else if (initial_expression[iter] == LBRACKET) {  // left bracket
            push_token_Backward(&support_stack, 0.0, PRIOR_5, LBRACKET);
        } else if (initial_expression[iter] == RBRACKET) {  // right bracket
            staks_calculation_RBRACKET(&polish_notation, &support_stack);
        } else if (initial_expression[iter] == UMINUS) {  // unary minus
            push_token_Backward(&polish_notation, -1, PRIOR_0, NUM);
            staks_calculation(&polish_notation, &support_stack, PRIOR_2, LE);
            push_token_Backward(&support_stack, 0.0, PRIOR_2, MULTIPLY);
        }
    }
    while (support_stack) {
        pop_token(&support_stack, &value, &priority, &operand);
        if (operand == PLUS || operand == MINUS || operand <= EXTENT) {
            calculate_numbers(&polish_notation, operand);
        } else if (operand <= LOG) {
            calculate_function(&polish_notation, operand);
        }
    }
    if (polish_notation) {
        *result = polish_notation->value;
        while (polish_notation) pop_token(&polish_notation, NULL, NULL, NULL);
    }
}

void staks_calculation(struct stack **polish, struct stack **support, char priority, char cmp) {
    char picked_operand = 0, picked_priority = 0;

    if (*support) {
        picked_priority = (*support)->priority;
        picked_operand = (*support)->operand;
        while (compare_int(picked_priority, priority) > cmp && *support && picked_operand != LBRACKET) {
            pop_token(support, NULL, NULL, NULL);
            if (picked_operand == PLUS || picked_operand == MINUS || picked_operand <= EXTENT) {
                calculate_numbers(polish, picked_operand);
            } else if (picked_operand <= LOG) {
                calculate_function(polish, picked_operand);
            }
            if (*support) {
                picked_priority = (*support)->priority;
                picked_operand = (*support)->operand;
            }
        }
    }
}

void staks_calculation_RBRACKET(struct stack **polish, struct stack **support) {
    char picked_operand = 0, picked_priority = 0;

    if (*support) {
        picked_priority = (*support)->priority;
        picked_operand = (*support)->operand;
        while (picked_operand != LBRACKET) {
            pop_token(support, NULL, NULL, NULL);
            if (picked_operand == PLUS || picked_operand == MINUS || picked_operand <= EXTENT) {
                calculate_numbers(polish, picked_operand);
            } else if (picked_operand <= LOG) {
                calculate_function(polish, picked_operand);
            }
            if (*support) {
                picked_priority = (*support)->priority;
                picked_operand = (*support)->operand;
            }
        }
    }
    pop_token(support, NULL, NULL, NULL);  // remove left bracket
}

void calculate_numbers(struct stack **polish, char operand) {
    double num1 = 0, num2 = 0, result = 0.0;

    pop_token(polish, &num2, NULL, NULL);
    pop_token(polish, &num1, NULL, NULL);
    if (operand == PLUS) {
        result = num1 + num2;
    } else if (operand == MINUS) {
        result = num1 - num2;
    } else if (operand == MULTIPLY) {
        result = num1 * num2;
    } else if (operand == DIVIDE) {
        result = num1 / num2;
    } else if (operand == MOD) {
        result = fmod(num1, num2);
    } else if (operand == EXTENT) {
        result = pow(num1, num2);
    }
    push_token_Backward(polish, result, PRIOR_0, NUM);
}

void calculate_function(struct stack **polish, char operand) {
    double num = 0, result = 0.0;

    pop_token(polish, &num, NULL, NULL);
    if (operand == COS) {
        result = cos(num * M_PI / 180);
    } else if (operand == SIN) {
        result = sin(num * M_PI / 180);
    } else if (operand == TAN) {
        result = tan(num * M_PI / 180);
    } else if (operand == ACOS) {
        result = acos(num) * 180 / M_PI;
    } else if (operand == ASIN) {
        result = asin(num) * 180 / M_PI;
    } else if (operand == ATAN) {
        result = atan(num) * 180 / M_PI;
    } else if (operand == SQRT) {
        result = sqrt(num);
    } else if (operand == LN) {
        result = log(num);
    } else if (operand == LOG) {
        result = log10(num);
    }
    push_token_Backward(polish, result, PRIOR_0, NUM);
}

char compare_int(char a, char b) {
    char res = 0;

    if (a > b)
        res = 1;
    else if (a == b)
        res = 0;
    else
        res = -1;

    return res;
}

double make_number(char *number, int *iter) {
    double double_number = 0.0;

    for (int i = 0; ((number[i] >= '0' && number[i] <= '9') || number[i] == POINT || number[i] == EXP) \
                                                                                    && i < 40; i += 1) {
        if (number[i] == EXP && (number[i + 1] == '+' || number[i + 1] == '-')) {
           *iter += 1;
           i += 1;
        }
        *iter += 1;
    }
    *iter -= 1;
    double_number = atof(number);

    return double_number;
}

double factorial(double val) {
    double returnable = 0.0, frac = 0.0;
    unsigned long int ceil = 0, ceil_fctrl = 1;

    if (val >= 0) {
        ceil = (int)val;
        frac = val - ceil;
        for (unsigned long int i = 1; i <= ceil; i++) ceil_fctrl *= i;
        returnable = pow(10, (log10(ceil_fctrl) + frac * log10(ceil + 1)));
    } else {
        returnable = NAN;
    }

    return returnable;
}

void push_token_Backward(struct stack **expression, double value, char priority, char operand) {
    stack *buff_token = NULL;

    buff_token = (struct stack *)malloc(sizeof(struct stack));
    buff_token->next = (*expression);
    (*expression) = buff_token;

    (*expression)->operand = operand;
    (*expression)->priority = priority;
    if (operand > NUM) {
        (*expression)->value = 0.0;
    } else {
        (*expression)->value = value;
    }
}

void pop_token(struct stack **expression, double *value, char *priority, char *operand) {
    stack *buff_token = NULL;

    if (value) *value = (*expression)->value;
    if (priority) *priority = (*expression)->priority;
    if (operand) *operand = (*expression)->operand;

    buff_token = (*expression)->next;
    free((*expression));
    (*expression) = buff_token;
}

// Credit mode
void calculate_credit(double total_in, double term_in, double rate_in,
                                double *mon_pay_high, double *mon_pay_low,
                                    double *overpayment, double *total_payment, int mode) {
    double main_debt = 0.0;

    rate_in /= 100;
    if (mode == ANNUITY) {
        rate_in /= 12;
        *mon_pay_high = total_in * (rate_in + (rate_in / (pow(1 + rate_in, term_in) - 1)));
        *total_payment = *mon_pay_high * term_in;
    } else if (mode == DIFFERENTIATED) {
        main_debt = total_in / term_in;
        *total_payment = 0;

        *mon_pay_high = main_debt + total_in * rate_in / 12;
        *mon_pay_low = main_debt + (total_in - main_debt * (term_in - 1)) * rate_in / 12;
        for (int i = 0; i < term_in; i++) {
            *total_payment += main_debt + (total_in - main_debt * i) * rate_in / 12;
        }
    }
    *overpayment = *total_payment - total_in;
}

// Deposit mode
void calculate_deposit(deposit_t **deposit_data, deposit_act_t **act_list, date_t start,
                            double *res_interest, double *res_tax, double *res_end_deposit) {
    double current_deposit = (*deposit_data)->deposit_total, one_end_tax = 0.0;
    date_t end_date, current_date;

    assign_date(start, &end_date);
    assign_date(start, &current_date);
    add_to_date(&end_date, 0, (*deposit_data)->deposit_term, 0);
    while (compare_date(current_date, end_date, FULL_CMP) == DATE_LT) {
        if ((*deposit_data)->interest_capitalization == INTEREST_CAPITALIZATION) {
            periodic_payment(start, current_date, (*deposit_data)->interest_rate,
                                (*deposit_data)->tax_rate, (*deposit_data)->payments_frequency,
                                                                        res_tax, &current_deposit);
        }
        if ((*act_list)) {
            extra_action(current_date, act_list, &current_deposit, res_tax, (*deposit_data)->tax_rate);
        }
        add_to_date(&current_date, 0, 0, 1);
    }
    current_deposit += current_deposit * (*deposit_data)->interest_rate *
                                            (*deposit_data)->deposit_term / 12;
    one_end_tax = (current_deposit - (*deposit_data)->deposit_total) * (*deposit_data)->tax_rate;
    *res_end_deposit = current_deposit  - one_end_tax;
    *res_tax += one_end_tax;
    *res_interest = *res_end_deposit / (*deposit_data)->deposit_total - 1;
}

void extra_action(date_t current_date, deposit_act_t **act_list, double *deposit,
                                                        double *tax_summ, double tax) {
    deposit_act_t *cell = (*act_list);
    date_t temp;

    while (cell) {
        temp.YY = cell->yy;
        temp.MM = cell->mm;
        temp.DD = cell->dd;
        if (cell->act_type == REPLENISH) {
            if (cell->periodicity == AT_ONCE) {
                if (compare_date(current_date, temp, FULL_CMP) == DATE_EQ) {
                    *deposit += cell->amount;
                }
            } else if (cell->periodicity == EVERY_MONTH) {
                if (compare_date(current_date, temp, DAY_CMP) == DATE_EQ) {
                    *deposit += cell->amount;
                }
            } else if (cell->periodicity == EVERY_YEAR) {
                if (compare_date(current_date, temp, MONTH_CMP) == DATE_EQ) {
                    *deposit += cell->amount;
                }
            }
        } else if (cell->act_type == WITHDRAW) {
            if (compare_date(current_date, temp, FULL_CMP) == DATE_EQ) {
                *deposit -= cell->amount;
                *tax_summ += cell->amount * tax;
                if (*deposit < 0) *deposit = 0;
            }
        }
        pick_next_deposit_action(&cell);
    }
}

void periodic_payment(date_t start_date, date_t current_date, double interest_rate,
                                                double tax_rate, int payments_frequency,
                                                        double *tax_summ, double *deposit) {
    if (payments_frequency == MONTHLY &&
            compare_date(start_date, current_date, DAY_CMP) == DATE_EQ) {
        *deposit += (*deposit * interest_rate / 12) * (1 - tax_rate);
        *tax_summ += *deposit * interest_rate * tax_rate;
    } else if (payments_frequency == ANNUALY) {
        date_t end_of_year;
        initialize_default_date(&end_of_year, current_date, EOY);
        if (compare_date(end_of_year, current_date, MONTH_CMP) == DATE_EQ) {
            *deposit += (*deposit * interest_rate) * (1 - tax_rate);
            *tax_summ += *deposit * interest_rate * tax_rate;
        }
    } else if (payments_frequency == QUARTERLY) {
        date_t quart1, quart2, quart3, quart4;
        initialize_default_date(&quart1, current_date, QUARTER_1);
        initialize_default_date(&quart2, current_date, QUARTER_2);
        initialize_default_date(&quart3, current_date, QUARTER_3);
        initialize_default_date(&quart4, current_date, QUARTER_4);
        if (compare_date(quart1, current_date, MONTH_CMP) == DATE_EQ ||
            compare_date(quart2, current_date, MONTH_CMP) == DATE_EQ ||
            compare_date(quart3, current_date, MONTH_CMP) == DATE_EQ ||
            compare_date(quart4, current_date, MONTH_CMP) == DATE_EQ) {
            *deposit += (*deposit * interest_rate / 4) * (1 - tax_rate);
            *tax_summ += *deposit * interest_rate * tax_rate;
        }
    }
}

void initialize_default_date(date_t *date, date_t bearing, int type) {
    date->YY = bearing.YY;
    if (type == EOY) {
        date->DD = 20;
        date->MM = 12;
    } else if (type == QUARTER_1) {
        date->DD = 1;
        date->MM = 3;
    } else if (type == QUARTER_2) {
        date->DD = 1;
        date->MM = 6;
    } else if (type == QUARTER_3) {
        date->DD = 1;
        date->MM = 9;
    } else if (type == QUARTER_4) {
        date->DD = 1;
        date->MM = 12;
    }
}

int compare_date(date_t d1, date_t d2, int mode) {
    int returnable = DATE_EQ;

    if (d1.YY > d2.YY && mode == FULL_CMP)
        returnable = DATE_GT;
    else if (d1.YY < d2.YY && mode == FULL_CMP)
        returnable = DATE_LT;
    else if (d1.MM > d2.MM && (mode == FULL_CMP || mode == MONTH_CMP))
        returnable = DATE_GT;
    else if (d1.MM < d2.MM && (mode == FULL_CMP || mode == MONTH_CMP))
        returnable = DATE_LT;
    else if (d1.DD > d2.DD && (mode == FULL_CMP || mode == MONTH_CMP || mode == DAY_CMP))
        returnable = DATE_GT;
    else if (d1.DD < d2.DD && (mode == FULL_CMP || mode == MONTH_CMP || mode == DAY_CMP))
        returnable = DATE_LT;

    return returnable;
}

void add_to_date(date_t *to, int yy, int mm, int dd) {
    int monthDays_leapY[] = { 31, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int monthDays_commonY[] = { 31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int *monthDays = &monthDays_leapY[0];

    to->YY += yy;
    to->MM += mm;
    to->DD += dd;
    if (to->YY % 400 == 0) {
        monthDays = &monthDays_leapY[0];
    }
    while (to->DD > monthDays[to->MM % 12]) {
        to->DD -= monthDays[to->MM % 12];
        to->MM += 1;
    }
    if (to->DD == 0) to->DD = 1;
    while (to->MM > 12) {
        to->MM -= 12;
        to->YY += 1;
    }
    if (to->MM == 0) to->MM = 1;
}

void assign_date(date_t from, date_t *to) {
    to->YY = from.YY;
    to->MM = from.MM;
    to->DD = from.DD;
}

void push_deposit_action(struct deposit_act_t **action) {
    deposit_act_t *buff_token = NULL;

    buff_token = (struct deposit_act_t *)malloc(sizeof(struct deposit_act_t));
    buff_token->next = (*action);
    (*action) = buff_token;
}

void pick_next_deposit_action(struct deposit_act_t **action) {
    (*action) = (*action)->next;
}

void clear_deposit_action(struct deposit_act_t **action) {
    deposit_act_t *buff_token = NULL;

    while ((*action)) {
        buff_token = (*action)->next;
        free((*action));
        (*action) = buff_token;
    }
}
