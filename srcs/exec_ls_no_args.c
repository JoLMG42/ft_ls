#include "ft_ls.h"

int	exec_ls_no_args(void)
{
	struct dirent 	*tab;
	DIR 		*actu = opendir("./");
	char 		**store;
	int 		size = 0;

	tab = readdir(actu);
	while (tab)
	{
		size++;
		tab = readdir(actu);
	}
	store = malloc(sizeof(char *) * size + 50);
	if (!store)
	{
		closedir(actu);
		ft_putstr_fd("Error malloc in exec_ls_no_args\n", 2);
		return 0;
	}
	closedir(actu);
	actu = opendir("./");
	tab = readdir(actu);
	int i = 0;
	while (tab)
	{
		if (tab->d_name[0] != '.')
		{
			store[i] = ft_strdup(tab->d_name);
			i++;
		}
		tab = readdir(actu);
	}
	store[i] = 0;
	store = ft_swap(i, store);
	one_line_print(store, store);
	freetab(store);
	closedir(actu);
	return (0);
}

void	one_line_print(char **d, char **tab)
{
	int	i = 0;
	struct	stat	info;

	while (tab[i])
	{
		if (lstat(d[i], &info))
			return ;
		ft_putstr(tab[i]);
		i++;
		if (tab[i])
			ft_putstr("  ");
	}
	ft_putstr("\n");
}