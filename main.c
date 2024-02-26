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

char	*ft_strdup(char *str);

int	ft_strlen(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		;

	return (i);
}
int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		write(1, &str[i], 1);
		i++;
	}
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		write(fd, &str[i], 1);
		i++;
	}
}

char	**ft_swap(int ac, char **av)
{
	int		i;
	int		j;
	char	*swap;

	i = 0;
	while (i != ac)
	{
		j = 0;
		while (j < ac)
		{
			if (ft_strcmp(av[i], av[j]) < 0)
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

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

int	check_options(char *opt)
{
	int	i;

	i = -1;
	while (opt[++i])
	{
		if (opt[i] == '-' || opt[i] == 'l' || opt[i] == 'R' || opt[i] == 'a' || opt[i] == 'r' || opt[i] == 't')
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

int	is_a_file(char *str)
{
	DIR    *dir_ptr;

	dir_ptr = opendir(str);
	if (dir_ptr == NULL)
	{
		if (errno == ENOTDIR)
			return (1);
		return (0);
	}
	if (closedir(dir_ptr))
		perror("closedir");
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

void	freetab(char **tab);

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
			printf("%s\n", av[i] + 1);
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
			i++;
		}
		n++;
	}
	freetab(options);
	return (0);
}

int	tablen(char **tab)
{
	int	i = 0;

	while (tab[i])
		i++;
	return (i);
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

char	**ft_strduptab(char **tab)
{
	int	i = 0;
	char	**res;

	res = malloc(sizeof(char *) * (tablen(tab) + 1));
	if (!res)
		return (NULL);
	while (tab[i])
	{
		res[i] = ft_strdup(tab[i]);
		i++;
	}
	res[i] = 0;
	return (res);
}

void	add_maillon(t_recu **head_ref, char *new_data, char **all)
{	
	t_recu	*new_node = malloc(sizeof(t_recu));
	t_recu	*last = *head_ref;
	
	new_node->pwd = ft_strdup(new_data);
	new_node->dirs = ft_strduptab(all);
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
		if (strcmp(dirs2->d_name, ".") != 0 && strcmp(dirs2->d_name, "..") != 0)
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
	add_maillon(recu, dir, foradd);
	freetab(foradd);
	i = 0;
	while (all[i])
	{
		if (strcmp(all[i], ".") != 0 && strcmp(all[i], "..") != 0)
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
	add_maillon(recu, dir, foradd);
	freetab(foradd);

}

void	recup_args(t_files *data, t_recu **recu)
{
	int	i = 0;

	while (data->toread[i])
	{
		fill_recu(data, recu, data->toread[i]);
		i++;
	}
}

void	print_list(t_files *data, t_recu **recu)
{
	struct	stat		test;
	struct	timespec	ts;
	t_recu *lst = *recu;
	time_t	modif;
	char			buff[100];

	stat(lst->dirs[0], &test);
	modif = test.st_mtime;


	char *timeString = ctime(&modif);
	printf("%.3s %.*s %.2s:%.2s\n", timeString + 4, 2, timeString + 8, timeString + 11, timeString + 14);
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
