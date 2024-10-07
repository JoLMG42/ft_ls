#include "ft_ls.h"

void	print_more_infos2(t_files *data, t_recu **recu, char ***tab)
{
	t_recu *lst = *recu;
	
	struct	stat	info;
	int		u = 0;
	int	sizeL = 0;
	int	sizeU = 0;
	int	sizeG = 0;
	int	sizeS = 0;
	int	sizeD = 0;
	int	sizeH = 0;
	int	sizeBloc = 0;
	int	plus = 0;
	int sizeMajor = 0;
	int sizeMinor = 0;

	int i = 0;
	while (tab[i])
	{
		int j = 0;
		while (tab[i][j])
		{
			char *tmpALL = ft_strjoin(ft_strjoin(ft_strdup(lst->pwd), "/"), tab[i][j]);
			if (lstat(tmpALL, &info) != 0)
			{
				if (lstat(tmpALL, &info) != 0)
				{
					free(tmpALL);
					return ;
				}
			}
			dev_t device = info.st_rdev;
			unsigned int minor_number = minor(device);
			char *s = ft_ltoa(minor_number);
			if (ft_strlen(s) > sizeMinor)
				sizeMinor = ft_strlen(s);
			free(s);
			unsigned int major_number = major(device);
			s = ft_ltoa(major_number);
			if (ft_strlen(s) > sizeMajor)
				sizeMajor = ft_strlen(s);
			free(s);

			if (info.st_mode & S_ISVTX)
				plus = 1;
			char *tmp = ft_ltoa((long)info.st_nlink);
			if (sizeL < ft_strlen(tmp))
				sizeL = ft_strlen(tmp);

			struct passwd *pw = getpwuid(info.st_uid);
			struct group *gr = getgrgid(info.st_gid);
			if (!pw)
			{
				u++;
				free(tmp);
				continue;
			}

			if (sizeU < ft_strlen(pw->pw_name))
				sizeU = ft_strlen(pw ? pw->pw_name : "unknown");
			if (sizeG < ft_strlen(gr->gr_name))
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
			lstat(tmpALL, &bloc);
			sizeBloc += (long)bloc.st_blocks;
			j++;
			freetab(timeSplit);
			free(tmpALL);
		}
		i++;
	}

	if (!is_a_file(lst->pwd))
	{
		char *str = ft_ltoa(sizeBloc / 2);
		ft_putstr("total ");
		ft_putstr(str);
		ft_putstr("\n");
		free(str);
	}

	i = 0;
	while (tab[i])
	{
		int j = 0;
		while (tab[i][j])
		{
			char *tmpALL = ft_strjoin(ft_strjoin(ft_strdup(lst->pwd), "/"), tab[i][j]);
			if (lstat(tmpALL, &info) != 0)
			{
				if (lstat(tmpALL, &info) != 0)
				{
					free(tmpALL);
					return ;
				}
			}
			// Affichage des droits
			if ((info.st_mode & S_IFMT) == S_IFBLK)
				ft_putstr("b");
			else if ((info.st_mode & S_IFMT) == S_IFCHR)
				ft_putstr("c");
			else
				ft_putstr((S_ISLNK(info.st_mode)) ? "l" : (S_ISDIR(info.st_mode)) ? "d" : "-");
			ft_putstr((info.st_mode & S_IRUSR) ? "r" : "-");
			ft_putstr((info.st_mode & S_IWUSR) ? "w" : "-");
			if (info.st_mode & S_ISUID)
				ft_putstr("s");
			else
				ft_putstr((info.st_mode & S_IXUSR) ? "x" : "-");
			ft_putstr((info.st_mode & S_IRGRP) ? "r" : "-");
			ft_putstr((info.st_mode & S_IWGRP) ? "w" : "-");
			if (info.st_mode & S_ISUID)
				ft_putstr("s");
			else
				ft_putstr((info.st_mode & S_IXGRP) ? "x" : "-");
			ft_putstr((info.st_mode & S_IROTH) ? "r" : "-");
			ft_putstr((info.st_mode & S_IWOTH) ? "w" : "-");
			ft_putstr((info.st_mode & S_ISVTX) ? "t" : (info.st_mode & S_IXOTH) ? "x" : "-");
			if (plus == 1)
			{
				if (info.st_mode & S_ISVTX)
					ft_putstr("+");
				else
					ft_putstr(" ");
			}

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
			
			if (!pw)
			{
				int lenU = ft_strlen("???");
				ft_putstr(" ");
				lenU = sizeU - lenU;
				ft_putstr("???");
				while (lenU && lenU > 0)
				{
					ft_putstr(" ");
					lenU--;
				}
			}
			else if (data->g == false)
			{
				int lenU = ft_strlen(pw->pw_name);
				ft_putstr(" ");
				lenU = sizeU - lenU;
				ft_putstr(pw ? pw->pw_name : "unknown");
				while (lenU && lenU > 0)
				{
					ft_putstr(" ");
					lenU--;
				}
			}

			if (data->G == false)
			{
				int lenG = ft_strlen(gr->gr_name);
				ft_putstr(" ");
				lenG = sizeG - lenG;
				ft_putstr(gr ? gr->gr_name : "unknown");
				while (lenG && lenG > 0)
				{
					ft_putstr(" ");
					lenG--;
				}
			}

			dev_t device = info.st_rdev;
			unsigned int minor_number = minor(device);
			unsigned int major_number = major(device);
			if ((info.st_mode & S_IFMT) == S_IFBLK || (info.st_mode & S_IFMT) == S_IFCHR)
			{
				char *s = ft_ltoa(major_number);
				int spaces = sizeMajor - ft_strlen(s);
				ft_putstr(" ");
				while (spaces && spaces > 0)
				{
					ft_putstr(" ");
					spaces--;
				}
				ft_putstr(s);
				ft_putstr(",");
				ft_putstr(" ");
				free(s);

				s = ft_ltoa(minor_number);
				spaces = sizeMinor - ft_strlen(s);
				while (spaces && spaces > 0)
				{
					ft_putstr(" ");
					spaces--;
				}
				ft_putstr(s);
				free(s);
			}
			else
			{
				char *s = ft_ltoa(sizeMajor);
				char *s1 = ft_ltoa(sizeMinor);
				int spaces = ft_strlen(s) + ft_strlen(s1) + 1;
				while (spaces && spaces > 0 && sizeMajor!= 1 && sizeMinor != 1)
				{
					ft_putstr(" ");
					spaces--;
				}
				free(s);
				free(s1);
				ft_putstr(" ");
			}

			// Affichage de la taille
			if ((info.st_mode & S_IFMT) == S_IFBLK || (info.st_mode & S_IFMT) == S_IFCHR)
				ft_putstr(" ");
			else
			{
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
			}
			time_t	modif;
			if (!data->u)
				modif = info.st_mtime;
			else
				modif = info.st_atime;
			char *timeString = ctime(&modif);
			char **timeSplit = ft_split(timeString, ' ');
			time_t actu = time(NULL);
			char	*date_actu = ctime(&actu);
			int	annee = atoi(date_actu + 20);
			
			ft_putstr(timeSplit[1]);
			int lenD = ft_strlen(timeSplit[2]);
			lenD = sizeD - lenD;
			ft_putstr(" ");
			while (lenD && lenD > 0)
			{
				ft_putstr(" ");
				lenD--;
			}
			ft_putstr(timeSplit[2]);
			ft_putstr(" ");
			if (annee ==  atoi(timeSplit[4]) || actu - modif < 15770000 )
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
				ssize_t symSize = readlink(tmpALL, symL, sizeof(symL) - 1);
				if (symSize != -1)
				{
					symL[symSize] = 0;
				}
			}

			struct stat links;

			ft_putstr(" ");
			if (!data->f && (info.st_mode & S_IFMT) == S_IFBLK)
				write(STDOUT_FILENO, COLOR_BROWN, ft_strlen(COLOR_BROWN));
			else if (!data->f && (info.st_mode & S_IFMT) == S_IFCHR)
				write(STDOUT_FILENO, COLOR_BROWN, ft_strlen(COLOR_BROWN));
			else if (S_ISDIR(info.st_mode) && !data->f && data->C)
				write(STDOUT_FILENO, COLOR_BLUE, ft_strlen(COLOR_BLUE));
			else if (S_ISLNK(info.st_mode) && !data->f && data->C)
			{
				if (lstat(symL, &links))
					write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
				else
					write(STDOUT_FILENO, COLOR_CYAN, ft_strlen(COLOR_CYAN));
			}
			else if (info.st_mode & S_IXUSR && !data->f && data->C)
				write(STDOUT_FILENO, COLOR_GREEN, ft_strlen(COLOR_GREEN));
			if (info.st_mode & S_ISVTX && !data->f && data->C)
				write(STDOUT_FILENO, COLOR_GREEN_BACK, ft_strlen(COLOR_GREEN_BACK));
			ft_putstr(tab[i][j]);
			if (S_ISLNK(info.st_mode) && !data->f && data->C)
			{
				write(STDOUT_FILENO, COLOR_RESET, ft_strlen(COLOR_RESET));
				ft_putstr(" -> ");
				if (lstat(symL, &links))
					write(STDOUT_FILENO, COLOR_RED, ft_strlen(COLOR_RED));
				else
					write(STDOUT_FILENO, COLOR_BLUE, ft_strlen(COLOR_BLUE));
				ft_putstr(symL);
			}
			if (S_ISLNK(info.st_mode) && !data->f && !data->C)
			{
				ft_putstr(" -> ");
				ft_putstr(symL);
			}
			write(STDOUT_FILENO, COLOR_RESET, ft_strlen(COLOR_RESET));
			ft_putstr("\n");
			j++;
			freetab(timeSplit);
			free(tmpALL);
		}
		i++;
	}
}