NAME = webserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I $(INCLUDES)

LDFLAGS =

SERVER = Server.cpp Cgi.cpp Header.cpp

CLIENT = Client.cpp

REQUESTS = Request.cpp

RESPONSE = autoindex.cpp Response.cpp

IO = ServerInput.cpp ServerOutput.cpp

CONFIG = Config.cpp

SRCS			= $(addprefix srcs/,	\
					$(SERVER)			\
					$(CLIENT)			\
					$(REQUESTS)			\
					$(RESPONSE)			\
					$(CONFIG)			\
					$(IO)			\
					main.cpp)


OBJS_DIR = objs

HEADER = $(addprefix $(INCLUDES)/, webserv.hpp Server.hpp Client.hpp Request.hpp Response.hpp ServerInput.hpp ServerOutput.hpp Cgi.hpp Config.hpp Header.hpp)

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
