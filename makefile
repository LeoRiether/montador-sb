.PHONY: run

FLAGS := -fsanitize=address,undefined \
		 -g -DDEBUG \
		 -Wall -Wextra

run: montador
	./montador exemplo.asm exemplo.o

montador: main.cpp lexer.cpp assembler.cpp *.hpp
	g++ main.cpp lexer.cpp assembler.cpp \
		-o montador \
		$(FLAGS)

interpret: interpreter run
	./interpreter exemplo.o

interpreter: interpreter.cpp
	g++ interpreter.cpp \
		-o interpreter \
		$(FLAGS)
