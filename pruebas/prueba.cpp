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
void data_insert(ifstream& file, section* node);
section* new_section(section* head);


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

void data_insert(ifstream& file, section* node){
    string line;
    int i = 0;
    while(!file.eof()){
        getline(file,line);
        if(!node->name_class.empty() && line.find("Seccion:")!=string::npos){
            node = new_section(node);
        }
        if (line.find("Seccion:")!=string::npos){
            node->name_class = line.substr(10);
        }
        else if (line.find("Bloques")!=string::npos){
            int num;
            string substr = line.substr(9);
            stringstream ss(substr);
            while(ss >> num){
            node->blocks.push_back(num);
            }
        }
        else if (line.find("Profesor")!=string::npos){
            string substr = line.substr(9);
            node->prof = substr;
        }
        else if(line.find("ci:")!=string::npos){
                node->students.push_back(line);
            }
        }
        cout << node->next->name_class << " ";
        for (int block : node->next->blocks){cout << block << " ";}
        cout << node->next->prof << " ";
        for (string student : node->next->students) {cout << student << " ";}
}

bool open_file(string name){
    section* nodo = new section;
    section* cabeza = nullptr;
    ifstream file;
    file.open(name.c_str(),ios::in);
    if(file.fail()){
        return true;
    }
    data_insert(file, nodo);
    file.close();
    return false;
}

section* new_section(section* head){
        section* node = new section;
        node->next = head;
        head = node;
        return head;
    }