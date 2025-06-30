#include "asignacion_datos.h"
std::map<std::string, std::map<int, std::set<int>>> disponibilidad_profesores; 
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

section* asignar_horarios_por_conflicto(lista_secciones head) {
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

void asignar_horarios_general(lista_secciones head) {
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
                cout << "No se pudo asignar el bloque de tamano " << tam << " para la materia " << s->name_class << " (" << s->prof << ")\n";
                break; // Si no se pudo asignar el bloque, salir
            }
        }
    }
}

 // Devuelve true si hay conflicto entre dos secciones (profesor o estudiante en común)
 bool hay_conflicto(lista_secciones s1, lista_secciones s2) {
    if (s1->prof_ci == s2->prof_ci && !s1->prof_ci.empty() && !s2->prof_ci.empty()) return true;
    for (auto& est1 : s1->students) {
        for (auto& est2 : s2->students) {
            if (est1.second == est2.second) return true;
        }
    }
    return false;
 }

 // Devuelve las horas disponibles para una sección, evitando conflictos
 vector<int> horas_disponibles(lista_secciones head, lista_secciones actual) {
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

 vector<int> horas_disponibles_conflictos(lista_secciones head, lista_secciones actual) {
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

bool conflictos_estudiantes(lista_secciones clase_no_asignada, lista_secciones clase_asignada){
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