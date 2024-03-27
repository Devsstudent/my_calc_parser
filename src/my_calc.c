#include "my_calc.h"
#include <errno.h>
#include <stdbool.h>
#include <mach/error.h> /*#include <error.h>*/
#include <stdio.h>

int                    count_expression(struct parser *p)
{
    int count = 0;
    for (int idx = 0; p->content[idx] != '\0'; idx += 1)
    {
        if (p->content[idx] == ';')
        {
			count++;
        }
    }
	return (count);
}

int                     read_int(struct parser *p)
{
    (void) p;
	if (readrange(p, '0', '9')){
    	while (readrange(p, '0', '9')){
    	}
    	return (1);
	}
	return (false);
}


int read_basic_symbol(struct parser *p){
	int save = p->current_pos;
	if (readchar(p, '+') || readchar(p, '-')){
		return (true);
	}
	p->current_pos = save;
	return (false);
}

int read_priority_symbol(struct parser *p){
	int save = p->current_pos;
	if (readchar(p, '/') || readchar(p, '*') || readchar(p, '%')){
		return (true);
	}
	p->current_pos = save;
	return (false);
}

int read_assign_symbol(struct parser *p){
	int save = p->current_pos;
	if (readchar(p, '=')){
		return (true);
	}
	p->current_pos = save;
	return (false);
}

int read_end_symbol(struct parser *p){
	int save = p->current_pos;
	if (readchar(p, ';')){
		return (true);
	}
	p->current_pos = save;
	return (false);
}

int read_space(struct parser *p){
	int save = p->current_pos;
	if (readrange(p, 9, 13) || readchar(p, ' ')){
		while (readrange(p, 9, 13) || readchar(p, ' ')){
		}
		return (true);
	}
	else{
		p->current_pos = save;
		return (false);
	}
}

int read_var(struct parser *p){
	int save = p->current_pos;
	if (readrange(p, 'a', 'z') || readrange(p, 'A', 'Z')){
		while (readrange(p, 'a', 'z') || readrange(p, 'A', 'Z')){
		}
		return (true);
	}
	else{
		p->current_pos = save;
		return (false);
	}
}

int read_expression(struct parser *p){
	if (!begin_capture())
	{
		return (false);
	}
	while (read_int(p) \
		|| read_var(p) \
		|| read_basic_symbol(p) \
		|| read_priority_symbol(p) \
		|| read_assign_symbol(p) \
		|| read_space(p))
	{
	}
	if (read_end_symbol(p)){
		return (true);
	}
	return (false);
}

int parse(struct parser *p){
	int i;

	int nb_expression = count_expression(p);
	i = 0;
	printf("Nombres d'expression dans le content de mon parseur : %i\n\nParser: ||%s||\n", nb_expression, p->content);
	while (i < nb_expression){
		if (!read_expression(p)){
			return (false);
		}
		i++;
	}
	return (true);
}

int     my_calc(struct parser *p, struct scope *s){
	// Should make a function parse_parser
	(void) s;
	return (parse(p));
}

int main()
{ //...
	struct scope s;
	struct parser *p = new_parser("12;12313;123123123;123123123       				;123123123;");

	//p = new_parser("(2 + 3 * 2 --+- 5) ^ 5 / 3 ;")
	if (my_calc(p, &s))
	{
		if (s.current_val != 12)
			fprintf(stderr, "Erreur de calcul %ld\n", s.current_val);
	}
	else
	{
		// gestion d'erreur
		char *errline = get_line_error(p);
		struct position pos;
		count_lines(p, &pos); fprintf(stderr, "Erreur line: %d, col: %d\n", pos.line, pos.col);
		fprintf(stderr, "%s\n", errline);
		for (int i = 0; i < pos.col - 1; i += 1
		fprintf(stderr, " ");
		fprintf(stderr, "^\n");
	}
}
