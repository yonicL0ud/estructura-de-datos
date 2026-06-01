#include <iostream>
using namespace std;

int main (){
    int option;

    do{
        cout<<"\n========== SISTEMA DE GESTION DE PROCESOS ========";
        cout<<"\n1. Gestor de procesos";
        cout<<"\n2. Planificador de CPU";
        cout<<"\n3. Gestor de momeria";
        cout<<"\n4. Guardar datos";
        cout<<"\n5. Cargar datos";
        cout<<"\n6. SALIR";
        cout<<\n"Seleccione opcion";

    }
    cin>>opcion;
    switch(opcion){
        case 1:
        cout<<"Modulo de proceso";
        break;

        case 2:
        cout<<"Modulo CPU";
        break;

        case 3:
        cout<<"Modulo memoria";
        break;

        case 4:
        cout<<"Guardando datos. . .";
        break;

        case 5:
        cout<<"Cargando datos. . .";
        break;

        case 6:
        cout<<"Saliendo. . .";
        break;

        default:
        cout<<"Opcion invalida";
    }
    while(opcion !=6);

    return 0;
}