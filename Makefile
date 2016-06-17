CC=@g++ -Wall -Werror -pedantic -std=c++11 -O2
DEBUG= -g3
OBJS=Token.o File.o Error.o Lexical.o Syntax.o Context_Free_Grammar.o Semantic.o main.o


pascal: $(OBJS)
	$(CC) $(OBJS) -o pascal

main.o: Syntax.h File.h
	$(CC) -c main.cpp

Token.o: Token.h
	$(CC) -c Token.cpp

File.o: File.h
	$(CC) -c File.cpp

Error.o: Error.h Syntax.h
	$(CC) -c Error.cpp

Lexical.o: Lexical.h Token.h
	$(CC) -c Lexical.cpp

Semantic.o: Semantic.h Token.h
	$(CC) -c Semantic.cpp

Syntax.o: Syntax.h Lexical.h File.h Context_Free_Grammar.h Semantic.h
	$(CC) -c Syntax.cpp

Context_Free_Grammar.o: Context_Free_Grammar.h Syntax.h Semantic.h Error.h
	$(CC) -c Context_Free_Grammar.cpp

.PHONY: run
run : pascal
	@clear
	@./pascal # o @ oculta o comando no prompt

.PHONY: debug
debug :
	@clear
	gdb ./pascal

.PHONY: clean
clean:
	@rm  pascal *.o
	@clear
#main.cpp AFD.cpp File.cpp Token.cpp SyntaticalAnalizer.cpp VariableStatement.cpp SubProgramsStatements.cpp Stack.cpp
