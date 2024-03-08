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

void merge(char **arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    char **L = malloc(n1 * sizeof(char *));
    char **R = malloc(n2 * sizeof(char *));

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (ft_strcmp(L[i], R[j]) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(char **arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

char **ft_swap(int ac, char **av)
{
    mergeSort(av, 0, ac - 1);
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

char **reverse_tab(char **tab, int len) {
    mergeSort(tab, 0, len - 1);

    // Inversion du tableau trié
    int start = 0;
    int end = len - 1;

    while (start < end) {
        char *tmp = tab[start];
        tab[start] = tab[end];
        tab[end] = tmp;
        start++;
        end--;
    }
    return tab;
}

void mergeTime(char **arr, int l, int m, int r, char *pwd, int mode) {
    int n1 = m - l + 1;
    int n2 = r - m;

    char **L = malloc(n1 * sizeof(char *));
    char **R = malloc(n2 * sizeof(char *));

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        struct stat st1, st2;
        struct timespec modif1, modif2;
        if (mode == 1) {
            char *tmp1 = ft_strjoin(ft_strdup(pwd), "/");
            tmp1 = ft_strjoin(tmp1, L[i]);
            char *tmp2 = ft_strjoin(ft_strdup(pwd), "/");
            tmp2 = ft_strjoin(tmp2, R[j]);
            if (lstat(tmp1, &st1) != 0 || lstat(tmp2, &st2) != 0) {
                free(tmp1);
                free(tmp2);
                free(L);
                free(R);
                return;
            }
            free(tmp1);
            free(tmp2);
        } else {
            if (lstat(L[i], &st1) != 0 || lstat(R[j], &st2) != 0) {
                free(L);
                free(R);
                return;
            }
        }
        modif1 = st1.st_mtim;
        modif2 = st2.st_mtim;
        if (modif1.tv_sec > modif2.tv_sec || (modif1.tv_sec == modif2.tv_sec && modif1.tv_nsec > modif2.tv_nsec)) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSortTime(char **arr, int l, int r, char *pwd, int mode) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortTime(arr, l, m, pwd, mode);
        mergeSortTime(arr, m + 1, r, pwd, mode);
        mergeTime(arr, l, m, r, pwd, mode);
    }
}

char **sort_by_time(int ac, char **av, char *pwd, int mode) {
    mergeSortTime(av, 0, ac - 1, pwd, mode);
    return av;
}

void mergeTimeAccess(char **arr, int l, int m, int r, char *pwd, int mode) {
    int n1 = m - l + 1;
    int n2 = r - m;

    char **L = malloc(n1 * sizeof(char *));
    char **R = malloc(n2 * sizeof(char *));

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        struct stat st1, st2;
        struct timespec modif1, modif2;
        if (mode == 1) {
            char *tmp1 = ft_strjoin(ft_strdup(pwd), "/");
            tmp1 = ft_strjoin(tmp1, L[i]);
            char *tmp2 = ft_strjoin(ft_strdup(pwd), "/");
            tmp2 = ft_strjoin(tmp2, R[j]);
            if (lstat(tmp1, &st1) != 0 || lstat(tmp2, &st2) != 0) {
                free(tmp1);
                free(tmp2);
                free(L);
                free(R);
                return;
            }
            free(tmp1);
            free(tmp2);
        } else {
            if (lstat(L[i], &st1) != 0 || lstat(R[j], &st2) != 0) {
                free(L);
                free(R);
                return;
            }
        }
        modif1 = st1.st_atim;
        modif2 = st2.st_atim;
        if (modif1.tv_sec > modif2.tv_sec || (modif1.tv_sec == modif2.tv_sec && modif1.tv_nsec > modif2.tv_nsec)) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSortTimeAccess(char **arr, int l, int r, char *pwd, int mode) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortTimeAccess(arr, l, m, pwd, mode);
        mergeSortTimeAccess(arr, m + 1, r, pwd, mode);
        mergeTimeAccess(arr, l, m, r, pwd, mode);
    }
}

char **sort_by_time_acces(int ac, char **av, char *pwd, int mode) {
    mergeSortTimeAccess(av, 0, ac - 1, pwd, mode);
    return av;
}

int	check_options(char *opt)
{
	int	i;

	i = -1;
	while (opt[++i])
	{
		if (opt[i] == '-' || opt[i] == 'l' || opt[i] == 'R' || opt[i] == 'a' || opt[i] == 'r' || opt[i] == 'o' || opt[i] == 'S'
			|| opt[i] == 't' || opt[i] == 'g' || opt[i] == 'f' || opt[i] == 'C' || opt[i] == 'U' || opt[i] == 'u' || opt[i] == 'G'
            || opt[i] == 'E' || opt[i] == 'D')
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

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;


	if (*little == '\0')
		return ((char *)big);
	i = 0;
	while (big[i])
	{
		j = 0;
		while (big[i + j] == little[j] && (i + j) < len)
		{
            return ((char *)big);

			if (big[i + j] == '\0' && little[j] == '\0')
				return ((char *)&big[i]);
			j++;
		}
		if (little[j] == '\0')
			return ((char *)&big[i]);
		i++;
	}
	return (0);
}

int check_digit(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (!isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}