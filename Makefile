MAKE	= make

#==============================================================================#
#                                RESOURCES URLS                                #
#==============================================================================#

MLX_URL			= https://cdn.intra.42.fr/document/document/40925/minilibx-linux.tgz

#==============================================================================#
#                                     NAMES                                    #
#==============================================================================#

NAME 			= cub3D
#NAME_BONUS 		= cub3D_bonus

### Message Vars

_SUCCESS 		= [$(GRN)SUCCESS$(D)]

#==============================================================================#
#                                    PATHS                                     #
#==============================================================================#

SRC_PATH		= src
INC_PATH		= includes
LIBS_PATH		= lib
BUILD_PATH		= .build

#BONUS_SRC_PATH		= src_bonus
#BONUS_INC_PATH		= includes_bonus
#BONUS_BUILD_PATH		= .build_bonus


FILES			=	main.c \
					canvas/canvas.c \
					canvas/parsing.c \
					game.c \
					player.c

SRC						= $(addprefix $(SRC_PATH)/, $(FILES))
OBJS					= $(SRC:$(SRC_PATH)/%.c=$(BUILD_PATH)/%.o)
HEADERS				= $(INC_PATH)/cub.h

#BONUS_FILES	= main_bonus.c

#BONUS_SRC						= $(addprefix $(BONUS_SRC_PATH)/, $(BONUS_FILES))
#BONUS_OBJS					= $(BONUS_SRC:$(BONUS_SRC_PATH)/%.c=$(BONUS_BUILD_PATH)/%.o)
#BONUS_HEADERS				= $(BONUS_INC_PATH)/so_long_bonus.h


MLX_PATH			= $(LIBS_PATH)/mlx
MLX_ARC				= $(MLX_PATH)/libmlx_Linux.a
LIBFT_PATH		= $(LIBS_PATH)/libft
LIBFT_ARC		= $(LIBFT_PATH)/libft.a


#==============================================================================#
#                              COMPILER & FLAGS                                #
#==============================================================================#

CC						= cc

CFLAGS				= -Wall -Wextra -Werror -g

RFLAGS				= -L ./lib/mlx -lmlx -L/usr/lib/X11 -lXext -lX11 -lm -g3 -lbsd

INC						= -I $(INC_PATH)
#BONUS_INC						= -I $(BONUS_INC_PATH)

RM		= rm -rf
AR		= ar rcs
MKDIR_P	= mkdir -p

#==============================================================================#
#                                  RULES                                       #
#==============================================================================#


all: libs $(BUILD_PATH) $(NAME)	## Compile

$(NAME): $(BUILD_PATH) $(LIBFT_ARC) $(MLX_ARC) $(OBJS)			## Compile
	@echo "$(GOLD)Compiling $(SILV)$(NAME)$(GOLD) mandatory version$(D)"
	$(CC) $(CFLAGS) $(OBJS) $(INC) $(LIBFT_ARC) $(MLX_ARC) $(RFLAGS) -o $(NAME)
	@echo "[$(_SUCCESS) compiling $(SILV)$(NAME)$(D) $(GOLD)🖔$(D)]"
	@clear

deps:		## Download/Update deps
	@if test -d "$(MLX_PATH)"; then \
		echo " $(RED)$(D) [$(GRN)Nothing to be done!$(D)]"; fi
	@if test ! -d "$(MLX_PATH)"; then make get_mlx; \
		else echo "$(GOLD)[mlx]$(D) folder found 🖔"; fi


libs: $(LIBFT_ARC) $(MLX_ARC)
	@echo "* $(GOLD)Libraries ready$(D)"
	

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@echo -n "$(GRN)█$(D)"
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(BUILD_PATH):
	$(MKDIR_P) $(BUILD_PATH)
	@echo "* $(GOLD)Creating $(SILV)$(BUILD_PATH)$(GOLD) folder:$(D) $(_SUCCESS)"

$(MLX_ARC):
	@if test ! -d "$(MLX_PATH)"; then \
		$(MAKE) get_mlx; \
	fi
	$(MAKE) -C $(MLX_PATH)

$(LIBFT_ARC):
	$(MAKE) -C $(LIBFT_PATH) extra

get_mlx:
	@echo "$(BLU)Getting MLX submodule$(D)"
	@if test -d "$(MLX_PATH)"; then \
		echo "$(GRN)MLX submodule already exists 🖔"; \
	else \
		cd $(LIBS_PATH) && curl -kL $(MLX_URL) | tar zxvf - && \
		if [ -d minilibx-linux ]; then mv minilibx-linux mlx; fi && \
		echo "$(GRN)MLX submodule download$(D): $(_SUCCESS)"; \
	fi

#BONUS RULES
#bonus: all $(BONUS_BUILD_PATH) $(NAME_BONUS)	## Compile
#
#$(NAME_BONUS): $(BONUS_BUILD_PATH) $(LIBFT_ARC) $(MLX_ARC) $(BONUS_OBJS)
#	@echo "$(GOLD)Compiling $(MAG)$(NAME)$(GOLD) mandatory version$(D)"
#	$(CC) $(CFLAGS) $(BONUS_OBJS) $(BONUS_INC) $(LIBFT_ARC) $(MLX_ARC) $(RFLAGS) -o $(NAME_BONUS)
#	@echo "[$(_SUCCESS) compiling $(MAG)$(NAME_BONUS)$(D) $(GOLD)🖔$(D)]"
#
#$(BONUS_BUILD_PATH)/%.o: $(BONUS_SRC_PATH)/%.c $(BONUS_HEADERS)
#	@mkdir -p $(dir $@)
#	@echo -n "$(MAG)█$(D)"
#	$(CC) $(CFLAGS) $(BONUS_INC) -c $< -o $@
#
#$(BONUS_BUILD_PATH):
#	$(MKDIR_P) $(BONUS_BUILD_PATH)
#	@echo "* $(GOLD)Creating $(BLU)$(BONUS_BUILD_PATH)$(GOLD) folder:$(D) $(_SUCCESS)"
#

clean:				## Remove object files
	@echo "$(GOLD)Cleaning object files$(D)"
	$(RM) $(BUILD_PATH);
#$(RM) $(BONUS_BUILD_PATH);
	@echo "$(GOLD)Removing $(SILV)$(BUILD_PATH) $(D) folder & files$(D): $(_SUCCESS)"; \

fclean: clean			## Remove executable and .gdbinit
	@echo "$(GOLD)Cleaning executables$(D)"
	$(RM) $(NAME) 
# $(RM) $(NAME_BONUS);
	echo "$(GOLD)Removing $(SILV)$(NAME)$(D) file: $(_SUCCESS)"; \

libclean: fclean	## Remove libs
	@echo "$(GOLD)Cleaning libraries$(D)"
	$(RM) $(MLX_PATH)
	@echo "$(GOLD)Removing mlx folder & files!$(D) : $(_SUCCESS)"

val: re
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck ./$(NAME) $(MAP)

re: libclean all	## Purge & Recompile

.PHONY: bonus clean fclean re

#==============================================================================#
#                                  UTILS                                       #
#==============================================================================#

# Colors

B  		= $(shell tput bold)
RED		= $(shell tput setaf 1)
GRN		= $(shell tput setaf 2)
BLU		= $(shell tput setaf 4)

# Extended colors used in your makefile
GOLD    = $(shell tput setaf 220)
SILV    = $(shell tput setaf 250)

# Reset
D       = $(shell tput sgr0)