#ifndef CALCULATOR_MODE_H
#define CALCULATOR_MODE_H

#include <QRegularExpression>
#include <QDoubleValidator>
#include <QProgressBar>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QString>
#include <QLabel>
#include "qcplotic.h"

extern "C" {
    #include "../s21_calc_v1_0_header.h"
};

// maximum length countable array
#define CHAR_ARRAY_MAX 256

// maximum one number length
#define MAX_NUM_LEN 31

// graph range bounds
#define LOW_MAX 999999
#define LOW_MIN -1000000
#define HIGH_MAX 1000000
#define HIGH_MIN -999999

// graph range bounds switchers
#define LRE 1
#define HRE 2

// enums-switchers to differ expression and static x fields in functions
typedef enum {
    EXPR,
    STAX
} field_num_switch;

// enums-switcher to append or change last operator
typedef enum {
    APPEND_OP,
    CHANGE_OP,
    SCIENCE_OP,
    SCIENCE_CH_OP
} field_op_switch;

// Unicode symbols
#define PI_U QChar(0x03C0)
#define SQROOT QChar(0x221A)
#define DIV QChar(0x00F7)
#define BSPACE QChar(0x21E4)
#define MUL QChar(0x00D7)
#define SUB QChar(0x002D)


namespace Ui { class Calculator_mode; }

class Calculator_mode : public QWidget {
    Q_OBJECT

signals:
    void build_lines(QString,double*,double*,int,QString,double*,double*,int,int,int);
    void  open_the_window();

public:
    explicit Calculator_mode(QWidget *parent = nullptr);
    ~Calculator_mode();
    QWidget *calculator = new QWidget(this);
    QCPlotic *graph_view;
    void clean_graph_data(graph_t **graph_data);

private slots:
    void numPad(QAbstractButton *btn);
    void operators(QAbstractButton *btn);
    void x_pi_btns(QAbstractButton* btn);
    void functions(QAbstractButton* btn);
    void brackets();
    void exponent();
    void clean_field();
    void check_staticX_value();
    void clean_staticX();
    void equal();
    void factorial();
    void back_space();
    void check_range_value();
    void line1_expression(QAbstractButton *btn);
    void line2_expression(QAbstractButton *btn);
    void draw_graph();
    void ch_graph_view_button();
    void show_calculator_window();

private:
    Ui::Calculator_mode *ui;
    QPropertyAnimation *progress;
    QString buffer = NULL;
    void set_up_regular_expressions();
    graph_t *graph_data1 = NULL;
    graph_t *graph_data2 = NULL;
    char *temp = NULL;

    bool hasFocus() const;
    bool has_staticX_point = false;
    bool has_staticX_sign = false;
    bool has_staticX_Uminus = false;
    bool science_e_x = false;
    bool has_expression_point = false;
    bool science_e_expr = false;
    bool wroten = false;

    double calculation_result = 0.0;
    double variable_x = 0.0;

    int edit_range_int= 0;
    int cmp_range_int = 0;
    int min_value = 0;
    int max_value = 0;
    int delta_range = 0;
    QLineEdit *edit_f;
    QLineEdit *cmp_f;
    QString range_value_str;

    int brackets_counter = 0;
    qsizetype current_sym_expr = 0;
    qsizetype current_sym_staX = 1;
    short int num_len = 0;

    char *expression = NULL;
    char *line1_expr = NULL;
    char *line2_expr = NULL;
    void clean_char_array(char *);
    void write_expr_to_calc(QString token);
    void clean_main_expression();
    void graph_expression_manage(QAbstractButton *btn, QLabel *label, char *expr);
    void check_values(int editable);
    void write_number_to_expression(QAbstractButton *btn);
    void write_value_string(QLineEdit *field, QAbstractButton *btn, qsizetype *iter, bool *point, int mode);
    void end_of_number_condition();
    void append_sign(QLineEdit *field, QChar sym, qsizetype *iter);
    void put_minus_to_range_field(QLineEdit *field);
    void write_static_x_field(QAbstractButton *btn);
    void write_range_bound(QAbstractButton *btn, QLineEdit *field_l, QLineEdit *field_h, int editable);
    void write_static_x_val(QAbstractButton *btn);
    void write_expression_field(QAbstractButton *btn);
    void put_e_to_expression();
    void put_bracket(char bracket, char type);
    void put_operator(QChar sym, int mode, int cut_last);
    void put_function(QAbstractButton* btn);
    void unary_minus_handle(QAbstractButton *btn);
    void put_unary_minus();
    void change_last_operator(QAbstractButton *btn);
    void prepare_expression();
    void delete_expression_sign(int del_len, int tokens);
    void delete_expression_token();
    void delete_brackets();
    void delete_number();
    void delete_function();
    void delete_range_value_sign(QLineEdit *field_l, QLineEdit *field_h, int editable);
    void initialize_check_values(QLineEdit *field_l, QLineEdit *field_h, int editable);
    void delete_staticX_sign();

    // initialization function
    void regEx_initialization(QRegularExpression *e1, QRegularExpression *e2,
        QRegularExpression *e3, QRegularExpression *e4, QRegularExpressionMatch *m1,
            QRegularExpressionMatch *m2, QRegularExpressionMatch *m3, QRegularExpressionMatch *m4);

    // Regular expressions pattern
        // exponent
    QRegularExpression exp_0x;
    QRegularExpression exp_x;
    QRegularExpression exp;
    QRegularExpression exp_no_e;
        // numbers
    QRegularExpression nums_x;
    QRegularExpression nums;
    QRegularExpression point;
    QRegularExpression point_0;
        // brackets
    QRegularExpression br_L;
    QRegularExpression br_R;
    QRegularExpression br_0brR;
    QRegularExpression br_x_brL;
        //operators
    QRegularExpression op;
    QRegularExpression op_science;
    QRegularExpression op_change;
    QRegularExpression op_unary;
        // x_pi
    QRegularExpression x_pi;
    QRegularExpression x_pi_x;
    QRegularExpression x_pi_0x;
        // factorial
    QRegularExpression fctrl;
        // end
    QRegularExpression end;
    QRegularExpression end_br;
    QRegularExpression end_func;
        // back space
    QRegularExpression back_op;
    QRegularExpression back_func;
    QRegularExpression back_br;
    QRegularExpression back_sci;
        // matchers
    QRegularExpressionMatch match_1;
    QRegularExpressionMatch match_2;
    QRegularExpressionMatch match_3;
    QRegularExpressionMatch match_4;
};

#endif // CALCULATOR_MODE_H
