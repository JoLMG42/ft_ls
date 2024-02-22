#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

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
	write(1, "\n", 1);
}

char	**ft_swap(int ac, char **av)
{
	int		i;
	int		j;
	char	*swap;

	i = 1;
	while (i != ac)
	{
		j = 1;
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

int	check_options(char *opt)
{
	int	i;

	i = -1;
	while (opt[++i])
	{
		if (opt[i] == '-' || opt[i] == 'l' || opt[i] == 'R' || opt[i] == 'a' || opt[i] == 'r' || opt[i] == 't')
			i++;
		else
			return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	if (ac == 1)
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
	}
	else
	{
		int	i = 1;
		int	c = 0;
		int	k = 0;
		int	j;
		char	**toopen;

		while (av[i])
		{
			if (opendir(av[i]))
				c++;
			else if (check_options(av[i]))
			{
				printf("Error\n");
				return (1);
			}
			i++;
		}
		toopen = malloc(sizeof(char *) + c + 1);
		i = 1;
		j = 0;
		while (av[i])
		{
			if (opendir(av[i]))
			{
				toopen[j] = ft_strdup(av[i]);
				j++;
			}
			else
				k++;
			i++;
		}
		toopen[j] = 0;
		j = 0;
		while (toopen[j])
		{
			printf("%s\n", toopen[j]);
			j++;
		}



		
	}

}
