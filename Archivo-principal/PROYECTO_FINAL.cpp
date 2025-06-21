#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <map>

using namespace std;

// Estructuras de datos
struct Profesor {
    string ci;
    map<string, vector<int>> disponibilidad; // Día -> horas disponibles
    Profesor* next;
};

struct Estudiante {
    string ci;
    Estudiante* next;
};

struct Section {
    string name_class;
    vector<int> blocks;
    string prof;
    string prof_ci;
    vector<pair<string, string>> students;
    map<string, vector<pair<int, int>>> horarios; // Día -> horarios asignados
    int num_conflictos = 0;
    Section* next;
};

// Listas enlazadas
struct ListaProfesores {
    Profesor* cabeza = nullptr;
    
    void agregarProfesor(Profesor* nuevo) {
        nuevo->next = cabeza;
        cabeza = nuevo;
    }
    
    Profesor* buscarProfesor(string ci) {
        Profesor* actual = cabeza;
        while (actual != nullptr) {
            if (actual->ci == ci) return actual;
            actual = actual->next;
        }
        return nullptr;
    }
};

struct ListaEstudiantes {
    Estudiante* cabeza = nullptr;
    
    void agregarEstudiante(Estudiante* nuevo) {
        nuevo->next = cabeza;
        cabeza = nuevo;
    }
    
    bool existeEstudiante(string ci) {
        Estudiante* actual = cabeza;
        while (actual != nullptr) {
            if (actual->ci == ci) return true;
            actual = actual->next;
        }
        return false;
    }
};

// Funciones auxiliares
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "Presione Enter para continuar...";
    cin.ignore();
}

// Funciones para leer archivos
Section* leer_secciones(string nombre_archivo) {
    ifstream file(nombre_archivo);
    if (!file.is_open()) return nullptr;

    Section* head = nullptr;
    Section* tail = nullptr;
    string line;
    Section* actual = nullptr;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.find("Seccion:") != string::npos) {
            Section* nueva = new Section;
            nueva->name_class = line.substr(line.find(":") + 1);
            nueva->horarios["Lunes"] = vector<pair<int, int>>();
            nueva->horarios["Martes"] = vector<pair<int, int>>();
            nueva->horarios["Miercoles"] = vector<pair<int, int>>();
            nueva->horarios["Jueves"] = vector<pair<int, int>>();
            nueva->horarios["Viernes"] = vector<pair<int, int>>();
            nueva->next = nullptr;
            if (!head) head = nueva;
            if (tail) tail->next = nueva;
            tail = nueva;
            actual = nueva;
        }
        else if (line.find("Bloques:") != string::npos && actual) {
            stringstream ss(line.substr(line.find(":") + 1));
            int num;
            while (ss >> num) actual->blocks.push_back(num);
        }
        else if (line.find("Profesor:") != string::npos && actual) {
            string datos = line.substr(line.find(":") + 1);
            size_t pos = datos.find("ci:");
            if (pos != string::npos) {
                actual->prof = datos.substr(0, pos);
                actual->prof_ci = datos.substr(pos + 3);
            } else {
                actual->prof = datos;
                actual->prof_ci = "";
            }
        }
        else if (line.find("Estudiantes:") != string::npos && actual) {
            while (getline(file, line) && !line.empty()) {
                size_t pos = line.find("ci:");
                if (pos != string::npos) {
                    string nombre = line.substr(0, pos);
                    string ci = line.substr(pos + 3);
                    actual->students.push_back({nombre, ci});
                }
            }
        }
    }
    file.close();
    return head;
}

ListaProfesores leer_profesores(string nombre_archivo) {
    ifstream file(nombre_archivo);
    ListaProfesores lista;
    string line;
    Profesor* current = nullptr;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.find("Cedula:") != string::npos) {
            current = new Profesor;
            current->ci = line.substr(line.find(":") + 1);
            lista.agregarProfesor(current);
        }
        else if (current) {
            size_t colon_pos = line.find(":");
            if (colon_pos != string::npos) {
                string dia = line.substr(0, colon_pos);
                stringstream ss(line.substr(colon_pos + 1));
                int hora;
                while (ss >> hora) {
                    current->disponibilidad[dia].push_back(hora);
                }
            }
        }
    }
    file.close();
    return lista;
}

ListaEstudiantes leer_estudiantes(string nombre_archivo) {
    ifstream file(nombre_archivo);
    ListaEstudiantes lista;
    string line;

    while (getline(file, line)) {
        if (!line.empty()) {
            Estudiante* nuevo = new Estudiante;
            nuevo->ci = line;
            lista.agregarEstudiante(nuevo);
        }
    }
    file.close();
    return lista;
}

// Funciones para asignación de horarios
bool hay_conflicto(Section* s1, Section* s2) {
    if (s1->prof_ci == s2->prof_ci && !s1->prof_ci.empty()) return true;
    for (auto& est1 : s1->students) {
        for (auto& est2 : s2->students) {
            if (est1.second == est2.second) return true;
        }
    }
    return false;
}

void contar_conflictos(Section* head) {
    // Inicializar contadores
    for (Section* s = head; s != nullptr; s = s->next) {
        s->num_conflictos = 0;
    }
    
    // Contar conflictos
    for (Section* s1 = head; s1 != nullptr; s1 = s1->next) {
        for (Section* s2 = head; s2 != nullptr; s2 = s2->next) {
            if (s1 != s2 && hay_conflicto(s1, s2)) {
                s1->num_conflictos++;
            }
        }
    }
}

Section* invertir_lista(Section* head) {
    Section* prev = nullptr;
    Section* current = head;
    Section* next = nullptr;
    
    while (current != nullptr) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    
    return prev;
}

vector<int> horas_disponibles(Section* head, Section* actual, string dia) {
    vector<bool> ocupadas(12, false); // Horas de 7:00 (1) a 18:00 (12)
    
    // Marcar horas ocupadas por secciones en conflicto
    for (Section* s = head; s != nullptr; s = s->next) {
        if (s == actual) continue;
        if (hay_conflicto(s, actual)) {
            for (auto& h : s->horarios[dia]) {
                for (int i = h.first; i < h.second; ++i) {
                    if (i > 0 && i < 12) ocupadas[i] = true;
                }
            }
        }
    }
    
    // Recolectar horas disponibles
    vector<int> libres;
    for (int i = 1; i < 12; ++i) {
        if (!ocupadas[i]) libres.push_back(i);
    }
    return libres;
}

void asignar_horarios_orden_normal(Section* head, ListaProfesores& profesores) {
    const string dias[] = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes"};
    
    for (Section* s = head; s != nullptr; s = s->next) {
        Profesor* prof = profesores.buscarProfesor(s->prof_ci);
        
        for (string dia : dias) {
            vector<int> horas_disp_prof;
            if (prof && prof->disponibilidad.find(dia) != prof->disponibilidad.end()) {
                horas_disp_prof = prof->disponibilidad[dia];
            }
            
            vector<int> libres = horas_disponibles(head, s, dia);
            vector<int> horas_validas;
            
            // Filtrar por disponibilidad del profesor
            for (int h : libres) {
                if (horas_disp_prof.empty() || 
                    find(horas_disp_prof.begin(), horas_disp_prof.end(), h) != horas_disp_prof.end()) {
                    horas_validas.push_back(h);
                }
            }
            
            // Asignar bloques
            int idx = 0, horas_asignadas = 0;
            for (int b = 0; b < s->blocks.size() && horas_asignadas < 4; ++b) {
                int tam = s->blocks[b];
                bool asignado = false;
                
                while (idx + tam <= horas_validas.size()) {
                    bool contiguo = true;
                    for (int j = 1; j < tam; ++j) {
                        if (horas_validas[idx + j] != horas_validas[idx] + j) {
                            contiguo = false;
                            break;
                        }
                    }
                    
                    if (contiguo) {
                        s->horarios[dia].push_back({horas_validas[idx], horas_validas[idx] + tam});
                        idx += tam;
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
}

void asignar_horarios_orden_inverso(Section** head, ListaProfesores& profesores) {
    *head = invertir_lista(*head);
    asignar_horarios_orden_normal(*head, profesores);
}

void asignar_horarios_mayor_conflicto(Section** head, ListaProfesores& profesores) {
    // Contar conflictos para cada sección
    contar_conflictos(*head);
    
    // Ordenar por número de conflictos (mayor primero)
    bool cambios;
    do {
        cambios = false;
        Section* prev = nullptr;
        Section* curr = *head;
        
        while (curr != nullptr && curr->next != nullptr) {
            if (curr->num_conflictos < curr->next->num_conflictos) {
                // Intercambiar nodos
                Section* temp = curr->next;
                curr->next = temp->next;
                temp->next = curr;
                
                if (prev == nullptr) {
                    *head = temp;
                } else {
                    prev->next = temp;
                }
                
                prev = temp;
                cambios = true;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    } while (cambios);
    
    // Asignar horarios al orden resultante
    asignar_horarios_orden_normal(*head, profesores);
}

// Funciones para mostrar y guardar horarios
void mostrar_horarios(Section* head) {
    const string dias[] = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes"};
    
    for (string dia : dias) {
        cout << "\n=== " << dia << " ===\n";
        if (head == nullptr) {
            cout << "No hay secciones cargadas\n";
            continue;
        }
        
        bool dia_vacio = true;
        for (Section* s = head; s != nullptr; s = s->next) {
            if (!s->horarios[dia].empty()) {
                cout << s->name_class << " - " << s->prof << " (";
                for (auto& h : s->horarios[dia]) {
                    cout << h.first+6 << ":00-" << h.second+6 << ":00 ";
                }
                cout << ")\n";
                dia_vacio = false;
            }
        }
        if (dia_vacio) cout << "No hay clases programadas\n";
    }
}

void guardar_horarios(Section* head, string nombre_archivo) {
    ofstream out(nombre_archivo);
    const string dias[] = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes"};
    
    for (string dia : dias) {
        out << "=== " << dia << " ===\n";
        
        for (Section* s = head; s != nullptr; s = s->next) {
            if (!s->horarios[dia].empty()) {
                out << s->name_class << " - " << s->prof << " (";
                for (auto& h : s->horarios[dia]) {
                    out << h.first+6 << ":00-" << h.second+6 << ":00 ";
                }
                out << ")\n";
            }
        }
        out << "\n";
    }
    out.close();
}

// Menú principal
void menu() {
    Section* head = nullptr;
    ListaProfesores lista_profesores;
    ListaEstudiantes lista_estudiantes;
    bool salir = false;

    while (!salir) {
        clear_screen();
        cout << "===== SISTEMA DE ASIGNACION DE HORARIOS =====\n";
        cout << "1. Cargar secciones\n";
        cout << "2. Cargar disponibilidad de profesores\n";
        cout << "3. Cargar lista de estudiantes\n";
        cout << "4. Asignar horarios (orden normal)\n";
        cout << "5. Asignar horarios (orden inverso)\n";
        cout << "6. Asignar horarios (mayor conflicto primero)\n";
        cout << "7. Mostrar horarios\n";
        cout << "8. Guardar horarios en archivo\n";
        cout << "9. Salir\n";
        cout << "Seleccione una opcion: ";
        
        int opcion;
        string input;
        getline(cin, input);
        stringstream ss(input);
        
        if (!(ss >> opcion)) {
            cout << "Opcion invalida. Intente nuevamente.\n";
            pause();
            continue;
        }

        switch (opcion) {
            case 1: {
                cout << "Nombre del archivo de secciones: ";
                string nombre;
                getline(cin, nombre);
                if (head != nullptr) {
                    // Liberar memoria de las secciones anteriores
                    Section* current = head;
                    while (current != nullptr) {
                        Section* temp = current;
                        current = current->next;
                        delete temp;
                    }
                }
                head = leer_secciones(nombre);
                if (head == nullptr) {
                    cout << "Error al leer el archivo de secciones.\n";
                } else {
                    cout << "Secciones cargadas exitosamente.\n";
                }
                pause();
                break;
            }
            case 2: {
                cout << "Nombre del archivo de profesores: ";
                string nombre;
                getline(cin, nombre);
                lista_profesores = leer_profesores(nombre);
                cout << "Profesores cargados exitosamente.\n";
                pause();
                break;
            }
            case 3: {
                cout << "Nombre del archivo de estudiantes: ";
                string nombre;
                getline(cin, nombre);
                lista_estudiantes = leer_estudiantes(nombre);
                cout << "Estudiantes cargados exitosamente.\n";
                pause();
                break;
            }
            case 4: {
                if (head == nullptr) {
                    cout << "Primero debe cargar las secciones.\n";
                } else {
                    asignar_horarios_orden_normal(head, lista_profesores);
                    cout << "Horarios asignados (orden normal) exitosamente.\n";
                }
                pause();
                break;
            }
            case 5: {
                if (head == nullptr) {
                    cout << "Primero debe cargar las secciones.\n";
                } else {
                    asignar_horarios_orden_inverso(&head, lista_profesores);
                    cout << "Horarios asignados (orden inverso) exitosamente.\n";
                }
                pause();
                break;
            }
            case 6: {
                if (head == nullptr) {
                    cout << "Primero debe cargar las secciones.\n";
                } else {
                    asignar_horarios_mayor_conflicto(&head, lista_profesores);
                    cout << "Horarios asignados (mayor conflicto primero) exitosamente.\n";
                }
                pause();
                break;
            }
            case 7: {
                mostrar_horarios(head);
                pause();
                break;
            }
            case 8: {
                if (head == nullptr) {
                    cout << "No hay horarios para guardar.\n";
                } else {
                    cout << "Nombre del archivo para guardar: ";
                    string nombre;
                    getline(cin, nombre);
                    guardar_horarios(head, nombre);
                    cout << "Horarios guardados exitosamente en " << nombre << endl;
                }
                pause();
                break;
            }
            case 9: {
                salir = true;
                break;
            }
            default: {
                cout << "Opcion invalida. Intente nuevamente.\n";
                pause();
                break;
            }
        }
    }

    // Liberar memoria al salir
    while (head != nullptr) {
        Section* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    menu();
    return 0;
}