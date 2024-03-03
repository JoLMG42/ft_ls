#include "ft_ls.h"

void	ft_lstdelone(t_recu *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	free(lst->pwd);
	freetab(lst->paths);
	freetab(lst->dirs);
	free(lst);
}

void	ft_lstclear(t_recu **lst, void (*del)(void *))
{
	t_recu	*list;
	t_recu	*tmp;

	if (!lst || !del)
		return ;
	list = *lst;
	while (list)
	{
		tmp = list->next;
		ft_lstdelone(list, del);
		list = tmp;
	}
	*lst = NULL;
}

void	freetab(char **tab)
{
	int	i = 0;

	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_all(t_files *data, t_recu **recu)
{
	ft_lstclear(recu, free);
	freetab(data->toread);
}