#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

struct Mascota {
    int id;
    string nombre;
    string dueno;
    int prioridad;
    int consultorio;
    string estado;
    Mascota* sig;
};

struct NodoCola {
    Mascota* mascota;
    NodoCola* sig;
};

struct PilaConsultorios {
    int num;
    PilaConsultorios* sig;
};

struct GestorMascotas {
    Mascota* cabeza;
    int sigId;
    
    void iniciar() {
        cabeza = NULL;
        sigId = 1;
    }
    
    void registrar(string nom, string du, int pri) {
        Mascota* nuevo = new Mascota();
        nuevo->id = sigId;
        nuevo->nombre = nom;
        nuevo->dueno = du;
        nuevo->prioridad = pri;
        nuevo->consultorio = 0;
        nuevo->estado = "Registrado";
        nuevo->sig = NULL;
        
        if (cabeza == NULL) {
            cabeza = nuevo;
        } else {
            Mascota* temp = cabeza;
            while (temp->sig != NULL) {
                temp = temp->sig;
            }
            temp->sig = nuevo;
        }
        cout << "Mascota " << sigId << " registrada" << endl;
        sigId++;
    }
    
    void eliminar(int id) {
        if (cabeza == NULL) {
            cout << "No hay mascotas" << endl;
            return;
        }
        
        if (cabeza->id == id) {
            Mascota* temp = cabeza;
            cabeza = cabeza->sig;
            delete temp;
            cout << "Mascota " << id << " eliminada" << endl;
            return;
        }
        
        Mascota* act = cabeza;
        while (act->sig != NULL && act->sig->id != id) {
            act = act->sig;
        }
        
        if (act->sig != NULL) {
            Mascota* temp = act->sig;
            act->sig = act->sig->sig;
            delete temp;
            cout << "Mascota " << id << " eliminada" << endl;
        } else {
            cout << "Mascota " << id << " no existe" << endl;
        }
    }
    
    Mascota* buscar(int id) {
        Mascota* act = cabeza;
        while (act != NULL) {
            if (act->id == id) {
                return act;
            }
            act = act->sig;
        }
        return NULL;
    }
    
    void cambiarPrioridad(int id, int nuevaPri) {
        Mascota* m = buscar(id);
        if (m != NULL) {
            m->prioridad = nuevaPri;
            cout << "Prioridad de " << id << " ahora es " << nuevaPri << endl;
        } else {
            cout << "Mascota no existe" << endl;
        }
    }
    
    void mostrar() {
        if (cabeza == NULL) {
            cout << "No hay mascotas registradas" << endl;
            return;
        }
        Mascota* act = cabeza;
        cout << "\nID\tNombre\t\tDueño\t\tPri\tCons\tEstado" << endl;
        cout << "--------------------------------------------------------" << endl;
        while (act != NULL) {
            cout << act->id << "\t" << act->nombre << "\t\t" 
                 << act->dueno << "\t\t" << act->prioridad << "\t" 
                 << act->consultorio << "\t" << act->estado << endl;
            act = act->sig;
        }
    }
    
    void cambiarEstado(int id, string nuevoEstado, int cons = 0) {
        Mascota* m = buscar(id);
        if (m != NULL) {
            m->estado = nuevoEstado;
            if (cons > 0) {
                m->consultorio = cons;
            }
        }
    }
    
    void guardar() {
        ofstream archivo("mascotas.txt");
        if (!archivo.is_open()) return;
        
        Mascota* act = cabeza;
        while (act != NULL) {
            archivo << act->id << "," << act->nombre << "," 
                    << act->dueno << "," << act->prioridad << "," 
                    << act->consultorio << "," << act->estado << endl;
            act = act->sig;
        }
        archivo.close();
    }
    
    void cargar() {
        ifstream archivo("mascotas.txt");
        if (!archivo.is_open()) return;
        
        int id, pri, cons;
        string nom, dueno, estado;
        char coma;
        
        while (archivo >> id >> coma >> nom >> coma >> dueno >> coma >> pri >> coma >> cons >> coma >> estado) {
            Mascota* nuevo = new Mascota();
            nuevo->id = id;
            nuevo->nombre = nom;
            nuevo->dueno = dueno;
            nuevo->prioridad = pri;
            nuevo->consultorio = cons;
            nuevo->estado = estado;
            nuevo->sig = NULL;
            
            if (cabeza == NULL) {
                cabeza = nuevo;
            } else {
                Mascota* temp = cabeza;
                while (temp->sig != NULL) {
                    temp = temp->sig;
                }
                temp->sig = nuevo;
            }
            if (id >= sigId) {
                sigId = id + 1;
            }
        }
        archivo.close();
    }
};

struct ColaEspera {
    NodoCola* frente;
    NodoCola* final;
    
    void iniciar() {
        frente = NULL;
        final = NULL;
    }
    
    void encolar(Mascota* m) {
        if (m == NULL) return;
        
        NodoCola* nuevo = new NodoCola();
        nuevo->mascota = m;
        nuevo->sig = NULL;
        
        if (frente == NULL) {
            frente = nuevo;
            final = nuevo;
        } else {
            final->sig = nuevo;
            final = nuevo;
        }
        cout << "Mascota " << m->id << " en fila de espera" << endl;
    }
    
    Mascota* desencolar() {
        if (frente == NULL) {
            cout << "Fila vacia" << endl;
            return NULL;
        }
        
        NodoCola* temp = frente;
        Mascota* m = frente->mascota;
        frente = frente->sig;
        
        if (frente == NULL) {
            final = NULL;
        }
        
        delete temp;
        return m;
    }
    
    void mostrar() {
        if (frente == NULL) {
            cout << "Fila de espera vacia" << endl;
            return;
        }
        
        NodoCola* act = frente;
        cout << "Fila: ";
        while (act != NULL) {
            cout << act->mascota->id << " ";
            act = act->sig;
        }
        cout << endl;
    }
    
    int vacia() {
        return frente == NULL;
    }
};

struct PilaConsultorios {
    PilaConsultorios* tope;
    
    void iniciar() {
        tope = NULL;
        for (int i = 5; i >= 1; i--) {
            PilaConsultorios* nuevo = new PilaConsultorios();
            nuevo->num = i;
            nuevo->sig = tope;
            tope = nuevo;
        }
        cout << "Consultorios 1-5 disponibles" << endl;
    }
    
    int asignar() {
        if (tope == NULL) {
            cout << "No hay consultorios disponibles" << endl;
            return 0;
        }
        PilaConsultorios* temp = tope;
        int num = tope->num;
        tope = tope->sig;
        delete temp;
        cout << "Consultorio " << num << " asignado" << endl;
        return num;
    }
    
    void liberar(int num) {
        PilaConsultorios* nuevo = new PilaConsultorios();
        nuevo->num = num;
        nuevo->sig = tope;
        tope = nuevo;
        cout << "Consultorio " << num << " liberado" << endl;
    }
    
    void mostrar() {
        if (tope == NULL) {
            cout << "No hay consultorios disponibles" << endl;
            return;
        }
        PilaConsultorios* act = tope;
        cout << "Consultorios disponibles: ";
        while (act != NULL) {
            cout << act->num << " ";
            act = act->sig;
        }
        cout << endl;
    }
};

void mostrarMenu() {
    cout << "\n=== SISTEMA DE GESTION VETERINARIA ===" << endl;
    cout << "1. Registrar mascota (lista)" << endl;
    cout << "2. Ver todas las mascotas" << endl;
    cout << "3. Buscar mascota por ID" << endl;
    cout << "4. Cambiar prioridad (emergencia)" << endl;
    cout << "5. Eliminar mascota" << endl;
    cout << "6. Agregar a fila de espera (cola)" << endl;
    cout << "7. Atender siguiente (asignar consultorio)" << endl;
    cout << "8. Ver fila de espera" << endl;
    cout << "9. Ver consultorios disponibles (pila)" << endl;
    cout << "10. Liberar consultorio (terminar atencion)" << endl;
    cout << "11. Guardar y salir" << endl;
    cout << "Opcion: ";
}

int main() {
    GestorMascotas gm;
    ColaEspera cola;
    PilaConsultorios pc;
    
    gm.iniciar();
    cola.iniciar();
    pc.iniciar();
    gm.cargar();
    
    int op, id, pri;
    string nom, dueno;
    
    do {
        mostrarMenu();
        cin >> op;
        
        switch(op) {
            case 1:
                cout << "Nombre mascota: ";
                cin >> nom;
                cout << "Nombre dueño: ";
                cin >> dueno;
                cout << "Prioridad (1=emergencia, 2=consulta, 3=vacuna): ";
                cin >> pri;
                gm.registrar(nom, dueno, pri);
                break;
                
            case 2:
                gm.mostrar();
                break;
                
            case 3:
                cout << "ID: ";
                cin >> id;
                if (gm.buscar(id) != NULL) {
                    cout << "Mascota encontrada" << endl;
                } else {
                    cout << "No existe" << endl;
                }
                break;
                
            case 4:
                cout << "ID: ";
                cin >> id;
                cout << "Nueva prioridad (1=emergencia): ";
                cin >> pri;
                gm.cambiarPrioridad(id, pri);
                break;
                
            case 5:
                cout << "ID a eliminar: ";
                cin >> id;
                gm.eliminar(id);
                break;
                
            case 6: {
                cout << "ID a agregar a fila: ";
                cin >> id;
                Mascota* m = gm.buscar(id);
                if (m != NULL && m->estado == "Registrado") {
                    cola.encolar(m);
                    gm.cambiarEstado(id, "En espera");
                } else if (m != NULL && m->estado != "Registrado") {
                    cout << "Mascota no disponible" << endl;
                } else {
                    cout << "Mascota no existe" << endl;
                }
                break;
            }
                
            case 7: {
                Mascota* m = cola.desencolar();
                if (m != NULL) {
                    cout << "Atendiendo a " << m->nombre << " (dueño: " << m->dueno << ")" << endl;
                    int cons = pc.asignar();
                    if (cons > 0) {
                        gm.cambiarEstado(m->id, "Atendido", cons);
                        cout << "Atencion completada. Consultorio " << cons << endl;
                    } else {
                        cout << "No hay consultorios. Reintente" << endl;
                    }
                }
                break;
            }
                
            case 8:
                cola.mostrar();
                break;
                
            case 9:
                pc.mostrar();
                break;
                
            case 10: {
                cout << "Numero de consultorio a liberar: ";
                cin >> id;
                pc.liberar(id);
                break;
            }
                
            case 11:
                gm.guardar();
                cout << "Datos guardados. Saliendo..." << endl;
                break;
                
            default:
                cout << "Opcion no valida" << endl;
        }
    } while (op != 11);
    
    return 0;
}
