#ifndef ENTRADA_DATOS_H 
#define ENTRADA_DATOS_H 
#include "estructuras_datos.h"

void clear_screen();
void menu();
void pause();

section* leer_secciones(string nombre_archivo);
void guardar_resultado(lista_secciones head, string nombre_archivo, bool version_larga);
DisponibilidadProfesor leer_disponibilidad_profesores(const std::string& archivo);
NodoABB* leer_cedulas_estudiantes(const std::string& archivo);
void guardar_materias_por_dia(lista_secciones head, const std::string& nombre_archivo);


#endif