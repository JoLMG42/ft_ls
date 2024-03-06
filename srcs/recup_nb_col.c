#include "ft_ls.h"

char	***recup_nb_col(t_files *data, char **toprint, t_recu *lst)
{
	int     line = 1;
	char	***ret = NULL;
    int     allLen = tablen(toprint);
	int     maxRowSize = (allLen -1) * 2 + len_all_tab(toprint);
	int     colterm = 80;
    int     *padding = malloc(sizeof(int) * allLen);
	struct	winsize size;
	(void)data;
	(void)lst;

	if (allLen == 0)
	{
		free(padding);
		return NULL;
	}
	if (data->l && !data->R)
		maxRowSize = -1;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1)
    {
        perror("Error: ioctl\n");
        return NULL;
    }
    else
	{
    	colterm = size.ws_col;
	}


	if (!maxRowSize || maxRowSize > colterm)
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
            maxRowSize = ((allLen / predict) + (allLen % predict > 1) -1) * 2;
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
			return (NULL);
	}
	ret[k] = 0;
	int i = 0;
	while (toprint[i])
	{
		int x = i % line;
		int y = i / line;
		if (padding[y] < ft_strlen(toprint[i]) + 2)
			padding[y] = ft_strlen(toprint[i]) + 2;
		ret[x][y] = ft_strdup(toprint[i]);
		ret[x][y + 1] = 0;
		i++;
	}
	lst->padding = padding;
	return (ret);
}

	// if (maxRowSize + 1 > colterm)
	// {
	// 	while (maxRowSize + 1 > colterm && allLen != line)
	// 	{
	// 		if (padding)
	// 			free(padding);
	// 		padding = malloc(sizeof(int) * (int)(allLen / line+5));
	// 		if (!padding)
	// 			return (NULL);
	// 		int k = 0;
	// 		while (k < allLen / line+5)
	// 		{
	// 			padding[k] = 0;
	// 			k++;
	// 		}
	// 		if (ret)
	// 		 	freebigtab(ret);
	// 		ret = malloc(sizeof(char **) * (line + 1));
	// 		if (!ret)
	// 			return (NULL);
	// 		for (k = 0; k < line; ++k)
	// 		{
	// 			ret[k] = malloc(sizeof(char *) * ((allLen / line) + (allLen % line > k) + 1));
	// 			if (!ret[k])
	// 				return (NULL);
	// 		}
	// 		ret[k] = 0;
	// 		int i = 0;
	// 		while (toprint[i])
	// 		{
    //             int x = i % line;
    //             int y = i / line;
	// 			if (padding[y] < ft_strlen(toprint[i]) + 2)
	// 				padding[y] = ft_strlen(toprint[i]) + 2;
	// 			ret[x][y] = ft_strdup(toprint[i]);
	// 			ret[x][y + 1] = 0;
	// 			i++;
	// 		}
	// 		i = 0;
	// 		int u = 0;
	// 		int	allpad = 0;
	// 		while (u < allLen / line+1)
	// 		{
	// 			allpad += padding[u];
	// 			u++;
	// 		}
	// 		maxRowSize = allpad;
	// 		// if (line == 1)
    //         //     line = maxRowSize /colterm;
    //         // else
    //             line++;
	// 	}
	// 	lst->padding = padding;
	// 	data->padding = padding;
	// }
	// else
	// {
	// 	ret = malloc(sizeof(char **) * (line + 1));
	// 	if (!ret)
	// 		return (NULL);
	// 	int k = 0;
	// 	for (k = 0; k < line; ++k)
	// 	{
	// 		ret[k] = malloc(sizeof(char *) * ((allLen / line) + (allLen % line > k) + 1));
	// 		if (!ret[k])
	// 			return (NULL);
	// 	}
	// 	ret[k] = 0;
	// 	int i = 0;
	// 	while (toprint[i])
	// 	{
	// 		ret[i % line][i / line] = ft_strdup(toprint[i]);
	// 		ret[i % line][i / line + 1] = 0;
	// 		i++;
	// 	}
	// 	data->padding = NULL;
	// 	lst->padding = NULL;
	// }
