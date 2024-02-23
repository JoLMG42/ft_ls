#ifndef FT_LS_H

#define FT_LS_H

#include <stdbool.h>

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


#endif
