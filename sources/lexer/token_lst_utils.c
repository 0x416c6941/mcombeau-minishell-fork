/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexa <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 23:50:28 by alexa             #+#    #+#             */
/*   Updated: 2022/09/16 00:06:45 by alexa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lst_new_token(char *str, int type, int status)
{
	t_token	*new_node;

	new_node = (t_token *)malloc(sizeof(t_token));
	if (!(new_node))
		return (NULL);
	new_node->str = str;
	new_node->type = type;
	new_node->status = status;
	new_node->join = false;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	lst_add_back_token(t_token **alst, t_token *new_node)
{
	t_token	*start;

	start = *alst;
	if (start == NULL)
	{
		*alst = new_node;
		return ;
	}
	if (alst && *alst && new_node)
	{
		while (start->next != NULL)
			start = start->next;
		start->next = new_node;
		new_node->prev = start;
	}
}

void	lstdelone_token(t_token *lst, void (*del)(void *))
{
	if (del && lst && lst->str)
	{	
		(*del)(lst->str);
		lst->str = NULL;
	}
	free(lst);
}

void	lstclear_token(t_token **lst, void (*del)(void *))
{
	t_token	*tmp;

	tmp = NULL;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		lstdelone_token(*lst, del);
		*lst = tmp;
	}
}
