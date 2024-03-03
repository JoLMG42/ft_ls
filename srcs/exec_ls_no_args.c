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
		return 0;
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
		if (S_ISDIR(info.st_mode))
			write(STDOUT_FILENO, COLOR_BLUE, ft_strlen(COLOR_BLUE));
		else if (S_ISLNK(info.st_mode))
			write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
		else if (info.st_mode & S_IXUSR)
			write(STDOUT_FILENO, COLOR_GREEN, ft_strlen(COLOR_GREEN));
		ft_putstr(tab[i]);
		write(STDOUT_FILENO, COLOR_RESET, ft_strlen(COLOR_RESET));
		i++;
		if (tab[i])
			ft_putstr("  ");
	}
	ft_putstr("\n");
}