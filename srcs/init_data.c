#include "ft_ls.h"

int	init_data(char **argv, t_files *data)
{
	int	i = 1;
	int	c = 0;
	int o = 0;
	int	quit = 0;
	int	n;
	int	j;
	char	**toread;
	char	**options;
	char	**av = ft_strduptab(argv);
	data->sizeO = 0;
	while (av[i])
	{
		DIR *tmp = opendir(av[i]);
		if (tmp)
			c++;
		else if (is_a_file(av[i]))
			c++;
		else if (av[i][0] == '-')
		{
			if (ft_strcmp("--help", av[i]) == 0)
			{
				print_msg_help();
				freetab(av);
				return (135);
			}
			if (!check_options(av[i]))
			{
				if (ft_strnstr(av[i], "S", ft_strlen(av[i])))
				{
					if (!av[i + 1] || !check_digit(av[i + 1]) || ft_strlen(av[i + 1]) < 1)
					{
						ft_putstr_fd("ft_ls: please provide a size with -S\n", 2);
						freetab(av);
						return (2);
					}
					data->sizeO = atol(av[i + 1]);
					i++;
				}
				if (ft_strnstr(av[i], "D", ft_strlen(av[i])))
				{
					if (!av[i + 1] || !check_digit(av[i + 1]) || ft_strlen(av[i + 1]) < 1)
					{
						ft_putstr_fd("ft_ls: please provide a power of dig with -D\n", 2);
						freetab(av);
						return (2);
					}
					data->powerDig = atoi(av[i + 1]);
					i++;
				}
				o++;
			}
			else
			{
				freetab(av);
				return (2);
			}
		}
		else
		{
			if (errno == 13)
			{
				ft_putstr_fd("ft_ls: cannot open directory '", 2);
				ft_putstr_fd(av[i], 2);
				ft_putstr_fd("': ", 2);
				ft_putstr_fd(strerror(errno), 2);
				ft_putstr_fd("\n", 2);
			}
			else
			{
				ft_putstr_fd("ft_ls: cannot access '", 2);
				ft_putstr_fd(av[i], 2);
				ft_putstr_fd("'", 2);
				ft_putstr_fd(": No such file or directory\n", 2);
			}
			free(av[i]);
			av[i] = ft_strdup("-");
			quit = 1;
		}
		closedir(tmp);
		i++;
	}
	if (c == 0)
	{
		if (quit == 1)
		{
			freetab(av);
			return (2);
		}
		char **tmp = malloc(sizeof(char *) * (c + o + 10));
		if (!tmp)
		{
			freetab(av);
			ft_putstr_fd("Error malloc in init_data\n", 2);
			return (1);
		}
		int u = 1;
		int l = 0;
		while (av[u])
		{
			tmp[l] = ft_strdup(av[u]);
			l++;
			u++;
		}
		tmp[l] = ft_strdup("./");
		tmp[l + 1] = ft_strdup(0);
		freetab(av);
		av = ft_strduptab(tmp);
		freetab(tmp);
	}
	toread = malloc(sizeof(char *) * (c + 10));
	if (!toread)
	{
		freetab(av);
		ft_putstr_fd("Error malloc in init_data\n", 2);
		return (1);
	}
	options = malloc(sizeof(char *) * ((i - c) + 10));
	if (!options)
	{
		freetab(av);
		ft_putstr_fd("Error malloc in init_data\n", 2);
		return (1);
	}
	i = 1;
	if (c == 0)
		i = 0;
	j = 0;
	n = 0;
	while (av[i])
	{
		DIR *tmp = opendir(av[i]);
		if (tmp)
		{
			toread[j] = ft_strdup(av[i]);
			j++;
		}
		else if (is_a_file(av[i]))
		{
			toread[j] = ft_strdup(av[i]);
			j++;
		}
		else
		{
			options[n] = ft_strdup(av[i] + 1);
			n++;
		}
		closedir(tmp);
		i++;
	}
	options[n] = 0;
	toread[j] = 0;

	data->toread = toread;
	n = 0;

	data->R = false;
	data->r = false;
	data->l = false;
	data->a = false;
	data->t = false;
	data->g = false;
	data->G = false;
	data->f = false;
	data->C = false;
	data->u = false;
	data->U = false;
	data->S = false;
	data->E = false;
	data->D = false;
	data->flagQuote = 0;
	data->powerDig = 999999;
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
			{
				data->t = true;
				data->U = false;
			}
			if (options[n][i] == 'g')
			{
				data->l = true;
				data->g = true;
			}
			if (options[n][i] == 'G')
			{
				data->G = true;
			}
			if (options[n][i] == 'o')
			{
				data->l = true;
				data->G = true;
			}
			if (options[n][i] == 'C')
				data->C = true;
			if (options[n][i] == 'U')
			{
				data->t = false;
				data->r = false;
				data->U = true;
			}
			if (options[n][i] == 'u')
				data->u = true;
			if (options[n][i] == 'f')
			{
				data->U = true;
				data->C = false;
				data->l = false;
				data->a = true;
				data->f = true;
			}
			if (options[n][i] == 'S')
				data->S = true;
			if (options[n][i] == 'E')
				data->E = true;
			if (options[n][i] == 'D')
				data->D = true;
			i++;
		}
		n++;
	}
	if (!data->U)
		data->toread = ft_swap(j, data->toread);
	freetab(options);
	freetab(av);
	if (data->D && !data->R)
	{
		free_all(data, NULL);
		ft_putstr_fd("ft_ls: please provide the -R option if you want to use -D\n", 2);
		return (2);
	}
	return (0);
}
