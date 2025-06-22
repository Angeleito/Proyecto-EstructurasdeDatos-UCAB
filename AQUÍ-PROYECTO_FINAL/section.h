#include <iostream>       // Permite entrada y salida estándar
#include <fstream>        // Permite manejo de archivos
#include <string>         // Permite uso de cadenas de texto
#include <vector>         // Permite uso de vectores (arreglos dinámicos)
#include <sstream>        // Permite manipulación de strings como flujos
#include <cstdlib>        // Permite uso de funciones del sistema
#include <algorithm>      // Permite uso de algoritmos estándar (no se usa mucho aquí)
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