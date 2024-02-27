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

#define COLOR_BLUE   "\x1b[34m"
#define COLOR_RESET  "\x1b[0m"
#define COLOR_GREEN  "\x1b[32m"

typedef	struct s_recu
{
	char		*pwd;
	char		*oldpwd;
	char		**files;
	char		**paths;
	char		**dirs;
	struct s_recu	*next;
}	t_recu;

typedef struct s_files
{
	char	**toopen;
	char	**toread;
	char	**files;
	bool	l;
	bool	a;
	bool	R;
	bool	r;
	bool	t;
	bool	g;
}	t_files;

int	ft_strlen(char *str);
char	*ft_strjoin(char *s1, char *s2);
char	**ft_split(char *s, char c);
int	ft_strcmp(char *s1, char *s2);
void	ft_putstr(char *str);
void	ft_putstr_fd(char *str, int fd);
char	**ft_swap(int ac, char **av);
void	ft_putchar_fd(char c, int fd);
int	is_a_file(char *str);
int	tablen(char **tab);
void	freetab(char **tab);
char	**ft_strduptab(char **tab);
void	reverse_tab(char **tab, int len);
char	*ft_strdup(char *str);


#endif
