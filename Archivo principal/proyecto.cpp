#include <iostream>      // Permite entrada y salida estándar
#include <fstream>       // Permite manejo de archivos
#include <string>        // Permite uso de cadenas de texto
#include <vector>        // Permite uso de vectores (arreglos dinámicos)
#include <sstream>       // Permite manipulación de strings como flujos
#include <cstdlib>       // Permite uso de funciones del sistema
#include <algorithm>     // Permite uso de algoritmos estándar (no se usa mucho aquí)

using namespace std;     // Usa el espacio de nombres estándar para evitar std::

struct section {
    string name_class;                           // Nombre de la materia/sección
    vector<int> blocks;                          // Vector con los bloques de horas de la sección
    string prof;                                 // Nombre del profesor
    string prof_ci;                              // Cédula del profesor
    vector<pair<string, string>> students;       // Vector de pares (nombre, ci) de estudiantes
    vector<pair<int, int>> horarios;             // Vector de pares (hora inicio, hora fin) asignados a la sección
    section* next;                               // Puntero al siguiente nodo de la lista enlazada de secciones
};

void clear_screen() {
#ifdef _WIN32
    system("cls");      // Limpia pantalla en Windows
#else
    system("clear");    // Limpia pantalla en otros sistemas
#endif
}

// Declaración de funciones
void menu();
section* leer_secciones(string nombre_archivo);
void liberar_lista(section* head);
void asignar_horarios_orden(section* head);
void mostrar_todas_las_secciones(section* head);
void imprimir_resultado(section* head, bool version_larga);
void guardar_resultado(section* head, string nombre_archivo, bool version_larga);
bool hay_conflicto(section* s1, section* s2);
vector<int> horas_disponibles(section* head, section* actual);
section* invertirLista(section* head);
void pause();


int main() {
    menu();     // Llama al menú principal
    return 0;
}

void menu() {
    string estado = "No cargado";
    section* lista_inv = nullptr;
    string archivo_entrada, archivo_salida;
    section* head = nullptr;   // Puntero al inicio de la lista de secciones
    string _;     
    int opcion;
    bool salir = false;
    while (!salir) {
        clear_screen();          // Limpia la pantalla
        // Muestra el menú principal
        cout << "===== MENU PRINCIPAL =====\n";
        cout << "1. Leer archivo de secciones"<<"(" << estado << ")"<< "\n";
        cout << "2. Asignar horarios (orden original)\n";
        cout << "3. Asignar horarios(orden inverso)\n";
        cout << "4 Asignar horarios(estrategia por mayor conflicto)\n";
        cout << "5. Guardar resultado en archivo\n";
        cout << "6. Mostrar resultado por pantalla\n";
        cout << "7. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();
            if (std::cin.fail()) {
                std::cout << "Entrada invalida. Debe ingresar un numero entero.\n";
                std::cin.clear(); // Limpia el estado de error
                std::cin.ignore(10000, '\n'); // Descarta la entrada incorrecta
                pause();
                continue; // Vuelve al inicio del bucle para mostrar el menú de nuevo
            }
        
        switch (opcion) {
            case 1:
                // Lee el archivo de entrada y carga las secciones
                cout << "Ingrese el nombre del archivo de entrada: ";
                getline(cin, archivo_entrada);
                liberar_lista(head); // Libera memoria de la lista anterior
                head = leer_secciones(archivo_entrada); // Lee y carga las secciones
                if (head) cout << "Archivo cargado correctamente.\n", estado = "Cargado";
                else cout << "Error al cargar el archivo.\n";
                pause();
                break;
            case 2:
                // Asigna horarios a las secciones en orden
                if (!head) {
                    cout << "Primero debe cargar un archivo de secciones.\n";
                } else {
                    mostrar_todas_las_secciones(head); // Muestra las materias antes de asignar
                    asignar_horarios_orden(head);      // Asigna horarios
                    cout << "Horarios asignados para un dia (lunes).\n";
                }
                pause();
                break;
            case 3:
                //Asigna horarios a las secciones en orden inverso
                head = invertirLista(head);
                if (!head) {
                    cout << "Primero debe cargar un archivo de secciones.\n";
                } else {
                    mostrar_todas_las_secciones(head); // Muestra las materias antes de asignar
                    asignar_horarios_orden(head);      // Asigna horarios
                    cout << "Horarios asignados para un dia (lunes).\n";
                }
                pause();
                break;
            case 4:
                //Asigna horarios a las secciones en orden de mayor conflicto
                if (!head) {
                    cout << "Primero debe cargar un archivo de secciones.\n";
                } else {
                    mostrar_todas_las_secciones(head); // Muestra las materias antes de asignar
                    asignar_horarios_orden(head);      // Asigna horarios
                    cout << "Horarios asignados para un dia (lunes).\n";
                }
                pause();
                break;
            case 5:
                // Guarda el resultado en un archivo
                if (!head) {
                    cout << "Primero debe cargar y asignar horarios.\n";
                } else {
                    cout << "Ingrese el nombre del archivo de salida: ";
                    getline(cin, archivo_salida);
                    cout << "Mostrar version larga (1) o corta (0): ";
                    bool larga;
                    cin >> larga;
                    cin.ignore();
                    guardar_resultado(head, archivo_salida, larga); // Guarda el resultado
                    cout << "Archivo guardado.\n";
                }
                pause();
                break;
            case 6:
                // Muestra el resultado por pantalla
                if (!head) {
                    cout << "Primero debe cargar y asignar horarios.\n";
                } else {
                    cout << "Mostrar version larga (1) o corta (0): ";
                    bool larga;
                    cin >> larga;
                    cin.ignore();
                    imprimir_resultado(head, larga); // Imprime el resultado
                }
                pause();
                break;
            case 7:
                salir = true; // Sale del menú
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
    vector<bool> ocupadas(13, false); // horas 1 a 12 (7:00 a 19:00)
    for (section* s = head; s != nullptr; s = s->next) {
        if (s == actual) break;
        if (hay_conflicto(s, actual)) {
            for (auto& h : s->horarios) {
                for (int i = h.first; i < h.second; ++i) ocupadas[i] = true;
            }
        }
    }
    vector<int> libres;
    for (int i = 1; i <= 12; ++i) if (!ocupadas[i]) libres.push_back(i);
    return libres;
}

// Asigna horarios a cada sección en orden, cumpliendo restricciones
void asignar_horarios_orden(section* head) {
    for (section* s = head; s != nullptr; s = s->next) {
        s->horarios.clear();
        vector<int> libres = horas_disponibles(head, s);
        int idx = 0, bloques_asignados = 0, horas_asignadas = 0;
        for (int b = 0; b < s->blocks.size(); ++b) {
            int tam = s->blocks[b];
            // Restricciones de bloques y horas máximas
            if (s->blocks.size() == 3 && tam == 2 && bloques_asignados >= 2) break;
            if (s->blocks.size() == 3 && s->blocks == vector<int>{2,2,1} && tam == 2 && bloques_asignados >= 1) {
                if(tam == 2){
                    tam == 1;
                    continue;
                } else break;
            }
            if (horas_asignadas + tam > 4) break;
            // Busca bloques contiguos de horas libres
            bool asignado = false;
            while (idx + tam <= libres.size()) {
                bool contiguo = true;
                for (int j = 1; j < tam; ++j) {
                    if (libres[idx+j] != libres[idx]+j) {
                        contiguo = false;
                        break;
                    }
                }
                if (contiguo) {
                    s->horarios.push_back({libres[idx], libres[idx]+tam});
                    idx += tam;
                    bloques_asignados++;
                    horas_asignadas += tam;
                    asignado = true;
                    break;
                }
                idx++;
            }
            if (!asignado) break;
        }
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

// Muestra todas las secciones/materias disponibles
void mostrar_todas_las_secciones(section* head) {
    cout << "Secciones/Materias disponibles:\n";
    for (section* s = head; s != nullptr; s = s->next) {
        cout << "- " << s->name_class << endl;
    }
}

// Imprime el resultado por pantalla
void imprimir_resultado(section* head, bool version_larga) {
    cout << "Asignacion horaria de este dia (lunes):\n";
    for (section* s = head; s != nullptr; s = s->next) {
        cout << "Curso: " << s->name_class << "\n";
        cout << "Profesor: " << s->prof << " ci: " << s->prof_ci << "\n";
        cout << "Horario: ";
        for (auto& h : s->horarios) {
            int ini = h.first, fin = h.second;
            cout << (6+ini) << ":00 a " << (6+fin) << ":00";
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
    ofstream out(nombre_archivo);
    out << "Asignacion horaria de este dia (lunes):\n";
    for (section* s = head; s != nullptr; s = s->next) {
        out << "Curso: " << s->name_class << "\n";
        out << "Profesor: " << s->prof << " ci: " << s->prof_ci << "\n";
        out << "Horario: ";
        for (auto& h : s->horarios) {
            int ini = h.first, fin = h.second;
            out << (6+ini) << ":00 a " << (6+fin) << ":00";
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