#include "my_parser.h"
#include "my_calc.h"
#include <criterion/criterion.h>
#include <stdio.h>

int read_val(struct parser *p)
{
    int tmp = p->current_pos;
    if (readint(p))
        return 1;
    else if (readid(p))
        return 1;
    p->current_pos = tmp;
    return 0;
}

int     read_test(struct parser *p)
{
    int tmp = p->current_pos;
    if (read_val(p) && readchar(p, '+') && read_val(p) && readchar(p, ';'))
        return 1;
    p->current_pos = tmp;
    return 0;
}

Test(my, check_calc_int)
{
    struct parser *p;
    struct scope s;

    p = new_parser("12;");
    my_calc(p, &s);
    cr_assert(s.current_val == 12);
    clean_parser(p);
}

Test(my, check_calc_int_with_spaces)
{
    struct parser *p;
    struct scope s;

    p = new_parser("  12  \n  \n;");
    my_calc(p, &s);
    cr_assert(s.current_val == 12);
    clean_parser(p);
}

Test(my, check_calc_product)
{
    struct parser *p;
    struct scope s;

    p = new_parser("14*2;");
    my_calc(p, &s);
    cr_assert(s.current_val == 28);
    clean_parser(p);
}

Test(my, check_calc_product_with_spaces)
{
    struct parser *p;
    struct scope s;

    p = new_parser("  14  \t  \n* \n2 \n;  \n");
    my_calc(p, &s);
    cr_assert(s.current_val == 28);
    clean_parser(p);
}

Test(my, check_calc_division)
{
    struct parser *p;
    struct scope s;

    p = new_parser("14 / 2;");
    my_calc(p, &s);
    cr_assert(s.current_val == 7);
    clean_parser(p);
}

Test(my, check_calc_division_with_spaces)
{
    struct parser *p;
    struct scope s;

    p = new_parser("  14  \t  \n/ \n2 \n;  \n");
    my_calc(p, &s);
    cr_assert(s.current_val == 7);
    clean_parser(p);
}

Test(my, check_calc_modulo)
{
    struct parser *p;
    struct scope s;

    p = new_parser("14%8;");
    my_calc(p, &s);
    cr_assert(s.current_val == 6);
    clean_parser(p);
}

Test(my, check_calc_modulo_with_spaces)
{
    struct parser *p;
    struct scope s;

    p = new_parser("  14  \t  \n% \n8 \n;  \n");
    my_calc(p, &s);
    cr_assert(s.current_val == 6);
    clean_parser(p);
}

Test(my, check_calc_sum)
{
    struct parser *p;
    struct scope s;

    p = new_parser("144+6;");
    my_calc(p, &s);
    cr_assert(s.current_val == 150);
    clean_parser(p);
}

Test(my, check_calc_sum_with_spaces)
{
    struct parser *p;
    struct scope s;

    p = new_parser("  144  \t  \n+ \n6 \n;  \n");
    my_calc(p, &s);
    cr_assert(s.current_val == 150);
    clean_parser(p);
}

Test(my, check_calc_sub)
{
    struct parser *p;
    struct scope s;

    p = new_parser("144-6;");
    my_calc(p, &s);
    cr_assert(s.current_val == 138);
    clean_parser(p);
}

Test(my, check_calc_sub_with_spaces)
{
    struct parser *p;
    struct scope s;

    p = new_parser("  144  \t  \n- \n6 \n;  \n");
    my_calc(p, &s);
    cr_assert(s.current_val == 138);
    clean_parser(p);
}

Test(my, check_calc_unary)
{
    struct parser *p;
    struct scope s;

    p = new_parser("-6;");
    my_calc(p, &s);
    cr_assert(s.current_val == -6);
    clean_parser(p);
}

Test(my, check_calc_unary_with_spaces)
{
    struct parser *p;
    struct scope s;

    p = new_parser(" -6  \t  \n;  \n");
    my_calc(p, &s);
    cr_assert(s.current_val == -6);
    clean_parser(p);
}


Test(my, check_calc_complex_with_spaces)
{
    struct parser *p;
    struct scope s;

    p = new_parser("  144  \t  \n- -\n6 \n;  \n");
    my_calc(p, &s);
    cr_assert(s.current_val == 150);
    clean_parser(p);
}

Test(my, check_calc_parenthesis_with_sum)
{
    struct parser *p;
    struct scope s;

    p = new_parser("(144+6);");
    my_calc(p, &s);
    cr_assert(s.current_val == 150);
    clean_parser(p);
}

Test(my, check_calc_priority)
{
    struct parser *p;
    struct scope s;

    p = new_parser("2+3*4;");
    my_calc(p, &s);
    cr_assert(s.current_val == 14);
    clean_parser(p);
}

Test(my, check_calc_priority_with_parenthesis)
{
    struct parser *p;
    struct scope s;

    p = new_parser("(2+3)*4;");
    my_calc(p, &s);
    cr_assert(s.current_val == 20);
    clean_parser(p);
}

Test(my, check_calc_priority_with_parenthesis_and_spaces)
{
    struct parser *p;
    struct scope s;

    p = new_parser("  6 \t  \n/ \n2 \n+ \n1 \n;  \n");
    my_calc(p, &s);
    cr_assert(s.current_val == 4);
    clean_parser(p);
}

Test(my, check_calc_variable)
{
    struct parser *p;
    struct scope s;

    p = new_parser("a=12; a;");
    my_calc(p, &s);
    cr_assert(s.current_val == 12);
    clean_parser(p);
}

Test(my, check_calc_variable_with_expression)
{
    struct parser *p;
    struct scope s;

    p = new_parser("a = 1 + 2 * 3; a * 2;");
    my_calc(p, &s);
    cr_assert(s.current_val == 14);
    clean_parser(p);
}

Test(my, check_calc_multi_variable_with_expression)
{
    struct parser *p;
    struct scope s;

    p = new_parser(" a=(2+3) * 4; c=4; a*c;");
    my_calc(p, &s);
    cr_assert(s.current_val == 80);
    clean_parser(p);
}

Test(my, check_calc_multi_variable_with_expression_and_spaces)
{
    struct parser *p;
    struct scope s;

    p = new_parser("a=2+1; 12-a \n;");
    my_calc(p, &s);
    cr_assert(s.current_val == 9);
    clean_parser(p);
}


