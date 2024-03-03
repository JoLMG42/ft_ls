#include "ft_ls.h"

char	***recup_nb_col(t_files *data, char **toprint, t_recu **recu, t_recu *lst)
{
	int	line = 1;
	char	***ret;// = malloc(sizeof(char **) * (line + 1));
	int	maxRowSize = len_all_tab(toprint) + (tablen(toprint) - 1) * 2;
	int	colterm = 80;
	struct winsize size;
	int	*padding;
	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1)
	{
		perror("Error: ioctl\n");
		return NULL;
	}
	else
		colterm = size.ws_col;

	if (maxRowSize + 3 > colterm)
	{
		while (maxRowSize + 3 > colterm)
		{
			padding = malloc(sizeof(int) * (int)(tablen(toprint) / line));
			int k = 0;
			while (k < tablen(toprint) / line)
			{
				padding[k] = 0;
				k++;
			}
			
			ret = malloc(sizeof(char **) * (line + 1));
			for (k = 0; k < line; ++k)
			{
				ret[k] = malloc(sizeof(char *) * ((tablen(toprint) / line) + (tablen(toprint) % line > k) + 1));
			}
			ret[k] = 0;
			int i = 0;
			while (toprint[i])
			{
				if (padding[i / line] < ft_strlen(toprint[i]) + 2)
				{
					padding[i / line] = ft_strlen(toprint[i]) + 2;
				}
				ret[i % line][i / line] = ft_strdup(toprint[i]);
				ret[i % line][i / line + 1] = 0;
				i++;
			}
			i = 0;
			int newMaxRowSize = 0;
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
		data->padding = padding;
	}
	else
	{
		ret = malloc(sizeof(char **) * (line + 1));
		int k = 0;
		for (k = 0; k < line; ++k)
		{
			ret[k] = malloc(sizeof(char *) * ((tablen(toprint) / line) + (tablen(toprint) % line > k) + 1));
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
	}
	lst->padding = data->padding;
	return (ret);
}