#include "my_calc.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* ITOA CODE */
static int	ft_power_static(int nb, int power)
{
	if (power < 0)
		return (0);
	else if (power < 1)
		return (1);
	else if (power < 2)
		return (nb);
	return (nb * ft_power_static(nb, power - 1));
}

static int	ft_size_and_tools(long long *temp, int *buff_size)
{
	int		size;
	long	buff_temp;

	size = 1;
	*buff_size = 1;
	if (*temp < 0)
	{
		size++;
		*temp = *temp * -1;
	}
	buff_temp = *temp;
	while (buff_temp > 9)
	{
		buff_temp = buff_temp / 10;
		size++;
		*buff_size = *buff_size + 1;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	int			size;
	int			i;
	int			buff_size;
	char		*res;
	long long	temp;

	i = 0;
	temp = (long long) n;
	size = ft_size_and_tools(&temp, &buff_size);
	res = (char *) malloc(sizeof(*res) * (size + 1));
	if (!res)
		return (NULL);
	if (size - buff_size > 0)
		res[i++] = '-';
	while (i < size)
	{
		res[i] = temp / ft_power_static(10, buff_size - 1) + 48;
		temp = temp % ft_power_static(10, buff_size - 1);
		i++;
		buff_size--;
	}
	res[i] = '\0';
	return (res);
}

/*END OF ITOA CODE */

/* USEFUL */

t_block *new_block(t_leaf_type type, char *content, int layer){
	t_block *new_block = malloc(sizeof(t_block));

	if (!new_block) {
		return (NULL);
	}

	new_block->type = type;
	new_block->content = content;
	new_block->next = NULL;
	new_block->layer = layer;
	return (new_block);
}

bool	add_block_to_expression(t_expression *expr, t_block *new_block){
	if (!expr->head){
		expr->head = new_block;
	}
	expr->next = new_block;
	expr->size += 1;
}


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
	int save = p->current_pos;
	while (read_space(p)){}
	if (readrange(p, '0', '9')){
    	while (readrange(p, '0', '9')){
    	}
    	return (1);
	}
	p->current_pos = save;
	//printf("False from read_ind\n");
	return (false);
}


int read_basic_symbol(struct parser *p){
	int save = p->current_pos;
	while (read_space(p)){}
	if (readchar(p, '+') || readchar(p, '-')){
		return (true);
	}
	p->current_pos = save;
	//printf("False from symbol\n");
	return (false);
}

int read_priority_symbol(struct parser *p){
	int save = p->current_pos;
	while (read_space(p)){}
	if (readchar(p, '/') || readchar(p, '*') || readchar(p, '%')){
		return (true);
	}
	p->current_pos = save;
	//printf("False from priority\n");
	return (false);
}

int read_assign_symbol(struct parser *p){
	int save = p->current_pos;
	while (read_space(p)){}
	if (readchar(p, '=')){
		return (true);
	}
	p->current_pos = save;
	//printf("False from assinn %c\n", p->content[save]);
	return (false);
}

int read_end_symbol(struct parser *p){
	int save = p->current_pos;
	while (read_space(p)){}
	if (readchar(p, ';')){
		return (true);
	}
	p->current_pos = save;
	//printf("False from end_symbol\n");
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
		//printf("False from read_space\n");
		return (false);
	}
}

int read_var(struct parser *p){
	int save = p->current_pos;
	while (read_space(p)){}
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

int read_expression(struct parser *p, char *idx){
	if (!begin_capture(p, idx))
	{
		return (false);
	}
	while(read_int(p) \
		|| read_var(p) \
		|| read_basic_symbol(p) \
		|| read_priority_symbol(p) \
		|| read_assign_symbol(p) \
		|| read_space(p))
	{
	}
	if (read_end_symbol(p)){
		if (!end_capture(p, idx))
		{
			return (false);
		}
		return (true);
	}
	return (false);
}

int parse(struct parser *p, int nb_expression){
	int i = 0;

	while (i < nb_expression) {
		//Maybe array of name for the free
		if (!read_expression(p, ft_itoa(i))){
			return (false);
		}
		i++;
	}
	return (true);
}

bool push_to_def(struct scope *s, char *name, int value){
	(void) s;
	struct def_list *var = calloc(1, sizeof(struct def_list));
	if (!var){
		return (false);
	}

	var->name = name;
	var->val = value;
	var->next = NULL;
	return true;
}

char *get_var_name(struct parser *p, const struct capture_list *node, int size){
	return (strndup(&p->content[node->begin], size));
}

bool isAssignment(struct parser *p) {
	if (read_var(p) \
	&& (read_assign_symbol(p) \
	|| read_assign_symbol(p)) \
	&& (read_int(p) \
		|| read_var(p) \
		|| read_basic_symbol(p) \
		|| read_priority_symbol(p) \
		|| read_space(p)))
	{
		return (true);
	}
	return (false);
}

bool is_value(struct parser *p){
	if (read_int(p)){
		if (read_end_symbol(p)){
			return (true);
		}
	}
	return (false);
}

bool is_expression(struct parser *p) {

	if ((read_int(p) \
		|| read_var(p) || read_space(p)) \
		&& (read_basic_symbol(p) \
		|| read_priority_symbol(p)))
	{
		while ((read_int(p) \
		|| read_var(p) || read_space(p)) \
		&& (read_basic_symbol(p) \
		|| read_priority_symbol(p)))
		{}
	//	printf("ah oui |%c|\n", p->content[p->current_pos]);
		if (read_end_symbol(p)){
			return (true);
		}
	}
//	printf("ah oui |%c| %i\n", p->content[p->current_pos], read_space(p));
	return (false);
}

bool doAssignement(struct parser *p, struct capture_list *node, struct scope *s)
{
	char *var_name;

	p->current_pos = node->begin;
	if (read_var(p)) {
		var_name = get_var_name(p, node, p->current_pos - node->begin);
		printf("we have a var %s\n", var_name);
		if (read_space(p) || read_assign_symbol(p)){
			//Do Calc
		}
	}
	else
	{
		return (false);
	}

/*
	&& (read_assign_symbol(p) || (read_space(p) || read_assign_symbol(p)))
	&& (read_int(p) \
		|| read_var(p) \
		|| read_basic_symbol(p) \
		|| read_priority_symbol(p) \
		|| read_space(p))\)
	{
		return (true);
	}
*/
	return (false);
}

bool doCalc(struct parser *p, struct capture_list *node, struct scope *s){

	int value = 0;
	while (read_int(p)){
		value += atoi(p);
		while (read_space(p) || read_basic_symbol(p)){

		}
	}
}

bool ast(struct scope *s, struct capture_list *node, int pos){
	//Browser util find a value
	if (s->expression[pos] <= '9' && s->expression[pos] >= '0'){
		s->
	}

	if (!ast(p, s, pos)){
		return (false);
	}
}

t_leaf *new_leaf(t_content *content, t_leaf_type type){
	t_leaf *new_leaf = calloc(1, sizeof(t_leaf));
	if (!new_leaf){
		return (NULL);
	}
	t_leaf->type = type;
	leaf->right = NULL;
	leaf->left = NULL;
	
}

bool isparentheses(char a){
	if (a == '(' || a == ')')
		return (true);
	return (false);
}

bool issymbol(char a){
	if (a == '/' || a == '+' || a == '-' || a == '^' || a == '%')
		return (true);
	return (false);
}

bool isValidNextToNumber(char a){
	if (issymbol(a) || isspace(a))
		return (true);
	return (false);
}

bool get_value(int &layer, int &i);

bool get_symbol(int &layer, int &i);

bool get_parentheses(int &layer, int &i); //celle-ci pepite //rappeller is digit et isymbol

//bool is variable() -> getValue de la def_list // faire une fonciton is variable [a-z][A-Za-z_]*
//Recup l'index du debut et de la fin pour cree le nom de la variable a passer a get_value_from_variable(s)


bool	parse_current_expr(struct scope *s, char *line){
	int i = 0;
	int layer = 0;

//s->current_expr

	while (line[i]){
		if (line[i] == ';')
			break ;

		if (isdigit(line[i])){
			value = atof(line[i]);
			while (line[i] && isdigit(line[i])){
				i++;
			}
			if (line[i] == '.'){
				i++;
			}
			if (line[i] && isdigit(line[i])){
				while (line[i] && isdigit(line[i])){
					i++;
				}
			}
			if (line[i] && !isValidNextToNumber(line[i]))
				return (false);
			//New block val = value
			continue ;
		}
		if (issymbol(line[i])){
			if (line[i] == '/'){
				//new block division
			}
			if (line[i] == '*'){
				//new block division
			}
			if (line[i] == '+'){
				//new block division
			}
			if (line[i] == '-'){
				//new block division
			}
			if (line[i] == '^'){
				//new block power
			}
		}
		if (line[i] == '('){
			while (line[i] && line [i] != ')'){
				i++;
			}
			layer++;
			continue ;
		}
		if ((line[i] >= 9 && line[i] <= 13) || line[i] == ' '){
			i++;
			continue ;
		}
		i++;
	}
	if (line[i] != ';')
		return (false);
}


//definir si c'est une expression ou pas
int     my_calc(struct parser *p, struct scope *s) {
	int nb_expression = count_expression(p);

	printf("Nombres d'expression dans le content de mon parseur : %i\n\nParser: ||%s||\n", nb_expression, p->content);

	if (!parse(p, nb_expression)){
		return (false);
	}

	int i = 0;

	s->tree = calloc(1, sizeof(t_leaf));
	s->tree->head = true;
	if (!s->tree)
		return (false);

	while (i < nb_expression) {
		struct capture_list *node = get_node(p, ft_itoa(i));

		p->current_pos = node->begin;
		if (is_value(p)) {
			s->current_val = atoi(&p->content[node->begin]);
			printf("idx : %i expr : %s\n", i, get_value(p, node));
			i++;
			continue ;
		}
		/*
		if (isAssignment(p, pos)){ --> node begin ensuite parse current expr
			printf("This is assignment ->\n");
			p->current_pos = node->begin;
			doAssignement(p, node, s);
		}*/

		s->current_expr = calloc(1, sizeof(t_expression));
		if (!s->current_expr)
			return (false);

		char *line = get_value(p, node);

		if (!parse_current_expr(s, line))
			return (false);
		if (is_expression(p)) {
			fill_expression
			if (!ast(s, node, node->begin))
				return (false);
			printf("This is expression ->\n");
		}
		printf("idx : %i expr : %s\n", i, get_value(p, node));
		i++;
		free(line);
		free(s->current_expr);
	}
	return (true);
}

int main()
{ //...
	struct scope s;
	struct parser *p = new_parser("12/125*9000;12313 + 2600 - 36;a = 12 - 12 + 2;a = a + 3; 				123123123;");

	//p = new_parser("(2 + 3 * 2 --+- 5) ^ 5 / 3 ;")
	if (my_calc(p, &s))
	{
		fprintf(stderr, "current value : %li\n", s.current_val);
		//if (s.current_val != 12)
		//	fprintf(stderr, "Erreur de calcul %ld\n", s.current_val);
	}
	else
	{
		// gestion d'erreur
		char *errline = get_line_error(p);
		struct position pos;
		count_lines(p, &pos); fprintf(stderr, "Erreur line: %d, col: %d\n", pos.line, pos.col);
		fprintf(stderr, "%s\n", errline);
		for (int i = 0; i < pos.col - 1; i += 1)
			fprintf(stderr, " ");
		fprintf(stderr, "^\n");
	}
}
