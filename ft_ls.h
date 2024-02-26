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

typedef	struct s_recu
{
	char		*pwd;
	char		*oldpwd;
	char		**files;
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
}	t_files;

int	ft_strlen(char *str);
char	*ft_strjoin(char *s1, char *s2);



#endif
