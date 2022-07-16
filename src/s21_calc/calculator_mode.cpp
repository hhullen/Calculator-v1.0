#include "calculator_mode.h"
#include "ui_calculator_mode.h"

Calculator_mode::Calculator_mode(QWidget *parent) : QWidget(parent), ui(new Ui::Calculator_mode) {
    ui->setupUi(this);
    calculator = ui->QW_Calculator;
    expression = new char[CHAR_ARRAY_MAX];
    line1_expr = new char[CHAR_ARRAY_MAX];
    line2_expr = new char[CHAR_ARRAY_MAX];
    clean_char_array(expression);
    clean_char_array(line1_expr);
    clean_char_array(line2_expr);
    graph_data1 = new graph_t();
    graph_data1->expression = NULL;
    graph_data1->vals_x = NULL;
    graph_data1->vals_y = NULL;
    graph_data2 = new graph_t();
    graph_data2->expression = NULL;
    graph_data2->vals_x = NULL;
    graph_data2->vals_y = NULL;
    clean_char_array(expression);
    set_up_regular_expressions();
    graph_view = new QCPlotic();

    // expression field setup
    ui->Field_expression->setValidator(new QDoubleValidator);
    ui->StaticX->setFocusPolicy(Qt::ClickFocus);
    ui->Field_expression->setFocus();
    ui->Field_expression->setMaxLength(512);

    // static X value field setup
    ui->StaticX->setValidator(new QDoubleValidator);
    ui->StaticX->setFocusPolicy(Qt::ClickFocus);
    ui->StaticX->setMaxLength(20);

    // Range values setup
    ui->Range1_low->setValidator(new QIntValidator);
    ui->Range1_low->setFocusPolicy(Qt::ClickFocus);
    ui->Range1_low->setMaxLength(8);

    ui->Range1_high->setValidator(new QIntValidator);
    ui->Range1_high->setFocusPolicy(Qt::ClickFocus);
    ui->Range1_high->setMaxLength(8);

    ui->Range2_low->setValidator(new QIntValidator);
    ui->Range2_low->setFocusPolicy(Qt::ClickFocus);
    ui->Range2_low->setMaxLength(8);

    ui->Range2_high->setValidator(new QIntValidator);
    ui->Range2_high->setFocusPolicy(Qt::ClickFocus);
    ui->Range2_high->setMaxLength(8);

    // calculator buttons connections
    connect(ui->NumsButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
                                        this, SLOT(numPad(QAbstractButton*)));
    connect(ui->OperatorsButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
                                        this, SLOT(operators(QAbstractButton*)));
    connect(ui->XPiButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
                                    this, SLOT(x_pi_btns(QAbstractButton*)));
    connect(ui->FuncButtonGroup,SIGNAL(buttonClicked(QAbstractButton*)),
                                    this, SLOT(functions(QAbstractButton*)));
    connect(ui->BTNclear, SIGNAL(clicked()), this, SLOT(clean_field()));
    connect(ui->BTNclear_X, SIGNAL(clicked()), this, SLOT(clean_staticX()));
    connect(ui->BTNbrackets, SIGNAL(clicked()), this, SLOT(brackets()));
    connect(ui->BTNexp, SIGNAL(clicked()), this, SLOT(exponent()));
    connect(ui->BTNequal, SIGNAL(clicked()), this, SLOT(equal()));
    connect(ui->BTNfactorial, SIGNAL(clicked()), this, SLOT(factorial()));
    connect(ui->BTNback, SIGNAL(clicked()), this, SLOT(back_space()));
    connect(ui->Line1ButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
                                this, SLOT(line1_expression(QAbstractButton*)));
    connect(ui->Line2ButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
                                this, SLOT(line2_expression(QAbstractButton*)));
    connect(ui->BTNDrawGraph, SIGNAL(clicked()), this, SLOT(draw_graph()));

    // calculator fields connection
    connect(ui->StaticX, SIGNAL(editingFinished()), this, SLOT(check_staticX_value()));
    connect(ui->Range1_low, SIGNAL(editingFinished()), this, SLOT(check_range_value()));
    connect(ui->Range1_high, SIGNAL(editingFinished()), this, SLOT(check_range_value()));
    connect(ui->Range2_low, SIGNAL(editingFinished()), this, SLOT(check_range_value()));
    connect(ui->Range2_high, SIGNAL(editingFinished()), this, SLOT(check_range_value()));

    // calculator signal connection
    connect(this, SIGNAL(build_lines(QString,double*,double*,int,QString,double*,double*,int,int,int)),
                                    graph_view, SLOT(graph_data_processing(QString,double*,double*,int,
                                                                    QString,double*,double*,int,int,int)));
    connect(graph_view, SIGNAL(graph_view_closed()), this, SLOT(ch_graph_view_button()));
    connect(graph_view, SIGNAL(open_calculator()), this, SLOT(show_calculator_window()));

}

Calculator_mode::~Calculator_mode() {
    delete ui;
    delete calculator;
    delete[] expression;
    delete[] line1_expr;
    delete[] line2_expr;
    clean_graph_data(&graph_data1);
    clean_graph_data(&graph_data2);
    delete graph_data1;
    delete graph_data2;
}

void Calculator_mode::show_calculator_window() {
    emit open_the_window();

}

void Calculator_mode::ch_graph_view_button() {
    ui->BTNDrawGraph->setText("Open graph view");
}

void Calculator_mode::draw_graph() {
    temp = new char[CHAR_ARRAY_MAX];
    int from = 0, to = 0;

    clean_graph_data(&graph_data1);
    clean_graph_data(&graph_data2);

    if (!(ui->Expr1->text().at(0) == '-' && ui->Expr1->text().length() == 1)) {
        clean_char_array(temp);
        for (int i = 0; static_cast<int>(line1_expr[i]) != END; i++) temp[i] = (line1_expr[i]);
        calculate_range(temp, ui->Range1_low->text().toInt(),ui->
                                            Range1_high->text().toInt(), &graph_data1);
    }
    if (!(ui->Expr2->text().at(0) == '-' && ui->Expr2->text().length() == 1)) {
        clean_char_array(temp);
        for (int i = 0; static_cast<int>(line2_expr[i]) != END; i++) temp[i] = (line2_expr[i]);
        calculate_range(temp, ui->Range2_low->text().toInt(), ui->
                                            Range2_high->text().toInt(), &graph_data2);
    }
    ui->BTNDrawGraph->setText("Update graph view");

    emit build_lines(ui->Expr1->text(), graph_data1->vals_x, graph_data1->vals_y,
                                            graph_data1->points, ui->Expr2->text(), graph_data2->
                                        vals_x, graph_data2->vals_y, graph_data2->points, from, to);
    delete[] temp;
}

void Calculator_mode::line1_expression(QAbstractButton *btn) {
    graph_expression_manage(btn, ui->Expr1, line1_expr);
}

void Calculator_mode::line2_expression(QAbstractButton *btn) {
    graph_expression_manage(btn, ui->Expr2, line2_expr);
}

void Calculator_mode::graph_expression_manage(QAbstractButton *btn, QLabel *label, char *expr) {
    if (btn->text().contains("get") && !ui->Field_expression->text().isEmpty()) {
        regEx_initialization(&end_func, &end_br, &end,  NULL, &match_1, &match_2, &match_3, NULL);
        prepare_expression();
        if ( !ui->Field_expression->text().isEmpty()) {
            label->setText(ui->Field_expression->text());
            clean_char_array(expr);
            for (int i = 0; static_cast<int>(expression[i]) != END; i++) expr[i] = expression[i];
        } else {
            label->setText("-");
            clean_char_array(expr);
        }
    } else if (btn->text().contains("x") || ui->Field_expression->text().isEmpty()) {
        label->setText("-");
        clean_char_array(expr);
    }
}

void Calculator_mode::check_range_value() {
    if (ui->Range1_low->text().length() == 1 && ui->Range1_low->text().at(0) == '-')
        ui->Range1_low->setText("0");
    if (ui->Range1_high->text().length() == 1 && ui->Range1_high->text().at(0) == '-')
        ui->Range1_high->setText("0");
    if (ui->Range2_low->text().length() == 1 && ui->Range2_low->text().at(0) == '-')
        ui->Range2_low->setText("0");
    if (ui->Range2_high->text().length() == 1 && ui->Range2_high->text().at(0) == '-')
        ui->Range2_high->setText("0");
}

// back space processing
void Calculator_mode::back_space() {
    if (ui->QW_Calculator->pos().x() == 0) {
        if (!ui->Field_expression->hasFocus() && !ui->StaticX->hasFocus() &&
                !ui->Range1_low->hasFocus() && !ui->Range1_high->hasFocus() &&
                    !ui->Range2_low->hasFocus() && !ui->Range2_high->hasFocus()) ui->
                                                            Field_expression->setFocus();
        if (ui->Field_expression->hasFocus()) {
            if (current_sym_expr > 0) delete_expression_token();
        } else if (ui->StaticX->hasFocus()) {
            delete_staticX_sign();
        } else if (ui->Range1_low->hasFocus()) {
            delete_range_value_sign(ui->Range1_low, ui->Range1_high, LRE);
        } else if (ui->Range1_high->hasFocus()) {
            delete_range_value_sign(ui->Range1_low, ui->Range1_high, HRE);
        } else if (ui->Range2_low->hasFocus()) {
            delete_range_value_sign(ui->Range2_low, ui->Range2_high, LRE);
        } else if (ui->Range2_high->hasFocus()) {
            delete_range_value_sign(ui->Range2_low, ui->Range2_high, HRE);
        }
    }
}

void Calculator_mode::delete_range_value_sign(QLineEdit *field_l, QLineEdit *field_h, int editable) {
    initialize_check_values(field_l, field_h, editable);

    edit_f->setText(edit_f->text().left(edit_f->text().length() - 1));
    if (edit_f->text().length() == 0) edit_f->setText("0");
    edit_range_int = edit_f->text().toInt();
    check_values(editable);
    edit_f->setText(QString::number(edit_range_int));
    cmp_f->setText(QString::number(cmp_range_int));
}

void Calculator_mode::initialize_check_values(QLineEdit *field_l, QLineEdit *field_h, int editable) {
    if (editable == LRE) {
        edit_f = field_l;
        cmp_f = field_h;
        edit_range_int = edit_f->text().toInt();
        cmp_range_int = cmp_f->text().toInt();
        max_value = LOW_MAX;
        min_value = LOW_MIN;
        delta_range = 1;
    } else if (editable == HRE) {
        edit_f = field_h;
        cmp_f = field_l;
        edit_range_int = edit_f->text().toInt();
        cmp_range_int = cmp_f->text().toInt();
        max_value = HIGH_MAX;
        min_value = HIGH_MIN;
        delta_range = -1;
    }
}

void Calculator_mode::delete_staticX_sign() {
    if (ui->StaticX->text().length() == 1 &&
         ui->StaticX->text().at(ui->StaticX->text().length() - 1) == '-') {
        has_staticX_Uminus = false;
    } else if (ui->StaticX->text().at(ui->StaticX->text().length() - 1) == '+' &&
                                                    ui->StaticX->text().length() > 1) {
        has_staticX_sign = false;
    } else if (ui->StaticX->text().at(ui->StaticX->text().length() - 1) == '-' &&
                                                    ui->StaticX->text().length() > 1) {
        has_staticX_sign = false;
    } else if (ui->StaticX->text().at(ui->StaticX->text().length() - 1) == '.') {
        has_staticX_point = false;
    } else if (ui->StaticX->text().at(ui->StaticX->text().length() - 1) == 'e') {
        science_e_x = false;
    }
    ui->StaticX->setText(ui->StaticX->text().left(ui->StaticX->text().length() - 1));
    if (ui->StaticX->text().length() == 0) ui->StaticX->setText("0");
}

void Calculator_mode::delete_expression_token() {
    regEx_initialization(&back_op, &back_func, &back_br, &back_sci, &match_1, &match_2, &match_3, &match_4);

    if (match_4.hasMatch()) {
        delete_number();
    } else if (match_2.hasMatch()) {
        delete_function();
    } else if (match_3.hasMatch()) {
        delete_brackets();
    } else if (match_1.hasMatch()) {
        delete_expression_sign(1, 1);
    }
}

void Calculator_mode::delete_brackets() {
    if (match_3.captured() == '(')
        brackets_counter -= 1;
    else if (match_3.captured() == ')')
        brackets_counter += 1;
    delete_expression_sign(1, 1);
}

void Calculator_mode::delete_number() {
    delete_expression_sign(1, 1);
    regEx_initialization(&back_op, &back_func, &back_br, &back_sci, &match_1, &match_2, &match_3, &match_4);
    num_len = match_4.captured().length();

    if (match_4.captured().contains('e'))
        science_e_expr = true;
    else
        science_e_expr = false;

    if (match_4.captured().contains('.'))
        has_expression_point = true;
    else
        has_expression_point = false;
}

void Calculator_mode::delete_function() {
    if (match_2.captured().contains("mod")) {
        delete_expression_sign(match_2.capturedLength(), 1);
    } else {
        delete_expression_sign(match_2.capturedLength(), 2);
    }
    brackets_counter -= 1;
}

// equal button pressed processing
void Calculator_mode::equal() {
    regEx_initialization(&end_func, &end_br, &end,  NULL, &match_1, &match_2, &match_3, NULL);

    if (ui->QW_Calculator->pos().x() == 0) {
        check_range_value();
        prepare_expression();
        variable_x = ui->StaticX->text().toDouble();
        calculate_expression_result(expression, variable_x, &calculation_result);
        ui->Field_result->setText(QString::number(calculation_result, 'f', 7));
    }
}

void Calculator_mode::prepare_expression() {
    while (match_1.hasMatch() || match_2.hasMatch() || match_3.hasMatch()) {
        if (match_1.hasMatch()) {
            if (match_1.captured().contains("mod"))
                delete_expression_sign(match_1.capturedLength(), 1);
            else
                delete_expression_sign(match_1.capturedLength(), 2);
            brackets_counter -= 1;
        } else if (match_2.hasMatch()) {
            delete_expression_sign(1, 1);
            brackets_counter -= 1;
        } else if (match_3.hasMatch()) {
            delete_expression_sign(1, 1);
        }
        regEx_initialization(&end_func, &end_br, &end,  NULL, &match_1, &match_2, &match_3, NULL);
    }
    if ((expression[0] != END && expression[current_sym_expr - 1] == EXP && science_e_expr))
        append_sign(ui->Field_expression, '0', &current_sym_expr);
    if (brackets_counter > 0) for (; brackets_counter > 0; brackets_counter -= 1)
        append_sign(ui->Field_expression, ')', &current_sym_expr);
    qDebug() << science_e_expr;
}

// factorial input processing
void Calculator_mode::factorial() {
    regEx_initialization(&fctrl, NULL, NULL, NULL, &match_1, NULL, NULL, NULL);

    if (ui->QW_Calculator->pos().x() == 0) {
        if (!ui->Field_expression->hasFocus()) ui->Field_expression->setFocus();
        if (match_1.hasMatch()) {
            append_sign(ui->Field_expression, '!', &current_sym_expr);
        }
    }
}

// functions input processing
void Calculator_mode::functions(QAbstractButton* btn) {
    regEx_initialization(&x_pi, &x_pi_x, &x_pi_0x, NULL, &match_1, &match_2, &match_3, NULL);

    if (ui->QW_Calculator->pos().x() == 0) {
        if (!ui->Field_expression->hasFocus()) ui->Field_expression->setFocus();
        if (match_3.hasMatch()) {
            append_sign(ui->Field_expression, '0', &current_sym_expr);
            append_sign(ui->Field_expression, MUL, &current_sym_expr);
            put_function(btn);
        } else if (match_2.hasMatch()) {
            append_sign(ui->Field_expression, MUL, &current_sym_expr);
            put_function(btn);
        } else if (match_1.hasMatch()) {
            put_function(btn);
        }
    }
}

void Calculator_mode::put_function(QAbstractButton* btn) {
    write_expr_to_calc(btn->text());
    ui->Field_expression->setText(ui->Field_expression->text().append(btn->text()));
    current_sym_expr += 1;
    append_sign(ui->Field_expression, '(', &current_sym_expr);
    brackets_counter += 1;
}

// x and pi input processing
void Calculator_mode::x_pi_btns(QAbstractButton* btn) {
    regEx_initialization(&x_pi, &x_pi_x, &x_pi_0x, NULL, &match_1, &match_2, &match_3, NULL);

    if (ui->QW_Calculator->pos().x() == 0) {
        if (!ui->Field_expression->hasFocus()) ui->Field_expression->setFocus();
        if (match_3.hasMatch()) {
            append_sign(ui->Field_expression, '0', &current_sym_expr);
            append_sign(ui->Field_expression, MUL, &current_sym_expr);
            append_sign(ui->Field_expression, btn->text().at(0), &current_sym_expr);
        } else if (match_2.hasMatch()) {
            append_sign(ui->Field_expression, MUL, &current_sym_expr);
                append_sign(ui->Field_expression, btn->text().at(0), &current_sym_expr);
        } else if (match_1.hasMatch()) {
                append_sign(ui->Field_expression, btn->text().at(0), &current_sym_expr);
        }
    }
}

// Exponent input processing
void Calculator_mode::exponent() {
    if (ui->QW_Calculator->pos().x() == 0) {
        if (ui->StaticX->hasFocus()) {
            if (!science_e_x) {
                science_e_x = true;
                if (ui->StaticX->text().at(ui->StaticX->text().length() - 1) == '.' ||
                        ui->StaticX->text().at(ui->StaticX->text().length() - 1) == '-') {
                    ui->StaticX->setText(ui->StaticX->text().append('0'));
                }
                ui->StaticX->setText(ui->StaticX->text().append('e'));
                current_sym_staX += 1;
            }
        } else {
            ui->Field_expression->setFocus();
            put_e_to_expression();
        }
    }
}

void Calculator_mode::put_e_to_expression() {
    regEx_initialization(&exp_0x, &exp_x, &exp_no_e, &exp, &match_1, &match_2, &match_3, &match_4);

    if (match_1.hasMatch()) {
        append_sign(ui->Field_expression, '0', &current_sym_expr);
        append_sign(ui->Field_expression, MUL, &current_sym_expr);
        append_sign(ui->Field_expression, 'e', &current_sym_expr);
        end_of_number_condition();
    } else if (match_2.hasMatch()) {
        append_sign(ui->Field_expression, MUL, &current_sym_expr);
        append_sign(ui->Field_expression, 'e', &current_sym_expr);
        end_of_number_condition();
    } else if (match_3.hasMatch() && !science_e_expr) {
        if (current_sym_expr > 0) science_e_expr = true;
        append_sign(ui->Field_expression, 'e', &current_sym_expr);
    } else if (match_4.hasMatch()) {
        append_sign(ui->Field_expression, 'e', &current_sym_expr);
    }
}


// Brackets processing
void Calculator_mode::brackets() {
    regEx_initialization(&br_L, &br_R, &br_0brR, &br_x_brL, &match_1, &match_2, &match_3, &match_4);

    if (ui->QW_Calculator->pos().x() == 0) {
        if (!ui->Field_expression->hasFocus()) ui->Field_expression->setFocus();
        if (match_1.hasMatch()) {
            put_bracket('(', '+');
        } else if (brackets_counter > 0 && match_2.hasMatch()) {
            put_bracket(')', '-');
        } else if (match_3.hasMatch() && brackets_counter > 0) {
            append_sign(ui->Field_expression, '0', &current_sym_expr);
            put_bracket(')', '-');
        } else if (brackets_counter == 0 && match_4.hasMatch()) {
            if (ui->Field_expression->text().at(ui->Field_expression->text().length() - 1) == 'e' &&
                                                                                        science_e_expr) {
                append_sign(ui->Field_expression, '0', &current_sym_expr);
            }
            append_sign(ui->Field_expression, MUL, &current_sym_expr);
            put_bracket('(', '+');
        }
    }
}

void Calculator_mode::put_bracket(char bracket, char type) {
    append_sign(ui->Field_expression, bracket, &current_sym_expr);
    if (type == '+') {
        brackets_counter += 1;
    } else if (type == '-') {
        brackets_counter -= 1;
    }
    end_of_number_condition();
}

// Operators input processing
void Calculator_mode::operators(QAbstractButton *btn) {
    regEx_initialization(&op, &op_science, &op_change, &op_unary, &match_1, &match_2, &match_3, &match_4);

    if (ui->QW_Calculator->pos().x() == 0) {
        if (!ui->Field_expression->hasFocus() && !ui->StaticX->hasFocus() &&
                !ui->Range1_low->hasFocus() && !ui->Range1_high->hasFocus() &&
                    !ui->Range2_low->hasFocus() && !ui->Range2_high->hasFocus()) {
            ui->Field_expression->setFocus();
        }

        if (ui->Field_expression->hasFocus()) {
            write_expression_field(btn);
        } else if (ui->StaticX->hasFocus()) {
            write_static_x_field(btn);
        } else if (ui->Range1_low->hasFocus()) {
            put_minus_to_range_field(ui->Range1_low);
        } else if (ui->Range1_high->hasFocus()) {
            put_minus_to_range_field(ui->Range1_high);
        } else if (ui->Range2_low->hasFocus()) {
            put_minus_to_range_field(ui->Range2_low);
        } else if (ui->Range2_high->hasFocus()) {
            put_minus_to_range_field(ui->Range2_high);
        }
    }
}

void Calculator_mode::put_minus_to_range_field(QLineEdit *field) {
    if (!field->text().contains('-') && field->text().length() == 1)
        field->setText("-");
    else if (field->text().length() == 1)
        field->setText("0");
}

void Calculator_mode::write_static_x_field(QAbstractButton *btn) {
    if (btn->text().at(0) == '-' && has_staticX_Uminus && ui->StaticX->text().length() == 1) {
           ui->StaticX->setText(ui->StaticX->text().left(ui->StaticX->text().length() - 1));
           ui->StaticX->setText(ui->StaticX->text().append(('0')));
           has_staticX_Uminus = false;
    } else if ((btn->text().at(0) == '+' || btn->text().at(0) == '-') && !has_staticX_sign &&
                                ui->StaticX->text().at(ui->StaticX->text().length() - 1) == 'e') {
        ui->StaticX->setText(ui->StaticX->text().append((btn->text().at(0))));
        has_staticX_sign = true;
    } else if (ui->StaticX->text().at(ui->StaticX->text().length() - 1) == '-' ||
                                 ui->StaticX->text().at(ui->StaticX->text().length() - 1) == '+') {
        ui->StaticX->setText(ui->StaticX->text().left(ui->StaticX->text().length() - 1));
        ui->StaticX->setText(ui->StaticX->text().append((btn->text().at(0))));
    } else if (btn->text().at(0) == '-' && ui->StaticX->text().length() == 1 &&
                                                    ui->StaticX->text() == "0" && !has_staticX_Uminus) {
        ui->StaticX->setText(ui->StaticX->text().left(ui->StaticX->text().length() - 1));
        ui->StaticX->setText(ui->StaticX->text().append(('-')));
        has_staticX_Uminus = true;
    }
}

void Calculator_mode::write_expression_field(QAbstractButton *btn) {
    if (btn->text().at(0) == SUB && match_4.hasMatch()) {  // unary minus
        unary_minus_handle(btn);
    } else if (match_3.hasMatch()) {  // change sign
        if (match_2.hasMatch())
            put_operator(btn->text().at(0), SCIENCE_CH_OP, 1);
        else
            change_last_operator(btn);
    } else if (match_2.hasMatch() && (btn->text().at(0) == SUB ||
                            btn->text().at(0) == '+')) {  // if "e[+-]*"
        put_operator(btn->text().at(0), SCIENCE_OP, 1);
    } else if (match_1.hasMatch()) {
        if (btn->text() == "mod") {
            put_operator('m', APPEND_OP, 0);
            put_operator('o', APPEND_OP, 0);
            put_operator('d', APPEND_OP, 0);
            write_expr_to_calc("%");
            current_sym_expr += 1;
        } else {
            put_operator(btn->text().at(0), APPEND_OP, 0);
        }
    }
}

void Calculator_mode::put_operator(QChar sym, int mode, int cut_last) {
    if (mode == APPEND_OP) {
        append_sign(ui->Field_expression, sym, &current_sym_expr);
        end_of_number_condition();
    } else if (mode == CHANGE_OP) {
        current_sym_expr -= 1;
        ui->Field_expression->setText(ui->Field_expression->
            text().left(ui->Field_expression->text().length() - cut_last));
        append_sign(ui->Field_expression, sym, &current_sym_expr);
        end_of_number_condition();
    } else if (mode == SCIENCE_OP) {
        append_sign(ui->Field_expression, sym, &current_sym_expr);
    } else if (mode == SCIENCE_CH_OP) {
        current_sym_expr -= 1;
        ui->Field_expression->setText(ui->Field_expression->
                text().left(ui->Field_expression->text().length() - cut_last));
        append_sign(ui->Field_expression, sym, &current_sym_expr);
    }
}

void Calculator_mode::unary_minus_handle(QAbstractButton *btn) {
    if (btn->text().at(0) == SUB &&
            (current_sym_expr > 0 && ui->Field_expression->
                text().at(ui->Field_expression->text().length() - 1) == '^')) {  // if "^" and "-" get "(-"
        put_bracket('(', '+');
        put_unary_minus();
    } else {
        put_unary_minus();
    }
}

void Calculator_mode::put_unary_minus() {
    ui->Field_expression->setText(ui->Field_expression->text().append(SUB));
    write_expr_to_calc("uminus");
    current_sym_expr += 1;
    end_of_number_condition();
}

void Calculator_mode::change_last_operator(QAbstractButton *btn) {
    if (btn->text().at(0) == SUB &&
            ui->Field_expression->text().at(ui->Field_expression->
                                        text().length() - 1) == SUB) {  // if "-" and "-" get "+"
        put_operator('+', CHANGE_OP, 1);
    } else if (btn->text() == "mod") {
        if (ui->Field_expression->text().at(ui->Field_expression->text().length() - 1) == 'd')
            put_operator('m', CHANGE_OP, 3);
        else
            put_operator('m', CHANGE_OP, 1);
        put_operator('o', APPEND_OP, 0);
        put_operator('d', APPEND_OP, 0);
        write_expr_to_calc("%");
        current_sym_expr += 1;
    } else {  // just change sign
        if (ui->Field_expression->text().at(ui->Field_expression->text().length() - 1) == 'd') {
            put_operator(btn->text().at(0), CHANGE_OP, 3);
        } else {
            put_operator(btn->text().at(0), CHANGE_OP, 1);
        }
    }
}

// Numbers input processing
void Calculator_mode::numPad(QAbstractButton *btn) {
    regEx_initialization(&nums_x, &point, &point_0, NULL, &match_1, &match_2, &match_3, NULL);

    if (ui->QW_Calculator->pos().x() == 0) {
        if (ui->StaticX->hasFocus()) {
            if (ui->StaticX->text().at(0) == '0' && ui->StaticX->
                            text().length() == 1 && btn->text().at(0) != '.') {
                ui->StaticX->clear();
            }
            write_static_x_val(btn);
        } else if (ui->Range1_low->hasFocus()) {
            write_range_bound(btn, ui->Range1_low, ui->Range1_high, LRE);
        } else if (ui->Range1_high->hasFocus()) {
            write_range_bound(btn, ui->Range1_low, ui->Range1_high, HRE);
        } else if (ui->Range2_low->hasFocus()) {
            write_range_bound(btn, ui->Range2_low, ui->Range2_high, LRE);
        } else if (ui->Range2_high->hasFocus()) {
            write_range_bound(btn, ui->Range2_low, ui->Range2_high, HRE);
        } else {
            write_number_to_expression(btn);
        }
    }
}

void Calculator_mode::write_number_to_expression(QAbstractButton *btn) {
    ui->Field_expression->setFocus();
    if (num_len < MAX_NUM_LEN) {
        buffer = ui->Field_expression->text();
        if (!(num_len == 1 && btn->text().at(0) == '0' && buffer[current_sym_expr - 1] == '0')) {
            if (match_1.hasMatch() && !science_e_expr) {
                append_sign(ui->Field_expression, MUL, &current_sym_expr);
                end_of_number_condition();
                write_value_string(ui->Field_expression, btn,
                                   &current_sym_expr, &has_expression_point, EXPR);
                num_len += 1;
            } else {
                write_value_string(ui->Field_expression, btn,
                                   &current_sym_expr, &has_expression_point, EXPR);
                num_len += 1;
            }
        }
    }
}

void Calculator_mode::write_range_bound(QAbstractButton *btn,
                                        QLineEdit *field_l, QLineEdit *field_h, int editable) {
    initialize_check_values(field_l, field_h, editable);

    if (!(edit_f->text().length() == 1 && edit_f->text().at(0) == '-') &&
            edit_range_int >= min_value && edit_range_int <= max_value) {
        range_value_str = QString::number(edit_range_int);
        range_value_str.append(btn->text().at(0));
        edit_range_int = range_value_str.toInt();

        check_values(editable);

        edit_f->setText(QString::number(edit_range_int));
        cmp_f->setText(QString::number(cmp_range_int));
    } else if (edit_range_int >= min_value && edit_range_int <= max_value) {
        if (edit_f->text().length() == 1 && edit_f->text().at(0) != '0') {
            edit_f->setText(edit_f->text().append(btn->text().at(0)));
        }
        edit_range_int = edit_f->text().toInt();
        check_values(editable);

        cmp_f->setText(QString::number(cmp_range_int));
        edit_f->setText(QString::number(edit_range_int));
    }
}

void Calculator_mode::check_values(int editable) {
    if (edit_range_int > max_value) {
        edit_range_int = max_value;
    } else if (edit_range_int < min_value) {
        edit_range_int = min_value;
    }
    if ((edit_range_int >= cmp_range_int && editable == LRE) ||
            (edit_range_int <= cmp_range_int && editable == HRE)) {
        cmp_range_int = edit_range_int + delta_range;
    }
}

void Calculator_mode::write_static_x_val(QAbstractButton *btn) {
    buffer = ui->StaticX->text();
    if (btn->text().at(0) == '.' && !has_staticX_point && !has_staticX_sign && !science_e_x) {
        if (ui->StaticX->text().at(0) == '-' && ui->StaticX->text().length() == 1) {
            ui->StaticX->setText(buffer.append('0'));
        }
        ui->StaticX->setText(buffer.append('.'));
        has_staticX_point = true;
    } else if (btn->text().at(0) != '.') {
        ui->StaticX->setText(buffer.append(btn->text().at(0)));
    }
}

void Calculator_mode::write_value_string(QLineEdit *field, QAbstractButton *btn,
                                                        qsizetype *iter, bool *point, int mode) {
    if (btn->text().at(0) == '.' && *point == false && match_2.hasMatch() && !science_e_expr) {
        *point = true;
        if (match_3.hasMatch()) {
            buffer.append("0");
            if (mode == EXPR) write_expr_to_calc(buffer.at(buffer.length() - 1));
            *iter += 1;
        }
        buffer.append(btn->text());
        field->setText(buffer);
        if (mode == EXPR) write_expr_to_calc(buffer.at(buffer.length() - 1));
        *iter += 1;
    }

    if (btn->text().at(0) != '.') {
        buffer.append(btn->text());
        field->setText(buffer);
        if (mode == EXPR) write_expr_to_calc(buffer.at(buffer.length() - 1));
        *iter += 1;
    }
}

// Static X input processing
void Calculator_mode::check_staticX_value() {
    if (ui->StaticX->text().isEmpty()) ui->StaticX->setText("0");
}

double get_staticX(QString staticX_str) {
    char *value_str_char = NULL;
    QByteArray bytes_buffer;
    double value = 0.0;

    bytes_buffer = staticX_str.toLocal8Bit();
    value_str_char = bytes_buffer.data();
    value = ::atof(value_str_char);

    return value;
}

// Cleaners
void Calculator_mode::clean_field() {
    ui->Field_expression->setFocus();
    if (ui->Field_expression->hasFocus()) {
        clean_main_expression();
    } else if (ui->StaticX->hasFocus()) {
        clean_staticX();
    } else if (ui->Range1_low->hasFocus()) {

    } else if (ui->Range1_high->hasFocus()) {

    } else if (ui->Range2_low->hasFocus()) {

    } else if (ui->Range1_high->hasFocus()) {

    }
}

void Calculator_mode::clean_main_expression() {
    ui->Field_expression->clear();
    clean_char_array(expression);
    has_expression_point = false;
    current_sym_expr = 0;
    num_len = 0;
    science_e_expr = false;
    brackets_counter = 0;
}

void Calculator_mode::end_of_number_condition() {
    science_e_expr = false;
    has_expression_point = false;
    num_len = 0;
}

void Calculator_mode::clean_staticX() {
    ui->StaticX->clear();
    ui->StaticX->setText("0");
    current_sym_staX = 1;
    has_staticX_point = false;
    has_staticX_sign = false;
    has_staticX_Uminus = false;
    science_e_x = false;
}

void Calculator_mode:: delete_expression_sign(int del_len, int tokens) {
    ui->Field_expression->setText(ui->Field_expression->text().left(ui->
                                Field_expression->text().length() - del_len));
    for (int i = 0; i < tokens; i++) {
        current_sym_expr -= 1;
        write_expr_to_calc("end");
    }
}

void Calculator_mode::clean_char_array(char * str) {
    for (int i = 0; i < CHAR_ARRAY_MAX; i++) str[i] = END;
}

void Calculator_mode::clean_graph_data(graph_t **graph_data) {
        if ((*graph_data)->vals_x) {
            delete (*graph_data)->vals_x;
            (*graph_data)->vals_x = NULL;
        }
        if ((*graph_data)->vals_y) {
            delete (*graph_data)->vals_y;
            (*graph_data)->vals_y = NULL;
        }
}

// Append symbol to field
void Calculator_mode::append_sign(QLineEdit *field, QChar sym, qsizetype *iter) {
    buffer = field->text();
    buffer.append(sym);
    field->setText(buffer);
    if (sym != 'm' && sym != 'o' && sym != 'd') {
        write_expr_to_calc(sym);
        *iter += 1;
    }
}

// Writing expression to calculate
void Calculator_mode::write_expr_to_calc(QString token) {
    char the_sym = 0;

    if (token == "end")
            the_sym = END;
    else if ((token.at(0) >= '0' && token.at(0)  <= '9' ) || token.at(0)  == '.' ||
                             token.at(0)  == 'e' || token.at(0)  == '+' || token.at(0)  == '-')
        the_sym = static_cast<char>(token.toLocal8Bit().at(0));
    else if (token.at(0)  == 'X')
        the_sym = VARX;
    else if (token.at(0) == MUL)
        the_sym = MULTIPLY;
    else if (token.at(0) == DIV)
        the_sym = DIVIDE;
    else if (token.at(0)  == '%')
        the_sym = MOD;
    else if (token.at(0)  == '^')
        the_sym = EXTENT;
    else if (token == "cos")
        the_sym = COS;
    else if (token == "sin")
        the_sym = SIN;
    else if (token == "tan")
        the_sym = TAN;
    else if (token == "acos")
        the_sym = ACOS;
    else if (token == "asin")
        the_sym = ASIN;
    else if (token == "atan")
        the_sym = ATAN;
    else if (token.at(0) == SQROOT)
        the_sym = SQRT;
    else if (token == "ln")
        the_sym = LN;
    else if (token == "log")
        the_sym = LOG;
    else if (token.at(0) == '(')
        the_sym = LBRACKET;
    else if (token.at(0) == ')')
        the_sym = RBRACKET;
    else if (token == "uminus")
        the_sym = UMINUS;
    else if (token.at(0) == '!')
        the_sym = FACTORIAL;
    else if (token.at(0) == PI_U)
        the_sym = PI;

    if (ui->Field_expression->text().length() <= 256) {
        expression[current_sym_expr] = the_sym;
    }
}

// regular expression initialization
void Calculator_mode::regEx_initialization(QRegularExpression *e1, QRegularExpression *e2,
                QRegularExpression *e3, QRegularExpression *e4, QRegularExpressionMatch *m1,
        QRegularExpressionMatch *m2, QRegularExpressionMatch *m3, QRegularExpressionMatch *m4) {
    if (e1 && m1) *m1 = e1->match(ui->Field_expression->text());
    if (e2 && m2) *m2 = e2->match(ui->Field_expression->text());
    if (e3 && m3) *m3 = e3->match(ui->Field_expression->text());
    if (e4 && m4) *m4 = e4->match(ui->Field_expression->text());
}

// Regular expression pattern initialization
void Calculator_mode::set_up_regular_expressions() {
    // numbers
nums_x.setPattern("(π$)|(X$)|([\\)\\!]$)|([^\\d]e$)");  // to put "*9"
nums.setPattern("([\\+\\-\\×\\÷\\.\\^]$)|([0-9]e[\\+\\-]{0,1}$)|(^$)|([\\(]$)|(mod$)");  // to put "9"
point.setPattern("(π$)|(x$)|([\\)]$)|(^e$)|([^0-9]e[\\+\\-]*$)|(^$)|([^e][0-9]*$)");  // to put "."
point_0.setPattern("(π$)|([^e][\\×\\÷\\-\\+\\^]$)|(([^\\d]$)|"
                        "(^)e[\\×\\÷\\-\\+\\^]*$)|(mod$)|(x$)|([\\)\\(]$)|(e$)|(^$)");  // to put "0."

    //exp
exp_0x.setPattern("([0-9]e[\\+\\-]{0,1}$)|([0-9]\\.$)");  // to put "0*e"
exp_x.setPattern("(π$)|(X$)|(e$)|([0-9]e[\\+\\-]*[0-9]*$)|([\\)\\!]$)");  // to put "*e"
exp_no_e.setPattern("([^\\(]*[\\d\\.]+$)");  // to put "e" with no "e" before in the num
exp.setPattern("(^$)|([\\(\\^\\+\\-\\×\\÷]$)|(mod$)");  // to put "e"

    // brackets
br_L.setPattern("(([^e]([\\+\\-\\÷\\×\\^]+)$|(([\\+\\-\\÷\\×\\^]+)$)|"
                "(mod)+)$)|([\(]$)|(^$)");  // to put "("
br_R.setPattern("([\\dXπ\\)]$)|([^\\d]e$)");  // to put ")"
br_0brR.setPattern("([\\d]e[\\+\\-]*$)|([\\d][\\.]$)");  // to put "0)"
br_x_brL.setPattern("([\\dX\\π)\\!]$)|([^\\d]*e$)");  // to put "*("

    // operators
op.setPattern("([Xπ\\d\\)\\!]$)|([^\\d]e$)|(^e$)");  // to put "/","*", "%" or "^"
op_science.setPattern("([π\\d]e[\\+\\-]*$)|(^e$)");  // to put "+" or "-" for science notation
op_change.setPattern("([^\\(][\\×\\÷\\-\\+\\^]$)|(mod$)");  // to change sign when anoter one is already put
op_unary.setPattern("(^$)|([\(\\^]$)");  // to put unary "-"
    // x_pi
x_pi.setPattern("([\\×\\÷\\^\\(]$)|(mod$)|([^e][\\+\\-]$)|"
                            "([^\\d]e[\\+\\-]$)|(^$)|(^[\\-]$)");  // to put "x" or "pi"
x_pi_x.setPattern("([πX\\)\\d\\!]$)|([^\\d]e$)|(^e$)");  // to put "*x" or "*pi"
x_pi_0x.setPattern("([\\d][\\.]$)|([\\d]e[\\+\\-]*$)");  // to put "0*x" or "0*pi"

    // factorial
fctrl.setPattern("([\\d\\)Xπ]$)");  // to put factorial

    // end
end.setPattern("([\\+\\-\\×\\÷\\.\\^]$)");  // to check expression end for operators
end_br.setPattern("([\\(]$)");  // to check expression end for opened bracket
end_func.setPattern("(sin\\($)|(cos\\($)|(tan\\($)|(asin\\($)|(acos\\($)|(atan\\($)|(ln\\($)|"
                        "(log\\($)|(√\\($)|(mod$)"); // to check expression end for unwroten function

    // back space
back_op.setPattern("([\\+\\-\\×\\÷\\^\\!]$)");  // to delete operator
back_func.setPattern("(sin\\($)|(cos\\($)|(tan\\($)|(asin\\($)|(acos\\($)|(atan\\($)|"
                                   "(ln\\($)|(log\\($)|(√\\($)|(mod$)");  // to delete function
back_br.setPattern("([\\(\\)]$)");  // to delete bracket
back_sci.setPattern("((([0-9]+e[\\+\\-]*[0-9]*)|"
                    "([0-9]+[\\.][0-9]+e[\\+\\-]*[0-9]*)|"
                        "([0-9]+)|([0-9]+[\\.][0-9]+)|([0-9]+[\\.]))$)|"
                                            "([\\+\\-\\×\\÷\\^\\(][eπX]$)|"
                                                "(^[eπX]$)");  // to delete numbers, variables and constants
}
