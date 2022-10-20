NAME = webserv

CXX = c++

CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -I $(INCLUDES)

LDFLAGS =

SERVER = Server.cpp Cgi.cpp

CLIENT = Client.cpp

RESPONSE = autoindex.cpp

SRCS			= $(addprefix srcs/,	\
					$(SERVER)			\
					$(CLIENT)			\
					$(RESPONSE)			\
					main.cpp)


OBJS_DIR = objs

HEADER = $(addprefix $(INCLUDES)/, webserv.hpp Server.hpp Client.hpp)

INCLUDES = includes

OBJS = $(SRCS:srcs%.cpp=$(OBJS_DIR)%.o)


$(OBJS_DIR)/%.o : srcs/*/%.cpp $(HEADER)
	@mkdir -p $(OBJS_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJS_DIR)/%.o : srcs/*/*/%.cpp $(HEADER)
	@mkdir -p $(OBJS_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJS_DIR)/%.o : srcs/%.cpp $(HEADER)
	@mkdir -p $(OBJS_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

all: $(NAME) 

$(NAME): $(OBJS) $(HEADER)
	$(CXX) -o $(NAME) $(CXXFLAGS) $(OBJS) $(LDFLAGS)


clean:
	rm -rf $(OBJS)
	@rm -rd $(OBJS_DIR) || true

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re