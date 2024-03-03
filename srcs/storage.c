#include "ft_ls.h"

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
	if (!all)
		return (NULL);
	foradd = malloc(sizeof(char *) * (c + 1));
	if (!foradd)
		return (NULL);
	int i = 0;
	while ((dirs2 = readdir(tmp2)))
	{
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
	}
	closedir(tmp2);
	all[i] = 0;
	foradd[i] = 0;
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
			if (!all)
				return ;
			foradd = malloc(sizeof(char *) * (2));
			if (!foradd)
				return ;
			all[0] = ft_strdup(data->toread[i]);
			all[1] = 0;
			foradd[0] = ft_strdup(data->toread[i]);
			foradd[1] = 0;
			add_maillon(recu, data->toread[i], foradd, all);
			freetab(all);
			freetab(foradd);
		}
		fill_recu(data, recu, data->toread[i]);
		i++;
	}
}

void	add_maillon(t_recu **head_ref, char *new_data, char **all, char **paths)
{	
	t_recu	*new_node = malloc(sizeof(t_recu));
	if (!new_node)
		return ;
	t_recu	*last = *head_ref;
	
	new_node->pwd = ft_strdup(new_data);
	new_node->dirs = ft_strduptab(all);
	new_node->paths = ft_strduptab(paths);
	new_node->padding = NULL;
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