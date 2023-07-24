NAME := ircsrv
CC := c++
RM := rm -f
FLAGS := -Wall -Werror -Wextra -std=c++98 -g

HEADDIR := includes/
HEADLIST := Channel.hpp \
			Client.hpp \
			Server.hpp \
			utils.hpp \
			numericsMarcros.hpp \
			irc.hpp

HEADERS := $(addprefix ${HEADDIR}, ${HEADLIST})

SRCSDIR := srcs/
SRCSLIST := mode.cpp \
			main.cpp \
			join.cpp \
			nick.cpp \
			notice.cpp \
			topic.cpp \
			user.cpp \
			Channel.cpp \
			utils.cpp \
			ping.cpp \
			Client.cpp \
			commands.cpp \
			invite.cpp \
			pass.cpp \
			kick.cpp \
			Server.cpp \
			privmsg.cpp \
			part.cpp \
			quit.cpp

SRCS := $(addprefix ${SRCSDIR}, ${SRCSLIST})

OBJSDIR := obj/
OBJSLIST := ${SRCSLIST:.cpp=.o}
OBJS := $(addprefix ${OBJSDIR}, ${OBJSLIST})

DEPFILES := $(OBJS:.o=.d)

CYAN := \033[0;36m
RESET := \033[0m

all: ${NAME}

${NAME}: ${OBJS}
	@echo "${CYAN}Compiling ${NAME} ...${RESET}"
	${CC} ${FLAGS} ${OBJS} -o ${NAME}
	@echo "${CYAN}${NAME} Created${RESET}"

-include ${DEPFILES}

${OBJSDIR}%.o: ${SRCSDIR}%.cpp | ${OBJSDIR}
	${CC} ${FLAGS} -MMD -c $< -o $@

${OBJSDIR}:
	mkdir -p ${OBJSDIR}

.PHONY: all clean fclean re

val: all
	make -C ./ clean
	valgrind --leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--verbose \
	--log-file=valgrind-out.txt \
		./$(NAME) 6667 pass123

clean:
	@echo "${CYAN}Deleting ${NAME} Objects ...${RESET}"
	${RM} -r ${OBJSDIR}

fclean: clean
	@echo "${CYAN}Deleting ${NAME} Executable ...${RESET}"
	${RM} ${NAME}

re: fclean all

