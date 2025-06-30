#ifndef ESTRUCTURAS_DATOS_H 
#define ESTRUCTURAS_DATOS_H 

#include <iostream>       // Permite entrada y salida estándar
#include <fstream>        // Permite manejo de archivos
#include <string>         // Permite uso de cadenas de texto
#include <vector>         // Permite uso de vectores (arreglos dinámicos)
#include <sstream>        // Permite manipulación de strings como flujos
#include <cstdlib>        // Permite uso de funciones del sistema
#include <algorithm>      // Permite uso de algoritmos estándar (no se usa mucho aquí)
#include <map>
#include <set>
using namespace std;       // Usa el espacio de nombres estándar para evitar std::

struct section {
    string name_class;                      // Nombre de la materia/sección
    vector<int> blocks;                     // Vector con los bloques de horas de la sección
    string prof;                            // Nombre del profesor
    string prof_ci;                         // Cédula del profesor
    vector<pair<string, string>> students;    // Vector de pares (nombre, ci) de estudiantes
    vector<pair<int, int>> horarios;        // Vector de pares (hora inicio, hora fin) asignados a la sección
    int num_conflictos;
    section* next;                          // Puntero al siguiente nodo de la lista enlazada de secciones
 };
 typedef section* lista_secciones;      //tipo lista_secciones para mayor legibilidad en futuras funciones

 struct NodoABB {               //tipo de dato Arbol Binario de Busqueda
    std::string cedula;
    NodoABB* izq;
    NodoABB* der;
    NodoABB(const std::string& c) : cedula(c), izq(nullptr), der(nullptr) {} //constructor del tipo de dato ABB
};
NodoABB* insertarABB(NodoABB* raiz, const std::string& cedula);  //funcion para insertar elementos al ABB
bool buscarABB(NodoABB* raiz, const std::string& cedula);   //busca un elemento en el arbol ABB
void liberarABB(NodoABB* raiz);            //convierte al ABB en un arbol vacío
NodoABB* abb_estudiantes_secciones(lista_secciones head);      //llena un ABB con lo estudiantes que estan en una sección
void estudiantes_no_en_secciones(NodoABB* raiz_inscritos, NodoABB* raiz_secciones, std::vector<std::string>& faltantes);//llena un ABB con lo estudiantes que no estan en una sección
void estudiantes_no_inscritos(NodoABB* raiz_secciones, NodoABB* raiz_inscritos, std::vector<std::string>& sobrantes);  //llena un ABB con los estudiantes no inscritos


struct ListaEnlazada {  //tipo de dato lista_enlazada, se crea para mayor legibilidad
    section* cabeza;
    section* sig_asignar;
 section* crearNodo(lista_secciones nodo);
 section* copiar_cabeza (lista_secciones nodo);
 void agregarNodoFinal(ListaEnlazada* lista, lista_secciones nodo);
 void copiar_lista(ListaEnlazada* destino, lista_secciones origen_head);
 void limpiar_lista(ListaEnlazada* lista);
 void expulsar_nodo(ListaEnlazada* ListaEnlazada,lista_secciones nodo);
 };

 
 ListaEnlazada* crearListaEnlazada();
void liberar_lista(lista_secciones head);
section* invertirLista(lista_secciones head);

//tipo de dato complejo, es un tipo de dato que va a almacenar la cedula del profesor, un map con los dias de la semana y las horas disponible que tiene cada dia
using DisponibilidadProfesor = std::map<std::string, std::map<int, std::set<int>>>;



#endif