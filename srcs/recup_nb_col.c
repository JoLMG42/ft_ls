#include "ft_ls.h"

char	***recup_nb_col(t_files *data, char **toprint, t_recu *lst)
{
	int	 line = 1;
	char	***ret = NULL;
	int	 colterm = 80;
	int	 allLen = tablen(toprint);
	int	 *padding = malloc(sizeof(int) * allLen);
	struct	winsize size;
	int	 maxRowSize = (allLen -1) * 2 + len_all_tab(toprint);
	
	data->flagQuote = 0;
	int n = 0;
	while (toprint[n])
	{
		if (ft_strnstr(toprint[n], "[{}&!=+()*", ft_strlen(toprint[n])))
		{
			data->flagQuote = 1;
			maxRowSize = (allLen -1) * 3 + len_all_tab(toprint);
			break ;
		}
		n++;
	}

	if (allLen == 0)
	{
		free(padding);
		return NULL;
	}
	if (data->l && !data->R)
		maxRowSize = -1;
	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1)
	{
		// perror("Error: ioctl\n");
		// return NULL;
	}
	else
	{
		colterm = size.ws_col;
	}


	if (!maxRowSize || maxRowSize >= colterm)
	{
		int min = 1;
		int max = allLen;
		while (min + 1 != max)
		{
			int predict = (min + max) / 2;
			for (int u = 0; u < allLen; u++)
				padding[u] = 0;
			int i = 0;
			while (toprint[i])
			{
				if (padding[i / predict] < ft_strlen(toprint[i]))
					padding[i / predict] = ft_strlen(toprint[i]);
				i++;
			}
			if (data->flagQuote)
				maxRowSize = ((allLen / predict) + (allLen % predict > 1)) * 3;
			else
				maxRowSize = ((allLen / predict) + (allLen % predict > 1)) * 2;
			int k = 0;
			while (k < (allLen / predict) + (allLen % predict > 1))
			{
				maxRowSize += padding[k];
				k++;
			}
			if (maxRowSize > colterm)
				min = predict;
			else
				max = predict;
			line = max;
			if (min == max)
				break ;
		}
	}
	for (int u = 0; u < allLen; u++)
		padding[u] = 0;
	ret = malloc(sizeof(char **) * (line + 1));
	if (!ret)
		return (NULL);
	ret[line] = 0;
	int k = 0;
	for (k = 0; k < line; ++k)
	{
		ret[k] = malloc(sizeof(char *) * ((allLen / line) + (allLen % line > k) + 1));
		if (!ret[k])
		{
			freebigtab(ret);
			return (NULL);
		}
	}
	ret[k] = 0;
	int i = 0;
	while (toprint[i])
	{
		int x = i % line;
		int y = i / line;
		if (data->flagQuote)
		{
			if (padding[y] < ft_strlen(toprint[i]) + 3)
				padding[y] = ft_strlen(toprint[i]) + 3;
		}
		else
		{
			if (padding[y] < ft_strlen(toprint[i]) + 2)
				padding[y] = ft_strlen(toprint[i]) + 2;
		}
		ret[x][y] = ft_strdup(toprint[i]);
		ret[x][y + 1] = 0;
		i++;
	}
	lst->padding = padding;
	return (ret);
}