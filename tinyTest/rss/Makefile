# 指定编译器
CXX := g++

# 编译选项
CXXFLAGS := -Wall -g -llog4cpp -lpthread -ltinyxml2

# 最终的可执行文件名
EXEC := result

# 查找当前目录下的所有 .cpp 和 .cc 文件
SRCS := $(wildcard *.cpp *.cc)

# 生成对应的 .o 文件
OBJS := $(SRCS:.cpp=.o)
OBJS := $(OBJS:.cc=.o)

# 默认目标（编译生成可执行文件）
$(EXEC): $(OBJS)
	$(CXX)  $^ -o $@ $(CXXFLAGS)
	rm -f $(OBJS)

# 单个源文件编译成 .o 文件
%.o: %.cpp
	$(CXX)  -c $< -o $@ $(CXXFLAGS)

%.o: %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# rebuild 目标：清理并重新编译
.PHONY: rebuild
rebuild: clean $(EXEC)

# 清理生成的文件
.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC) 

