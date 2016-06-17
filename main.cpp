#include "Syntax.h"

int main(){

	File::getInstance()->timeIn();//CONTA A HORA QUE INICIOU A EXECUCAO

	try{

		Syntax::getInstance()->program();

	}catch(runtime_error &e){
		std::cerr << e.what() << "\n";		
	}

	File::getInstance()->executionTime();//EXIBE O TEMPO DE EXECUCAO DO PROGRAMA

	delete [] File::getInstance();

	return 0;
}
