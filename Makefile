NAME = webserv

CXX = c++

CXXFLAGS = -Wall -Wextra -std=c++98  -g3 -I $(INCLUDES) #-Werror

LDFLAGS =

SERVER = Server.cpp Cgi.cpp

CLIENT = Client.cpp

REQUESTS = Request.cpp

RESPONSE = autoindex.cpp

IO = ServerInput.cpp ServerOutput.cpp

SRCS			= $(addprefix srcs/,	\
					$(SERVER)			\
					$(CLIENT)			\
					$(REQUESTS)			\
					$(RESPONSE)			\
					$(IO)			\
					main.cpp)


OBJS_DIR = objs

HEADER = $(addprefix $(INCLUDES)/, webserv.hpp Server.hpp Client.hpp Request.hpp ServerInput.hpp ServerOutput.hpp Cgi.hpp)

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
