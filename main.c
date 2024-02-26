#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
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

int	init_data(char **av, t_files *data)
{
	int	i = 1;
	int	c = 0;
	int	k = 0;
	int	f = 0;
	int	n;
	int	j;
	char	**toopen;
	char	**toread;
	char	**files;
	char	**options;
	struct	stat	*tmp;

	while (av[i])
	{
		if (opendir(av[i]))
			c++;
		else if (is_a_file(av[i]))
			f++;
		else if (check_options(av[i]))
		{
			return (1);
		}
		i++;
	}
	toopen = malloc(sizeof(char *) * (c + 1));
	toread = malloc(sizeof(char *) * (c + 1));
	files = malloc(sizeof(char *) * (f + 1));
	options = malloc(sizeof(char *) * ((i - c) + 1));
	i = 1;
	j = 0;
	f = 0;
	n = 0;
	while (av[i])
	{
		if (opendir(av[i]))
		{
			toopen[j] = ft_strdup(av[i]);
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
		i++;
	}
	toopen[j] = 0;
	files[f] = 0;
	options[n] = 0;
	data->toopen = toopen;
	data->toopen = ft_swap(j, data->toopen);
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

void	add_dir_file(t_files *data, char *dir, t_recu *recu, int mode, char *oldpwd)
{
	if (mode == 0)
	{
		char		**newd;
		char		**newf;
		int		i = 0;
		DIR		*tmp = opendir(dir);
		struct	dirent	*dirs = readdir(tmp);
		int		c = 0;
		int		f = 0;

		while (dirs)
		{
			printf("i = %s\n", dirs->d_name);
			if (opendir(dirs->d_name) && dirs->d_name[0] != '.')
				c++;
			else if (is_a_file(dirs->d_name))
				f++;
			dirs = readdir(tmp);
		}
		recu->dirs = malloc(sizeof(char *) * (c + 2));
		recu->files = malloc(sizeof(char *) * (f + 2));
		freetab(data->toread);
		//freetab(data->toopen);
		freetab(data->files);
		data->toopen = NULL;
		data->toread = NULL;
		data->files = NULL;
		data->toread = malloc(sizeof(char *) * (c + 1));
		data->toopen = malloc(sizeof(char *) * (c + 2));
		data->files = malloc(sizeof(char *) * (f + 2));
		int k = 0;
		int n = 0;
		tmp = opendir(dir);
		dirs = readdir(tmp);
		while (dirs)
		{
			if (opendir(dirs->d_name) && dirs->d_name[0] != '.')
			{
				recu->dirs[k] = ft_strdup(dirs->d_name);
				data->toopen[k] = ft_strdup(dirs->d_name);
				data->toread[k] = ft_strdup(dirs->d_name);
				printf("test = %s\n", data->toread[k]);
				k++;
			}
			else
			{
				recu->files[n] = ft_strdup(dirs->d_name);
				data->files[n] = ft_strdup(dirs->d_name);
				n++;
			}
			dirs = readdir(tmp);
		}
		recu->dirs[k + 1] = 0;
		recu->files[n + 1] = 0;
		recu->pwd = ft_strdup(dir);
		recu->oldpwd = oldpwd;
		recu->next = NULL;
		//data->toopen[k] = 0;
		data->toread[k] = 0;
		data->files[n + 1] = 0;
		data->toread = ft_swap(k, data->toread);
		//data->toopen = ft_swap(k, data->toopen);

		//*recu = new_elem(newd, newf, dir, 
	}
}



void	recursive(t_files *data, t_recu *recu)
{
	int	i = 0;
	t_recu	*tmp;
	char	*oldpwd;

	*tmp = *recu;	

	while (data->toread[i])
	{
		printf("data = %s\n", data->toread[i]);
		if (opendir(data->toread[i]))
			add_dir_file(data, data->toread[i], tmp, 0, oldpwd);
		oldpwd = tmp->pwd;
		tmp = tmp->next;
		i++;
	}

}

void	optionR(t_files *data)
{
	int i = 0;
	t_recu	recu;

	recursive(data, &recu);
}

int	exec_ls_args(char **av)
{
	t_files data;
	int ret = init_data(av, &data);
	if (ret == 1)
		return (1);
	if (data.R == true)
		optionR(&data);
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
