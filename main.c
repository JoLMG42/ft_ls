#include <sys/types.h>
#include <dirent.h>

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

int	main(int ac, char **av)
{
	if (ac == 1)
	{
		struct dirent *tab;
		DIR *actu = opendir("/mnt/nfs/homes/jtaravel/Documents/");
		char **store;
		int size = 0;
		tab = readdir(actu);
		while (tab)
		{
			size++;
			tab = readdir(actu);
		}
		store = malloc(sizeof(char *) * size + 1);
		actu = opendir("/mnt/nfs/homes/jtaravel/Documents/");
		tab = readdir(actu);
		int i = 0;
		while (tab)
		{
			store[i] = tab->d_name;
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
}
