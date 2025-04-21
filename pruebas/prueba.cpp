#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>

using namespace std;
//creo la estructura de datos que almacenará toda la información necesaria, schedule es horario y es un vector debido a
//la naturaleza de la información, necesitaremos ingresar numeros en string para luego transformar esos numeros a horas
struct section{
string name_class;
vector<int> blocks;
string prof;
vector<string> students;
int n_conflicts;
string day;
vector<string> schedule;
section* next;
};

//este es el head
bool open_file(string name);
void name_file();
void clear_screen();

int main (){ 
    name_file();
}

void clear_screen() {
    #ifdef _WIN32 
        std::system("cls");
    #else 
        std::system("clear");
    #endif
    }

bool open_file(string name){
    section* nodo = new section;
    ifstream file;
    string line;
    file.open(name.c_str(),ios::in);

    if(file.fail()){
        return true;
    }
    while(!file.eof()){
        getline(file,line);
        if (line.find("Seccion:")!=string::npos){
            nodo->name_class = line.substr(10);
        }
        else if (line.find("Bloques")!=string::npos){
            int num;
            string substr = line.substr(9);
            stringstream ss(substr);
            while(ss >> num){
            nodo->blocks.push_back(num);
            }
        }
    }
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

