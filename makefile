.PHONY: run

run: montador
	./montador exemplo.asm exemplo.o

montador: *.cpp *.hpp
	g++ *.cpp \
		-o montador \
		-fsanitize=address,undefined \
		-Wall -Wextra
