#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Proceso {
    int id;
    string nombre;
    int prioridad;
    int memoria;
    string estado;
    Proceso* sig;
};

struct NodoCola {
    Proceso* proc;
    NodoCola* sig;
};

struct BloqueMemoria {
    int dir;
    int tam;
    BloqueMemoria* sig;
};

struct GestorProcesos {
    Proceso* cabeza;
    int sigId;
    
    void iniciar() {
        cabeza = NULL;
        sigId = 1;
    }
    
    void insertar(string nom, int pri, int mem) {
        Proceso* nuevo = new Proceso();
        nuevo->id = sigId;
        nuevo->nombre = nom;
        nuevo->prioridad = pri;
        nuevo->memoria = mem;
        nuevo->estado = "Listo";
        nuevo->sig = NULL;
        
        if (cabeza == NULL) {
            cabeza = nuevo;
        } else {
            Proceso* temp = cabeza;
            while (temp->sig != NULL) {
                temp = temp->sig;
            }
            temp->sig = nuevo;
        }
        cout << "Proceso " << sigId << " registrado" << endl;
        sigId++;
    }
    
    void eliminar(int id) {
        if (cabeza == NULL) {
            cout << "No hay procesos" << endl;
            return;
        }
        
        if (cabeza->id == id) {
            Proceso* temp = cabeza;
            cabeza = cabeza->sig;
            delete temp;
            cout << "Proceso " << id << " eliminado" << endl;
            return;
        }
        
        Proceso* act = cabeza;
        while (act->sig != NULL && act->sig->id != id) {
            act = act->sig;
        }
        
        if (act->sig != NULL) {
            Proceso* temp = act->sig;
            act->sig = act->sig->sig;
            delete temp;
            cout << "Proceso " << id << " eliminado" << endl;
        } else {
            cout << "Proceso " << id << " no existe" << endl;
        }
    }
    
    Proceso* buscar(int id) {
        Proceso* act = cabeza;
        while (act != NULL) {
            if (act->id == id) {
                return act;
            }
            act = act->sig;
        }
        return NULL;
    }
    
    void modificarPrioridad(int id, int nuevaPri) {
        Proceso* p = buscar(id);
        if (p != NULL) {
            p->prioridad = nuevaPri;
            cout << "Prioridad de " << id << " ahora es " << nuevaPri << endl;
        } else {
            cout << "Proceso no existe" << endl;
        }
    }
    
    void mostrar() {
        if (cabeza == NULL) {
            cout << "No hay procesos" << endl;
            return;
        }
        Proceso* act = cabeza;
        cout << "\nID\tNombre\tPri\tMem\tEstado" << endl;
        cout << "--------------------------------" << endl;
        while (act != NULL) {
            cout << act->id << "\t" << act->nombre << "\t" 
                 << act->prioridad << "\t" << act->memoria << "\t" 
                 << act->estado << endl;
            act = act->sig;
        }
    }
    
    void cambiarEstado(int id, string nuevoEstado) {
        Proceso* p = buscar(id);
        if (p != NULL) {
            p->estado = nuevoEstado;
        }
    }
    
    void guardar() {
        ofstream archivo("procesos.txt");
        if (!archivo.is_open()) return;
        
        Proceso* act = cabeza;
        while (act != NULL) {
            archivo << act->id << "," << act->nombre << "," 
                    << act->prioridad << "," << act->memoria << "," 
                    << act->estado << endl;
            act = act->sig;
        }
        archivo.close();
    }
    
    void cargar() {
        ifstream archivo("procesos.txt");
        if (!archivo.is_open()) return;
        
        int id, pri, mem;
        string nom, estado;
        char coma;
        
        while (archivo >> id >> coma >> nom >> coma >> pri >> coma >> mem >> coma >> estado) {
            Proceso* nuevo = new Proceso();
            nuevo->id = id;
            nuevo->nombre = nom;
            nuevo->prioridad = pri;
            nuevo->memoria = mem;
            nuevo->estado = estado;
            nuevo->sig = NULL;
            
            if (cabeza == NULL) {
                cabeza = nuevo;
            } else {
                Proceso* temp = cabeza;
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

struct PlanificadorCPU {
    NodoCola* frente;
    NodoCola* final;
    
    void iniciar() {
        frente = NULL;
        final = NULL;
    }
    
    void encolar(Proceso* p) {
        if (p == NULL) return;
        
        NodoCola* nuevo = new NodoCola();
        nuevo->proc = p;
        nuevo->sig = NULL;
        
        if (frente == NULL) {
            frente = nuevo;
            final = nuevo;
        } else {
            final->sig = nuevo;
            final = nuevo;
        }
        cout << "Proceso " << p->id << " encolado" << endl;
    }
    
    Proceso* desencolar() {
        if (frente == NULL) {
            cout << "Cola vacia" << endl;
            return NULL;
        }
        
        NodoCola* temp = frente;
        Proceso* p = frente->proc;
        frente = frente->sig;
        
        if (frente == NULL) {
            final = NULL;
        }
        
        delete temp;
        return p;
    }
    
    void mostrar() {
        if (frente == NULL) {
            cout << "Cola de ejecucion vacia" << endl;
            return;
        }
        
        NodoCola* act = frente;
        cout << "Cola: ";
        while (act != NULL) {
            cout << act->proc->id << " ";
            act = act->sig;
        }
        cout << endl;
    }
    
    int vacia() {
        return frente == NULL;
    }
};

struct GestorMemoria {
    BloqueMemoria* tope;
    int dirActual;
    
    void iniciar() {
        tope = NULL;
        dirActual = 1000;
    }
    
    void asignar(int tam) {
        BloqueMemoria* nuevo = new BloqueMemoria();
        nuevo->dir = dirActual;
        nuevo->tam = tam;
        nuevo->sig = tope;
        tope = nuevo;
        cout << "Memoria asignada: " << dirActual << " (" << tam << " unidades)" << endl;
        dirActual = dirActual + tam;
    }
    
    void liberar() {
        if (tope == NULL) {
            cout << "No hay memoria asignada" << endl;
            return;
        }
        
        BloqueMemoria* temp = tope;
        tope = tope->sig;
        cout << "Memoria liberada: " << temp->dir << endl;
        delete temp;
    }
    
    void mostrar() {
        if (tope == NULL) {
            cout << "No hay bloques de memoria" << endl;
            return;
        }
        
        BloqueMemoria* act = tope;
        cout << "Memoria (tope primero):" << endl;
        while (act != NULL) {
            cout << "Dir " << act->dir << " | Tam " << act->tam << endl;
            act = act->sig;
        }
    }
};

void mostrarMenu() {
    cout << "\n=== SISTEMA GESTION DE PROCESOS ===" << endl;
    cout << "1.  nuevo proceso" << endl;
    cout << "2. Mostrar Registrartodos los procesos" << endl;
    cout << "3. Buscar proceso por ID" << endl;
    cout << "4. Modificar prioridad" << endl;
    cout << "5. Eliminar proceso" << endl;
    cout << "6. Enviar proceso a cola de ejecucion" << endl;
    cout << "7. Ejecutar proceso (desencolar)" << endl;
    cout << "8. Mostrar cola de ejecucion" << endl;
    cout << "9. Asignar memoria" << endl;
    cout << "10. Liberar memoria" << endl;
    cout << "11. Mostrar memoria" << endl;
    cout << "12. Guardar y salir" << endl;
    cout << "Opcion: ";
}

int main() {
    GestorProcesos gp;
    PlanificadorCPU pc;
    GestorMemoria gm;
    
    gp.iniciar();
    pc.iniciar();
    gm.iniciar();
    gp.cargar();
    
    int op, id, pri, mem;
    string nom;
    
    do {
        mostrarMenu();
        cin >> op;
        
        switch(op) {
            case 1:
                cout << "Nombre: ";
                cin >> nom;
                cout << "Prioridad (1-10): ";
                cin >> pri;
                cout << "Memoria requerida: ";
                cin >> mem;
                gp.insertar(nom, pri, mem);
                break;
                
            case 2:
                gp.mostrar();
                break;
                
            case 3:
                cout << "ID a buscar: ";
                cin >> id;
                if (gp.buscar(id) != NULL) {
                    cout << "Proceso encontrado" << endl;
                } else {
                    cout << "No existe" << endl;
                }
                break;
                
            case 4:
                cout << "ID: ";
                cin >> id;
                cout << "Nueva prioridad: ";
                cin >> pri;
                gp.modificarPrioridad(id, pri);
                break;
                
            case 5:
                cout << "ID a eliminar: ";
                cin >> id;
                gp.eliminar(id);
                break;
                
            case 6: {
                cout << "ID a encolar: ";
                cin >> id;
                Proceso* p = gp.buscar(id);
                if (p != NULL && p->estado == "Listo") {
                    pc.encolar(p);
                    gp.cambiarEstado(id, "En cola");
                } else if (p != NULL && p->estado != "Listo") {
                    cout << "Proceso no esta listo" << endl;
                } else {
                    cout << "Proceso no existe" << endl;
                }
                break;
            }
                
            case 7: {
                Proceso* ejec = pc.desencolar();
                if (ejec != NULL) {
                    cout << "Ejecutando proceso " << ejec->id << " (" << ejec->nombre << ")" << endl;
                    gp.cambiarEstado(ejec->id, "Ejecutando");
                    gm.asignar(ejec->memoria);
                    cout << "Proceso " << ejec->id << " finalizado" << endl;
                    gp.cambiarEstado(ejec->id, "Finalizado");
                    gm.liberar();
                }
                break;
            }
                
            case 8:
                pc.mostrar();
                break;
                
            case 9:
                cout << "Tamano a asignar: ";
                cin >> mem;
                gm.asignar(mem);
                break;
                
            case 10:
                gm.liberar();
                break;
                
            case 11:
                gm.mostrar();
                break;
                
            case 12:
                gp.guardar();
                cout << "Datos guardados. Saliendo..." << endl;
                break;
                
            default:
                cout << "Opcion no valida" << endl;
        }
    } while (op != 12);
    
    return 0;
}
