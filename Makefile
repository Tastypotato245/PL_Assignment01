NAME	= parser
SRCS	= lexer.cpp parser.cpp main.cpp
OBJS	= $(SRCS:.cpp=.o)

CC		= clang++


all		: $(NAME)

$(NAME)	: $(OBJS) $(HEAD)
	$(CC) $(OBJS) -o $@ -I./

%.o		: %.cpp
	$(CC) -c $^ -o $@

clean	:
	rm -f $(OBJS)

fclean	:
	rm -f $(OBJS)
	rm -f $(NAME)

re		:
	make all
	make fclean

.PHONY	: all clean fclean re
