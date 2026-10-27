#include <iostream>
#include <string>
using namespace std;

const int MAX_CANT_PALABRAS_X_LINEA = 3;

typedef unsigned int Posicion;


enum _retorno 
{ 
    OK, 
    ERROR, 
    NO_IMPLEMENTADA 
};
typedef enum _retorno TipoRetorno;

struct Palabras
{
    string Palabra;
    Palabras * sigP;
    Palabras * antP;
};
typedef Palabras * Pal;

struct Linea
{
    Palabras * primP;
    Linea * sigL;
    Linea * antL;
};
typedef Linea * LN;

struct CabezalLinea
{
    Linea * primL;
    Linea * ultL;       
};
typedef CabezalLinea * CL;


struct Diccionario{
    string Palabra;
    Diccionario *izq;
    Diccionario *der;
};
typedef Diccionario * DIC;

LN BuscarLinea(Posicion linea);
Pal BuscarPalabra(LN linea, Posicion pos);
Posicion ContPalabras(LN linea);
Posicion ContarLineas(CL cabezal);
TipoRetorno BorrarPalabra(Posicion linea, Posicion pos);
TipoRetorno BorrarLinea(Posicion linea);
void Destruir();
TipoRetorno ImprimirTexto();
TipoRetorno InsertarLinea();
TipoRetorno IngresarPalabraDiccionario(string palabra);
TipoRetorno AgregarIncorrectasAlDiccionario();
bool existeEnDiccionario(DIC d,string palabra);
TipoRetorno InsertarPalabra(Posicion linea, Posicion pos, string palabra);


CL cabezal = NULL;
DIC diccionario = NULL;
void Inicializar(){
    
    Destruir();
    cabezal = new CabezalLinea;
    cabezal->primL = cabezal->ultL = NULL;
    diccionario = NULL;
}

TipoRetorno BorrarLinea(Posicion linea){
    LN borrar = BuscarLinea(linea);
    if(borrar==NULL){
        return ERROR;
    }

    while(borrar->primP != NULL){
        BorrarPalabra(linea, 1);
    }
    
    if(borrar->antL!=NULL){
        borrar->antL->sigL=borrar->sigL;
    }else{
        cabezal->primL=borrar->sigL;
    }
    
    if(borrar->sigL!=NULL){
        borrar->sigL->antL=borrar->antL;
    }else{
        cabezal->ultL=borrar->antL;
    }
    delete borrar;
    return OK;
}

Posicion ContarLineas(CL cabezal){
    Posicion cont = 0;
    LN aux = cabezal->primL;
    while(aux!=NULL){
        aux=aux->sigL;
        cont++;
    }
    return cont;
}

Posicion ContPalabras(LN linea){                    
    Posicion cont = 0;
    Pal aux = linea->primP;
    while(aux!=NULL){
        aux=aux->sigP;
        cont++;
    }
    return cont;
}

LN BuscarLinea(Posicion linea){
    if(cabezal == NULL || linea == 0)
        return NULL;

    LN aux = cabezal->primL;
    Posicion cont = 1;

    while(aux != NULL && cont < linea){
        aux = aux->sigL;
        cont++;
    }

    return aux;
}

Pal BuscarPalabra(LN linea, Posicion pos){
    if(linea == NULL || pos == 0)
        return NULL;

    Pal aux = linea->primP;
    Posicion cont = 1;

    while(aux != NULL && cont < pos){
        aux = aux->sigP;
        cont++;
    }

    return aux;
}
    

TipoRetorno BorrarPalabra(Posicion linea, Posicion pos){
    
    LN aux = BuscarLinea(linea);
    Pal borrar = BuscarPalabra(aux,pos);
    if(borrar==NULL){
        return ERROR;
    }
    if(borrar->antP!=NULL){
        borrar->antP->sigP=borrar->sigP;
    }
    else{
        aux->primP=borrar->sigP;
    }
    if(borrar->sigP!=NULL){
        borrar->sigP->antP=borrar->antP;
    }
    delete borrar;

    return OK;
}

void BorrarDiccionario(DIC d){
    if(d==NULL){
        return;  
    }
    BorrarDiccionario(d->izq);
    BorrarDiccionario(d->der);
    delete d;

    
}


void Destruir(){

    if(cabezal != NULL){

        while(cabezal->primL != NULL){
            BorrarLinea(1);
        }

        delete cabezal;
        cabezal = NULL;
    }

    BorrarDiccionario(diccionario);
    diccionario = NULL;
}

TipoRetorno ImprimirTexto()
{
    if(cabezal == NULL||cabezal->primL==NULL)
    {
        cout << "Texto vacio" <<endl;
        return OK;
    }
    LN lAux = cabezal->primL;
    Posicion numLinea = 1;

    while (lAux != NULL) 
    {
        cout << numLinea << ":";
        Pal pAux = lAux->primP;
        while (pAux != NULL) {
            cout << " " << pAux->Palabra;
            pAux = pAux->sigP;
        }
        cout << endl;
        lAux = lAux->sigL;
        numLinea++;
    }
    return OK;
}

TipoRetorno InsertarLinea()
{
    if (cabezal == NULL) 
    {
        cabezal = new CabezalLinea;
        cabezal->primL = cabezal->ultL = NULL;
    }
    LN nueva = new Linea;
    nueva->primP = NULL;
    nueva->sigL = NULL;
    nueva->antL = cabezal->ultL;
    if (cabezal->ultL != NULL)
        cabezal->ultL->sigL = nueva;
    else
        cabezal->primL = nueva;
    cabezal->ultL = nueva;
    return OK;
}

bool palabraValida(string palabra){
    if (palabra.empty() || palabra.find(' ') != string::npos) 
        return false;
    return true;
}

TipoRetorno IngresarPalabraDiccionario(string palabra) 
{
    if (!palabraValida(palabra)){
        return ERROR;
    }

    if(existeEnDiccionario(diccionario,palabra)){
        return ERROR;
    }
    if (diccionario == NULL)
    {
        diccionario = new Diccionario;
        diccionario->Palabra = palabra;
        diccionario->izq = NULL;
        diccionario->der = NULL;
        return OK;
    }
    DIC aux = diccionario;
    DIC padre = NULL;
    
    while (aux != NULL) 
    {
        padre = aux;
        if (palabra < aux->Palabra) 
            aux = aux->izq;
        else 
            aux = aux->der;
    }

    DIC nuevo = new Diccionario;
    nuevo->Palabra = palabra;
    nuevo->izq = NULL;
    nuevo->der = NULL;

    if (palabra < padre->Palabra)
    {
        padre->izq = nuevo;
    } 
    else 
    {
        padre->der = nuevo;
    }

    return OK;
}
void ImprimirDiccionarioAux(DIC d){
    if (d == NULL) 
        return;
    ImprimirDiccionarioAux(d->izq);
    cout << d->Palabra << endl;
    ImprimirDiccionarioAux(d->der);
}

TipoRetorno ImprimirDiccionario() 
{
    if (diccionario == NULL)
    {
        cout << "Diccionario vacio" << endl;
        return OK;
    }
    ImprimirDiccionarioAux(diccionario);
    
    return OK;
}

TipoRetorno InsertarPalabra(Posicion linea, Posicion pos, string palabra){
    if(!palabraValida(palabra)){
        return ERROR;
    }
    LN l = BuscarLinea(linea);
    if(l==NULL){
        return ERROR;
    }

    Posicion cant = ContPalabras(l);
    if (pos < 1 || pos > cant + 1) {
        return ERROR;
    }

    Pal nuevo = new Palabras;
    nuevo->Palabra = palabra;
    nuevo->sigP = NULL;
    nuevo->antP = NULL;

    if (pos == 1) {
        nuevo->sigP = l->primP;
        if (l->primP != NULL) {
            l->primP->antP = nuevo;
        }
        l->primP = nuevo;
    } else {
        Pal ant = BuscarPalabra(l, pos - 1);
        nuevo->sigP = ant->sigP;
        nuevo->antP = ant;
        if (ant->sigP != NULL) {
            ant->sigP->antP = nuevo;
        }
        ant->sigP = nuevo;
    }

    while (ContPalabras(l) > MAX_CANT_PALABRAS_X_LINEA) {

        Pal ultPal = BuscarPalabra(l, ContPalabras(l));
        string palabraApasar = ultPal->Palabra;

        if (ultPal->antP != NULL) {
            ultPal->antP->sigP = NULL;
        } else {
            l->primP = NULL;
        }
        delete ultPal;

        if (l->sigL == NULL) {
            InsertarLinea();
        }

        l = l->sigL;

        Pal nodoPasado = new Palabras;
        nodoPasado->Palabra = palabraApasar;
        nodoPasado->sigP = l->primP;
        nodoPasado->antP = NULL;

        if (l->primP != NULL) {
            l->primP->antP = nodoPasado;
        }
        l->primP = nodoPasado;
    }

    return OK;
}

bool existeEnDiccionario(DIC d,string palabra){
    if(d==NULL){
        return false;
    }
    if(palabra==d->Palabra){
        return true;
    }
    if(palabra>d->Palabra){
        return existeEnDiccionario(d->der,palabra);
    }else{
        return existeEnDiccionario(d->izq,palabra);
    }

}

TipoRetorno ImprimirTextoIncorrecto(){
    if(cabezal == NULL||cabezal->primL==NULL)
    {
        cout << "Texto vacio" <<endl;
        return OK;
    }
    LN lAux = cabezal->primL;
    Posicion numLinea = 1;

    while (lAux != NULL) 
    {
        cout << numLinea << ":";
        Pal pAux = lAux->primP;
        while (pAux != NULL) {
            if(!existeEnDiccionario(diccionario,pAux->Palabra)){
                cout << " " << pAux->Palabra;
            }
            pAux = pAux->sigP;
        }
        cout << endl;
        lAux = lAux->sigL;
        numLinea++;
    }
    return OK;
}

TipoRetorno AgregarIncorrectasAlDiccionario(){
    if(cabezal==NULL||cabezal->primL==NULL){
        return OK;
    }
    LN lAux = cabezal->primL;
    while(lAux!=NULL){
        Pal pAux = lAux->primP;
        while(pAux!=NULL){
            IngresarPalabraDiccionario(pAux->Palabra);
            pAux=pAux->sigP;
        }
        lAux=lAux->sigL;
    }
    return OK;
}


int main(){

    return 0;
}
