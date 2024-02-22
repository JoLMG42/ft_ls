#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

int	ft_strlen(char *str);

char	*ft_strdup(char *s)
{
	char	*cpy;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	cpy = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!cpy)
		return (0);
	while (s[i])
	{
		cpy[i] = s[i];
		i++;
	}
	cpy[i] = 0;
	return (cpy);
}
