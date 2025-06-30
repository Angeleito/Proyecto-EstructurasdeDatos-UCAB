#include "salida_datos.h"

 // Muestra todas las secciones/materias disponibles
 void mostrar_todas_las_secciones(lista_secciones head) {
    cout << "Secciones/Materias disponibles:\n";
    for (section* s = head; s != nullptr; s = s->next) {
        cout << "- " << s->name_class << endl;
    }
 }

 // Imprime el resultado por pantalla
void imprimir_resultado(lista_secciones head, bool version_larga) {
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

void imprimir_materias_por_dia(lista_secciones head) {
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

