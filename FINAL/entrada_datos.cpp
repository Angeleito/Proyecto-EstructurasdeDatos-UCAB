#include "entrada_datos.h"
#include "estructuras_datos.h"
#include "asignacion_datos.h"
#include "salida_datos.h"
string archivo_disponibilidad = "profesores.txt";
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
    cout << "3. Cargar archivo de cedulas de estudiantes inscritos\n";
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
            cout << "Ingrese el nombre del archivo de cedulas de estudiantes inscritos: ";
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

  // Guarda el resultado en un archivo
void guardar_resultado(lista_secciones head, string nombre_archivo, bool version_larga) {
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

  void clear_screen() {
 #ifdef _WIN32
    system("cls");       // Limpia pantalla en Windows
 #else
    system("clear");     // Limpia pantalla en otros sistemas
 #endif
 }

 void guardar_materias_por_dia(lista_secciones head, const std::string& nombre_archivo) {
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