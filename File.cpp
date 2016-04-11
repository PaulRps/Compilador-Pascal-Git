#include "File.h"

/*--------------------------------- CABECALHO ----------------------------------
UNIVERSIDADE FEDERAL DA PARAIBA - UFPB
DATA: 25/02/2016
ALUNO: PAULO RICARDO PEREIRA DA SILVA - 11218638
TRABALHO DA DISCIPLINA DE COMPILADORES I - ANALISADOR LEXICO DA LINGUAGEM PASCAL

ESTA CLASSE IMPLEMENTA A LOGICA PARA ENTRADA DE DADOS PELA LEITURA DE ARQUIVOS E
CONTAGEM DO TEMPO DE EXECUCAO DO PROGRAMA.
--------------------------------------------------------------------------------*/

float File::time_begin;//ARMAZENA O TEMPO QUE O PROGRAMA COMECOU A RODAR

/*********** INICIO DAS DEFINICOES DAS FUNCOES **********/
string File::dataInput(string fileName){

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

    cout << "O ARQUIVO " << fileName << " NAO EXISTE NESSE DIRETORIO!" << endl;
    exit (EXIT_FAILURE);

  }

  return result;
}

void File::timeIn(){

  //File::time_begin = GetTickCount();
  File::time_begin = clock();

}

void File::executionTime(){

  cout.precision(5);
  //cout << fixed << "\nExecution Time: " << (double)((GetTickCount() - File::time_begin) / 1000.00) << "s\n" << endl;
  cout << fixed << "\nExecution Time: " << (double)( clock() - time_begin )/1000.0 << "s\n" << endl;
}
/*********** FIM DAS DEFINICOES DAS FUNCOES **********/
