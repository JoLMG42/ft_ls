#include "ft_ls.h"

int	exec_ls_args(char **av)
{
	t_recu	*recu = NULL;
	t_files data;
	int ret = init_data(av, &data);
	if (ret == 1)
		return (1);
	if (data.R == true)
		optionR(&data, &recu);
	else
		recup_args(&data, &recu);

	print_list(&data, &recu);
	free_all(&data, &recu);
	return (0);
}

int	main(int ac, char **av)
{
	
	if (ac == 1)
		exec_ls_no_args();
	else
	{
		if (exec_ls_args(av))
			return (1);
	}
}
