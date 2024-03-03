#include "ft_ls.h"

char	**add_dir_file(t_files *data, char *dir, t_recu **recu)
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
	if (!all)
	{
		ft_putstr_fd("Error malloc in add_dir_files\n", 2);
		return (NULL);
	}
	foradd = malloc(sizeof(char *) * (c + 1));
	if (!foradd)
	{
		ft_putstr_fd("Error malloc in add_dir_files\n", 2);
		return (NULL);
	}
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
	foradd[j] = 0;
	int alr = 0;
	if (data->f == true)
	{
		if (data->t == true)
		{
			all = sort_by_time(tablen(all), all, dir, 0);
			foradd = sort_by_time(tablen(foradd), foradd, dir, 1);
		}
	}
	else if (data->t == true && !data->U)
	{
		all = sort_by_time(tablen(all), all, dir, 0);
		foradd = sort_by_time(tablen(foradd), foradd, dir, 1);
	}
	if (data->u == true && data->t == true && data->l == true && !data->U)
	{
		all = sort_by_time_acces(tablen(all), all, dir, 0);
		foradd = sort_by_time_acces(tablen(foradd), foradd, dir, 1);
		alr = 1;
	}
	if (data->r == true && !data->U)
	{
		all = reverse_tab(all, tablen(all));
		foradd = reverse_tab(foradd, tablen(foradd));
	}
	else if (!data->U && !data->f && !data->t && alr == 0)
	{
		all = ft_swap(i, all);
		foradd = ft_swap(i, foradd);
	}
	add_maillon(recu, dir, foradd, all);
	freetab(foradd);
	i = 0;
	while (all[i])
	{
		if (ft_strcmp(all[i], ".") != 0 && ft_strcmp(all[i], "..") != 0)
		{
			path = ft_strjoin(path, all[i]);
			if (!is_a_file(all[i]))
				add_dir_file(data, path, recu);
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
	if (data->t)
	 	data->toread = sort_by_time(tablen(data->toread), data->toread, NULL, 0);
	int k = 0;
	while (data->toread[k])
	{
		if (is_a_file(data->toread[k]))
		{
			char **all;
			char **foradd;
			all = malloc(sizeof(char *) * (2));
			if (!all)
			{
				ft_putstr_fd("Error malloc in recursive\n", 2);
				return ;
			}
			foradd = malloc(sizeof(char *) * (2));
			if (!foradd)
			{
				ft_putstr_fd("Error malloc in recursive\n", 2);
				return ;
			}
			all[0] = ft_strdup(data->toread[k]);
			all[1] = 0;
			foradd[0] = ft_strdup(data->toread[k]);
			foradd[1] = 0;
			add_maillon(recu, data->toread[k], foradd, all);
			freetab(all);
			freetab(foradd);
		}
		add_dir_file(data, data->toread[k], recu);
		k++;
	}
}

void	optionR(t_files *data, t_recu **recu)
{
	recursive(data, recu);
}