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

char	**sort_by_time(int ac, char **av)
{
	int		i;
	int		j;
	char	*swap;
	struct	stat		st;
	struct	stat		st2;
	time_t	modif;
	time_t	modif2;

	i = 0;
	while (i != ac)
	{
		j = 0;
		while (j < ac)
		{
			if (stat(av[i], &st) != 0)
				exit(1);
			if (stat(av[j], &st2) != 0)
				exit(1);
			modif = st.st_mtime;
			modif2 = st2.st_mtime;
			//printf("m1 = %ld   m2 = %ld\n", (long)modif, (long)modif2);
			if (modif < modif2)
			{
				swap = av[i];
				av[i] = av[j];
				av[j] = swap;
			}
			j++;
		}
		i++;
	}
	return (av);
}

int	check_options(char *opt)
{
	int	i;

	i = -1;
	while (opt[++i])
	{
		if (opt[i] == '-' || opt[i] == 'l' || opt[i] == 'R' || opt[i] == 'a' || opt[i] == 'r' || opt[i] == 't' || opt[i] == 'g')
			;
		else
		{
			ft_putstr_fd("ft_ls: invalid option -- '", 2);
			ft_putchar_fd(opt[i], 2);
			ft_putstr_fd("'\n", 2);
			ft_putstr_fd("try 'ft_ls --help' for more information.\n", 2);
			return (1);
		}
	}
	return (0);
}

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
	store = malloc(sizeof(char *) * size + 1);
	actu = opendir("./");
	tab = readdir(actu);
	int i = 0;
	while (tab)
	{
		store[i] = ft_strdup(tab->d_name);
		i++;
		tab = readdir(actu);
	}
	store = ft_swap(size, store);
	i = 0;
	while (store[i])
	{
		if (store[i][0] != '.')
			printf("%s\n", store[i]);
		i++;
	}
	return (0);
}

int	init_data(char **av, t_files *data)
{
	int	i = 1;
	int	c = 0;
	int	k = 0;
	int	f = 0;
	int	n;
	int	j;
	//char	**toopen;
	char	**toread;
	char	**files;
	char	**options;
	struct	stat	*tmp;

	while (av[i])
	{
		DIR *tmp = opendir(av[i]);
		if (tmp)
			c++;
		else if (is_a_file(av[i]))
			f++;
		else if (check_options(av[i]))
		{
			return (1);
		}
		closedir(tmp);
		i++;
	}
	//toopen = malloc(sizeof(char *) * (c + 1));
	toread = malloc(sizeof(char *) * (c + 1));
	files = malloc(sizeof(char *) * (f + 1));
	options = malloc(sizeof(char *) * ((i - c) + 1));
	i = 1;
	j = 0;
	f = 0;
	n = 0;
	while (av[i])
	{
		DIR *tmp = opendir(av[i]);
		if (tmp)
		{
			//toopen[j] = ft_strdup(av[i]);
			toread[j] = ft_strdup(av[i]);
			j++;
		}
		else if (is_a_file(av[i]))
		{
			files[f] = ft_strdup(av[i]);
			f++;
		}
		else
		{
			options[n] = ft_strdup(av[i] + 1);
			n++;
		}
		closedir(tmp);
		i++;
	}
	//toopen[j] = 0;
	files[f] = 0;
	options[n] = 0;
	toread[j] = 0;
	if (tablen(toread) < 1)
	{
		freetab(toread);
		freetab(files);
		freetab(options);
		char *tmp[n+j+f];
		int u = 0;
		while (av[u])
		{
			tmp[u] = av[u];
			u++;
		}
		tmp[u] = "./";
		tmp[u + 1] = 0;
		init_data(tmp, data);
		return (0);
	}
	//data->toopen = toopen;
	//data->toopen = ft_swap(j, data->toopen);
	data->toread = toread;
	data->toread = ft_swap(j, data->toread);
	data->files = files;
	data->files = ft_swap(f, data->files);
	j = 0;
	n = 0;

	data->R = false;
	data->r = false;
	data->l = false;
	data->a = false;
	data->t = false;
	data->g = false;
	while (options[n])
	{
		i = 0;
		while (options[n][i])
		{
			if (options[n][i] == 'R')
				data->R = true;
			if (options[n][i] == 'r')
				data->r = true;
			if (options[n][i] == 'l')
				data->l = true;
			if (options[n][i] == 'a')
				data->a = true;
			if (options[n][i] == 't')
				data->t = true;
			if (options[n][i] == 'g')
				data->g = true;
			i++;
		}
		n++;
	}
	freetab(options);
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
				foradd[j] = ft_strdup(dirs2->d_name);
				j++;
			}
		}

	}
	closedir(tmp2);
	all[i] = 0;
	all = ft_swap(i, all);
	foradd[i] = 0;
	foradd = ft_swap(i, foradd);
	int u = 0;
	/*while (all[u])
	{
		printf("all[u] = %s\n", all[u]);
		u++;
	}
	printf("dir = %s\n", dir);*/
	add_maillon(recu, dir, foradd, all);
	freetab(foradd);
	i = 0;
	while (all[i])
	{
		if (ft_strcmp(all[i], ".") != 0 && ft_strcmp(all[i], "..") != 0)
		{
			//char	*cpydir = ft_strdup(dir);
			//path = ft_strjoin(cpydir, "/");
			path = ft_strjoin(path, all[i]);
			if (!is_a_file(all[i]))
			{
				//printf("DIR = %s\n", path);
				add_dir_file(data, path, recu, 0, NULL);
			}
			else
			{
				//printf("FILE = %s\n", path);
			}
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

	t_recu *lst = *recu;

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
	}

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
	/*while (all[u])
	{
		printf("all[u] = %s\n", all[u]);
		u++;
	}
	printf("dir = %s\n", dir);*/
	add_maillon(recu, dir, foradd, all);
	freetab(foradd);
	return (NULL);

}

void	fill_data_no_ars(t_files *data)
{
	struct dirent 	*tab;
	DIR 		*actu = opendir("./");
	char 		**store;
	int 		size = 0;

	tab = readdir(actu);
	while (tab)
	{
		if (ft_strcmp(tab->d_name, ".") != 0 && ft_strcmp(tab->d_name, "..") != 0)
		{
			size++;
		}
		tab = readdir(actu);
	}
	data->toread = malloc(sizeof(char *) * size + 1);
	actu = opendir("./");
	tab = readdir(actu);
	int i = 0;
	while (tab)
	{
		if (ft_strcmp(tab->d_name, ".") != 0 && ft_strcmp(tab->d_name, "..") != 0)
		{
			data->toread[i] = ft_strdup(tab->d_name);
			i++;
		}
		tab = readdir(actu);
	}
	data->toread = ft_swap(size, data->toread);
}

void	recup_args(t_files *data, t_recu **recu)
{
	int	i = 0;

	/*if (tablen(data->toread) < 1)
	{
		freetab(data->toread);
		fill_data_no_ars(data);
	}*/
	while (data->toread[i])
	{
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
		ft_putstr(lst->pwd);
		ft_putstr(":\n");

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
		if (stat(lst->paths[u], &info) != 0)
			return ;
		char *tmp = ft_ltoa((long)info.st_nlink);
		if (sizeL < ft_strlen(tmp))
			sizeL = ft_strlen(tmp);
		free(tmp);

		struct passwd *pw = getpwuid(info.st_uid);
		struct group *gr = getgrgid(info.st_gid);
		if (sizeU < ft_strlen(tmp))
			sizeU = ft_strlen(pw ? pw->pw_name : "unknown");
		if (sizeG < ft_strlen(tmp))
			sizeG = ft_strlen(gr ? gr->gr_name : "unknown");

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
	char *str = ft_ltoa(sizeBloc / 2);
	ft_putstr("total ");
	ft_putstr(str);
	ft_putstr("\n");

	u = 0;
	while (lst->paths[u])
	{
		if (stat(lst->paths[u], &info) != 0)
			return ;

		// Affichage des droits

		ft_putstr((S_ISDIR(info.st_mode)) ? "d" : "-");
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

		ft_putstr(" ");
		if (S_ISDIR(info.st_mode))
			write(STDOUT_FILENO, COLOR_BLUE, ft_strlen(COLOR_BLUE));
		else if (info.st_mode & S_IXUSR)
			write(STDOUT_FILENO, COLOR_GREEN, ft_strlen(COLOR_BLUE));
		ft_putstr(lst->dirs[u]);
		write(STDOUT_FILENO, COLOR_RESET, ft_strlen(COLOR_RESET));
		ft_putstr("\n");
		u++;
	}
		lst = lst->next;
		if (lst)
			ft_putstr("\n");
	}

}
void	print_list(t_files *data, t_recu **recu)
{
	struct	stat		test;
	struct	timespec	ts;
	t_recu *lst = *recu;
	time_t	modif;
	char			buff[100];

	if (data->R == false)
	{
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
		char	**recup = malloc(sizeof(char *) * (c + 1));
		tmp = *recu;
		int j = 0;
		while (tmp)
		{
			int i = 0;
			while (tmp->paths[i])
			{
				recup[j] = tmp->paths[i];
				i++;
				j++;
			}
			tmp = tmp->next;
		}
		recup[j] = 0;
		if (data->t == true)
			recup = sort_by_time(j, recup);
		if (data->r == true)
			reverse_tab(recup, j);
		if (data->l == true)
			print_more_infos(data, recup, recu);
		else
		{
			int u = 0;
			while (recup[u])
			{
				printf("test = %s\n", recup[u]);
				u++;
			}
		}

	}
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
	freetab(data.files);
	return (0);
}

int	main(int ac, char **av)
{
	/*struct winsize size;
	 if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1) {
        perror("Erreur lors de la récupération de la taille du terminal");
        return 1;
    }	

    printf("Taille du terminal : lignes = %d, colonnes = %d\n", size.ws_row, size.ws_col);*/
	if (ac == 1)
		exec_ls_no_args();
	else
	{
		if (exec_ls_args(av))
			return (1);
	}
}
