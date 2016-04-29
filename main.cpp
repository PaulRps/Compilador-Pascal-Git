#include "Lexical.h"
#include "Token.h"
#include "File.h"
#include "SyntaticalAnalizer.h"
#include "Stack.h"
#include <string>

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
