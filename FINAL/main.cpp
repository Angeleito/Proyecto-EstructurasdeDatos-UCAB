#include "asignacion.h"
#include "menu.h"
#include <map>
#include <set>
string archivo_disponibilidad = "profesores.txt"; // Valor por defecto
void imprimir_materias_por_dia(section* head);
void guardar_materias_por_dia(section* head, const std::string& nombre_archivo);
using DisponibilidadProfesor = std::map<std::string, std::map<int, std::set<int>>>;
// Cedula -> (dia 0-4) -> set de horas disponibles

// --- Árbol Binario de Búsqueda para cédulas ---
struct NodoABB {
    std::string cedula;
    NodoABB* izq;
    NodoABB* der;
    NodoABB(const std::string& c) : cedula(c), izq(nullptr), der(nullptr) {}
};

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

NodoABB* leer_cedulas_estudiantes(const std::string& archivo) {
    NodoABB* raiz = nullptr;
    std::ifstream in(archivo);
    std::string cedula;
    while (getline(in, cedula)) {
        if (!cedula.empty())
            raiz = insertarABB(raiz, cedula);
    }
    return raiz;
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

 int main() {
    menu();     // Llama al menú principal
    return 0;
 }
DisponibilidadProfesor disponibilidad_profesores;

void menu() {
    string estado = "No cargado";
    section* lista_inv = nullptr;
    string archivo_entrada, archivo_salida, archivo_cedulas;
    section* head = nullptr;
    NodoABB* abb_inscritos = nullptr;
    string _;
    int opcion;
    bool salir = false;
while (!salir) {
    clear_screen();
    cout << "1. Leer archivo de secciones (" << estado << ")\n";
    cout << "2. Cargar archivo de disponibilidad de profesores\n";
    cout << "3. Cargar archivo de cédulas de estudiantes inscritos\n";
    cout << "4. Asignar horarios (orden original)\n";
    cout << "5. Asignar horarios (orden inverso)\n";
    cout << "6. Asignar horarios (estrategia por mayor conflicto)\n";
    cout << "7. Mostrar resultado por pantalla\n";
    cout << "8. Mostrar materias agrupadas por dia\n";
    cout << "9. Guardar resultado en archivo\n";
    cout << "10. Guardar materias agrupadas por dia\n";
    cout << "11. Comparar estudiantes inscritos y en secciones\n";
    cout << "12. Salir\n";
    cout << "Seleccione una opcion: ";
    cin >> opcion;
    cin.ignore();
    if (std::cin.fail()) {
        std::cout << "Entrada invalida. Debe ingresar un numero entero.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        pause();
        continue;
    }

    switch (opcion) {
        case 1:
            // Leer archivo de secciones
            cout << "Ingrese el nombre del archivo de entrada: ";
            getline(cin, archivo_entrada);
            liberar_lista(head);
            head = leer_secciones(archivo_entrada);
            if (head) cout << "Archivo cargado correctamente.\n", estado = "Cargado";
            else cout << "Error al cargar el archivo.\n";
            pause();
            break;
        case 2:
            // Cargar archivo de disponibilidad de profesores
            cout << "Ingrese el nombre del archivo de disponibilidad de profesores: ";
            getline(cin, archivo_disponibilidad);
            disponibilidad_profesores = leer_disponibilidad_profesores(archivo_disponibilidad);
            cout << "Archivo de disponibilidad cargado.\n";
            pause();
            break;
        case 3:
            // Cargar archivo de cédulas de estudiantes inscritos
            cout << "Ingrese el nombre del archivo de cédulas de estudiantes inscritos: ";
            getline(cin, archivo_cedulas);
            abb_inscritos = leer_cedulas_estudiantes(archivo_cedulas);
            cout << "Archivo de cédulas cargado.\n";
            pause();
            break;
        case 4:
            // Asignar horarios (orden original)
            if (!head) {
                cout << "Primero debe cargar un archivo de secciones.\n";
            } else {
                if (disponibilidad_profesores.empty()) {
                    cout << "Advertencia: No se ha cargado la disponibilidad de profesores.\n";
                }
                mostrar_todas_las_secciones(head);
                asignar_horarios_general(head);
                cout << "Horarios asignados para toda la semana.\n";
            }
            pause();
            break;
        case 5:
            // Asignar horarios (orden inverso)
            head = invertirLista(head);
            if (!head) {
                cout << "Primero debe cargar un archivo de secciones.\n";
            } else {
                mostrar_todas_las_secciones(head);
                asignar_horarios_general(head);
                cout << "Horarios asignados para toda la semana.\n";
            }
            pause();
            break;
        case 6:
            // Asignar horarios (mayor conflicto)
            if (!head) {
                cout << "Primero debe cargar un archivo de secciones.\n";
            } else {
                head = asignar_horarios_por_conflicto(head);
                cout << "Horarios asignados para toda la semana.\n";
            }
            pause();
            break;
        case 7:
            // Mostrar resultado por pantalla
            if (!head) {
                cout << "Primero debe cargar y asignar horarios.\n";
            } else {
                cout << "Mostrar version larga (1) o corta (0): ";
                bool larga;
                cin >> larga;
                cin.ignore();
                imprimir_resultado(head, larga);
            }
            pause();
            break;
        case 8:
            // Mostrar materias agrupadas por día
            if (!head) {
                cout << "Primero debe cargar y asignar horarios.\n";
            } else {
                imprimir_materias_por_dia(head);
            }
            pause();
            break;
        case 9:
            // Guardar resultado en archivo
            if (!head) {
                cout << "Primero debe cargar y asignar horarios.\n";
            } else {
                cout << "Ingrese el nombre del archivo de salida: ";
                getline(cin, archivo_salida);
                if (archivo_salida.empty()) {
                    cout << "Nombre de archivo no válido.\n";
                } else {
                    cout << "Mostrar version larga (1) o corta (0): ";
                    bool larga;
                    cin >> larga;
                    cin.ignore();
                    guardar_resultado(head, archivo_salida, larga);
                    cout << "Archivo guardado.\n";
                }
            }
            pause();
            break;
        case 10:
            // Guardar materias agrupadas por día
            if (!head) {
                cout << "Primero debe cargar y asignar horarios.\n";
            } else {
                string archivo_dia;
                cout << "Ingrese el nombre del archivo para guardar las materias agrupadas por día: ";
                getline(cin, archivo_dia);
                guardar_materias_por_dia(head, archivo_dia);
            }
            pause();
            break;
        case 11:
            // Comparar estudiantes inscritos y en secciones
            if (!head) {
                cout << "Primero debe cargar y asignar horarios.\n";
            } else if (!abb_inscritos) {
                cout << "Primero debe cargar el archivo de cédulas de estudiantes inscritos.\n";
            } else {
                NodoABB* abb_secciones = abb_estudiantes_secciones(head);
                std::vector<std::string> no_en_secciones, no_inscritos;
                estudiantes_no_en_secciones(abb_inscritos, abb_secciones, no_en_secciones);
                estudiantes_no_inscritos(abb_secciones, abb_inscritos, no_inscritos);

                cout << "\nEstudiantes inscritos que NO están en ninguna sección:\n";
                for (auto& c : no_en_secciones) cout << c << "\n";
                cout << "\nEstudiantes en secciones que NO están inscritos:\n";
                for (auto& c : no_inscritos) cout << c << "\n";

                liberarABB(abb_secciones);
            }
            pause();
            break;
        case 12:
            salir = true;
            break;
        default:
            cout << "Opcion invalida.\n";
            pause();
    }
}
    liberar_lista(head); // Libera memoria al salir
 }

 // Lee el archivo de secciones y construye la lista enlazada
 section* leer_secciones(string nombre_archivo) {
    ifstream file(nombre_archivo); // Abre el archivo
    if (!file.is_open()) return nullptr;
    section* head = nullptr;
    section* tail = nullptr;
    string line;
    string pending_line;
    section* actual = nullptr;
    bool has_pending = false;

    while (true) {
        // Lee la siguiente línea o usa la pendiente
        if (!has_pending) {
            if (!getline(file, line)) break;
        } else {
            line = pending_line;
            has_pending = false;
        }
        if (line.empty()) continue; // Ignora líneas vacías
        if (line.find("Seccion:") != string::npos) {
            // Crea una nueva sección
            section* nueva = new section;
            nueva->name_class = line.substr(line.find(":") + 1);
            nueva->name_class.erase(0, nueva->name_class.find_first_not_of(" \t"));
            nueva->next = nullptr;
            if (!head) head = nueva;
            if (tail) tail->next = nueva;
            tail = nueva;
            actual = nueva;
        } else if (line.find("Bloques:") != string::npos && actual) {
            // Lee los bloques de horas
            actual->blocks.clear();
            stringstream ss(line.substr(line.find(":") + 1));
            int num;
            while (ss >> num) actual->blocks.push_back(num);
        } else if (line.find("Profesor:") != string::npos && actual) {
            // Lee el nombre y la cédula del profesor
            string datos = line.substr(line.find(":") + 1);
            size_t pos = datos.find("ci:");
            if (pos != string::npos) {
                actual->prof = datos.substr(0, pos);
                actual->prof_ci = datos.substr(pos + 3);
                actual->prof.erase(actual->prof.find_last_not_of(" \t")+1);
                actual->prof_ci.erase(0, actual->prof_ci.find_first_not_of(" \t"));
            } else {
                actual->prof = datos;
                actual->prof_ci = "";
            }
        } else if (line.find("Estudiantes:") != string::npos && actual) {
            // Lee la lista de estudiantes hasta la siguiente sección o fin de archivo
            string next_line;
            while (getline(file, next_line)) {
                if (next_line.empty()) continue;
                if (next_line.find("Seccion:") != string::npos) {
                    pending_line = next_line;
                    has_pending = true;
                    break;
                }
                size_t pos = next_line.find("ci:");
                if (pos != string::npos && actual) {
                    string nombre = next_line.substr(0, pos);
                    string ci = next_line.substr(pos + 3);
                    nombre.erase(nombre.find_last_not_of(" \t")+1);
                    ci.erase(0, ci.find_first_not_of(" \t"));
                    // Evita duplicados de estudiantes
                    bool repetido = false;
                    for (auto& est : actual->students) {
                        if (est.second == ci) {
                            repetido = true;
                            break;
                        }
                    }
                    if (!repetido) {
                        actual->students.push_back({nombre, ci});
                    }
                }
            }
        }
    }
    file.close();
    return head;
 }

DisponibilidadProfesor leer_disponibilidad_profesores(const std::string& archivo) {
    DisponibilidadProfesor disp;
    std::ifstream in(archivo);
    if (!in.is_open()) return disp;
    std::string linea, cedula;
    std::map<std::string, int> dias = {{"Lunes",0},{"Martes",1},{"Miercoles",2},{"Jueves",3},{"Viernes",4}};
    while (getline(in, linea)) {
        if (linea.find("Cedula:") != std::string::npos) {
            cedula = linea.substr(linea.find(":")+1);
            cedula.erase(0, cedula.find_first_not_of(" \t"));
        } else {
            for (auto& par : dias) {
                if (linea.find(par.first + ":") != std::string::npos) {
                    std::set<int> horas;
                    std::stringstream ss(linea.substr(linea.find(":")+1));
                    int h;
                    while (ss >> h) horas.insert(h);
                    disp[cedula][par.second] = horas;
                }
            }
        }
    }
    return disp;
}

 // pausa antes de la siguiente instrucción
 void pause(){
    string _;
    cout << "presione cualquier tecla para continuar";
    getline(cin,_);
 }

 // Libera la memoria de la lista enlazada de secciones
 void liberar_lista(section* head) {
    while (head) {
        section* temp = head;
        head = head->next;
        delete temp;
    }
 }

 // Devuelve true si hay conflicto entre dos secciones (profesor o estudiante en común)
 bool hay_conflicto(section* s1, section* s2) {
    if (s1->prof_ci == s2->prof_ci && !s1->prof_ci.empty() && !s2->prof_ci.empty()) return true;
    for (auto& est1 : s1->students) {
        for (auto& est2 : s2->students) {
            if (est1.second == est2.second) return true;
        }
    }
    return false;
 }

 // Devuelve las horas disponibles para una sección, evitando conflictos
 vector<int> horas_disponibles(section* head, section* actual) {
    vector<bool> ocupadas(12, false); // horas 1 a 12 (7:00 a 19:00)
    for (section* s = head; s != nullptr; s = s->next) {
        if (s == actual) break;
        if (hay_conflicto(s, actual)) {
            for (auto& h : s->horarios) {
                for (int i = h.first; i < h.second; ++i) ocupadas[i] = true;
            }
        }
    }
    vector<int> libres;
    for (int i = 1; i < 12; ++i) if (!ocupadas[i]) libres.push_back(i);
    return libres;
 }

 vector<int> horas_disponibles_conflictos(section* head, section* actual) {
    vector<bool> ocupadas(12, false); // horas 1 a 12 (7:00 a 19:00)
    for (section* s = head; s != nullptr; s = s->next) {
            for (auto& h : s->horarios) {
                for (int i = h.first; i < h.second; ++i) ocupadas[i] = true;
            }
        }
    vector<int> libres;
    for (int i = 1; i < 12; ++i) if (!ocupadas[i]) libres.push_back(i);
    return libres;
 }

 // Asigna horarios a cada sección en orden, cumpliendo restricciones
void asignar_horarios_orden(section* head) {
    const vector<string> dias = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes"};
    const int horas_por_dia = 11; // 7am a 6pm (1 a 11)
    for (section* s = head; s != nullptr; s = s->next) {
        s->horarios.clear();
        int bloques_restantes = 0;
        for (int b : s->blocks) bloques_restantes += b;
        int dia = 0, hora = 1;
        for (int b = 0; b < s->blocks.size(); ++b) {
            int tam = s->blocks[b];
            // Si se acaban los días, no se puede asignar más
            if (dia >= dias.size()) break;
            // Si el bloque no cabe en el día actual, pasa al siguiente día
            if (hora + tam - 1 > horas_por_dia) {
                dia++;
                hora = 1;
                if (dia >= dias.size()) break;
            }
            // Asigna el bloque en el día y hora actual
            // Guardamos el día como parte del horario (puedes cambiar la estructura si lo necesitas)
            // Aquí usamos pair<int,int> para hora, pero puedes usar una estructura más compleja si quieres guardar el día
            s->horarios.push_back({dia * 100 + hora, dia * 100 + hora + tam}); // Ejemplo: 0*100+1=1 (Lunes 7am), 1*100+1=101 (Martes 7am)
            hora += tam;
        }
    }
}

 void conflictos_asignadas(section* actual, section* head){

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



 // Muestra todas las secciones/materias disponibles
 void mostrar_todas_las_secciones(section* head) {
    cout << "Secciones/Materias disponibles:\n";
    for (section* s = head; s != nullptr; s = s->next) {
        cout << "- " << s->name_class << endl;
    }
 }

 // Imprime el resultado por pantalla
void imprimir_resultado(section* head, bool version_larga) {
    const vector<string> dias = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes"};
    cout << "Asignacion horaria de la semana:\n";
    for (section* s = head; s != nullptr; s = s->next) {
        cout << "Curso: " << s->name_class << "\n";
        cout << "Profesor: " << s->prof << " ci: " << s->prof_ci << "\n";
        cout << "Horario: ";
        for (auto& h : s->horarios) {
            int ini = h.first, fin = h.second;
            int dia = ini / 100;
            int hora_ini = ini % 100;
            int hora_fin = fin % 100;
            cout << dias[dia] << " " << (6 + hora_ini) << ":00 a " << (6 + hora_fin) << ":00";
            if (&h != &s->horarios.back()) cout << ", ";
        }
        cout << "\n";
        if (version_larga) {
            cout << "Estudiantes:\n";
            for (auto& est : s->students) {
                cout << est.first << " ci: " << est.second << "\n";
            }
        }
        cout << "\n";
    }
}

 // Guarda el resultado en un archivo
void guardar_resultado(section* head, string nombre_archivo, bool version_larga) {
    const vector<string> dias = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes"};
    ofstream out(nombre_archivo);
    out << "Asignacion horaria de la semana:\n";
    for (section* s = head; s != nullptr; s = s->next) {
        out << "Curso: " << s->name_class << "\n";
        out << "Profesor: " << s->prof << " ci: " << s->prof_ci << "\n";
        out << "Horario: ";
        for (auto& h : s->horarios) {
            int ini = h.first, fin = h.second;
            int dia = ini / 100;
            int hora_ini = ini % 100;
            int hora_fin = fin % 100;
            out << dias[dia] << " " << (6 + hora_ini) << ":00 a " << (6 + hora_fin) << ":00";
            if (&h != &s->horarios.back()) out << ", ";
        }
        out << "\n";
        if (version_larga) {
            out << "Estudiantes:\n";
            for (auto& est : s->students) {
                out << est.first << " ci: " << est.second << "\n";
            }
        }
        out << "\n";
    }
    out.close();
}

 bool conflictos_estudiantes(section* clase_no_asignada, section* clase_asignada){
    string estudiante_no_asignada;
    string estudiante_asignada;
    for (int i = 0; i < clase_no_asignada->students.size(); i++){
        estudiante_no_asignada = clase_no_asignada->students[i].second;
        for (int ind = 0; ind < clase_asignada->students.size(); ind++){
            estudiante_asignada = clase_asignada->students[ind].second;
            if (estudiante_no_asignada == estudiante_asignada){
                return true;
            }
        }
    }
    return false;
 }

 vector<int> num_conflictos(ListaEnlazada* lista_asignadas, ListaEnlazada* lista_por_asignar){//simplemente cuento los conflictos de cada clase y los meto en un vector
 vector<int> num_conflictos;
 for (section* clase_no_asignada = lista_por_asignar->cabeza; clase_no_asignada != nullptr; clase_no_asignada = clase_no_asignada->next){
    int conflictos = 0;
    clase_no_asignada->num_conflictos = 0;
    for(section* clase_asignada = lista_asignadas->cabeza; clase_asignada != nullptr; clase_asignada = clase_asignada->next){
        if (clase_asignada->prof_ci == clase_no_asignada->prof_ci || conflictos_estudiantes(clase_no_asignada, clase_asignada)){
            conflictos += 1;
        }
    }
    clase_no_asignada->num_conflictos = conflictos;
    num_conflictos.push_back(conflictos);
 }
 return num_conflictos;
 }

bool mayor_conflicto(vector<int>& conflictos, ListaEnlazada* lista_por_asignar) {
    lista_por_asignar->sig_asignar = nullptr;
    if (conflictos.empty()) return false;

    // Buscar el máximo valor de conflictos
    int max_conflictos = conflictos[0];
    for (size_t i = 1; i < conflictos.size(); ++i) {
        if (conflictos[i] > max_conflictos)
            max_conflictos = conflictos[i];
    }

    // Encontrar el primero con ese valor
    size_t index_max = 0;
    int count_max = 0;
    for (size_t i = 0; i < conflictos.size(); ++i) {
        if (conflictos[i] == max_conflictos) {
            if (count_max == 0) index_max = i;
            count_max++;
        }
    }

    // Apuntar sig_asignar al nodo correspondiente SIEMPRE (aunque haya empate)
    section* sig_asignar = lista_por_asignar->cabeza;
    for (size_t i = 0; i < index_max && sig_asignar != nullptr; ++i) {
        sig_asignar = sig_asignar->next;
    }
    lista_por_asignar->sig_asignar = sig_asignar;

    // true si hay uno solo con el máximo, false si hay más de uno (empate)
    return (count_max == 1);
}

void asignar_horarios(ListaEnlazada* lista_por_asignar, ListaEnlazada* lista_asignadas){
    if (lista_por_asignar->sig_asignar == nullptr) {
        std::cerr << "Error: lista_por_asignar->sig_asignar es nulo en asignar_horarios.\n";
        return;
    }
    lista_por_asignar->sig_asignar->horarios.clear();
    const int dias_totales = 5; // Lunes a Viernes
    const int horas_por_dia = 11; // 7am a 6pm (1 a 11)
    for (int b = 0; b < lista_por_asignar->sig_asignar->blocks.size(); ++b) {
        int tam = lista_por_asignar->sig_asignar->blocks[b];
        bool asignado = false;
        // Reiniciar dia para cada bloque
        for (int dia = 0; dia < dias_totales && !asignado; ++dia) {
            vector<bool> ocupadas(horas_por_dia + 1, false); // 1 a 11

            // Marcar ocupadas por conflictos con otras secciones ya asignadas
            for (section* s = lista_asignadas->cabeza; s != nullptr; s = s->next) {
                if (hay_conflicto(s, lista_por_asignar->sig_asignar)) {
                    for (auto& h : s->horarios) {
                        int h_dia = h.first / 100;
                        if (h_dia == dia) {
                            int ini = h.first % 100;
                            int fin = h.second % 100;
                            for (int i = ini; i < fin; ++i) {
                                if (i >= 1 && i <= horas_por_dia) ocupadas[i] = true;
                            }
                        }
                    }
                }
            }
            // Marcar ocupadas por bloques ya asignados de la misma sección
            for (auto& h : lista_por_asignar->sig_asignar->horarios) {
                int h_dia = h.first / 100;
                if (h_dia == dia) {
                    int ini = h.first % 100;
                    int fin = h.second % 100;
                    for (int i = ini; i < fin; ++i) {
                        if (i >= 1 && i <= horas_por_dia) ocupadas[i] = true;
                    }
                }
            }

            // Buscar bloque contiguo de tamaño tam en este día
            for (int hora = 1; hora + tam - 1 <= horas_por_dia; ++hora) {
                bool libre = true;
                for (int k = 0; k < tam; ++k) {
                    if (ocupadas[hora + k]) {
                        libre = false;
                        break;
                    }
                }
                if (libre) {
                    // Asignar bloque en este día
                    lista_por_asignar->sig_asignar->horarios.push_back({dia * 100 + hora, dia * 100 + hora + tam});
                    asignado = true;
                    break;
                }
            }
        }
        // Si no se pudo asignar el bloque, salir
        if (!asignado) {
            cout << "No se pudo asignar el bloque de tamaño " << tam
                << " para la materia " << lista_por_asignar->sig_asignar->name_class
                << " (" << lista_por_asignar->sig_asignar->prof << ")\n";
            break;
        }
    }
}

section* asignar_horarios_por_conflicto(section* head) {
    ListaEnlazada* lista_original = crearListaEnlazada();
    lista_original->copiar_lista(lista_original, head);
    ListaEnlazada* lista_asignadas = crearListaEnlazada();
    ListaEnlazada* lista_por_asignar = crearListaEnlazada();
    lista_por_asignar->copiar_lista(lista_por_asignar, head);
    vector<int> conflictos_asignadas;
    vector<int> conflictos_no_asignadas;
    lista_por_asignar->sig_asignar = lista_por_asignar->cabeza;

    while (lista_por_asignar->cabeza != nullptr) {
        conflictos_asignadas = num_conflictos(lista_asignadas, lista_por_asignar);
        mayor_conflicto(conflictos_asignadas, lista_por_asignar);

        // Copia el nodo a asignar antes de expulsarlo
        section* nodo_a_asignar = new section(*lista_por_asignar->sig_asignar);
        nodo_a_asignar->next = nullptr;
        nodo_a_asignar->horarios.clear();

        // Asigna el horario sobre el nodo copiado
        ListaEnlazada temp_lista;
        temp_lista.cabeza = nodo_a_asignar;
        temp_lista.sig_asignar = nodo_a_asignar;
        asignar_horarios(&temp_lista, lista_asignadas);

        // Agrega el nodo copiado a lista_asignadas
        lista_asignadas->agregarNodoFinal(lista_asignadas, nodo_a_asignar);

        // Expulsa el nodo original de lista_por_asignar
        lista_por_asignar->expulsar_nodo(lista_por_asignar, lista_por_asignar->sig_asignar);
    }
    section* resultado = lista_asignadas->cabeza;
    // Limpieza de memoria de listas auxiliares si es necesario
    // limpiar_lista(lista_original); limpiar_lista(lista_por_asignar);
    return resultado;
}

void asignar_horarios_general(section* head) {
    for (section* s = head; s != nullptr; s = s->next) {
        s->horarios.clear();
        const int dias_totales = 5; // Lunes a Viernes
        const int horas_por_dia = 11; // 7am a 6pm (1 a 11)
        for (int b = 0; b < s->blocks.size(); ++b) {
            int tam = s->blocks[b];
            bool asignado = false;
            for (int dia = 0; dia < dias_totales && !asignado; ++dia) {
                vector<bool> ocupadas(horas_por_dia + 1, false); // 1 a 11
                // Marcar ocupadas por conflictos con secciones anteriores
                for (section* prev = head; prev != s; prev = prev->next) {
                    if (hay_conflicto(prev, s)) {
                        for (auto& h : prev->horarios) {
                            int h_dia = h.first / 100;
                            if (h_dia == dia) {
                                int ini = h.first % 100;
                                int fin = h.second % 100;
                                for (int i = ini; i < fin; ++i) {
                                    if (i >= 1 && i <= horas_por_dia) ocupadas[i] = true;
                                }
                            }
                        }
                    }
                }
                // Marcar ocupadas por bloques ya asignados de la misma sección
                for (auto& h : s->horarios) {
                    int h_dia = h.first / 100;
                    if (h_dia == dia) {
                        int ini = h.first % 100;
                        int fin = h.second % 100;
                        for (int i = ini; i < fin; ++i) {
                            if (i >= 1 && i <= horas_por_dia) ocupadas[i] = true;
                        }
                    }
                }
                // Buscar bloque contiguo de tamaño tam en este día
                for (int hora = 1; hora + tam - 1 <= horas_por_dia; ++hora) {
                    bool libre = true;
                    // Verifica conflicto con otras secciones
                    for (int k = 0; k < tam; ++k) {
                        if (ocupadas[hora + k]) {
                            libre = false;
                            break;
                        }
                    }
                    // Verifica disponibilidad del profesor
                    if (libre) {
                        std::string ci = s->prof_ci;
                        bool disponible = true;
                        for (int k = 0; k < tam; ++k) {
                            if (disponibilidad_profesores[ci][dia].count(hora + k) == 0) {
                                disponible = false;
                                break;
                            }
                        }
                        if (disponible) {
                            s->horarios.push_back({dia * 100 + hora, dia * 100 + hora + tam});
                            asignado = true;
                            break;
                        }
                    }
                }
            }
            if (!asignado) {
                cout << "No se pudo asignar el bloque de tamaño " << tam << " para la materia " << s->name_class << " (" << s->prof << ")\n";
                break; // Si no se pudo asignar el bloque, salir
            }
        }
    }
}
  void clear_screen() {
 #ifdef _WIN32
    system("cls");       // Limpia pantalla en Windows
 #else
    system("clear");     // Limpia pantalla en otros sistemas
 #endif
 }
void imprimir_materias_por_dia(section* head) {
    const std::vector<std::string> dias = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes"};
    for (int dia = 0; dia < 5; ++dia) {
        std::cout << "Dia " << dias[dia] << ":\n";
        bool hay_materias = false;
        for (section* s = head; s != nullptr; s = s->next) {
            std::vector<std::pair<int, int>> bloques_dia;
            for (auto& h : s->horarios) {
                int h_dia = h.first / 100;
                if (h_dia == dia) {
                    bloques_dia.push_back(h);
                }
            }
            if (!bloques_dia.empty()) {
                hay_materias = true;
                std::cout << "Curso: " << s->name_class << "\n";
                std::cout << "Profesor: " << s->prof << " ci: " << s->prof_ci << "\n";
                std::cout << "Horario: ";
                for (size_t i = 0; i < bloques_dia.size(); ++i) {
                    int hora_ini = bloques_dia[i].first % 100;
                    int hora_fin = bloques_dia[i].second % 100;
                    std::cout << (6 + hora_ini) << ":00 a " << (6 + hora_fin) << ":00";
                    if (i + 1 < bloques_dia.size()) std::cout << ", ";
                }
                std::cout << "\nEstudiantes:\n";
                for (auto& est : s->students) {
                    std::cout << est.first << " ci: " << est.second << "\n";
                }
                std::cout << "\n";
            }
        }
        if (!hay_materias) {
            std::cout << "No hay materias asignadas este día.\n";
        }
        std::cout << "----------------------------------------\n";
    }
}

void guardar_materias_por_dia(section* head, const std::string& nombre_archivo) {
    const std::vector<std::string> dias = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes"};
    std::ofstream out(nombre_archivo);
    if (!out.is_open()) {
        std::cout << "No se pudo abrir el archivo para guardar.\n";
        return;
    }
    for (int dia = 0; dia < 5; ++dia) {
        out << "Dia " << dias[dia] << ":\n";
        bool hay_materias = false;
        for (section* s = head; s != nullptr; s = s->next) {
            std::vector<std::pair<int, int>> bloques_dia;
            for (auto& h : s->horarios) {
                int h_dia = h.first / 100;
                if (h_dia == dia) {
                    bloques_dia.push_back(h);
                }
            }
            if (!bloques_dia.empty()) {
                hay_materias = true;
                out << "Curso: " << s->name_class << "\n";
                out << "Profesor: " << s->prof << " ci: " << s->prof_ci << "\n";
                out << "Horario: ";
                for (size_t i = 0; i < bloques_dia.size(); ++i) {
                    int hora_ini = bloques_dia[i].first % 100;
                    int hora_fin = bloques_dia[i].second % 100;
                    out << (6 + hora_ini) << ":00 a " << (6 + hora_fin) << ":00";
                    if (i + 1 < bloques_dia.size()) out << ", ";
                }
                out << "\nEstudiantes:\n";
                for (auto& est : s->students) {
                    out << est.first << " ci: " << est.second << "\n";
                }
                out << "\n";
            }
        }
        if (!hay_materias) {
            out << "No hay materias asignadas este día.\n";
        }
        out << "----------------------------------------\n";
    }
    out.close();
    std::cout << "Materias agrupadas por día guardadas en " << nombre_archivo << "\n";
}