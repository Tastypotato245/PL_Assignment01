NAME	= parser
SRCS	= parser.cpp main.cpp treenode.cpp utils.cpp
OBJS	= $(SRCS:.cpp=.o)
HEAD	= parser.h treenode.h utils.h

CC		= clang++ -std=c++11

all		: $(NAME)

$(NAME)	: $(OBJS) $(HEAD)
	$(CC) $(OBJS) -I./ -o $@

%.o		: %.cpp
	$(CC) -c $^ -o $@

clean	:
	rm -f $(OBJS)

fclean	:
	rm -f $(OBJS)
	rm -f $(NAME)

re		:
	make fclean
	make all

test	:
	make re
	./parser file.txt

.PHONY	: all clean fclean re test
