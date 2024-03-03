#include "ft_ls.h"

void	print_more_infos(t_files *data, char **recup, t_recu **recu)
{
	t_recu *lst = *recu;
	
	int flag = 0;
	if (lst->next)
		flag = 1;
	while (lst)
	{
		if (!is_a_file(lst->pwd) && flag == 1)
		{
			ft_putstr(lst->pwd);
			ft_putstr(":\n");
		}

	struct	stat	info;
	int		u = 0;
	int	sizeL = 0;
	int	sizeU = 0;
	int	sizeG = 0;
	int	sizeS = 0;
	int	sizeD = 0;
	int	sizeH = 0;
	int	sizeBloc = 0;
	while (lst->paths[u])
	{
		if (lstat(lst->paths[u], &info) != 0)
			return ;

		char *tmp = ft_ltoa((long)info.st_nlink);
		if (sizeL < ft_strlen(tmp))
			sizeL = ft_strlen(tmp);


		struct passwd *pw = getpwuid(info.st_uid);
		struct group *gr = getgrgid(info.st_gid);
		if (sizeU < ft_strlen(tmp))
			sizeU = ft_strlen(pw ? pw->pw_name : "unknown");
		if (sizeG < ft_strlen(tmp))
			sizeG = ft_strlen(gr ? gr->gr_name : "unknown");

		free(tmp);
		tmp = ft_ltoa((long)info.st_size);
		if (sizeS < ft_strlen(tmp))
			sizeS = ft_strlen(tmp);
		free(tmp);

		time_t	modif;
		modif = info.st_mtime;
		char *timeString = ctime(&modif);
		char **timeSplit = ft_split(timeString, ' ');
		time_t actu = time(NULL);
		char	*date_actu = ctime(&actu);
		int	annee = atoi(date_actu + 20);

		if (annee == atoi(timeSplit[3]) && sizeH < 5)
			sizeH = 5;
		else
			sizeH = 4;
		if (ft_strlen(timeSplit[2]) > sizeD)
			sizeD = ft_strlen(timeSplit[2]);
		struct stat bloc;
		stat(lst->paths[u], &bloc);
		sizeBloc += (long)bloc.st_blocks;
		u++;
		freetab(timeSplit);
	}
	if (!is_a_file(lst->pwd))
	{
		char *str = ft_ltoa(sizeBloc / 2);
		ft_putstr("total ");
		ft_putstr(str);
		ft_putstr("\n");
		free(str);
	}


	u = 0;
	while (lst->paths[u])
	{
		if (lstat(lst->paths[u], &info) != 0)
			return ;

		// Affichage des droits

		ft_putstr((S_ISLNK(info.st_mode)) ? "l" : (S_ISDIR(info.st_mode)) ? "d" : "-");
		ft_putstr((info.st_mode & S_IRUSR) ? "r" : "-");
		ft_putstr((info.st_mode & S_IWUSR) ? "w" : "-");
		ft_putstr((info.st_mode & S_IXUSR) ? "x" : "-");
		ft_putstr((info.st_mode & S_IRGRP) ? "r" : "-");
		ft_putstr((info.st_mode & S_IWGRP) ? "w" : "-");
		ft_putstr((info.st_mode & S_IXGRP) ? "x" : "-");
		ft_putstr((info.st_mode & S_IROTH) ? "r" : "-");
		ft_putstr((info.st_mode & S_IWOTH) ? "w" : "-");
		ft_putstr((info.st_mode & S_IXOTH) ? "x" : "-");
		

		// Affichage du nombre de liens

		char *tmp = ft_ltoa((long)info.st_nlink);
		
		int lenL = ft_strlen(tmp);
		ft_putstr(" ");
		lenL = sizeL - lenL;
		while (lenL)
		{
			ft_putstr(" ");
			lenL--;
		}
		ft_putstr(tmp);
		free(tmp);

		// Affichage user et groupe
		
		struct passwd *pw = getpwuid(info.st_uid);
		struct group *gr = getgrgid(info.st_gid);
		
		int lenU = ft_strlen(pw->pw_name);
		ft_putstr(" ");
		lenU = sizeU - lenU;
		while (lenU)
		{
			ft_putstr(" ");
			lenU--;
		}
		ft_putstr(pw ? pw->pw_name : "unknown");
		if (data->g == false)
		{
			int lenG = ft_strlen(gr->gr_name);
			ft_putstr(" ");
			lenG = sizeG - lenG;
			while (lenG)
			{
				ft_putstr(" ");
				lenG--;
			}
			ft_putstr(gr ? gr->gr_name : "unknown");
		}


		// Affichage de la taille

		tmp = ft_ltoa((long)info.st_size);
		int lenS = ft_strlen(tmp);
		ft_putstr(" ");
		lenS = sizeS - lenS;
		while (lenS)
		{
			ft_putstr(" ");
			lenS--;
		}
		ft_putstr(tmp);
		free(tmp);

		ft_putstr(" ");

		time_t	modif;
		modif = info.st_mtime;
		char *timeString = ctime(&modif);
		char **timeSplit = ft_split(timeString, ' ');
		time_t actu = time(NULL);
		char	*date_actu = ctime(&actu);
		int	annee = atoi(date_actu + 20);
		
		ft_putstr(timeSplit[1]);
		int lenD = ft_strlen(timeSplit[2]);
		lenD = sizeD - lenD;
		ft_putstr(" ");
		while (lenD)
		{
			ft_putstr(" ");
			lenD--;
		}
		ft_putstr(timeSplit[2]);
		//printf("an = %d    ts = %d\n", atoi(timeSplit[4]), annee);
		ft_putstr(" ");
		if (annee ==  atoi(timeSplit[4]))
		{
			char **hours = ft_split(timeSplit[3], ':');
			ft_putstr(hours[0]);
			ft_putstr(":");
			ft_putstr(hours[1]);
			freetab(hours);
		}
		else
		{
			ft_putstr(" ");
			char **del = ft_split(timeSplit[4], '\n');
			ft_putstr(del[0]);
			freetab(del);
		}

		char	symL[4096];
		if (S_ISLNK(info.st_mode))
		{
			ssize_t symSize = readlink(lst->paths[u], symL, sizeof(symL) - 1);
			if (symSize != -1)
				symL[symSize] = 0;
		}

		ft_putstr(" ");
		if (S_ISDIR(info.st_mode))
			write(STDOUT_FILENO, COLOR_BLUE, ft_strlen(COLOR_BLUE));
		else if (S_ISLNK(info.st_mode))
			write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
		else if (info.st_mode & S_IXUSR)
			write(STDOUT_FILENO, COLOR_GREEN, ft_strlen(COLOR_GREEN));
		if (ft_strlen(lst->pwd) < ft_strlen(lst->paths[u]))
			ft_putstr(lst->paths[u] + ft_strlen(lst->pwd));
		else
			ft_putstr(lst->paths[u]);
		if (S_ISLNK(info.st_mode))
		{
			write(STDOUT_FILENO, COLOR_RESET, ft_strlen(COLOR_RESET));
			ft_putstr(" -> ");
			write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
			ft_putstr(symL);
		}
		write(STDOUT_FILENO, COLOR_RESET, ft_strlen(COLOR_RESET));
		ft_putstr("\n");
		u++;
		freetab(timeSplit);
	}
		lst = lst->next;
		if (lst)
			ft_putstr("\n");
	}

}