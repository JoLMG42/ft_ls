#include "ft_ls.h"

char	*ft_strjoin2(char *s1, char *s2, char *res)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (s1[++i])
	{
		res[j] = s1[i];
		j++;
	}
	i = -1;
	while (s2[++i])
	{
		res[j] = s2[i];
		j++;
	}
	res[j] = '\0';
	return (res);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*res;

	if (!s1)
	{
		s1 = malloc(1);
		s1[0] = '\0';
	}
	if (!s1 || !s2)
		return (NULL);
	res = malloc(sizeof(char) * ((ft_strlen((char *)s1)
					+ ft_strlen((char *)s2)) + 1));
	if (!res)
		return (0);
	res = ft_strjoin2(s1, s2, res);
	free(s1);
	return (res);
}
