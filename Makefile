#  |  |  ___ \    \  |         |
#  |  |     ) |  |\/ |   _  |  |  /   _ 
# ___ __|  __/   |   |  (   |    <    __/ 
#    _|  _____| _|  _| \__,_| _|\_\ \___|
#                              by jcluzet
################################################################################
#                                     CONFIG                                   #
################################################################################
NAME        := irc
CC         := c++
FLAGS    :=  -g -std=c++98 -Wall -Werror -Wextra
 
################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

SRCS        :=      srcs/main.cpp \
                          srcs/mode.cpp \
                          srcs/join.cpp \
                          srcs/nick.cpp \
                          srcs/notice.cpp \
                          srcs/topic.cpp \
                          srcs/user.cpp \
                          srcs/Channel.cpp \
                          srcs/utils.cpp \
                          srcs/ping.cpp \
                          srcs/Client.cpp \
                          srcs/commands.cpp \
                          srcs/invite.cpp \
                          srcs/pass.cpp \
                          srcs/kick.cpp \
                          srcs/Server.cpp \
                          srcs/privmsg.cpp \
                          srcs/part.cpp \
                          srcs/quit.cpp
                          
OBJS        := ${SRCS:.cpp=.o}

.cpp.o:
	${CC} ${FLAGS} -c $< -o ${<:.cpp=.o}

################################################################################
#                                  Makefile  objs                              #
################################################################################


CLR_RMV		:= \033[0m
RED		    := \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m
RM		    := rm -f

${NAME}:	${OBJS}
			@echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			${CC} ${FLAGS} -o ${NAME} ${OBJS}
			@echo "$(GREEN)$(NAME) created[0m ✔️"

all:		${NAME}

bonus:		all

clean:
			@ ${RM} *.o */*.o */*/*.o
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)objs ✔️"

fclean:		clean
			@ ${RM} ${NAME}
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)binary ✔️"

re:			fclean all

.PHONY:		all clean fclean re


