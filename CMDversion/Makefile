# 编译器
CC = gcc

# 编译选项（可以视情况添加调试信息）
CFLAGS = -Wall -Wextra -std=c11 -g

# 项目结构
# 最终生成./game可执行文件
TARGET = game
SRC = main.c npc_controller.c dealer.c
OBJ = $(SRC:.c=.o)

# 默认目标
all: $(TARGET)

# 链接阶段
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# 编译 .c 为 .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成文件
clean:
	rm -f $(OBJ) $(TARGET)

# 伪目标（防止和文件重名冲突）
.PHONY: all clean
