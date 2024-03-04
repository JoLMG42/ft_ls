#include "ft_ls.h"

char	***recup_nb_col(t_files *data, char **toprint, t_recu *lst)
{
	int	line = 1;
	char	***ret = NULL;
	int	maxRowSize = len_all_tab(toprint) + (tablen(toprint) - 1) * 2;
	int	colterm = 80;
	struct winsize size;
	int	*padding = NULL;
	if (tablen(toprint) == 0)
		line = 0;
	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1)
	{
		perror("Error: ioctl\n");
		return NULL;
	}
	else
		colterm = size.ws_col;

	if (maxRowSize + 15 > colterm)
	{
		while (maxRowSize + 15 > colterm)
		{
			if (padding)
				free(padding);
			padding = malloc(sizeof(int) * (int)(tablen(toprint) / line+5));
			if (!padding)
				return (NULL);
			int k = 0;
			while (k < tablen(toprint) / line+5)
			{
				padding[k] = 0;
				k++;
			}
			if (ret)
				freebigtab(ret);
			ret = malloc(sizeof(char **) * (line + 1));
			if (!ret)
				return (NULL);
			for (k = 0; k < line; ++k)
			{
				ret[k] = malloc(sizeof(char *) * ((tablen(toprint) / line) + (tablen(toprint) % line > k) + 1));
				if (!ret[k])
					return (NULL);
			}
			ret[k] = 0;
			int i = 0;
			while (toprint[i])
			{
				if (padding[i / line] < ft_strlen(toprint[i]) + 2)
					padding[i / line] = ft_strlen(toprint[i]) + 2;
				ret[i % line][i / line] = ft_strdup(toprint[i]);
				ret[i % line][i / line + 1] = 0;
				i++;
			}
			i = 0;
			int u = 0;
			int	allpad = 0;
			while (u < tablen(toprint) / line)
			{
				allpad += padding[u];
				u++;
			}
			maxRowSize = allpad;
			line++;
		}
		lst->padding = padding;
		data->padding = padding;
	}
	else
	{
		ret = malloc(sizeof(char **) * (line + 1));
		if (!ret)
			return (NULL);
		int k = 0;
		for (k = 0; k < line; ++k)
		{
			ret[k] = malloc(sizeof(char *) * ((tablen(toprint) / line) + (tablen(toprint) % line > k) + 1));
			if (!ret[k])
				return (NULL);
		}
		ret[k] = 0;
		int i = 0;
		while (toprint[i])
		{
			ret[i % line][i / line] = ft_strdup(toprint[i]);
			ret[i % line][i / line + 1] = 0;
			i++;
		}
		data->padding = NULL;
		lst->padding = NULL;
	}
	return (ret);
}
