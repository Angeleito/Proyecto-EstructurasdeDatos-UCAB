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
 typedef section* lista_secciones;

 struct NodoABB {
    std::string cedula;
    NodoABB* izq;
    NodoABB* der;
    NodoABB(const std::string& c) : cedula(c), izq(nullptr), der(nullptr) {}
};
NodoABB* insertarABB(NodoABB* raiz, const std::string& cedula);
bool buscarABB(NodoABB* raiz, const std::string& cedula);
void liberarABB(NodoABB* raiz);
NodoABB* abb_estudiantes_secciones(section* head);
void estudiantes_no_en_secciones(NodoABB* raiz_inscritos, NodoABB* raiz_secciones, std::vector<std::string>& faltantes);
void estudiantes_no_inscritos(NodoABB* raiz_secciones, NodoABB* raiz_inscritos, std::vector<std::string>& sobrantes);


struct ListaEnlazada {
    section* cabeza;
    section* sig_asignar;
 section* crearNodo(section* nodo);
 section* copiar_cabeza (section* nodo);
 void agregarNodoFinal(ListaEnlazada* lista, section* nodo);
 void copiar_lista(ListaEnlazada* destino, section* origen_head);
 void limpiar_lista(ListaEnlazada* lista);
 void expulsar_nodo(ListaEnlazada* ListaEnlazada,section* nodo);
 };

 
 ListaEnlazada* crearListaEnlazada();
void liberar_lista(section* head);
section* invertirLista(section* head);

// Cedula -> (dia 0-4) -> set de horas disponibles
using DisponibilidadProfesor = std::map<std::string, std::map<int, std::set<int>>>;



#endif