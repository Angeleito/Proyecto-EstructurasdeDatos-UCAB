#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;
//creo la estructura de datos que almacenará toda la información necesaria, schedule es horario y es un vector debido a
//la naturaleza de la información, necesitaremos ingresar numeros en string para luego transformar esos numeros a horas
struct section{
string name_class;
vector<int> blocks;
string prof;
vector<string> students;
int n_conflicts;
string dia;
vector<string> schedule;
};

//este es el head
bool open_file(string name);
void name_file();
void clear_screen();

int main (){ 
    name_file();
}


bool open_file(string name){
    ifstream file;
    string line;
    string txt;
    file.open(name.c_str(),ios::in);

    if(file.fail()){
        return true;
    }
    while(!file.eof()){
        getline(file,line);
        txt += line;
        txt += "\n";
    }
    cout << txt;
    file.close();
    return false;
}

void name_file(){
    clear_screen();
    string name;
    cout << "ingrese el nombre del archivo: ";
    while (true){
        getline(cin, name);
        if (open_file(name)){
            clear_screen();
            cout << "no se encontro el archivo, vuelva a intertarlo: ";
        }
        else {
            break;
        }
    }
}

void clear_screen() {
    #ifdef _WIN32 
        std::system("cls");
    #else 
        std::system("clear");
    #endif
    }