.PHONY: run

FLAGS := -fsanitize=address,undefined \
		 -g -DDEBUG \
		 -Wall -Wextra

EXEC = montador
IEXEC = interpreter
ifeq ($(OS),Windows_NT)
	EXEC := montador.exe
	IEXEC := interpreter.exe
endif

run: $(EXEC)
	./$(EXEC) exemplo.asm exemplo.o

$(EXEC): main.cpp lexer.cpp montador.cpp *.hpp
	g++ main.cpp lexer.cpp montador.cpp \
		-o $(EXEC) \
		$(FLAGS)

interpret: $(IEXEC) run
	./$(IEXEC) exemplo.o

$(IEXEC): interpreter.cpp
	g++ interpreter.cpp \
		-o $(IEXEC) \
		$(FLAGS)
