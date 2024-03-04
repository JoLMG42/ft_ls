#include "ft_ls.h"

void	big_print(char ***dirs, char *pwd, t_files *data, t_recu *lst)
{
	int	i = 0;

	(void)data;
	while (dirs[i])
	{
		int j = 0;
		while (tablen(dirs[i]) > 0 && dirs[i][j])
		{
			struct	stat	info;
			struct	stat	links;
            
            char *tmp = ft_strjoin(ft_strdup(pwd), "/");
            tmp = ft_strjoin(tmp, dirs[i][j]);
            if (lstat(dirs[i][j], &info) != 0)
            {
			    if (lstat(tmp, &info) && !is_a_file(dirs[i][j]))
				    return ;
            }
			if (!data->f && (info.st_mode & S_IFMT) == S_IFBLK)
				write(STDOUT_FILENO, COLOR_BROWN, ft_strlen(COLOR_BROWN));
            else if (!data->f && (info.st_mode & S_IFMT) == S_IFCHR)
				write(STDOUT_FILENO, COLOR_BROWN, ft_strlen(COLOR_BROWN));
			else if (!is_a_file(tmp) && S_ISDIR(info.st_mode) && !data->f && data->C)
				write(STDOUT_FILENO, COLOR_BLUE, ft_strlen(COLOR_BLUE));
            else if (S_ISLNK(info.st_mode) && !data->f && data->C)
            {
                if (lstat(tmp, &links))
				    write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
                else
				    write(STDOUT_FILENO, COLOR_CYAN, ft_strlen(COLOR_CYAN));
            }
			else if (info.st_mode & S_IXUSR && !data->f && data->C)
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
			free(tmp);
		}
		i++;
		if (dirs[i])
			ft_putchar_fd('\n', 1);
	}
}

void	print_in_col(t_files *data, t_recu **recu)
{
	t_recu	*lst = *recu;

	while (lst)
	{
		if (!is_a_file(lst->pwd))
		{
			ft_putstr(lst->pwd);
			ft_putstr(":\n");
		}
		char	***newdirs = recup_nb_col(data, lst->dirs, lst);
		big_print(newdirs, lst->pwd,  data, lst);
		ft_putstr("\n");
		freebigtab(newdirs);
		lst = lst->next;
		if (lst)
			ft_putstr("\n");
	}
}

void	print_list(t_files *data, t_recu **recu)
{
	t_recu	*lst = *recu;

	lst = *recu;
	if (data->l == true)
		print_more_infos(data, recu);
	else if (data->R == true)
		print_in_col(data, recu);
	else
	{
		lst = *recu;
		int flag = 0;
		if (lst->next)
			flag = 1;
		while (lst)
		{
			if (flag && !is_a_file(lst->pwd))
			{
				ft_putstr(lst->pwd);
				ft_putstr(":\n");
			}
			char ***newdirs = recup_nb_col(data, lst->dirs, lst);
			big_print(newdirs, lst->pwd, data, lst);
			freebigtab(newdirs);
			lst = lst->next;
            if (lst)
                ft_putstr("\n");
            ft_putstr("\n");
		}
	}
}

void	print_msg_help(void)
{
	char	mandato[] = "\nUsage: ls [OPTION]... [FILE]...\nList information about the FILEs (the current directory by default).\nSort entries alphabetically\n\nMandatory part arguments:\n -a    do not ignore entries starting with .\n -l    use a long listing format\n \
					\r -r    reverse order while sorting\n -R    list subdirectories recursively\n -t    sort by time, newest first\n";
	char	bonus[] = "\nBonus part arguments:\n -g    like -l, but do not list owner\n -G    with a long format, do not show group informations\n -f    do not sort, enable -aU, disable -l -C\n -U    do not sort; list entries in directory order\n -u    with -lt: sort by, and show, access time;\n \
				        \r         with -l: show access time and sort by name;\n         otherwise: sort by access time, newest first\n -C    show with colors\n -o    show long format without group informations\n";
	ft_putstr(mandato);
	ft_putstr(bonus);
}
