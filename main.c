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
		store[i] = ft_strdup(tab->d_name);
		i++;
		tab = readdir(actu);
	}
	store[i] = 0;
	store = ft_swap(size, store);
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
	
	
	int k = 0;
	while (data->toread[k])
	{
		printf("lol = %s\n", data->toread[k]);
		add_dir_file(data, data->toread[k], recu, 0, oldpwd);
		k++;
	}

	// t_recu *lst = *recu;

	// while (lst)
	// {
	// 	printf("%s\n", lst->pwd);
	// 	int i = 0;
	// 	while (lst->dirs[i])
	// 	{
	// 		printf("%s ", lst->dirs[i]);
	// 		i++;
	// 	}
	// 	printf("\n\n");
	// 	lst = lst->next;
	// }

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
	add_maillon(recu, dir, foradd, all);
	freetab(foradd);
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

	while (lst)
	{
		if (data->t == true)
		{
			lst->dirs = sort_by_time(tablen(lst->dirs), lst->dirs);
			lst->paths = sort_by_time(tablen(lst->paths), lst->paths);
		}
		if (!is_a_file(lst->pwd))
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
	}
	if (!is_a_file(lst->pwd))
	{
		char *str = ft_ltoa(sizeBloc / 2);
		ft_putstr("total ");
		ft_putstr(str);
		ft_putstr("\n");
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
		ft_putstr(lst->dirs[u]);
		write(STDOUT_FILENO, COLOR_RESET, ft_strlen(COLOR_RESET));
		if (S_ISLNK(info.st_mode))
		{
			ft_putstr(" -> ");
			write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
			ft_putstr(symL);
		}
		ft_putstr("\n");
		u++;
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

void	print_list(t_files *data, t_recu **recu)
{
	struct	stat		test;
	struct	timespec	ts;
	t_recu *lst = *recu;
	time_t	modif;
	char			buff[100];

	//if (data->R == false)
	//{
		t_recu *tmp = *recu;
		int	c = 0;
		while (tmp)
		{
			int i = 0;
			while (lst->paths[i])
			{
				i++;
				c++;
			}
			tmp = tmp->next;
		}
		char	**recup = malloc(sizeof(char *) * (c + 5));
		char	**toprint = malloc(sizeof(char *) * (c + 5));
		tmp = *recu;
		int j = 0;
		while (tmp)
		{
			int i = 0;
			while (tmp->paths[i])
			{
				recup[j] = tmp->paths[i];
				toprint[j] = tmp->dirs[i];
				i++;
				j++;
			}
			tmp = tmp->next;
		}
		recup[j] = 0;
		toprint[j] = 0;
		if (data->t == true)
		{
			recup = sort_by_time(j, recup);
			toprint = sort_by_time(j, toprint);
		}
		if (data->r == true)
			reverse_tab(recup, j);
		if (data->l == true)
			print_more_infos(data, recup, recu);
		else
		{
			int col = 80;
			struct winsize size;
			if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1)
			{
				perror("Error: ioctl\n");
				return ;
			}
			else
				col = size.ws_col;

			int	lenAll = len_all_tab(toprint);
			if (col > lenAll)
			{
				one_line_print(recup, toprint);
				return ;
			}

			printf("col = %d\n", col);

			int	nbfiles = (int)round(tablen(toprint) / 3 + 0.5);

			char	**col1 = malloc(sizeof(char *) * (nbfiles + 1));
			char	**col2 = malloc(sizeof(char *) * (nbfiles + 1));
			char	**col3 = malloc(sizeof(char *) * (nbfiles + 1));

			int	count = 0;
			int a = 0;
			while (count < nbfiles)
			{
				if (!toprint[count])
					break; 
				col1[a] = ft_strdup(toprint[count]);
				count++;
				a++;
				if (!toprint[count])
					break; 
			}
			col1[a] = 0;

			int b = 0;
			while (count < nbfiles * 2)
			{
				col2[b] = ft_strdup(toprint[count]);
				count++;
				b++;
			}
			col2[b] = 0;

			int c = 0;
			while (count < nbfiles * 3)
			{
				if (!toprint[count])
					break;

				col3[c] = ft_strdup(toprint[count]);
				count++;
				c++;
				if (!toprint[count])
					break;
			}
			col3[c] = 0;

			int g = 0;
			while (col1[g])
			{
				if (col1[g])
				{
					ft_putstr(col1[g]);
					int max = trouver_longueur_max(col1, tablen(col1));
					int l = ft_strlen(col1[g]);
					while (l <= max)
					{
						ft_putstr(" ");
						l++;
					}
				}
				if (col2[g])
				{
					ft_putstr(col2[g]);
					int max = trouver_longueur_max(col2, tablen(col2));
					int l = ft_strlen(col2[g]);
					while (l <= max)
					{
						ft_putstr(" ");
						l++;
					}
				}
				if (col3[g])
				{
					ft_putstr(col3[g]);
					int max = trouver_longueur_max(col3, tablen(col3));
					int l = ft_strlen(col3[g]);
					while (l <= max)
					{
						ft_putstr(" ");
						l++;
					}
				}
				ft_putstr("\n");
				g++;
			}
			freetab(col1);
			freetab(col2);
			freetab(col3);

			// printf("lenall = %d\n", lenAll);
			// printf("col term = %d\n", col);
			// printf("cols = %d\n", nb_col);
		}

	//}
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
	freetab(data.toread);
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
