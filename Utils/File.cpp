#include "File.h"

/*--------------------------------- CABECALHO ----------------------------------
UNIVERSIDADE FEDERAL DA PARAIBA - UFPB
DATA: 25/02/2016
ALUNO: PAULO RICARDO PEREIRA DA SILVA - 11218638
TRABALHO DA DISCIPLINA DE COMPILADORES I - ANALISADOR LEXICO DA LINGUAGEM PASCAL

ESTA CLASSE IMPLEMENTA A LOGICA PARA ENTRADA DE DADOS PELA LEITURA DE ARQUIVOS E
CONTAGEM DO TEMPO DE EXECUCAO DO PROGRAMA.
--------------------------------------------------------------------------------*/

File* File::instance_ = nullptr;

/*********** INICIO DAS DEFINICOES DAS FUNCOES **********/
File::File(): time_begin_(0){}

File* File::getInstance(){
    if(!instance_){
        instance_ = new File;
    }
    return instance_;
}

string File::dataInput(const string fileName){

  string result;
  char char_symbol;

  ifstream file(fileName.c_str());
  if (file.is_open()){

    while (file.get(char_symbol)){

      result.push_back(char_symbol);

    }

    result[ result.size() ] = '\0';

    file.close();

  }else{

    cerr << "\nO ARQUIVO " << fileName << " NAO EXISTE NESSE DIRETORIO!" << "\n\n";
    exit (EXIT_FAILURE);

  }

  return result;
}

void File::timeIn(){

  //File::time_begin = GetTickCount();
  File::time_begin_ = clock();

}

void File::executionTime(){

  cerr.precision(5);
  //cout << fixed << "\nExecution Time: " << (double)((GetTickCount() - File::time_begin) / 1000.00) << "s\n" << endl;
  cerr << fixed << "\nExecution Time: " << (double)( clock() - time_begin_ )/1000.0 << "s\n" << "\n";
}

/*********** FIM DAS DEFINICOES DAS FUNCOES **********/
