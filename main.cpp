#include "AFD.h"
#include "Token.h"
#include "File.h"
#include "SyntaticalAnalizer.h"
//#include "myStack.h"
#include "Stack.h"
#include <string>

//compilar classes: g++ .h -c .cpp ...
//compilar: g++ -g3 main.cpp AFD.cpp Token.cpp File.cpp SyntaticalAnalizer.cpp VariableStatement.cpp SubProgramsStatements.cpp -o pascal

using namespace std;

int main(){

	File::timeIn();//CONTA A HORA QUE INICIOU A EXECUCAO

	try{

		SyntaticalAnalizer::program(); //b >= ((c+10)/1)

	}catch(runtime_error &e){
		cout << e.what() << endl;
	}

	File::executionTime();//EXIBE O TEMPO DE EXECUCAO DO PROGRAMA

	return 0;
}
