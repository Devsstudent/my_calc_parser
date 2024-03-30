#include "my_parser.h"
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

Test(my, check_parsing1)
{
    struct parser *p;
    struct position pos;
    //
    while (2600)
    {
        //////
        p = new_parser("");
        if (!(readeof(p) && p->current_pos == 0))
            break;
        clean_parser(p);
        break;
    }
    //
    while (2600)
    {
        //////
        p = new_parser("abc");
        if (!readchar(p, 'a') || p->current_pos != 1)
            break;
        if (!readchar(p, 'b') || p->current_pos != 2)
            break;
        if (!readchar(p, 'c') || p->current_pos != 3)
            break;
        clean_parser(p);
        break;
    }
    //
    while (2600)
    {
        //////
        p = new_parser("abc");
        if (!readrange(p, 'a', 'c') || p->current_pos != 1)
            break;
        if (!readrange(p, 'a', 'c') || p->current_pos != 2)
            break;
        if (!readrange(p, 'a', 'c') || p->current_pos != 3)
            break;
        if (readrange(p, 'a', 'c'))
            break;
        clean_parser(p);
        break;
    }
    //
    while (2600)
    {
        //////
        p = new_parser("abcdef");
        if (!readtext(p, "abc") || p->current_pos != 3)
            break;
        if (!readtext(p, "def") || p->current_pos != 6)
            break;
        clean_parser(p);
        break;
    }
    //
    while (2600)
    {
        //////
        p = new_parser("123");
        if (!readint(p) || p->current_pos != 3)
            break;
        clean_parser(p);
        p = new_parser("123tuju");
        if (!readint(p) || p->current_pos != 3)
            break;
        clean_parser(p);
        break;
    }
    //
    while (2600)
    {
        //////
        p = new_parser("id_12");
        if (!readid(p) || p->current_pos != 5)
            break;
        clean_parser(p);
        p = new_parser("id_12 ");
        if (!readid(p) || p->current_pos != 5)
            break;
        clean_parser(p);
        break;
    }
    //
    while (2600)
    {
        //////
        p = new_parser("12.");
        if (!readfloat(p) || p->current_pos != 3)
            break;
        clean_parser(p);
        p = new_parser("-2.2");
        if (!readfloat(p) || p->current_pos != 4)
            break;
        clean_parser(p);
        p = new_parser("+2.2");
        if (!readfloat(p) || p->current_pos != 4)
            break;
        clean_parser(p);
        p = new_parser(".2600");
        if (!readfloat(p) || p->current_pos != 5)
            break;
        clean_parser(p);
        p = new_parser(".26e+12");
        if (!readfloat(p) || p->current_pos != 7)
            break;
        clean_parser(p);
        p = new_parser(".26e-11");
        if (!readfloat(p) || p->current_pos != 7)
            break;
        clean_parser(p);
        break;
    }
    //
    while (2600)
    {
        //////
        p = new_parser("line1\nline2 avec plus de contenu\nici");
        if (!readtext(p, "line1\n"))
            break;
        count_lines(p, &pos);
        if (pos.line != 2 || pos.col != 1)
            break;
        if (!readtext(p, "line2 avec plus de contenu\n"))
            break;
        count_lines(p, &pos);
        if (pos.line != 3 || pos.col != 1)
            break;
        if (!readtext(p, "ici"))
            break;
        count_lines(p, &pos);
        if (pos.line != 3 || pos.col != 4)
            break;
        clean_parser(p);
        break;
    }
    //
    while (2600)
    {
        //////
        p = new_parser("12+$a;");
        if (read_test(p))
            break;
        char *sub = get_line_error(p);
        if (strcmp(sub, "12+$a;"))
            break;
        free(sub);
        count_lines(p, &pos);
        if (pos.line != 1 || pos.col != 4)
            break;
        clean_parser(p);
        break;
    }
    //
    while (2600)
    {
        //////
        p = new_parser("12abcd");
        if (!(begin_capture(p, "val") && readint(p) && begin_capture(p, "val2") && readid(p) && end_capture(p, "val2") && end_capture(p, "val")))
            break;
        char *sub = get_value(p, get_node(p, "val2"));
        if (strcmp(sub, "abcd"))
            break;
        free(sub);
        sub = get_value(p, get_node(p, "val"));
        if (strcmp(sub, "12abcd"))
            break;
        free(sub);
        clean_parser(p);
        break;
    }
}
