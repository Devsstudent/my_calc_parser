#include "my_calc.h"
#include <stdbool.h>
#include <ctype.h>

/*
ADD <- SPACE_OR_COMMENT '+"

SPACE_OR_COMMENT <- "//" [^\n]* '\n'
                                          / [ \t\r\n]+
*/
/*
Expr             ← Sum
Sum              ← Product (('+' / '-') Product)*
Product          ← Power (('*' / '/') Power)*
Power            ← Value ('^' Power)?
Value            ← Variable / Number / '(' Expr ')'
Variable         ← [a-zA-Z_]+ '=' Expr / [a-zA-Z]+
Number           ← [0-9]+
SPACE_OR_COMMENT ← '//' [^\n]* '\n' / [ \t\r\n]+
*/

void skip_spaces_or_comments(struct parser *p) {
	int save = p->current_pos;
	if (readtext(p, "//")) {
		while (p->content[p->current_pos] && p->content[p->current_pos] != '\n' && p->content[p->current_pos] != '\0') {
			if (p->current_pos > p->last_pos)
				p->last_pos = p->current_pos;
		}
	} else if (p->content[p->current_pos] && isspace(p->content[p->current_pos])) {
		while (p->content[p->current_pos] && isspace(p->content[p->current_pos])) {
			p->current_pos += 1;
			if (p->current_pos > p->last_pos) {
				p->last_pos = p->current_pos; 
			}
		}
	} else {
		p->current_pos = save;
	}
}

bool	read_var_name(struct parser *p){
	skip_spaces_or_comments(p);
	if (readrange(p, 'a', 'z') || readrange(p, 'A', 'Z') || readchar(p, '_')){
		while (readrange(p, 'a', 'z') || readrange(p, 'A', 'Z') || readchar(p, '_')){
		}
		return (true);
	}
	return (false);
}

bool	is_assignment(struct parser *p, struct scope *s) {

	int save_pos = p->current_pos;

	if (read_var_name(p))
		skip_spaces_or_comments(p);
		if ( readchar(p, '=')){
			if (expression(p, s)){
				return (true);
		}
	}
	p->current_pos = save_pos;
	return (false);
}

bool	variable(struct parser *p, struct scope *s){
	if (is_assignment(p, s)){
		return (true);
	}
	if (read_var_name(p)){
		return (true);
	}
	return (false);
}


bool	value(struct parser *p, struct scope *s){
	skip_spaces_or_comments(p);
	if (readint(p)){
		return (true);
	}
	if (expression(p, s)){
		return (true);
	}
	if (variable(p, s)) {
		return (true);
	}
	return (false);
}

bool	power(struct parser *p, struct scope *s){
	if (value(p, s)) {
		skip_spaces_or_comments(p);
		if (readchar(p, '^')) {
			if (!power(p, s)) {
				return (false);
			}
		}
		return (true);
	}
	return (false);
}

bool	product(struct parser *p, struct scope *s){
	if (power(p, s)){
		if (readchar(p, '*') || readchar(p, '/')) {
			if (!power(p, s)) {
				return (false);
			}
		}
		return (true);
	}
	return (false);
}

bool	sum(struct parser *p, struct scope *s){
	skip_spaces_or_comments(p);
	if (product(p, s)) {
		if (readchar(p, '+') || readchar(p, '-')) {
			if (!product(p,s)) {
				return (false);
			}
		}
		return (true);
	}
	return (false);
}

bool	expression(struct parser *p, struct scope *s){
	if (!sum(p, s)){
		return (false);
	}
	return (true);
}

int     my_calc(struct parser *p, struct scope *s){
	return (expression(p, s));
}


int main()
{ //...
	struct scope s;
	struct parser *p = new_parser("12;");
	if (my_calc(p, &s))
	{
//		if (s.current_val != 12)
//		error(2600, 0, "Erreur de calcul %d\n", s.current_val);
	}
	else
	{
		// gestion d'erreur
		char *errline = get_line_error(p);
		struct position pos;
		count_lines(p, &pos);
		fprintf(stderr, "Erreur line: %d, col: %d\n", pos.line, pos.col);
		fprintf(stderr, "%s\n", errline);
		for (int i = 0; i < pos.col - 1; i += 1)
			fprintf(stderr, " ");
		fprintf(stderr, "^\n");
	}
 }
