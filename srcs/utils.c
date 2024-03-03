/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaravel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 19:35:54 by jtaravel          #+#    #+#             */
/*   Updated: 2024/02/29 19:50:31 by jtaravel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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

int	tablen(char **tab)
{
	int	i = 0;

	while (tab[i])
		i++;
	return (i);
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

char	**reverse_tab(char **tab, int len)
{
	int	start = 0;
	int	end = len - 1;

	while (start < end)
	{
		char *tmp = tab[start];
		tab[start] = tab[end];
		tab[end] = tmp;
		start++;
		end--;
	}
	return (tab);
}

char	**sort_by_time(int ac, char **av, char *pwd, int mode)
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
			if (mode == 1)
			{
				char *tmp1 = ft_strjoin(ft_strdup(pwd), "/");
				tmp1 = ft_strjoin(tmp1, av[i]);
				char *tmp2 = ft_strjoin(ft_strdup(pwd), "/");
				tmp2 = ft_strjoin(tmp2, av[j]);
				if (lstat(tmp1, &st) != 0)
					exit(4);
				if (lstat(tmp2, &st2) != 0)
					exit(3);
				free(tmp1);	
				free(tmp2);	
			}
			else
			{
				if (lstat(av[i], &st) != 0)
					exit(2);
				if (lstat(av[j], &st2) != 0)
				{
					exit(1);
				}
			}
			modif = st.st_mtime;
			modif2 = st2.st_mtime;
			if (modif > modif2)
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

char	**sort_by_time_acces(int ac, char **av, char *pwd, int mode)
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
			if (mode == 1)
			{
				char *tmp1 = ft_strjoin(ft_strdup(pwd), "/");
				tmp1 = ft_strjoin(tmp1, av[i]);
				char *tmp2 = ft_strjoin(ft_strdup(pwd), "/");
				tmp2 = ft_strjoin(tmp2, av[j]);
				if (lstat(tmp1, &st) != 0)
					exit(4);
				if (lstat(tmp2, &st2) != 0)
					exit(3);
				free(tmp1);	
				free(tmp2);	
			}
			else
			{
				if (lstat(av[i], &st) != 0)
					exit(2);
				if (lstat(av[j], &st2) != 0)
				{
					exit(1);
				}
			}
			modif = st.st_atime;
			modif2 = st2.st_atime;
			if (modif > modif2)
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
		if (opt[i] == '-' || opt[i] == 'l' || opt[i] == 'R' || opt[i] == 'a' || opt[i] == 'r' || opt[i] == 'o'
			|| opt[i] == 't' || opt[i] == 'g' || opt[i] == 'f' || opt[i] == 'C' || opt[i] == 'U' || opt[i] == 'u' || opt[i] == 'G')
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

