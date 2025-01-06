# 关于SHELL 为确保shell相同 先定义shell为 /bin/bash
# 1. Makefile  大部分逻辑可以按照shell脚本理解
# 2. 直接运行make 不加后缀 make 会自动执行第一个参数
# 	a. 本文件会执行tuturial
# 	b. 直接运行make = make tutorial
# 3.从vat-lost 和var-kept 可以得到的信息是
# 	a. 在makefile的语法中，shell 中export声明的变量是不会继承到下一行的，必须要使用 \ 继承
# 	b. 即makefile中的shell命令 每一行都是独立的，若想使其联动， 需增加\ 或者使用makefile的变量 

SHELL = /bin/bash

tutorial:
			@# todo: have this actully run some kind of wizard?
			@echo "Please read the 'Makefile' file to go through this tutorial"

var-lost:
			export foo=bar
			echo "foo=[$$foo]"

var-kept:
	export foo=bar; \
	echo "foo=[$$foo]"
source.txt:
			@echo "building source.txt"
			echo "this is the source" > source.txt

result.txt: source.txt
			@echo "building result.txt from source.txt"
			cp source.txt result.txt
result-using-var.txt: source.txt
	@echo "building result-using-var.txt using the $$< and $$@ vars"
	cp $< $@

srcfiles := $(shell echo src/{00..99}.txt)

src/%.txt:
	 		@# "First things first, create the dir if it doesn't exist."
			@# Prepend with @ because srsly who cares about dir creation
			@[ -d src ] || mkdir src
			@# then, we just echo some data into the file
			@# The $* expands to the "stem" bit matched by %
			@# So, we get a bunch of files with numeric names, containing their number
			echo $* > $@

source: $(srcfiles)

dest/%.txt: src/%.txt
			@[ -d dest ] || mkdir dest
			@ #echo "$<"
			@ #echo "$@"
			cp $< $@

destfiles := $(patsubst src/%.txt,dest/%.txt,$(srcfiles))
destination: $(destfiles)

kitty: $(destfiles)
			 @# Remember, $< is the input file, but $^ is ALL the input files.
			 @# Cat them into the kitty
			 cat $^ > kitty

test: kitty
				@echo "miao" &&echo "tests all pass!"

clean: 
	rm -rf *.txt src dest kitty

badkitty:
			$(MAKE) kitty # The special var $(MAKE) means "the make currently in use"
			false # <-- this will fail
			echo "should not get here"

.PHONY: source destination clean test badkitty

