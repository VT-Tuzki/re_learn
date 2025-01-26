
#hello.out 为执行二进制文件， 生成hello.out 依赖 hello.c 和def.c 所以添加依赖
hello.out: hello.o def.o
	cc -o hello.out hello.o def.o

hello.o: hello.c
	cc -c hello.c

def.o: def.c
	cc -c def.c

#make后生成了*.o和hello.out文件 故进行清除
clean: 
	rm -rf *.o hello.out
