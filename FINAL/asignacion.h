#include "lectura_e_impresion.h"
#include <map>
#include <set>
#include <string>

// Cedula -> (dia 0-4) -> set de horas disponibles
using DisponibilidadProfesor = std::map<std::string, std::map<int, std::set<int>>>;

// Declaración de la función para leer la disponibilidad de profesores
DisponibilidadProfesor leer_disponibilidad_profesores(const std::string& archivo);

void asignar_horarios_orden(section* head);
void asignar_horarios_general(section* head);
bool hay_conflicto(section* s1, section* s2);
vector<int> horas_disponibles(section* head, section* actual);
vector<int> horas_disponibles_conflictos(section* head, section* actual);
section* asignar_horarios_por_conflicto(section* head);
vector<int> num_conflictos(ListaEnlazada* asignadas, ListaEnlazada* por_asignar);
bool mayor_conflicto(vector<int>& conflictos_asignadas, ListaEnlazada* conflictos_por_asignar);
void asignar_horarios(ListaEnlazada* lista_por_asignar, ListaEnlazada* lista_asignadas);
vector<int> conflictos_totales(ListaEnlazada*,ListaEnlazada*);
bool conflictos_estudiantes(section* clase_no_asignada,section* clase_asignada);