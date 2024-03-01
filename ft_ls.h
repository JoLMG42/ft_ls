#ifndef FT_LS_H

#define FT_LS_H

#include <stdbool.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/ioctl.h>
#include <pwd.h>
#include <grp.h>
#include<math.h>


#define COLOR_BLUE	"\x1b[34m"
#define COLOR_RESET	"\x1b[0m"
#define COLOR_GREEN	"\x1b[32m"
#define COLOR_RED	"\x1b[31m"

typedef	struct s_col
{
	int		pading;
	struct s_col	*next;
}	t_col;

typedef	struct s_recu
{
	char		*pwd;
	char		**paths;
	char		**dirs;
	struct s_recu	*next;
}	t_recu;

typedef struct s_files
{
	char	**toread;
	int	*padding;
	bool	l;
	bool	a;
	bool	R;
	bool	r;
	bool	t;
	bool	g;
}	t_files;

int		ft_strlen(char *str);
char	*ft_strjoin(char *s1, char *s2);
char	**ft_split(char *s, char c);
int		ft_strcmp(char *s1, char *s2);
void	ft_putstr(char *str);
void	ft_putstr_fd(char *str, int fd);
char	**ft_swap(int ac, char **av);
void	ft_putchar_fd(char c, int fd);
int		is_a_file(char *str);
int		tablen(char **tab);
void	freetab(char **tab);
char	**ft_strduptab(char **tab);
char	**reverse_tab(char **tab, int len);
char	*ft_strdup(char *str);
char	**sort_by_time(int ac, char **av, char *pwd, int mode);
int		check_options(char *opt);
int		init_data(char **av, t_files *data);

#endif
