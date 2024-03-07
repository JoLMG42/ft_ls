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
    {
        if (errno == 13)
        {
            ft_putstr_fd("ft_ls: cannot open directory '", 2);
			ft_putstr_fd(dir, 2);
			ft_putstr_fd("': ", 2);
            ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd("\n", 2);
        }
		return NULL;
    }
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
        int link = 0;
        struct	stat	info;
        char *forchecklink = ft_strjoin(ft_strjoin(ft_strdup(dir), "/"), dirs2->d_name);
        if (path)
            free(path);
        path = NULL;
        if (lstat(forchecklink, &info) == 0)
        {
                if (S_ISLNK(info.st_mode))
                   link = 1;
        }
		if (ft_strcmp(dirs2->d_name, ".") != 0 && ft_strcmp(dirs2->d_name, "..") != 0 && link == 0)
		{
			path = ft_strjoin(path, dir);
			// printf("path = %s      dir = %s\n", path, dirs2->d_name);
			if (ft_strcmp(dir, "./") != 0 && ft_strcmp(dir, "../") != 0 && ft_strcmp(path, "/") != 0)
				path = ft_strjoin(path, "/");
			path = ft_strjoin(path, dirs2->d_name);
			// printf("path222222 = %s      dir = %s\n", path, dirs2->d_name);


			if (dirs2->d_name[0] == '.' && data->a == false)
				;
			else
			{
                if (is_a_file(forchecklink) && data->S)
                {
                    int lenS = (unsigned long)info.st_size;
                    if ((unsigned long)lenS < data->sizeO)
                    {
                        free(forchecklink);
                        continue ;
                    }
                }
				all[i] = ft_strdup(path);
				foradd[j] = ft_strdup(dirs2->d_name);
				j++;
				i++;
			}
		}
        else if (link == 1)
        {
			all[i] = ft_strdup(dirs2->d_name);
			foradd[j] = ft_strdup(dirs2->d_name);
			i++;
			j++;
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
        free(forchecklink);
	}

    if (path)
    {
        free(path);
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
	t_recu *lst = add_maillon(recu, dir, foradd, all);
    if (lst)
    {
	    char ***newdirs = recup_nb_col(data, lst->dirs, lst);
        if (!newdirs && data->S)
            ;
		else if (tablen(newdirs[0]) == 2 && data->S && data->a)
		{
			freebigtab(newdirs);
		}
        else
        {
            ft_putstr(lst->pwd);
            ft_putstr(":\n");
            if (!newdirs)
                ft_putstr("total 0\n");
            else
            {
                if (data->l)
                    print_more_infos2(data, &lst, newdirs);
                else
                    big_print(newdirs, lst->pwd, data, lst);
                ft_putstr("\n");
                ft_putstr("\n");
                freebigtab(newdirs);
            }
       }
    }
	freetab(foradd);
	i = 0;
    int link = 0;
    ft_lstclear(recu, &free);
	while (all[i])
	{
        // char *forchecklink = ft_strjoin(ft_strjoin(ft_strdup(dir), "/"), all[i]);
		// struct	stat	info;

        // if (lstat(all[i], &info) == 0)
        // {
        //     if (lstat(forchecklink, &info) != 0)
        //     {
        //         if (S_ISLNK(info.st_mode))
        //             link = 1;
        //     }
        // }
        // else
        // {
        //     if (S_ISLNK(info.st_mode))
        //         link = 1;
        // }
		if (ft_strcmp(all[i], ".") != 0 && ft_strcmp(all[i], "..") != 0 && link == 0)
		{
			char *here = NULL;
            here = ft_strjoin(here, all[i]);
			if (!is_a_file(all[i]))
				add_dir_file(data, here, recu);
			free(here);
			here = NULL;
		}
        link = 0;
		i++;
        // free(forchecklink);
	}
	freetab(all);
	return (NULL);
}



void	recursive(t_files *data, t_recu **recu)
{
	if (data->t)
	 	data->toread = sort_by_time(tablen(data->toread), data->toread, NULL, 0);
    if (data->r)
	 	data->toread = reverse_tab(data->toread, tablen(data->toread));
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
			t_recu *lst = add_maillon(recu, data->toread[k], foradd, all);
            char ***newdirs = recup_nb_col(data, lst->dirs, lst);
	        big_print(newdirs, lst->pwd, data, lst);
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