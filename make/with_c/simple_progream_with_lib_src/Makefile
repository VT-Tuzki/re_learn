#V1版本输入文件改为变量

OBJS = src/hello.o lib/def.o
TARGET = build/hello.out

#hello.out 为执行二进制文件， 生成hello.out 依赖 hello.c 和def.c 所以添加依赖
$(TARGET): $(OBJS)
	cc -o $(TARGET) $(OBJS)

# make隐式规则， 即使不定义hello.o def.o 文件生成规则， make会自动补全
#hello.o: src/hello.c
#	cc -c hello.c

def.o: lib/def.c
	cc -c def.c

#make后生成了*.o和hello.out文件 故进行清除
clean: 
	rm -rf *.o $(TARGET)
