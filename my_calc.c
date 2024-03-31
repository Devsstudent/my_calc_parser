#include "my_calc.h"

static inline size_t	ft_strlen(const char *s)
{
	size_t	size;

	size = 0;
	while (s[size] != '\0')
		size++;
	return (size);
}

static inline int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
	{
		if ((size_t)(i) == n - 1)
			break ;
		i++;
	}
	return ((unsigned char) s1[i] -(unsigned char) s2[i]);
}

static int      ft_power_static(int nb, int power)
{
        if (power < 0)
                return (0);
        else if (power < 1)
                return (1);
        else if (power < 2)
                return (nb);
        return (nb * ft_power_static(nb, power - 1));
}


bool push_to_def(struct scope *s, char *name, long int value){
        struct def_list *var = calloc(1, sizeof(struct def_list));
        if (!var || !s){
                return (false);
        }
        
        var->name = name;
        var->val = value;
        var->next = NULL;
        
        if (!s->defs){
                s->defs = var;
                return (true);
        }
        
        struct def_list *buf = s->defs;

		if (!ft_strncmp(name, buf->name, ft_strlen(name))){
			buf->val = value;
			return (true);
		}
        while (buf->next) {
			if (!ft_strncmp(name, buf->name, ft_strlen(name))){
				buf->val = value;
				return (true);
			}
            buf = buf->next;
        }
        buf->next = var;
        return true;
}

bool get_var_from_def_list(struct scope *s, char *name, long int *gotVal){

        if (!s) {
                return (false);
        }
        struct def_list *buf = s->defs;

        while (buf) {
                if (!ft_strncmp(buf->name, name, ft_strlen(name))) {
                        *gotVal = buf->val;
                        return (true);
                }
                buf = buf->next;
        }
        return (false);
}

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
//	printf("%c %s\n", p->content[p->current_pos], &p->content[p->current_pos]);
	if (readtext(p, "//")) {
		while (p->content[p->current_pos] && p->content[p->current_pos] != '\n' && p->content[p->current_pos] != '\0') {
			p->current_pos += 1;
			if (p->current_pos > p->last_pos)
				p->last_pos = p->current_pos;
		}
		if (p->content[p->current_pos] && isspace(p->content[p->current_pos])) {
			while (p->content[p->current_pos] && isspace(p->content[p->current_pos])) {
				p->current_pos += 1;
				if (p->current_pos > p->last_pos) {
					p->last_pos = p->current_pos; 
				}
			}
		}
		return ;
	}
	p->current_pos = save;
	if (p->content[p->current_pos] && isspace(p->content[p->current_pos])) {
		while (p->content[p->current_pos] && isspace(p->content[p->current_pos])) {
			p->current_pos += 1;
			if (p->current_pos > p->last_pos) {
				p->last_pos = p->current_pos; 
			}
		}
		return ;
	}
	p->current_pos = save;
}

int		handle_sign(struct parser *p){
	int sign = 1;
	while (p->content[p->current_pos] && (p->content[p->current_pos] ==  '-' || p->content[p->current_pos] ==  '+')) {
		if (p->content[p->current_pos] ==  '-'){
			sign *= -1;
		}
		p->current_pos += 1;
		if (p->current_pos > p->last_pos) {
			p->last_pos = p->current_pos; 
		}
	}
	return (sign);
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

	int save_pos_start_var = p->current_pos;
	if (read_var_name(p)) {
		int save_pos_end_var = p->current_pos;
		skip_spaces_or_comments(p);
		if (readchar(p, '=')) {
			if (expression(p, s)){
				char *var_name = my_strndup(&p->content[save_pos_start_var], save_pos_end_var - save_pos_start_var);
				if (!var_name)
					return (false);
				printf("assigned: %li name: %s\n", s->current_val, var_name);
				if (!push_to_def(s, var_name, s->current_val))
					return (false);
				return (true);
			}
		}
	}
	p->current_pos = save_pos_start_var;
	return (false);
}

bool getValFromVar(struct parser *p, struct scope *s, long int *val){
		struct capture_list *node = get_node(p, "var");
		if (!node)
			return (false);
		char *var_name = get_value(p, node);
		if (!var_name)
			return (false);
		if (!get_var_from_def_list(s, var_name, val))
		{
		        free(var_name);
		        return (false);
		}
		free(var_name);
		return (true);
}


bool	variable(struct parser *p, struct scope *s) {

	if (is_assignment(p, s)){
		return (true);
	}
	
	if (( p->content[p->current_pos] >= 'a' && p->content[p->current_pos] <= 'z') \
	|| (p->content[p->current_pos] >= 'A' && p->content[p->current_pos] <= 'Z') \
	|| p->content[p->current_pos] == '_')
	{
		if (begin_capture(p, "var") && read_var_name(p) && end_capture(p, "var")) {
			if (!getValFromVar(p, s, &s->current_val))
				return (false);
			return (true);
		}
	}
	return (false);
}

bool	value(struct parser *p, struct scope *s){
	skip_spaces_or_comments(p);

	int sign = handle_sign(p);

	skip_spaces_or_comments(p);
	//begin capture
	if (isdigit(p->content[p->current_pos])){
		s->lionel = true;
		if (begin_capture(p, "val") && readint(p) && end_capture(p, "val")){
			struct capture_list *node = get_node(p, "val");
			if (!node)
				return (false);
			char *value = get_value(p, node);
			if (!value)
				return (false);
			s->current_val = atoi(value) * sign;
			free(value);
			return (true);
		}
	}
	if (readchar(p, '(')) {
		if (expression(p, s)) {
			if (!readchar(p, ')'))
				return (false);
			s->current_val = s->current_val * sign;
			return (true);
		}
		return (false);
	}
	if (variable(p, s)) {
		return (true);
	}
	return (false);
}

bool	power(struct parser *p, struct scope *s){
	if (value(p, s)) {
		skip_spaces_or_comments(p);
		while (readchar(p, '^')) {
			long int previous = s->current_val;
			if (!power(p, s)) {
				return (false);
			}
			long int next = s->current_val;
			if (next < 0)
				return (false);
			s->current_val = ft_power_static(previous, next);
			skip_spaces_or_comments(p);
			skip_spaces_or_comments(p);
		}
		return (true);
	}
	return (false);
}

bool	product(struct parser *p, struct scope *s){
	skip_spaces_or_comments(p);
	if (power(p, s)){
		skip_spaces_or_comments(p);
		char _c = p->content[p->current_pos];
		while (readchar(p, '*') || readchar(p, '/') || readchar(p, '%')) {
			long int previous = s->current_val;

			if (!power(p, s)) {
				return (false);
			}

			long int next = s->current_val;
			if (_c == '*') {
				s->current_val = previous * next; }
			else if (_c == '/') {
				if (next == 0)
					return (false);
				s->current_val = previous / next;
			}
			else if (_c == '%') {
				if (next <= 0)
					return (false);
				s->current_val = previous % next;
			}
			skip_spaces_or_comments(p);
			_c = p->content[p->current_pos];
			skip_spaces_or_comments(p);
		}
		return (true);
	}
	return (false);
}

bool	sum(struct parser *p, struct scope *s){
	skip_spaces_or_comments(p);
	if (product(p, s)) {
		skip_spaces_or_comments(p);
		char _c = p->content[p->current_pos];
		while (readchar(p, '+') || readchar(p, '-')) {
			long int previous = s->current_val;
			if (!product(p, s)) {
				return (false);
			}
			long int next = s->current_val;
			if (_c == '+')
				s->current_val = previous + next;
			else if (_c == '-')
				s->current_val = previous - next;
			skip_spaces_or_comments(p);
			_c = p->content[p->current_pos];
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

int     my_calc(struct parser *p, struct scope *s) {
	s->current_val = 0;
	s->defs = NULL;
	bool comment = false;
	while (p->content[p->current_pos]){
		s->lionel = false;
		int save = p->current_pos;
		if (readtext(p, "//")) {
			comment = true;
			p->current_pos = save;
		}
		if (!expression(p, s)) {
			if (!p->content[p->current_pos]){
				if (readchar(p, ';') || comment)
					break ;
			}
			return (false);
		}
		skip_spaces_or_comments(p);
		if (!s->lionel && !p->content[p->current_pos + 1]){
			return (true);
		}
		if (!readchar(p, ';') && !comment)
			return (false);
	}
	return (true);
}
/*
int main()
{ //...
	struct scope s;
	struct parser *p = new_parser("  14  \t  \n/ \n2 \n; \n");
	if (my_calc(p, &s))
	{
		printf("no fail, current : %li\n", s.current_val);
//		if (s.current_val != 12)
//		error(2600, 0, "Erreur de calcul %d\n", s.current_val);
	}
	else
	{
		struct def_list *def = s.defs;
		while (def) {
			printf("%s %li\n", def->name, def->val);
			def = def->next;
		}
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
 }*/
