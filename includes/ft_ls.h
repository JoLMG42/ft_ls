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
#include <time.h>
#include <math.h>
#include <string.h>
#include <sys/acl.h>
#include <sys/sysmacros.h>
#include <ctype.h>

#define COLOR_BLUE			"\x1b[34m"
#define COLOR_RESET			"\x1b[0m"
#define COLOR_GREEN			"\x1b[32m"
#define COLOR_GREEN_BACK	"\x1b[42m"
#define COLOR_RED			"\x1b[31m"
#define COLOR_CYAN			"\x1b[36m"
#define COLOR_BROWN			"\x1b[33m"
#define COLOR_RED_BACK	"\x1b[41m"
#define COLOR_YELLOW_BACK	"\033[1;32m"

typedef	struct s_recu
{
	char		*pwd;
	char		**paths;
	char		**dirs;
	int			*padding;
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
	bool	G;
	bool	f;
	bool	C;
	bool	U;
	bool	u;
	bool	S;
	bool	E;
	bool	D;

    int		flagQuote;
    unsigned long		sizeO;
    int		powerDig;
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
void	ft_lstdelone(t_recu *lst, void (*del)(void *));
void	ft_lstclear(t_recu **lst, void (*del)(void *));
void	freetab(char **tab);
void	free_all(t_files *data, t_recu **recu);
int		exec_ls_no_args(void);
void	one_line_print(char **d, char **tab);
void	print_more_infos(t_files *data, t_recu **recu);
char	***recup_nb_col(t_files *data, char **toprint, t_recu *lst);
int		len_all_tab(char **tab);
void	big_print(char ***dirs, char *pwd, t_files *data, t_recu *lst);
void	print_in_col(t_files *data, t_recu **recu);
void	print_list(t_files *data, t_recu **recu);
char	*ft_ltoa(long nb);
t_recu  *add_maillon(t_recu **head_ref, char *new_data, char **all, char **paths);
void	optionR(t_files *data, t_recu **recu);
void	recup_args(t_files *data, t_recu **recu);
void	freebigtab(char ***tab);
char	**sort_by_time_acces(int ac, char **av, char *pwd, int mode);
void	print_msg_help(void);
void	print_more_infos2(t_files *data, t_recu **recu, char ***tab);
char	*ft_strnstr(const char *big, const char *little, size_t len);
int check_digit(char *str);

#endif
