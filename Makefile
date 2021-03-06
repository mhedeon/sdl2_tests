NAME = test

#------------------------------- HEADERS ---------------------------------------
H_DIR = ./inc

H_LIST = test.h

HEADERS = $(addprefix $(H_DIR)/, $(H_LIST))
#-------------------------------------------------------------------------------

#------------------------------- MAIN ------------------------------------------
MAIN_DIR = ./src

MAIN_SRC = main.c

MAIN_SRC_LIST = $(addprefix $(MAIN_DIR)/, $(MAIN_SRC))
#-------------------------------------------------------------------------------

#------------------------------- OBJECTS ---------------------------------------
OBJ_DIR = ./obj

OBJ_LIST = $(addprefix $(OBJ_DIR)/, $(MAIN_SRC:.c=.o))
#-------------------------------------------------------------------------------

#------------------------------- LIBRARIES -------------------------------------
LIB_DIR = ./libraries

LIB_LIST = libmgl/libmgl.a

LIBRARIES = $(addprefix $(LIB_DIR)/, $(LIB_LIST))
#-------------------------------------------------------------------------------

#------------------------------- INCLUDES --------------------------------------
#FLAGS = -Wall -Werror -Wextra -g
FLAGS = -g

INCLUDES = -I $(H_DIR) \
 			-I$(LIB_DIR)/libmgl/frameworks/SDL2.framework//Headers/ \
			-I$(LIB_DIR)/libmgl/frameworks/SDL2_image.framework/Headers/ \
			-I$(LIB_DIR)/libmgl/frameworks/SDL2_mixer.framework/Headers/ \
			-I$(LIB_DIR)/libmgl/frameworks/SDL2_ttf.framework/Headers/ \
			-I$(LIB_DIR)/libmgl/frameworks/SDL2_net.framework/Headers/ \
			-F$(LIB_DIR)/libmgl/frameworks \
			-I$(LIB_DIR)/libmgl/headers/

FRAMEWORKS = -F$(LIB_DIR)/libmgl/frameworks \
			-rpath $(LIB_DIR)/libmgl/frameworks \
			-framework SDL2 \
			-framework SDL2_image \
			-framework SDL2_mixer \
			-framework SDL2_ttf \
			-framework SDL2_net
#-------------------------------------------------------------------------------

start: all
	./test

all: lib $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ_LIST) $(HEADERS) $(LIBRARIES)
	@gcc -o $(NAME) $(OBJ_LIST) -L$(LIB_DIR)/libmgl -lmgl $(FRAMEWORKS)

$(OBJ_DIR)/%.o : $(MAIN_DIR)/%.c $(HEADERS)
	@gcc $(FLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

lib:
	@make -C $(LIB_DIR)/libmgl

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
