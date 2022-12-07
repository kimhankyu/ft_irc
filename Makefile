CC							=		c++
CXXFLAG						=		-std=c++98 -Wall -Wextra -Werror
RM							=		rm

NAME						=		ircserv
SRC_DIR						=		./srcs/
HEADER						=		./includes/
SRC_LIST					=		main.cpp\
									User.cpp\
									Server.cpp\
									Command.cpp\
									Util.cpp
									


SRC							=		$(addprefix $(SRC_DIR), $(SRC_LIST))
OBJS						=		$(SRC:.cpp=.o)

%.o							:		%.cpp
		$(CC) $(CXXFLAG) -I $(HEADER) -c $< -o $@


$(NAME)						:		$(OBJS)
		$(CC) $(CXXFLAG) -I $(HEADER) $(OBJS)  -o $(NAME)

.PHONY						:		all
all							:		$(NAME)

.PHONY						:		clean
clean						:
		$(RM) $(OBJS)

.PHONY						:		fclean
fclean						:		clean
		$(RM) $(NAME)

.PHONY						:		re
re							:		fclean all
