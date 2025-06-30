#include "estructuras_datos.h"
NodoABB* insertarABB(NodoABB* raiz, const std::string& cedula) { //verifica si el arbol esta vacío, y si no lo esta ubica el elemento de forma ordenada
    if (!raiz) return new NodoABB(cedula);
    if (cedula < raiz->cedula) raiz->izq = insertarABB(raiz->izq, cedula);
    else if (cedula > raiz->cedula) raiz->der = insertarABB(raiz->der, cedula);
    return raiz;
}

bool buscarABB(NodoABB* raiz, const std::string& cedula) { //va recorriendo el arbol buscando al elemento dependiendo si es mayor o menor a la raiz, y sigue asi de forma iterativa hasta encontrarlo o no
    if (!raiz) return false;
    if (cedula == raiz->cedula) return true;
    if (cedula < raiz->cedula) return buscarABB(raiz->izq, cedula);
    else return buscarABB(raiz->der, cedula);
}

void liberarABB(NodoABB* raiz) { //recorre el arbol de forma iterativa y va borrando la raiz anterior despues de cada iteracion
    if (!raiz) return;
    liberarABB(raiz->izq);
    liberarABB(raiz->der);
    delete raiz;
}

NodoABB* abb_estudiantes_secciones(lista_secciones head) { //Llena un ABB con los estudiantes que están en al menos una sección
    NodoABB* raiz = nullptr;
    for (section* s = head; s != nullptr; s = s->next) {
        for (auto& est : s->students) {
            raiz = insertarABB(raiz, est.second);
        }
    }
    return raiz;
}

void estudiantes_no_en_secciones(NodoABB* raiz_inscritos, NodoABB* raiz_secciones, std::vector<std::string>& faltantes) { //llena un ABB con los estudiantes que no estan en una sección, lo realiza comprobando de forma iterativa si está o no inscrito o en alguna sección
    if (!raiz_inscritos) return;
    if (!buscarABB(raiz_secciones, raiz_inscritos->cedula))
        faltantes.push_back(raiz_inscritos->cedula);
    estudiantes_no_en_secciones(raiz_inscritos->izq, raiz_secciones, faltantes);
    estudiantes_no_en_secciones(raiz_inscritos->der, raiz_secciones, faltantes);
}

void estudiantes_no_inscritos(NodoABB* raiz_secciones, NodoABB* raiz_inscritos, std::vector<std::string>& sobrantes) { //Verifica de forma iterativa si los estudiantes no pertenecen a ninguna seccion y no estan inscritos
    if (!raiz_secciones) return;
    if (!buscarABB(raiz_inscritos, raiz_secciones->cedula))
        sobrantes.push_back(raiz_secciones->cedula);
    estudiantes_no_inscritos(raiz_secciones->izq, raiz_inscritos, sobrantes);
    estudiantes_no_inscritos(raiz_secciones->der, raiz_inscritos, sobrantes);
}

section* ListaEnlazada::crearNodo(lista_secciones nodo) { // funcion método de lista enlazada
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

 section* ListaEnlazada::copiar_cabeza (lista_secciones nodo) {  // funcion método de lista enlazada
    section* nuevoNodo = new section;
    nuevoNodo = nodo;
    return nuevoNodo;
 }

 // Función para añadir un nodo al final de la lista
 void ListaEnlazada::agregarNodoFinal(ListaEnlazada* lista, lista_secciones nodo) {   // funcion método de lista enlazada
    section* nuevoNodo = crearNodo(nodo);

    if (lista->cabeza == nullptr) {  //si esta vacia retorna
        lista->cabeza = nuevoNodo;
        return;
    }

    section* actual = lista->cabeza;  //sino recorre hasta el final de la lista
    while (actual->next != nullptr) {
        actual = actual->next;
    }
    actual->next = nuevoNodo;
}
 void ListaEnlazada::copiar_lista(ListaEnlazada* destino, lista_secciones origen_head) { // funcion método de lista enlazada
    section* actual_origen = origen_head;
    section* ultimo_destino = nullptr;

    while (actual_origen != nullptr) { // mientras no sea vacio, entonces copie el nodo
        section* nuevo_nodo = new section;
        nuevo_nodo->name_class = actual_origen->name_class;
        nuevo_nodo->prof = actual_origen->prof;
        nuevo_nodo->prof_ci = actual_origen->prof_ci;
        nuevo_nodo->blocks = actual_origen->blocks;
        nuevo_nodo->horarios = actual_origen->horarios;
        nuevo_nodo->students = actual_origen->students;
        nuevo_nodo->num_conflictos = actual_origen->num_conflictos;
        nuevo_nodo->next = nullptr;

        if (destino->cabeza == nullptr) { //si el destino es vacio inserte 
            destino->cabeza = nuevo_nodo;
        } else {
            ultimo_destino->next = nuevo_nodo; //sino siga copiando la lista
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
void ListaEnlazada::expulsar_nodo(ListaEnlazada* lista_referencia, lista_secciones nodo_a_expulsar) {
    // 1. Manejo de lista vacía o nodo a expulsar nulo
    if (lista_referencia == nullptr || nodo_a_expulsar == nullptr) {
        return; // No hay nada que hacer
    }

    // 2. Si el nodo a expulsar es la cabeza
    if (lista_referencia->cabeza == nodo_a_expulsar) {
        lista_referencia->cabeza = nodo_a_expulsar->next; // La nueva cabeza es el siguiente
        delete nodo_a_expulsar; // Liberar la memoria
        // No hay necesidad de 'return' aquí si queremos que la lista se vacíe
        // Si quieres que el bucle de asignacion_horarios_por_conflicto termine
        // cuando solo queda un nodo y este se elimina, esto lo maneja.
        return; // Salir después de eliminar la cabeza
    }

    // 3. Buscar el nodo en el resto de la lista
    section* actual = lista_referencia->cabeza;
    // Iteramos mientras tengamos un nodo actual y un siguiente (para poder desenlazar)
    while (actual != nullptr && actual->next != nullptr) {
        if (actual->next == nodo_a_expulsar) { // Encontramos el nodo después del 'actual'
            section* temp = actual->next; // Guardar el nodo a borrar
            actual->next = temp->next; // 'actual' ahora apunta al nodo después de 'temp'
            delete temp; // ¡Liberar la memoria del nodo expulsado!
            return; // Nodo encontrado y expulsado
        }
        actual = actual->next; // Mover al siguiente nodo
    }

    // Si el bucle termina, significa que el nodo no fue encontrado
}

 ListaEnlazada* crearListaEnlazada() {
    ListaEnlazada* nuevaLista = new ListaEnlazada;
    nuevaLista->cabeza = nullptr; // La lista comienza vacía
    return nuevaLista;
 }

  // Libera la memoria de la lista enlazada de secciones
 void liberar_lista(lista_secciones head) {
    while (head) {
        section* temp = head;
        head = head->next;
        delete temp;
    }
 }

  section* invertirLista(lista_secciones head) {
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