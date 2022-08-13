#include "minishell.h"

 /*
 *  Checks if there are two consecutive operators or more in token
 *  linked list. Except the case where the operators are '|' and '>'
 *  Why token type >= 4? bc operators enums begin at number 4.
 */

static int	consecutive_ops(t_token *token_node)
{
	if (token_node->next)
	{
		if (token_node->type == TRUNC && token_node->next->type == PIPE)
			return (FALSE);
		if ((token_node->type >= 4 && token_node->next->type >= 4)
			&& token_node->next->type != END)
			return (TRUE);
	}
	return (FALSE);
}

int	check_consecutives(t_token **token_lst)
{
	t_token	*temp;

	temp = *token_lst;
	while (temp)
	{
		if (consecutive_ops(temp) == TRUE)
		{
			printf("Syntax error near unexpected token `%s'\n", temp->str);
			return (1);
		}
		temp = temp->next;
	}
	//print_token(*token_lst);
	return (0);
}