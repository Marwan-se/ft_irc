NAME = ircserv
CPP = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
INC = inc/ChannelFile.hpp inc/Message.hpp     inc/RPL.hpp         inc/Tools.hpp       inc/client.hpp      inc/server.hpp
SRC = main.cpp src/ChannelFile.cpp src/Commands.cpp    src/Message.cpp     src/Tools.cpp       src/auth.cpp        src/client.cpp      src/nick.cpp        src/pass.cpp        src/server.cpp      src/user.cpp
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