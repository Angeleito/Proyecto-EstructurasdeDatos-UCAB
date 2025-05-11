#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

struct section {
    string name_class;
    vector<int> blocks;
    string prof;
    string prof_ci;
    vector<pair<string, string>> students; // nombre, ci
    vector<pair<int, int>> horarios; // pares de hora inicio y fin por bloque
    section* next;
};

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void menu();
section* leer_secciones(string nombre_archivo);
void liberar_lista(section* head);
void asignar_horarios_orden(section* head);
void mostrar_todas_las_secciones(section* head);
void imprimir_resultado(section* head, bool version_larga);
void guardar_resultado(section* head, string nombre_archivo, bool version_larga);
bool hay_conflicto(section* s1, section* s2);
vector<int> horas_disponibles(section* head, section* actual);

int main() {
    menu();
    return 0;
}

void menu() {
    string archivo_entrada, archivo_salida;
    section* head = nullptr;
    int opcion;
    bool salir = false;
    while (!salir) {
        clear_screen();
        // Mostrar siempre las secciones/materias disponibles
        if (head) {
            mostrar_todas_las_secciones(head);
            cout << endl;
        }
        cout << "===== MENU PRINCIPAL =====\n";
        cout << "1. Leer archivo de secciones\n";
        cout << "2. Asignar horarios (orden original)\n";
        cout << "3. Guardar resultado en archivo\n";
        cout << "4. Mostrar resultado por pantalla\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();
        switch (opcion) {
            case 1:
                cout << "Ingrese el nombre del archivo de entrada: ";
                getline(cin, archivo_entrada);
                liberar_lista(head);
                head = leer_secciones(archivo_entrada);
                if (head) cout << "Archivo cargado correctamente.\n";
                else cout << "Error al cargar el archivo.\n";
                system("pause");
                break;
            case 2:
                if (!head) {
                    cout << "Primero debe cargar un archivo de secciones.\n";
                } else {
                    // Mostrar todas las materias antes de asignar
                    mostrar_todas_las_secciones(head);
                    asignar_horarios_orden(head);
                    cout << "Horarios asignados para un dia (lunes).\n";
                }
                system("pause");
                break;
            case 3:
                if (!head) {
                    cout << "Primero debe cargar y asignar horarios.\n";
                } else {
                    cout << "Ingrese el nombre del archivo de salida: ";
                    getline(cin, archivo_salida);
                    cout << "Mostrar version larga (1) o corta (0): ";
                    bool larga;
                    cin >> larga;
                    cin.ignore();
                    guardar_resultado(head, archivo_salida, larga);
                    cout << "Archivo guardado.\n";
                }
                system("pause");
                break;
            case 4:
                if (!head) {
                    cout << "Primero debe cargar y asignar horarios.\n";
                } else {
                    cout << "Mostrar version larga (1) o corta (0): ";
                    bool larga;
                    cin >> larga;
                    cin.ignore();
                    imprimir_resultado(head, larga);
                }
                system("pause");
                break;
            case 5:
                salir = true;
                break;
            default:
                cout << "Opcion invalida.\n";
                system("pause");
        }
    }
    liberar_lista(head);
}

section* leer_secciones(string nombre_archivo) {
    ifstream file(nombre_archivo);
    if (!file.is_open()) return nullptr;
    section* head = nullptr;
    section* tail = nullptr;
    string line;
    section* actual = nullptr;
    while (getline(file, line)) {
        if (line.find("Seccion:") != string::npos) {
            section* nueva = new section;
            nueva->name_class = line.substr(line.find(":") + 1);
            nueva->name_class.erase(0, nueva->name_class.find_first_not_of(" \t"));
            nueva->next = nullptr;
            if (!head) head = nueva;
            if (tail) tail->next = nueva;
            tail = nueva;
            actual = nueva;
        } else if (line.find("Bloques:") != string::npos && actual) {
            actual->blocks.clear();
            stringstream ss(line.substr(line.find(":") + 1));
            int num;
            while (ss >> num) actual->blocks.push_back(num);
        } else if (line.find("Profesor:") != string::npos && actual) {
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
            // leer estudiantes hasta la siguiente seccion o fin de archivo
            string next_line;
            while (getline(file, next_line)) {
                if (next_line.empty() || next_line.find("Seccion:") != string::npos) {
                    // Si es una nueva sección, procesar en la siguiente vuelta del while principal
                    line = next_line;
                    break;
                }
                // Buscar cedula (con o sin "ci:")
                string nombre, ci;
                size_t pos_ci = next_line.find("ci:");
                if (pos_ci != string::npos) {
                    nombre = next_line.substr(0, pos_ci);
                    ci = next_line.substr(pos_ci + 3);
                } else {
                    // Buscar último espacio como separador de cedula
                    size_t last_space = next_line.find_last_of(" ");
                    if (last_space != string::npos && last_space + 1 < next_line.size()) {
                        nombre = next_line.substr(0, last_space);
                        ci = next_line.substr(last_space + 1);
                    } else {
                        nombre = next_line;
                        ci = "";
                    }
                }
                nombre.erase(nombre.find_last_not_of(" \t")+1);
                ci.erase(0, ci.find_first_not_of(" \t"));
                // Evitar duplicados
                bool repetido = false;
                for (auto& est : actual->students) {
                    if (est.second == ci) {
                        repetido = true;
                        break;
                    }
                }
                if (!repetido && !ci.empty()) {
                    actual->students.push_back({nombre, ci});
                }
            }
            // Si next_line es "Seccion:", el while principal la procesará en la siguiente iteración
            continue;
        }
    }
    file.close();
    return head;
}

void liberar_lista(section* head) {
    while (head) {
        section* temp = head;
        head = head->next;
        delete temp;
    }
}

// Devuelve true si hay conflicto entre dos secciones (profesor o estudiante en comun)
bool hay_conflicto(section* s1, section* s2) {
    if (s1->prof_ci == s2->prof_ci && !s1->prof_ci.empty()) return true;
    for (auto& est1 : s1->students) {
        for (auto& est2 : s2->students) {
            if (est1.second == est2.second) return true;
        }
    }
    return false;
}

// Devuelve las horas disponibles para una seccion, evitando conflictos
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

// Asigna horarios a cada seccion en orden, cumpliendo restricciones
void asignar_horarios_orden(section* head) {
    for (section* s = head; s != nullptr; s = s->next) {
        s->horarios.clear();
        vector<int> libres = horas_disponibles(head, s);
        int idx = 0, bloques_asignados = 0, horas_asignadas = 0;
        for (int b = 0; b < s->blocks.size(); ++b) {
            int tam = s->blocks[b];
            // Restricciones
            if (s->blocks.size() == 3 && tam == 2 && bloques_asignados >= 2) break;
            if (s->blocks.size() == 3 && s->blocks == vector<int>{2,2,1} && tam == 2 && bloques_asignados >= 1) break;
            if (horas_asignadas + tam > 4) break;
            // Buscar bloque contiguo
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