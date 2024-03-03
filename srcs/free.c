#include "ft_ls.h"

void	ft_lstdelone(t_recu *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	free(lst->pwd);
	freetab(lst->paths);
	freetab(lst->dirs);
	if (lst->padding)
	{
		free(lst->padding);
		lst->padding = NULL;
	}
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

void	freebigtab(char ***tab)
{
	if (tab == NULL)
		return ;

	int i = 0;
	while (tab[i])
	{
		if (tab[i] != NULL)
		{
			int j = 0;
			while (tab[i][j])
			{
				if (tab[i][j] != NULL)
				{
					free(tab[i][j]);
					tab[i][j] = NULL;
				}
				j++;
			}
			free(tab[i]);
			tab[i] = NULL;
		}
		i++;
	}
	free(tab);
	tab = NULL;
}

void	free_all(t_files *data, t_recu **recu)
{
	ft_lstclear(recu, free);
	freetab(data->toread);
}