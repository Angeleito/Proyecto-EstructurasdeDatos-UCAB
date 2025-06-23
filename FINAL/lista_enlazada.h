#include "section.h"

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