#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include "ft_ls.h"




void	one_line_print(char **, char**);
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
	// i = 0;
	// while (store[i])
	// {
	// 	if (store[i][0] != '.')
	// 		printf("%s\n", store[i]);
	// 	i++;
	// }
	return (0);
}

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



void	add_maillon(t_recu **head_ref, char *new_data, char **all, char **paths)
{	
	t_recu	*new_node = malloc(sizeof(t_recu));
	t_recu	*last = *head_ref;
	
	new_node->pwd = ft_strdup(new_data);
	new_node->dirs = ft_strduptab(all);
	new_node->paths = ft_strduptab(paths);
	new_node->next = NULL;
	if (*head_ref == NULL)
	{
		*head_ref = new_node;
        	return;
	}
	while (last->next != NULL)
		last = last->next;
	last->next = new_node;
}

char	**add_dir_file(t_files *data, char *dir, t_recu **recu, int mode, char *oldpwd)
{
	char		*path;
	struct	dirent	*dirs;
	struct	dirent	*dirs2;
	DIR		*tmp = opendir(dir);
	DIR		*tmp2 = opendir(dir);
	char		**all;
	char		**foradd;

	path = NULL;
	if (!tmp)
		return NULL;
	int c = 0;
	while ((dirs = readdir(tmp)))
		c++;
	closedir(tmp);
	all = malloc(sizeof(char *) * (c + 1));
	foradd = malloc(sizeof(char *) * (c + 1));
	int i = 0;
	int j = 0;
	while ((dirs2 = readdir(tmp2)))
	{
		if (ft_strcmp(dirs2->d_name, ".") != 0 && ft_strcmp(dirs2->d_name, "..") != 0)
		{
			path = ft_strjoin(path, dir);
			if (ft_strcmp(dir, "./") != 0 && ft_strcmp(dir, "../") != 0)
				path = ft_strjoin(path, "/");
			path = ft_strjoin(path, dirs2->d_name);
			if (dirs2->d_name[0] == '.' && data->a == false)
				;
			else
			{
				all[i] = ft_strdup(path);
				foradd[j] = ft_strdup(dirs2->d_name);
				j++;
				i++;
			}
			free(path);
			path = NULL;
		}
		else
		{
			if (data->a == true)
			{
				all[i] = ft_strdup(dirs2->d_name);
				foradd[j] = ft_strdup(dirs2->d_name);
				i++;
				j++;
			}
		}

	}
	closedir(tmp2);
	all[i] = 0;
	all = ft_swap(i, all);
	foradd[j] = 0;
	foradd = ft_swap(j, foradd);
	
	if (data->t)
	{
		all = sort_by_time(tablen(all), all, NULL, 0);
		foradd = sort_by_time(tablen(foradd), foradd, dir, 1);
	}
	// int u = 0;
	// while (all[u])
	// {
	// 	printf("all[u] = %s\n", all[u]);
	// 	u++;
	// }
	// printf("dir = %s\n", dir);
	add_maillon(recu, dir, foradd, all);
	freetab(foradd);
	i = 0;
	while (all[i])
	{
		if (ft_strcmp(all[i], ".") != 0 && ft_strcmp(all[i], "..") != 0)
		{
			path = ft_strjoin(path, all[i]);
			if (!is_a_file(all[i]))
				add_dir_file(data, path, recu, 0, NULL);
			free(path);
			path = NULL;
		}
		i++;
	}
	freetab(all);
	return (NULL);
}



void	recursive(t_files *data, t_recu **recu)
{
	t_recu	*tmp;
	char	*oldpwd = NULL;
	
	if (data->t)
		data->toread = sort_by_time(tablen(data->toread), data->toread, NULL, 0);
	int k = 0;
	while (data->toread[k])
	{
		add_dir_file(data, data->toread[k], recu, 0, oldpwd);
		k++;
	}

	 /*t_recu *lst = *recu;

	 while (lst)
	 {
	 	printf("%s\n", lst->pwd);
	 	int i = 0;
	 	while (lst->dirs[i])
	 	{
	 		printf("%s ", lst->dirs[i]);
	 		i++;
	 	}
	 	printf("\n\n");
	 	lst = lst->next;
	 }*/

}

void	optionR(t_files *data, t_recu **recu)
{
	int i = 0;

	recursive(data, recu);
}

char	*fill_recu(t_files *data, t_recu **recu, char *dir)
{
	char		*path;
	struct	dirent	*dirs;
	struct	dirent	*dirs2;
	DIR		*tmp = opendir(dir);
	DIR		*tmp2 = opendir(dir);
	char		**all;
	char		**foradd;

	path = NULL;
	if (!tmp)
		return NULL;
	int c = 0;
	while ((dirs = readdir(tmp)))
		c++;
	closedir(tmp);
	all = malloc(sizeof(char *) * (c + 1));
	foradd = malloc(sizeof(char *) * (c + 1));
	int i = 0;
	while ((dirs2 = readdir(tmp2)))
	{
		//if (strcmp(dirs2->d_name, ".") != 0 && strcmp(dirs2->d_name, "..") != 0)
		//{
			path = ft_strjoin(path, dir);
			if (path[ft_strlen(path)-1] != '/')
				path = ft_strjoin(path, "/");
			path = ft_strjoin(path, dirs2->d_name);
			if (dirs2->d_name[0] == '.' && data->a == false)
				;
			else
			{
				all[i] = ft_strdup(path);
				foradd[i] = ft_strdup(dirs2->d_name);
				i++;
			}
			free(path);
			path = NULL;
		//}
	}
	closedir(tmp2);
	all[i] = 0;
	all = ft_swap(i, all);
	foradd[i] = 0;
	foradd = ft_swap(i, foradd);
	int u = 0;
	if (data->t == true)
	{
		all = sort_by_time(tablen(all), all, dir, 0);
		foradd = sort_by_time(tablen(foradd), foradd, dir, 1);
	}
	if (data->r == true)
	{
		all = reverse_tab(all, tablen(all));
		foradd = reverse_tab(foradd, tablen(foradd));
	}
	add_maillon(recu, dir, foradd, all);
	freetab(foradd);
	freetab(all);
	return (NULL);

}

void	recup_args(t_files *data, t_recu **recu)
{
	int	i = 0;

	while (data->toread[i])
	{
		if (is_a_file(data->toread[i]))
		{
			char **all;
			char **foradd;
			all = malloc(sizeof(char *) * (2));
			foradd = malloc(sizeof(char *) * (2));
			all[0] = ft_strdup(data->toread[i]);
			all[1] = 0;
			foradd[0] = ft_strdup(data->toread[i]);
			foradd[1] = 0;
			add_maillon(recu, data->toread[i], foradd, all);
		}
		fill_recu(data, recu, data->toread[i]);
		i++;
	}
}
char	*ft_ltoa(long n);

void	print_more_infos(t_files *data, char **recup, t_recu **recu)
{
	t_recu *lst = *recu;
	
	int flag = 0;
	if (lst->next)
		flag = 1;
	while (lst)
	{
		if (!is_a_file(lst->pwd) && flag == 1)
		{
			ft_putstr(lst->pwd);
			ft_putstr(":\n");
		}

	struct	stat	info;
	int		u = 0;
	int	sizeL = 0;
	int	sizeU = 0;
	int	sizeG = 0;
	int	sizeS = 0;
	int	sizeD = 0;
	int	sizeH = 0;
	int	sizeBloc = 0;
	while (lst->paths[u])
	{
		if (lstat(lst->paths[u], &info) != 0)
			return ;

		char *tmp = ft_ltoa((long)info.st_nlink);
		if (sizeL < ft_strlen(tmp))
			sizeL = ft_strlen(tmp);


		struct passwd *pw = getpwuid(info.st_uid);
		struct group *gr = getgrgid(info.st_gid);
		if (sizeU < ft_strlen(tmp))
			sizeU = ft_strlen(pw ? pw->pw_name : "unknown");
		if (sizeG < ft_strlen(tmp))
			sizeG = ft_strlen(gr ? gr->gr_name : "unknown");

		free(tmp);
		tmp = ft_ltoa((long)info.st_size);
		if (sizeS < ft_strlen(tmp))
			sizeS = ft_strlen(tmp);
		free(tmp);

		time_t	modif;
		modif = info.st_mtime;
		char *timeString = ctime(&modif);
		char **timeSplit = ft_split(timeString, ' ');
		time_t actu = time(NULL);
		char	*date_actu = ctime(&actu);
		int	annee = atoi(date_actu + 20);

		if (annee == atoi(timeSplit[3]) && sizeH < 5)
			sizeH = 5;
		else
			sizeH = 4;
		if (ft_strlen(timeSplit[2]) > sizeD)
			sizeD = ft_strlen(timeSplit[2]);
		struct stat bloc;
		stat(lst->paths[u], &bloc);
		sizeBloc += (long)bloc.st_blocks;
		u++;
		freetab(timeSplit);
	}
	if (!is_a_file(lst->pwd))
	{
		char *str = ft_ltoa(sizeBloc / 2);
		ft_putstr("total ");
		ft_putstr(str);
		ft_putstr("\n");
		free(str);
	}


	u = 0;
	while (lst->paths[u])
	{
		if (lstat(lst->paths[u], &info) != 0)
			return ;

		// Affichage des droits

		ft_putstr((S_ISLNK(info.st_mode)) ? "l" : (S_ISDIR(info.st_mode)) ? "d" : "-");
		ft_putstr((info.st_mode & S_IRUSR) ? "r" : "-");
		ft_putstr((info.st_mode & S_IWUSR) ? "w" : "-");
		ft_putstr((info.st_mode & S_IXUSR) ? "x" : "-");
		ft_putstr((info.st_mode & S_IRGRP) ? "r" : "-");
		ft_putstr((info.st_mode & S_IWGRP) ? "w" : "-");
		ft_putstr((info.st_mode & S_IXGRP) ? "x" : "-");
		ft_putstr((info.st_mode & S_IROTH) ? "r" : "-");
		ft_putstr((info.st_mode & S_IWOTH) ? "w" : "-");
		ft_putstr((info.st_mode & S_IXOTH) ? "x" : "-");
		

		// Affichage du nombre de liens

		char *tmp = ft_ltoa((long)info.st_nlink);
		
		int lenL = ft_strlen(tmp);
		ft_putstr(" ");
		lenL = sizeL - lenL;
		while (lenL)
		{
			ft_putstr(" ");
			lenL--;
		}
		ft_putstr(tmp);
		free(tmp);

		// Affichage user et groupe
		
		struct passwd *pw = getpwuid(info.st_uid);
		struct group *gr = getgrgid(info.st_gid);
		
		int lenU = ft_strlen(pw->pw_name);
		ft_putstr(" ");
		lenU = sizeU - lenU;
		while (lenU)
		{
			ft_putstr(" ");
			lenU--;
		}
		ft_putstr(pw ? pw->pw_name : "unknown");
		if (data->g == false)
		{
			int lenG = ft_strlen(gr->gr_name);
			ft_putstr(" ");
			lenG = sizeG - lenG;
			while (lenG)
			{
				ft_putstr(" ");
				lenG--;
			}
			ft_putstr(gr ? gr->gr_name : "unknown");
		}


		// Affichage de la taille

		tmp = ft_ltoa((long)info.st_size);
		int lenS = ft_strlen(tmp);
		ft_putstr(" ");
		lenS = sizeS - lenS;
		while (lenS)
		{
			ft_putstr(" ");
			lenS--;
		}
		ft_putstr(tmp);
		free(tmp);

		ft_putstr(" ");

		time_t	modif;
		modif = info.st_mtime;
		char *timeString = ctime(&modif);
		char **timeSplit = ft_split(timeString, ' ');
		time_t actu = time(NULL);
		char	*date_actu = ctime(&actu);
		int	annee = atoi(date_actu + 20);
		
		ft_putstr(timeSplit[1]);
		int lenD = ft_strlen(timeSplit[2]);
		lenD = sizeD - lenD;
		ft_putstr(" ");
		while (lenD)
		{
			ft_putstr(" ");
			lenD--;
		}
		ft_putstr(timeSplit[2]);
		//printf("an = %d    ts = %d\n", atoi(timeSplit[4]), annee);
		ft_putstr(" ");
		if (annee ==  atoi(timeSplit[4]))
		{
			char **hours = ft_split(timeSplit[3], ':');
			ft_putstr(hours[0]);
			ft_putstr(":");
			ft_putstr(hours[1]);
			freetab(hours);
		}
		else
		{
			ft_putstr(" ");
			char **del = ft_split(timeSplit[4], '\n');
			ft_putstr(del[0]);
			freetab(del);
		}

		char	symL[4096];
		if (S_ISLNK(info.st_mode))
		{
			ssize_t symSize = readlink(lst->paths[u], symL, sizeof(symL) - 1);
			if (symSize != -1)
				symL[symSize] = 0;
		}

		ft_putstr(" ");
		if (S_ISDIR(info.st_mode))
			write(STDOUT_FILENO, COLOR_BLUE, ft_strlen(COLOR_BLUE));
		else if (S_ISLNK(info.st_mode))
			write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
		else if (info.st_mode & S_IXUSR)
			write(STDOUT_FILENO, COLOR_GREEN, ft_strlen(COLOR_GREEN));
		if (ft_strlen(lst->pwd) < ft_strlen(lst->paths[u]))
			ft_putstr(lst->paths[u] + ft_strlen(lst->pwd));
		else
			ft_putstr(lst->paths[u]);
		if (S_ISLNK(info.st_mode))
		{
			write(STDOUT_FILENO, COLOR_RESET, ft_strlen(COLOR_RESET));
			ft_putstr(" -> ");
			write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
			ft_putstr(symL);
		}
		write(STDOUT_FILENO, COLOR_RESET, ft_strlen(COLOR_RESET));
		ft_putstr("\n");
		u++;
		freetab(timeSplit);
	}
		lst = lst->next;
		if (lst)
			ft_putstr("\n");
	}

}

int trouver_longueur_max(char **donnees, int nb_donnees) {
    int longueur_max = 0;
    for (int i = 0; i < nb_donnees; i++) {
        int longueur = ft_strlen(donnees[i]);
        if (longueur > longueur_max) {
            longueur_max = longueur;
        }
    }
    return longueur_max;
}

int	len_all_tab(char **tab)
{
	int	i = 0;
	int	ret = 0;
	while (tab[i])
	{
		ret += ft_strlen(tab[i]);
		i++;
	}
	return (ret);
}

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
			//printf("pwd = %s      dir = %s\n", pwd, dirs[i][j]);
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
			// if (S_ISLNK(info.st_mode))
			// {
			// 	ft_putstr(" -> ");
			// 	write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
			// 	ft_putstr(symL);
			// }
			if (lst->padding)
			{
				int space = lst->padding[j] - ft_strlen(dirs[i][j]) - 2;
				//printf("pad = %d\n", lst->padding[j]);
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

char	***recup_nb_col(t_files *data, char **toprint, t_recu **recu, t_recu *lst);

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
		// if (S_ISLNK(info.st_mode))
		// {
		// 	ft_putstr(" -> ");
		// 	write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
		// 	ft_putstr(symL);
		// }

		i++;
		if (tab[i])
			ft_putstr("  ");
	}
	ft_putstr("\n");
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

char	***recup_nb_col(t_files *data, char **toprint, t_recu **recu, t_recu *lst)
{
	int	line = 1;
	char	***ret;// = malloc(sizeof(char **) * (line + 1));
	int	maxRowSize = len_all_tab(toprint) + (tablen(toprint) - 1) * 2;
	int	colterm = 80;
	struct winsize size;
	//t_col	col;
	int	*padding;
	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1)
	{
		perror("Error: ioctl\n");
		return NULL;
	}
	else
		colterm = size.ws_col;

	if (maxRowSize + 3 > colterm)
	{
		while (maxRowSize + 3 > colterm)
		{
			padding = malloc(sizeof(int) * (int)(tablen(toprint) / line));
			int k = 0;
			while (k < tablen(toprint) / line)
			{
				padding[k] = 0;
				k++;
			}
			
			ret = malloc(sizeof(char **) * (line + 1));
			for (k = 0; k < line; ++k)
			{
				ret[k] = malloc(sizeof(char *) * ((tablen(toprint) / line) + (tablen(toprint) % line > k) + 1));
			}
			ret[k] = 0;
			int i = 0;
			while (toprint[i])
			{
				//if (padding[i / line] < ft_strlen(toprint[i]) + (tablen(toprint) - 1) * 2)
				if (padding[i / line] < ft_strlen(toprint[i]) + 2)
				{
			//		printf("toprint  = %s\n", toprint[i]);
					//padding[i / line] = ft_strlen(toprint[i]) + (tablen(toprint) - 1) * 2;
					padding[i / line] = ft_strlen(toprint[i]) + 2;
			//		printf("padding = %d\n", padding[i / line]);
				}
				ret[i % line][i / line] = ft_strdup(toprint[i]);
				ret[i % line][i / line + 1] = 0;
				i++;
			}
			i = 0;
			int newMaxRowSize = 0;
	int u = 0;
	int	allpad = 0;
	while (u < tablen(toprint) / line)
	{
			/*printf("ooo = %d\n", padding[u]);
			int y = 0;
			while (ret[y])
			{
				int g = 0;
				while (ret[y][g])
				{
					//printf("ret = %s  ", ret[y][g]);
					g++;
				}
				//printf("\n");
				y++;
			}*/
			allpad += padding[u];
		u++;
	}
		/*printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");*/
			/*while (ret[i])
			{
				if (newMaxRowSize < len_all_tab(ret[i]) + (tablen(ret[i]) - 1) * 2) 
					newMaxRowSize = len_all_tab(ret[i]) + (tablen(ret[i]) - 1) * 2;
				i++;
				maxRowSize = newMaxRowSize;
			}*/
			maxRowSize = allpad;
			//printf("maxrowsiz = %d\n", maxRowSize);
			line++;
		}
		data->padding = padding;
	}
	else
	{
		ret = malloc(sizeof(char **) * (line + 1));
		int k = 0;
		for (k = 0; k < line; ++k)
		{
			ret[k] = malloc(sizeof(char *) * ((tablen(toprint) / line) + (tablen(toprint) % line > k) + 1));
		}
		ret[k] = 0;
		int i = 0;
		while (toprint[i])
		{
			ret[i % line][i / line] = ft_strdup(toprint[i]);
			ret[i % line][i / line + 1] = 0;
			i++;
		}
		data->padding = NULL;
	}
	lst->padding = data->padding;
	return (ret);
}

void	print_list(t_files *data, t_recu **recu)
{
	t_recu	*lst = *recu;

	/*if (data->t == true)
	{
		while (lst)
		{
			lst->paths = sort_by_time(tablen(lst->paths), lst->paths, lst->pwd, 0);
			lst->dirs = sort_by_time(tablen(lst->dirs), lst->dirs, lst->pwd, 1);
			lst = lst->next;
		}
	}*/
	/*lst = *recu;
	if (data->r == true)
	{
		while (lst)
		{
			lst->paths = reverse_tab(lst->paths, tablen(lst->paths));
			lst->dirs = reverse_tab(lst->dirs, tablen(lst->dirs));
			lst = lst->next;
		}
	}*/
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

void	free_all(t_files *data, t_recu **recu)
{
	ft_lstclear(recu, free);
	freetab(data->toread);
}

int	exec_ls_args(char **av)
{
	t_recu	*recu = NULL;
	t_files data;
	int ret = init_data(av, &data);
	if (ret == 1)
		return (1);
	if (data.R == true)
		optionR(&data, &recu);
	else
		recup_args(&data, &recu);

	print_list(&data, &recu);
	free_all(&data, &recu);
	return (0);
}

int	main(int ac, char **av)
{
	
	if (ac == 1)
		exec_ls_no_args();
	else
	{
		if (exec_ls_args(av))
			return (1);
	}
}
