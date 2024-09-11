NAME = ircserv
CPP = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
INC = server.hpp commands/ChannelFile.hpp commands/Message.hpp commands/Tools.hpp
SRC = server.cpp main.cpp commands/ChannelFile.cpp commands/Commands.cpp    commands/Message.cpp commands/Tools.cpp
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(INC)
	$(CPP) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re