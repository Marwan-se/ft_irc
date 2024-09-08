NAME = ircserv
CPP = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
INC = src/server.hpp src/client.hpp
SRC = src/server.cpp main.cpp src/client.cpp
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