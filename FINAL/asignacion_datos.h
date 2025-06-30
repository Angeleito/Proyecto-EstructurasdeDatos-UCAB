#ifndef ASIGNACION_DATOS_H 
#define ASIGNACION_DATOS_H 
#include "estructuras_datos.h"
extern std::map<std::string, std::map<int, std::set<int>>> disponibilidad_profesores;

void asignar_horarios_orden(lista_secciones head);
void asignar_horarios_general(lista_secciones head);
bool hay_conflicto(section* s1, lista_secciones s2);
vector<int> horas_disponibles(lista_secciones head, lista_secciones actual);
vector<int> horas_disponibles_conflictos(lista_secciones head, lista_secciones actual);
section* asignar_horarios_por_conflicto(lista_secciones head);
vector<int> num_conflictos(ListaEnlazada* asignadas, ListaEnlazada* por_asignar);
bool mayor_conflicto(vector<int>& conflictos_asignadas, ListaEnlazada* conflictos_por_asignar);
void asignar_horarios(ListaEnlazada* lista_por_asignar, ListaEnlazada* lista_asignadas);
vector<int> conflictos_totales(ListaEnlazada*,ListaEnlazada*);
bool conflictos_estudiantes(lista_secciones clase_no_asignada,lista_secciones clase_asignada);


#endif