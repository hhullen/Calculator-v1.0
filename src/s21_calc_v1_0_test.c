#include "s21_calc_v1_0_test.h"

START_TEST(s21_operators_calculations) {
    char **oc_vals = (char **) calloc(10, sizeof(char *));

    init_operators_calculations_vals(oc_vals);
    run_test(oc_vals, OCRES);
    free(oc_vals);
} END_TEST

START_TEST(s21_operators_calculations_BRACKETS) {
    char **oc_vals = (char **) calloc(10, sizeof(char *));

    init_operators_calculations_vals_BRACKETS(oc_vals);
    run_test(oc_vals, OCBRRES);
    free(oc_vals);
} END_TEST

START_TEST(s21_functions_calculations) {
    char **oc_vals = (char **) calloc(10, sizeof(char *));

    init_functions_calculations_vals(oc_vals);
    run_test(oc_vals, FCRES);
    free(oc_vals);
} END_TEST

START_TEST(s21_range_calculations_01) {
    graph_t *graph = (graph_t *)calloc(1, sizeof(graph_t));
    struct stack_test *expression = NULL;

    split_initial_expression_into_stack(RC1, &expression);
    printf("INITIAL EXPRESSION:\n");
    print_stack(&expression);

    calculate_range(RC1, 0, 100, &graph);
    printf("\n");

    clear_expression_showed(&expression);
    free(graph->vals_x);
    free(graph->vals_y);
    free(graph);
    graph = NULL;
} END_TEST

START_TEST(s21_range_calculations_02) {
    graph_t *graph = (graph_t *)calloc(1, sizeof(graph_t));
    struct stack_test *expression = NULL;

    split_initial_expression_into_stack(RC2, &expression);
    printf("INITIAL EXPRESSION:\n");
    print_stack(&expression);

    calculate_range(RC2, -200701, 200080, &graph);
    printf("\n");

    clear_expression_showed(&expression);
    free(graph->vals_x);
    free(graph->vals_y);
    free(graph);
    graph = NULL;
} END_TEST

START_TEST(s21_range_calculations_03) {
    graph_t *graph = (graph_t *)calloc(1, sizeof(graph_t));
    struct stack_test *expression = NULL;

    split_initial_expression_into_stack(RC3, &expression);
    printf("INITIAL EXPRESSION:\n");
    print_stack(&expression);

    calculate_range(RC3, -440670, 500000, &graph);
    printf("\n");

    clear_expression_showed(&expression);
    free(graph->vals_x);
    free(graph->vals_y);
    free(graph);
    graph = NULL;
} END_TEST

START_TEST(s21_range_calculations_04) {
    graph_t *graph = (graph_t *)calloc(1, sizeof(graph_t));
    struct stack_test *expression = NULL;

    split_initial_expression_into_stack(RC4, &expression);
    printf("INITIAL EXPRESSION:\n");
    print_stack(&expression);

    calculate_range(RC4, -1000000, 1000000, &graph);
    printf("\n");

    clear_expression_showed(&expression);
    free(graph->vals_x);
    free(graph->vals_y);
    free(graph);
    graph = NULL;
} END_TEST

START_TEST(s21_range_calculations_05) {
    graph_t *graph = (graph_t *)calloc(1, sizeof(graph_t));
    struct stack_test *expression = NULL;

    split_initial_expression_into_stack(RC5, &expression);
    printf("INITIAL EXPRESSION:\n");
    print_stack(&expression);

    calculate_range(RC5, -1, 0, &graph);
    printf("\n");

    clear_expression_showed(&expression);
    free(graph->vals_x);
    free(graph->vals_y);
    free(graph);
    graph = NULL;
} END_TEST

START_TEST(s21_credit_mode_01) {
    double mon_pay_high = 0.0, mon_pay_low = 0.0, overpayment = 0.0, total_payment = 0.0;
    calculate_credit(120000.0, 24, 13.9, &mon_pay_high, &mon_pay_low,
                                    &overpayment, &total_payment, ANNUITY);
    ck_assert_msg(abs(mon_pay_high - 5755.88) <= MONEY_ACCURACY, "%lf", mon_pay_high - 5755.88);
    ck_assert_msg(abs(overpayment - 18141.12) <= MONEY_ACCURACY, "%lf", overpayment - 18141.12);
    ck_assert_msg(abs(total_payment - 138141.12) <= MONEY_ACCURACY, "%lf", total_payment - 138141.12);
} END_TEST

START_TEST(s21_credit_mode_02) {
    double mon_pay_high = 0.0, mon_pay_low = 0.0, overpayment = 0.0, total_payment = 0.0;
    calculate_credit(120000.0, 24, 13.9, &mon_pay_high, &mon_pay_low,
                                    &overpayment, &total_payment, DIFFERENTIATED);
    ck_assert_msg(abs(mon_pay_high - 6390.00) <= MONEY_ACCURACY, "%lf", mon_pay_high - 6390.00);
    ck_assert_msg(abs(mon_pay_low - 5057.92) <= MONEY_ACCURACY, "%lf", mon_pay_low - 5057.92);
    ck_assert_msg(abs(overpayment - 17375.00) <= MONEY_ACCURACY, "%lf", overpayment - 17375.00);
    ck_assert_msg(abs(total_payment - 137375.00) <= MONEY_ACCURACY, "%lf", total_payment - 137375.00);
} END_TEST

START_TEST(s21_deposit_mode_01) {
    deposit_t *data = (deposit_t *)malloc(sizeof(deposit_t));
    double interest = 0.0, tax = 0.0, deposit = 0.0;
    deposit_act_t *list = NULL;
    date_t date;

    data->deposit_total = 1000.0;
    data->deposit_term = 12;
    data->interest_rate = 0.05;
    data->tax_rate = 0.13;
    data->interest_capitalization = INTEREST_CAPITALIZATION;
    data->payments_frequency = MONTHLY;
    data->act_list = list;
    push_deposit_action(&list);
    list->act_type = REPLENISH;
    list->amount = 100;
    list->periodicity = AT_ONCE;
    list->dd = 14;
    list->mm = 9;
    list->yy = 22;
    push_deposit_action(&list);
    list->act_type = WITHDRAW;
    list->amount = 1;
    list->periodicity = NONE_OF;
    list->dd = 1;
    list->mm = 9;
    list->yy = 22;
    date.DD = 14;
    date.MM = 7;
    date.YY = 22;
    calculate_deposit(&data, &list, date, &interest, &tax, &deposit);
    ck_assert_msg(abs(interest - 78.53 / 100) <= MONEY_ACCURACY,
                        "%lf, %lf", interest - 78.53 / 100, interest);
    ck_assert_msg(abs(tax - 228.66) <= MONEY_ACCURACY, "%lf, %lf", tax - 228.66, tax);
    ck_assert_msg(abs(deposit - 1785.33) <= MONEY_ACCURACY, "%lf, %lf", deposit - 1785.33, deposit);

    free(data);
    clear_deposit_action(&list);
} END_TEST

START_TEST(s21_deposit_mode_02) {
    deposit_t *data = (deposit_t *)malloc(sizeof(deposit_t));
    double interest = 0.0, tax = 0.0, deposit = 0.0;
    deposit_act_t *list = NULL;
    date_t date;

    data->deposit_total = 1000.0;
    data->deposit_term = 12;
    data->interest_rate = 0.05;
    data->tax_rate = 0.13;
    data->interest_capitalization = INTEREST_CAPITALIZATION;
    data->payments_frequency = QUARTERLY;
    data->act_list = list;
    push_deposit_action(&list);
    list->act_type = REPLENISH;
    list->amount = 100;
    list->periodicity = EVERY_MONTH;
    list->dd = 14;
    list->mm = 9;
    list->yy = 22;
    push_deposit_action(&list);
    list->act_type = WITHDRAW;
    list->amount = 1;
    list->periodicity = NONE_OF;
    list->dd = 1;
    list->mm = 9;
    list->yy = 22;
    date.DD = 14;
    date.MM = 7;
    date.YY = 22;
    calculate_deposit(&data, &list, date, &interest, &tax, &deposit);
    ck_assert_msg(abs(interest - 141.58 / 100) <= MONEY_ACCURACY,
                        "%lf, %lf", interest - 141.58 / 100, interest);
    ck_assert_msg(abs(tax - 258.98) <= MONEY_ACCURACY, "%lf, %lf", tax - 258.98, tax);
    ck_assert_msg(abs(deposit - 2415.84) <= MONEY_ACCURACY, "%lf, %lf", deposit - 2415.84, deposit);

    free(data);
    clear_deposit_action(&list);
} END_TEST

START_TEST(s21_deposit_mode_03) {
    deposit_t *data = (deposit_t *)malloc(sizeof(deposit_t));
    double interest = 0.0, tax = 0.0, deposit = 0.0;
    deposit_act_t *list = NULL;
    date_t date;

    data->deposit_total = 1000.0;
    data->deposit_term = 12;
    data->interest_rate = 0.05;
    data->tax_rate = 0.13;
    data->interest_capitalization = INTEREST_CAPITALIZATION;
    data->payments_frequency = QUARTERLY;
    data->act_list = list;
    push_deposit_action(&list);
    list->act_type = REPLENISH;
    list->amount = 100;
    list->periodicity = EVERY_YEAR;
    list->dd = 14;
    list->mm = 9;
    list->yy = 22;
    push_deposit_action(&list);
    list->act_type = WITHDRAW;
    list->amount = 1;
    list->periodicity = NONE_OF;
    list->dd = 1;
    list->mm = 9;
    list->yy = 22;
    date.DD = 14;
    date.MM = 7;
    date.YY = 22;
    calculate_deposit(&data, &list, date, &interest, &tax, &deposit);
    ck_assert_msg(abs(interest - 31.59 / 100) <= MONEY_ACCURACY,
                        "%lf, %lf", interest - 31.59 / 100, interest);
    ck_assert_msg(abs(tax - 78.39) <= MONEY_ACCURACY, "%lf, %lf", tax - 78.39, tax);
    ck_assert_msg(abs(deposit - 1315.88) <= MONEY_ACCURACY, "%lf, %lf", deposit - 1315.88, deposit);

    free(data);
    clear_deposit_action(&list);
} END_TEST

START_TEST(s21_deposit_mode_04) {
    deposit_t *data = (deposit_t *)malloc(sizeof(deposit_t));
    double interest = 0.0, tax = 0.0, deposit = 0.0;
    deposit_act_t *list = NULL;
    date_t date;

    data->deposit_total = 1000.0;
    data->deposit_term = 12;
    data->interest_rate = 0.05;
    data->tax_rate = 0.13;
    data->interest_capitalization = INTEREST_CAPITALIZATION;
    data->payments_frequency = ANNUALY;
    data->act_list = list;
    push_deposit_action(&list);
    list->act_type = REPLENISH;
    list->amount = 100;
    list->periodicity = EVERY_YEAR;
    list->dd = 14;
    list->mm = 9;
    list->yy = 22;
    push_deposit_action(&list);
    list->act_type = WITHDRAW;
    list->amount = 1;
    list->periodicity = NONE_OF;
    list->dd = 1;
    list->mm = 9;
    list->yy = 22;
    date.DD = 14;
    date.MM = 7;
    date.YY = 22;
    calculate_deposit(&data, &list, date, &interest, &tax, &deposit);
    ck_assert_msg(abs(interest - 17.76 / 100) <= MONEY_ACCURACY,
                        "%lf, %lf", interest - 17.76 / 100, interest);
    ck_assert_msg(abs(tax - 34.12) <= MONEY_ACCURACY, "%lf, %lf", tax - 34.12, tax);
    ck_assert_msg(abs(deposit - 1177.61) <= MONEY_ACCURACY,
                        "%lf, %lf", deposit - 1315.88, deposit);

    free(data);
    clear_deposit_action(&list);
} END_TEST

int main(void) {
    int amountOfFailed = 0;
    SRunner *runner;
    Suite *suite;

    suite = s21_Polish_calc_test();
    runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    amountOfFailed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (amountOfFailed == 0) ? 0 : 1;
}

Suite *s21_Polish_calc_test(void) {
    Suite *suite;
    TCase *getCase;

    suite = suite_create("Polish_calc_test");
    getCase = tcase_create("core");

    tcase_add_test(getCase, s21_operators_calculations);
    tcase_add_test(getCase, s21_operators_calculations_BRACKETS);
    tcase_add_test(getCase, s21_functions_calculations);
    tcase_add_test(getCase, s21_range_calculations_01);
    tcase_add_test(getCase, s21_range_calculations_02);
    tcase_add_test(getCase, s21_range_calculations_03);
    tcase_add_test(getCase, s21_range_calculations_04);
    tcase_add_test(getCase, s21_range_calculations_05);
    tcase_add_test(getCase, s21_credit_mode_01);
    tcase_add_test(getCase, s21_credit_mode_02);
    tcase_add_test(getCase, s21_deposit_mode_01);
    tcase_add_test(getCase, s21_deposit_mode_02);
    tcase_add_test(getCase, s21_deposit_mode_03);
    tcase_add_test(getCase, s21_deposit_mode_04);

    suite_add_tcase(suite, getCase);

    return suite;
}

void run_test(char **oc_vals, double *ocres) {
    struct stack_test *expression = NULL;
    double result = 0.0;

    for (int i = 0; i < 10; i++) {
        split_initial_expression_into_stack(oc_vals[i], &expression);
        printf("INITIAL EXPRESSION:\n");
        print_stack(&expression);
        while (expression) pop_token_test(&expression, NULL, NULL, NULL);

        calculate_expression_result(oc_vals[i], 0.0, &result);
        printf("RESULT: %.8lf; AWAITED: %.8lf; DIFF: %.8lf;\n\n",
                        result, ocres[i], fabs(result - ocres[i]));
        if (result != INFINITY) {
            ck_assert_int_eq((fabs(result - ocres[i]) < ACCURACUY), 1);
        } else {
            ck_assert_int_eq(result == INFINITY && ocres[i] == INFINITY, 1);
        }
    }
}

void init_operators_calculations_vals(char **oc_vals) {
    oc_vals[0] = OC1;
    oc_vals[1] = OC2;
    oc_vals[2] = OC3;
    oc_vals[3] = OC4;
    oc_vals[4] = OC5;
    oc_vals[5] = OC6;
    oc_vals[6] = OC7;
    oc_vals[7] = OC8;
    oc_vals[8] = OC9;
    oc_vals[9] = OC10;
}

void init_operators_calculations_vals_BRACKETS(char **oc_vals) {
    oc_vals[0] = OCBR1;
    oc_vals[1] = OCBR2;
    oc_vals[2] = OCBR3;
    oc_vals[3] = OCBR4;
    oc_vals[4] = OCBR5;
    oc_vals[5] = OCBR6;
    oc_vals[6] = OCBR7;
    oc_vals[7] = OCBR8;
    oc_vals[8] = OCBR9;
    oc_vals[9] = OCBR10;
}

void init_functions_calculations_vals(char **oc_vals) {
    oc_vals[0] = FC1;
    oc_vals[1] = FC2;
    oc_vals[2] = FC3;
    oc_vals[3] = FC4;
    oc_vals[4] = FC5;
    oc_vals[5] = FC6;
    oc_vals[6] = FC7;
    oc_vals[7] = FC8;
    oc_vals[8] = FC9;
    oc_vals[9] = FC10;
}

void split_initial_expression_into_stack(char *initial_expression, struct stack_test **expression) {
    char *number_start = NULL;
    double value = 0.0;
    int iter = 0;

    for (; initial_expression[iter] != END; iter += 1) {
        if (initial_expression[iter] == PI) {  // pi number
            push_token_Forward(expression, M_PI, PRIOR_0, NUM);
        } else if (initial_expression[iter] >= '0' && initial_expression[iter] <= '9') {  // numbers
            number_start = &initial_expression[iter];
            push_token_Forward(expression, make_number(number_start, &iter), PRIOR_0, NUM);
        } else if (initial_expression[iter] == VARX) {  // "x"
            push_token_Forward(expression, 0.0, PRIOR_0, VARX);
        } else if (initial_expression[iter] == MINUS || initial_expression[iter] == PLUS) {  // "+" or "-"
            push_token_Forward(expression, value, PRIOR_1, initial_expression[iter]);
        } else if (initial_expression[iter] <= MOD) {  // "*", "/" and "%"
            push_token_Forward(expression, value, PRIOR_2, initial_expression[iter]);
        } else if (initial_expression[iter] == EXTENT) {  // "^"
            push_token_Forward(expression, value, PRIOR_3, initial_expression[iter]);
        } else if (initial_expression[iter] <= LOG) {  // functions
            push_token_Forward(expression, value, PRIOR_4, initial_expression[iter]);
        } else if (initial_expression[iter] == LBRACKET) {  // left bracket
            push_token_Forward(expression, value, PRIOR_5, LBRACKET);
        } else if (initial_expression[iter] == RBRACKET) {  // right bracket
            push_token_Forward(expression, value, PRIOR_5, RBRACKET);
        } else if (initial_expression[iter] == UMINUS) {  // unary minus
            push_token_Forward(expression, -1, PRIOR_0, NUM);
            push_token_Forward(expression, 0.0, PRIOR_2, MULTIPLY);
        }
    }
}

void push_token_Forward(struct stack_test **expression, double value, int priority, int operand) {
    if (!*expression) {
        *expression = (struct stack_test *)malloc(sizeof(struct stack_test));
        (*expression)->next = NULL;
        (*expression)->last = (*expression);
        (*expression)->value = value;
        (*expression)->priority = priority;
        (*expression)->operand = operand;
    } else {
        (*expression)->last->next = (struct stack_test *)malloc(sizeof(struct stack_test));
        (*expression)->last = (*expression)->last->next;
        (*expression)->last->next = NULL;
        (*expression)->last->last = NULL;
        (*expression)->last->value = value;
        (*expression)->last->priority = priority;
        (*expression)->last->operand = operand;
    }
}

void print_stack(struct stack_test **expression) {
    double val = 0.0;
    char prior = 0, oper = 0;
    const char *oper_str[] = TOKENS;
    struct stack_test *picked = NULL;

    picked = (*expression);
    while (picked) {
        pick_next_token_test(&picked, &val, &prior, &oper);
        if (oper == NUM) {
            printf("%.3lf ", val);
        } else {
            if (oper == PLUS) {
                printf("%s ", oper_str[2]);
            } else if (oper == MINUS) {
                printf("%s ", oper_str[3]);
            } else {
                printf("%s ", oper_str[oper - 10]);
            }
        }
    }
    printf("\n");
}

void pop_token_test(struct stack_test **expression, double *value, char *priority, char *operand) {
    stack_test *buff_token = NULL;

    if (value) *value = (*expression)->value;
    if (priority) *priority = (*expression)->priority;
    if (operand) *operand = (*expression)->operand;

    buff_token = (*expression)->next;
    free((*expression));
    (*expression) = buff_token;
}

void pick_next_token_test(struct stack_test **picked, double *value, char *priority, char *operand) {
    if (value) *value = (*picked)->value;
    if (priority) *priority = (*picked)->priority;
    if (operand) *operand = (*picked)->operand;

    (*picked) = (*picked)->next;
}

void clear_expression_showed(struct stack_test **expression) {
    stack_test *buff_token = NULL;

    while ((*expression) != NULL) {
        buff_token = (*expression)->next;
        free((*expression));
        (*expression) = buff_token;
    }
}
