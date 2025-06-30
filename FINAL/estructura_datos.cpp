#include "estructuras_datos.h"
NodoABB* insertarABB(NodoABB* raiz, const std::string& cedula) {
    if (!raiz) return new NodoABB(cedula);
    if (cedula < raiz->cedula) raiz->izq = insertarABB(raiz->izq, cedula);
    else if (cedula > raiz->cedula) raiz->der = insertarABB(raiz->der, cedula);
    return raiz;
}

bool buscarABB(NodoABB* raiz, const std::string& cedula) {
    if (!raiz) return false;
    if (cedula == raiz->cedula) return true;
    if (cedula < raiz->cedula) return buscarABB(raiz->izq, cedula);
    else return buscarABB(raiz->der, cedula);
}

void liberarABB(NodoABB* raiz) {
    if (!raiz) return;
    liberarABB(raiz->izq);
    liberarABB(raiz->der);
    delete raiz;
}

NodoABB* abb_estudiantes_secciones(section* head) {
    NodoABB* raiz = nullptr;
    for (section* s = head; s != nullptr; s = s->next) {
        for (auto& est : s->students) {
            raiz = insertarABB(raiz, est.second);
        }
    }
    return raiz;
}

void estudiantes_no_en_secciones(NodoABB* raiz_inscritos, NodoABB* raiz_secciones, std::vector<std::string>& faltantes) {
    if (!raiz_inscritos) return;
    if (!buscarABB(raiz_secciones, raiz_inscritos->cedula))
        faltantes.push_back(raiz_inscritos->cedula);
    estudiantes_no_en_secciones(raiz_inscritos->izq, raiz_secciones, faltantes);
    estudiantes_no_en_secciones(raiz_inscritos->der, raiz_secciones, faltantes);
}

void estudiantes_no_inscritos(NodoABB* raiz_secciones, NodoABB* raiz_inscritos, std::vector<std::string>& sobrantes) {
    if (!raiz_secciones) return;
    if (!buscarABB(raiz_inscritos, raiz_secciones->cedula))
        sobrantes.push_back(raiz_secciones->cedula);
    estudiantes_no_inscritos(raiz_secciones->izq, raiz_inscritos, sobrantes);
    estudiantes_no_inscritos(raiz_secciones->der, raiz_inscritos, sobrantes);
}

section* ListaEnlazada::crearNodo(section* nodo) {
    section* nuevoNodo = new section;
    nuevoNodo->name_class = nodo->name_class;
    nuevoNodo->prof = nodo->prof;
    nuevoNodo->prof_ci = nodo->prof_ci;
    nuevoNodo->blocks = nodo->blocks;
    nuevoNodo->horarios = nodo->horarios;
    nuevoNodo->students = nodo->students;
    nuevoNodo->num_conflictos = nodo->num_conflictos;
    nuevoNodo->next = nullptr;
    return nuevoNodo;
}

 section* ListaEnlazada::copiar_cabeza (section* nodo) {
    section* nuevoNodo = new section;
    nuevoNodo = nodo;
    return nuevoNodo;
 }

 // Función para añadir un nodo al final de la lista
 void ListaEnlazada::agregarNodoFinal(ListaEnlazada* lista, section* nodo) {
    section* nuevoNodo = crearNodo(nodo);

    if (lista->cabeza == nullptr) {
        lista->cabeza = nuevoNodo;
        return;
    }

    section* actual = lista->cabeza;
    while (actual->next != nullptr) {
        actual = actual->next;
    }
    actual->next = nuevoNodo;
}
 void ListaEnlazada::copiar_lista(ListaEnlazada* destino, section* origen_head) {
    section* actual_origen = origen_head;
    section* ultimo_destino = nullptr;

    while (actual_origen != nullptr) {
        section* nuevo_nodo = new section;
        nuevo_nodo->name_class = actual_origen->name_class;
        nuevo_nodo->prof = actual_origen->prof;
        nuevo_nodo->prof_ci = actual_origen->prof_ci;
        nuevo_nodo->blocks = actual_origen->blocks;
        nuevo_nodo->horarios = actual_origen->horarios;
        nuevo_nodo->students = actual_origen->students;
        nuevo_nodo->num_conflictos = actual_origen->num_conflictos;
        nuevo_nodo->next = nullptr;

        if (destino->cabeza == nullptr) {
            destino->cabeza = nuevo_nodo;
        } else {
            ultimo_destino->next = nuevo_nodo;
        }
        ultimo_destino = nuevo_nodo;
        actual_origen = actual_origen->next;
    }
 }
 void ListaEnlazada::limpiar_lista(ListaEnlazada* lista){
    section* actual = lista->cabeza;
    section* siguiente;
    while (actual != nullptr) {
        siguiente = actual->next;
        delete actual;
        actual = siguiente;
    }
 }
 void ListaEnlazada::expulsar_nodo(ListaEnlazada* ListaEnlazada,section* nodo){
    section* nodo_actual = ListaEnlazada->cabeza;
    section* nodo_siguiente = ListaEnlazada->cabeza->next;
    while(nodo_siguiente != nullptr){
        if (nodo_actual == nodo){
                ListaEnlazada->cabeza = nodo_siguiente;
                return;
        }
        else if(nodo_siguiente == nodo){
                nodo_actual->next = nodo_siguiente->next;
                return;
        }
        nodo_actual = nodo_actual->next;
        nodo_siguiente = nodo_siguiente->next;
    }
    return;
 }

 ListaEnlazada* crearListaEnlazada() {
    ListaEnlazada* nuevaLista = new ListaEnlazada;
    nuevaLista->cabeza = nullptr; // La lista comienza vacía
    return nuevaLista;
 }

  // Libera la memoria de la lista enlazada de secciones
 void liberar_lista(section* head) {
    while (head) {
        section* temp = head;
        head = head->next;
        delete temp;
    }
 }

  section* invertirLista(section* head) {
    section* anterior = nullptr;
    section* actual = head;
    section* siguienteTemporal = nullptr;

    while (actual != nullptr) {
        // Guarda el siguiente nodo
        siguienteTemporal = actual->next;

        // Invierte el puntero 'siguiente' del nodo actual
        actual->next = anterior;

        // Mueve los punteros un paso hacia adelante
        anterior = actual;
        actual = siguienteTemporal;
    };
    return anterior;
 }