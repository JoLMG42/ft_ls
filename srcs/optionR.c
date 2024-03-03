#include "ft_ls.h"

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
}

void	optionR(t_files *data, t_recu **recu)
{
	int i = 0;

	recursive(data, recu);
}