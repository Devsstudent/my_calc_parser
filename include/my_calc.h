#ifndef _MY_CALC_H
#define _MY_CALC_H
#include "my_parser.h"
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef enum leaf_type {
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	POWER,
	VAL
}	t_leaf_type;

typedef struct block {
	t_leaf_type		type;
	int				layer;
	float			value;
	struct block	*next;
}	t_block;

typedef struct expression {
	t_block		*head;
	int			size;
	bool		wasNumber;
	bool		wasSymbol;
	bool		wasVar;
	bool		wasOpenParenthese;
	bool		wasCloseParenthese;
	int			maxLayer;
}	t_expression;

typedef struct content {
	float value;
	char *content;
}	t_content;

typedef struct leaf {
	t_leaf_type	type;
	struct leaf *right;
	struct leaf *left;
	bool		head;
	t_content	content;
}	t_leaf;

struct def_list
{
    const char      *name;
    float	        val;
    struct def_list *next;
};

struct scope
{
    struct def_list     *defs;
    float            current_val;
	struct leaf			*tree;
	t_expression		*current_expr;
};


int     my_calc(struct parser *p, struct scope *s);
int read_space(struct parser *p);
bool	parse_current_expr(struct scope *s, char *line, int *i, int *layer);
#endif /* _MY_CALC_H */
