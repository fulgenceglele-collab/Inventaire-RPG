
#include <stdlib.h>
#include "list.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst == NULL)
		return ;
	if (del != NULL && lst->content != NULL)
		del(lst->content);
	free(lst);
}
