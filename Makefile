NAME	= parser
SRCS	= parser.cpp main.cpp
OBJS	= $(SRCS:.cpp=.o)
HEAD	= parser.h

CC		= clang++


all		: $(NAME)

$(NAME)	: $(OBJS) $(HEAD)
	$(CC) $(OBJS) -include $(HEAD) -o $@

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
