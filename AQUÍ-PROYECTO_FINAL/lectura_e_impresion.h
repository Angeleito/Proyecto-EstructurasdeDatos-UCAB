#include "lista_enlazada.h"
section* leer_secciones(string nombre_archivo);
void mostrar_todas_las_secciones(section* head);
void imprimir_resultado(section* head, bool version_larga);
void guardar_resultado(section* head, string nombre_archivo, bool version_larga);