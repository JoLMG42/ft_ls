#include "ft_ls.h"

void	big_print(char ***dirs, char *pwd, t_files *data, t_recu *lst)
{
	int	i = 0;
	int	pad = 0;
	
	while (dirs[i])
	{
		int j = 0;
		while (dirs[i][j])
		{
			struct	stat	info;
			char *tmp = ft_strjoin(ft_strdup(pwd), "/");
			tmp = ft_strjoin(tmp, dirs[i][j]);
			if (lstat(tmp, &info))
				return ;
			if (S_ISDIR(info.st_mode))
				write(STDOUT_FILENO, COLOR_BLUE, ft_strlen(COLOR_BLUE));
			else if (S_ISLNK(info.st_mode))
				write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
			else if (info.st_mode & S_IXUSR)
				write(STDOUT_FILENO, COLOR_GREEN, ft_strlen(COLOR_GREEN));
			ft_putstr(dirs[i][j]);
			write(STDOUT_FILENO, COLOR_RESET, ft_strlen(COLOR_RESET));
			if (lst->padding)
			{
				int space = lst->padding[j] - ft_strlen(dirs[i][j]) - 2;
				while (space > 0 && space < 1000)
				{
					ft_putchar_fd(' ', 1);
					space--;
				}
			}
			j++;
			ft_putstr("  ");
		}
		i++;
		if (dirs[i])
			ft_putchar_fd('\n', 1);
	}
}

void	print_in_col(t_files *data, char **recup, t_recu **recu)
{
	t_recu	*lst = *recu;

	while (lst)
	{
		ft_putstr(lst->pwd);
		ft_putstr(":\n");
		char	***newdirs = recup_nb_col(data, lst->dirs, recu, lst);
		big_print(newdirs, lst->pwd,  data, lst);
		ft_putstr("\n");
		ft_putstr("\n");
		lst = lst->next;
	}
}

void	print_list(t_files *data, t_recu **recu)
{
	t_recu	*lst = *recu;

	lst = *recu;
	if (data->l == true)
		print_more_infos(data, lst->paths, recu);
	else if (data->R == true)
		print_in_col(data, lst->paths, recu);
	else
	{
		lst = *recu;
		int flag = 0;
		if (lst->next)
			flag = 1;
		while (lst)
		{
			if (flag)
			{
				ft_putstr(lst->pwd);
				ft_putstr(":\n");
			}
			char ***newdirs = recup_nb_col(data, lst->dirs, recu, lst);
			big_print(newdirs, lst->pwd, data, lst);
			ft_putstr("\n\n");
			lst = lst->next;

		}
	}
}
