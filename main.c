#include <sys/types.h>
#include <dirent.h>

int	main(int ac, char **av)
{
	if (ac == 1)
	{
		struct dirent *tab;
		DIR *actu = opendir("/mnt/nfs/homes/jtaravel/Documents/");
		tab = readdir(actu);
		for (int k = 0; k < 10; k++)
		{
			printf("%s\n", tab[k].d_name);
		}
	}
}
