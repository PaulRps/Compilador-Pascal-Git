#include "Syntax.h"

using namespace std;

int main(){

	File::timeIn();//CONTA A HORA QUE INICIOU A EXECUCAO

	try{

		Syntax::program();

	}catch(runtime_error &e){
		cout << e.what() << endl;
	}

	File::executionTime();//EXIBE O TEMPO DE EXECUCAO DO PROGRAMA

	return 0;
}
