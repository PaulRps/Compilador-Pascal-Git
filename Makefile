CC=@clang++  -std=c++11
DEBUG= -g3

pascal : *.cpp *.h #main.cpp AFD.cpp File.cpp Token.cpp SyntaticalAnalizer.cpp VariableStatement.cpp SubProgramsStatements.cpp Stack.cpp
	@clear
	$(CC) $(DEBUG) main.cpp AFD.cpp File.cpp Token.cpp SyntaticalAnalizer.cpp VariableStatement.cpp SubProgramsStatements.cpp Stack.cpp -o pascal

.PHONY: run
run : pascal
	@clear
	@./pascal # o @ oculta o comando no prompt

.PHONY: debug
debug :
	@clear
	lldb ./pascal

.PHONY: clean
clean:
	@rm  pascal #*.o *.gch
	@clear
