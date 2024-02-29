#include "ft_ls.h"

int	init_data(char **argv, t_files *data)
{
	int	i = 1;
	int	c = 0;
	int	k = 0;
	int	f = 0;
    int o = 0;
	int	n;
	int	j;
	char	**toread;
	char	**options;
	struct	stat	*tmp;
    	char    **av = ft_strduptab(argv);

	while (av[i])
	{
		DIR *tmp = opendir(av[i]);
		if (tmp)
			c++;
		else if (is_a_file(av[i]))
			c++;
		else if (!check_options(av[i]))
            o++;
        else
			return (1);
		closedir(tmp);
		i++;
	}
    if (c == 0)
    {
        char **tmp = malloc(sizeof(char *) * (c + o + 10));
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
	options = malloc(sizeof(char *) * ((i - c) + 10));
    i = 1;
    if (c == 0)
	    i = 0;
	j = 0;
	f = 0;
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
	data->toread = ft_swap(j, data->toread);
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
	freetab(av);
	return (0);
}
