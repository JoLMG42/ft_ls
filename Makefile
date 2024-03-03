NAME	=	ft_ls

SRCS_FILES	=	../main.cpp exec_ls_no_args.c  free.c  ft_ltoa.c  ft_split.c  ft_strdup.c  \
				ft_strjoin.c  init_data.c  main.c  optionL.c  optionR.c  prints.c  recup_nb_col.c  \
				storage.c  utils.c

INC_FILES	=	ft_ls.h

SRCS	=	$(addprefix srcs/, $(SRCS_FILES))

INCS	=	$(addprefix includes/, $(INC_FILES))

CC	=	cc

FLAGS	=	-Wall -Wextra -Werror -I./includes -g3

OBJS_FILES	=	$(SRCS_FILES:%.cpp=%.o)
BOBJS_FILES	=	$(SRCS_FILES:%.cpp=%.o)
TOBJS_FILES	=	$(SRCS_FILES:%.cpp=%.o)

OBJS	=	$(addprefix objs/, $(OBJS_FILES))
BOBJS	=	$(addprefix bobjs/, $(BOBJS_FILES))
TOBJS	=	$(addprefix tobjs/, $(TOBJS_FILES))

DEP		=	$(OBJS:%.o=%.d)

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		$(CC) $(FLAGS) -o $(NAME) $(OBJS)

clean	:
	rm -rf $(OBJS) $(DEP)
	rm -rf $(BOBJS)
	rm -rf $(TOBJS)
	rm -rf objs/
	rm -rf main.d

fclean	:	clean
	rm -rf $(NAME)

re	:	fclean all

./objs/%.o :	./srcs/%.cpp $(INCS)
		mkdir -p objs
		$(CC) $(FLAGS) -MMD -I ./includes/ -o $@ -c $<

.PHONY : all re clean fclean