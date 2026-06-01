#include <iostream>
using namespace std;

struct BloqueMemoria{
    int idBloque;
    BloqueMemoria*siguiente;

};

class PilaMemoria{
private:
    BloqueMemoria*tope;
public:
    PilaMemoria(){
        tope nullptr;

}

int pop(){
    if (tope ==nullptr){
        return -1;
    }

    BloqueMemoria*temp = tope;
    int id = temp -> idBloque;

    tope = tope -> siguiente;
    delete temp;

    return id;
}

bool estaVacia (){
    reurn tope == nullptr;
}

void mostrar (){
    BloqueMemoria*aux = tope;

    while( aux != nullptr){
        cout<<"Bloque" << aux -> idBloque<<endl;
        aux=aux -> siguiente;
    
    }
  }
};