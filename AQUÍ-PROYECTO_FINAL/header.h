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
 void clear_screen();
 void menu();
 section* leer_secciones(string nombre_archivo);
 void liberar_lista(section* head);
 void asignar_horarios_orden(section* head);
 void mostrar_todas_las_secciones(section* head);
 void imprimir_resultado(section* head, bool version_larga);
 void guardar_resultado(section* head, string nombre_archivo, bool version_larga);
 bool hay_conflicto(section* s1, section* s2);
 vector<int> horas_disponibles(section* head, section* actual);
 vector<int> horas_disponibles_conflictos(section* head, section* actual);
 section* invertirLista(section* head);

 section* asignar_horarios_por_conflicto(section* head);
 vector<int> num_conflictos(ListaEnlazada* asignadas, ListaEnlazada* por_asignar);
 bool mayor_conflicto(vector<int>& conflictos_asignadas, ListaEnlazada* conflictos_por_asignar);
 void asignar_horarios(ListaEnlazada* lista_por_asignar, ListaEnlazada* lista_asignadas);
 vector<int> conflictos_totales(ListaEnlazada*,ListaEnlazada*);
 bool conflictos_estudiantes(section* clase_no_asignada,section* clase_asignada);

 void pause();