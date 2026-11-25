#include "list.h"

void	ft_lstadd_back(t_list **lst, t_list *new_node)
{
	t_list	*cur;

	if (lst == NULL || new_node == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new_node;
		return ;
	}
	cur = *lst;
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = new_node;
}
