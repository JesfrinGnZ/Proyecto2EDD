/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: jesfrin
 *
 * Created on 15 de abril de 2019, 10:44 AM
 */

#include <cstdlib>
#include <string.h>
#include <iostream>
#include <fstream>


using namespace std;


/*Declaracion de metodos*/
struct Nodo;
struct Atributo;
struct Lista;
struct ElementoDeTabla;
struct NodoInterno;
struct ListaDeNodosInternos;
struct NodoHoja;
struct ListaDeNodosHoja;
struct Pagina;
struct Arbol;
struct ListaDeLlavesForaneas;
struct LlaveForanea;
struct NombreDeTabla;
struct ListaDeNombreDeTabla;


//Metodos
void anadirAtributoA_Nodo(Atributo* atributo, Nodo* nodo);
void anadirTablaA_Lista(Lista* lista, Nodo* nodo);
Nodo* crearNodo();
Atributo* crearAtributo(string tipo, string id);
ElementoDeTabla* crearElementoTabla(string atributo, string valorDeAtributo);
Lista* crearLista();
//Metodos auxiliares a las operaciones siguientes
bool buscarAtributo(string idClave); //estructura
Nodo* buscarTabla(string nombreDeTabla); //entrada
void concatenarAtributo(Atributo *atributo); //entrada
string buscarLlavePrimaria(string clave, ElementoDeTabla *elemento); //entrada
bool buscarAtributoEnTabla(Nodo* tabla, string atributoTexto, string tipo);
//Metodos para archivo de ESTRUCTURA
void analizarEstructura();
void analisisDeLineaParaEstructura(string linea);
void analisisDeAtributo(string linea);
//Metodos para archivo de Entrada de datos
void analizarEntrada();
void analisisDeLineaParaEntrada(string linea);
//Metodos para reprotes
void analizarReportes();
void analisisDeLineaParaReporte(string cadenaFormada);
void analisisDeReporte(string linea);
Pagina* buscarPaginaMasALaIzquierda(Pagina* partida);
void mostrarDatosDePagina(Pagina* primerPagina);
//Metodos para manejo de escritura en archivos
void escribirEnArchivo(char* direccion, string mensaje);
//Metodos para manejo de arboles
ListaDeNodosInternos* crearListaDeNodosInternos(NodoInterno *nodoInterno);
ListaDeNodosHoja* crearListaDeNodosHoja(NodoHoja* nodoHoja);
NodoHoja* crearNodoHoja(ElementoDeTabla* elementoDeTabla, Nodo* tablaActual);
NodoInterno* crearNodoInterno(Pagina* hijoIzquierdo, Pagina* hijoDerecho, string clave);
Pagina* crearPagina(NodoHoja *nodoHoja, NodoInterno *nodoInterno, Pagina *padre);
Arbol* crearArbol(NodoHoja *nodoHoja, NodoInterno *nodoInterno, Pagina *padre);
void insertarEnArbol(ElementoDeTabla *elementoDeTabla);
void anadirNodoHojaA_Arbol(NodoHoja* nuevoNodo, Pagina* paginaDondeSeColocaraElNodoHoja);
NodoHoja* buscarNodoHoja(Pagina* paginaDeNodo, int posicion);
NodoInterno* buscarNodoInterno(Pagina* paginaDeNodo, int posicion);
Pagina *buscarPaginaDeInsercion(NodoInterno *partida, string claveBuscada);
void restructurarArbol(Pagina *pagina, ElementoDeTabla *elemento);
void anadirNodoInterno(Pagina* pagina, NodoInterno* nuevoNodo);


//Variable que guarda los mensajes de acciones y numero de fila
string acciones = "";
int numeroDeFila = 0;
int numeroDeInicioDeAccion;
//Varable que almacena las tablas y sus atributos
Lista* listaDeTablas;
//Variables para analisis de estructura, y formacion de estructuras
bool laClaveExiste = false;
bool existeErrorEnLlavesForaneas = false;
Nodo* nodoActual;
//Variables para analisis de entrada y asignacion de ElementosEstructura al arbol
bool existeErrorAlCrearElementoDeTabla = false;
ElementoDeTabla *elementoDeTabla = NULL;
Nodo* tablaParaElemento = NULL;
//main

int main(int argc, char** argv) {
    //Se vacia el archivo log.txt
    //Se inicia la lista de nodos q contiene las tablas
    //string hola = "solaaskdjnfkajsdnfkjasdnjkfnaskjdfnkasjdfnajskdnfjkasdf";
    //string l = hola + "dsdfnasjkodfniaoskdnfkasjdnf" + "lkmlksdmfklasndfioawnemr;oiqwjeasdifanweuitfnasiuhtniewjasdfncauisjfnasiodznfajisdfn";
    listaDeTablas = crearLista();
    acciones += "-----------------------estructura.xml--------------------------\n";
    analizarEstructura();
    numeroDeFila = 0;
    acciones += "-------------------------entrada.dat---------------------------\n";
    analizarEntrada();
    escribirEnArchivo("log.txt", acciones);
    numeroDeFila = 0;
    acciones += "-------------------------reportes.txt---------------------------\n";
    analizarReportes();
    return 0;
}

/*------------------------Nodo almacenara los tipos de tabla-----------------------*/
typedef struct Nodo {
    struct Nodo *siguiente; //Siguiente tabla
    string tabla; //Nombre de tabla
    string clave; //Clave de tabla
    Arbol *arbol; //Arbol de tabla
    ListaDeLlavesForaneas *llavesForaneas;
    struct Atributo *atributoPrimero;
} Nodo;

/*----------------------------------Elementos para claves foraneas---------------------------*/

typedef struct ListaDeLlavesForaneas {
    LlaveForanea *primerLlaveForanea;
} ListaDeLlavesForaneas;

typedef struct LlaveForanea {
    Nodo * tabla;
    string nombreDeAtributo;
    LlaveForanea *llaveSiguiente;
} LlaveForanea;

ListaDeLlavesForaneas* crearListaDeLlavesForaneas() {
    ListaDeLlavesForaneas *nuevaLista = new ListaDeLlavesForaneas;
    nuevaLista->primerLlaveForanea = NULL;
    return nuevaLista;
}

LlaveForanea* crearLlaveForanea(Nodo* tabla, string nombreDeAtributo) {
    LlaveForanea *llave = new LlaveForanea;
    llave->tabla = tabla;
    llave->nombreDeAtributo = nombreDeAtributo;
    return llave;
}

void anadirLlaveForanea(ListaDeLlavesForaneas *listaDeLlaves, LlaveForanea *nuevaLlave) {
    if (listaDeLlaves->primerLlaveForanea == NULL) {
        listaDeLlaves->primerLlaveForanea = nuevaLlave;
    } else {
        LlaveForanea *llave = listaDeLlaves->primerLlaveForanea;
        while (llave->llaveSiguiente != NULL) {
            llave = llave->llaveSiguiente;
        }
        llave->llaveSiguiente = nuevaLlave;
    }
}

/*----------------------------------------Elementos para nombres de tablas---------------------*/

typedef struct NombreDeTabla {
    string nombreTabla;
    NombreDeTabla *siguiente;
} NombreDeTabla;

typedef struct ListaDeNombreDeTabla {
    NombreDeTabla * primerNombre;
} ListaDeNombreDeTabla;

ListaDeNombreDeTabla* crearListaDeNombresDeTabla(NombreDeTabla *nombre) {
    ListaDeNombreDeTabla *lista = new ListaDeNombreDeTabla;
    lista->primerNombre = nombre;
    return lista;
}

NombreDeTabla* crearNombreDeTabla(string nombreDeTabla) {
    NombreDeTabla *nombre = new NombreDeTabla;
    nombre->nombreTabla = nombreDeTabla;
    nombre->siguiente = NULL;
    return nombre;
}

void anadirNombreDeTabla(ListaDeNombreDeTabla *lista, NombreDeTabla *nombre) {
    NombreDeTabla *nombreActual = lista->primerNombre;
    while (nombreActual->siguiente != NULL) {
        nombreActual = nombreActual->siguiente;
    }
    nombreActual->siguiente = nombre;
}

/*------------------------Estructuras para trabajo con arboles b+---------------------------*/


typedef struct NodoInterno {
    Pagina *hijoIzquierdo;
    Pagina *hijoDerecho;
    NodoInterno* nodoInternoSiguiente;
    string clave;
} NodoInterno;

typedef struct ListaDeNodosInternos {
    NodoInterno* nodoInterno;
    int cantidad;
} ListaDeNodosInternos;

typedef struct NodoHoja {
    ElementoDeTabla *elementoDeTabla;
    NodoHoja* nodoHojaSiguiente;
    NodoHoja* nodoHojaAnterior;
    string clave;
} NodoHoja;

typedef struct ListaDeNodosHoja {
    NodoHoja* nodoHoja;
    int cantidad;
} ListaDeNodosHoja;

typedef struct Pagina {
    string tipo; //Interna hoja o raiz
    ListaDeNodosInternos* listaDeNodosInternos;
    ListaDeNodosHoja* listaDeNodosHoja;
    Pagina *padre;
    Pagina *paginaSiguiente; //Pagina que servira commo apuntador en las paginas hojas
    Pagina *paginaAnterior;
} Pagina;

typedef struct Arbol {
    Pagina *paginaRaiz;
} Arbol;

/*--------------------------Almacenara los atributos que pertenecen a la tabla, y a los elementos-----------------*/
typedef struct Atributo {
    string tipo; //int o string.Llave primaria el id indicara cual es para identificarlo 
    string id;
    struct Atributo* atributoSiguiente;
} Atributo;

typedef struct ElementoDeTabla {
    string atributo; //int o string.Llave primaria el id indicara cual es para identificarlo 
    string valorDeAtributo;
    struct ElementoDeTabla *valorSiguiente;
} ElementoDeTabla;

/*-----------------------------Lista que almacenara los tipos de tabla------------------------*/
typedef struct Lista {
    struct Nodo* PrimerNodo;
} Lista;

/*----------------------------------------------Acciones para b+---------------------------------*/

ListaDeNodosInternos* crearListaDeNodosInternos(NodoInterno *nodoInterno) {
    ListaDeNodosInternos *lista = new ListaDeNodosInternos;
    lista->nodoInterno = nodoInterno;
    lista->cantidad = 1;
    return lista;
}

NodoInterno* crearNodoInterno(Pagina* hijoIzquierdo, Pagina* hijoDerecho, string clave) {
    NodoInterno *nodo = new NodoInterno;
    nodo->hijoIzquierdo = hijoIzquierdo;
    nodo->hijoDerecho = hijoDerecho; //Clave para el nodo hoja
    nodo->clave = clave;
    nodo->nodoInternoSiguiente = NULL;
    return nodo;
}

ListaDeNodosHoja* crearListaDeNodosHoja(NodoHoja* nodoHoja) {
    ListaDeNodosHoja *lista = new ListaDeNodosHoja;
    lista->nodoHoja = nodoHoja;
    lista->cantidad = 1;
    return lista;
}

NodoHoja* crearNodoHoja(ElementoDeTabla* elemento, Nodo* tablaActual) {
    NodoHoja *nodo = new NodoHoja;
    nodo->elementoDeTabla = elemento;
    nodo->clave = buscarLlavePrimaria(tablaActual->clave, elemento); //Clave para el nodo hoja
    nodo->nodoHojaSiguiente = NULL;
    nodo->nodoHojaAnterior = NULL; //SI es nulo es el primero de la lista
    return nodo;
}

void imprimirElementosDeTbla(Nodo* tablaParaElemento) {
    cout << "***************************************************" << endl;
    Pagina* paginaRaiz = tablaParaElemento->arbol->paginaRaiz;
    NodoHoja *nodoHoja = paginaRaiz->listaDeNodosHoja->nodoHoja;
    while (nodoHoja->nodoHojaSiguiente != NULL) {
        cout << nodoHoja->elementoDeTabla->valorDeAtributo << endl;
        nodoHoja = nodoHoja->nodoHojaSiguiente;
    }
    cout << nodoHoja->clave << endl;
    cout << "*****************************************************" << endl;
}

string buscarTipoDeClave(Nodo *tabla) {//Devuelve si la clave es int o char
    string nombreDeClave = tabla->clave;
    Atributo *atributoActual = tabla->atributoPrimero;
    while (atributoActual != NULL) {
        if (atributoActual->id.compare(nombreDeClave) == 0) {
            return atributoActual->tipo;
        }
        atributoActual = atributoActual->atributoSiguiente;
    }
    return atributoActual->tipo;
}

long convertirACodigoComparable(string cadena, Nodo* tabla) {
    if (buscarTipoDeClave(tabla).compare("char") == 0) {
        long valor = 0;
        for (int i = 0; i < cadena.size(); i++) {
            long suma = cadena.at(i);
            long mult = suma * (10 + i);
            valor += mult;
        }
        return valor;
    } else {
        long b = atoi(cadena.c_str());
        return b;
    }

}

void anadirNodoHojaA_Arbol(NodoHoja* nuevoNodo, Pagina* paginaDondeSeColocaraElNodoHoja) {
    cout << "SI ENTRO A HOJA" << endl << endl << endl;
    NodoHoja* nodoAnterior = NULL;
    NodoHoja* nodoActual = paginaDondeSeColocaraElNodoHoja->listaDeNodosHoja->nodoHoja;
    bool seCambio = false;
    //averiguar tipo
    long val1 = convertirACodigoComparable(nodoActual->clave, tablaParaElemento);
    long val2 = convertirACodigoComparable(nuevoNodo->clave, tablaParaElemento);
    while (nodoActual->nodoHojaSiguiente != NULL) {
        val1 = convertirACodigoComparable(nodoActual->clave, tablaParaElemento);
        val2 = convertirACodigoComparable(nuevoNodo->clave, tablaParaElemento);
        if (val1 == val2) {//Se compara si ya existe el elemento
            cout << "No se puede INGRESAR VALOR CLAVE ES REPETIDO x1" << endl;
            acciones += "No se puede INGRESAR VALOR CLAVE ES REPETIDO en fila:" + to_string(numeroDeInicioDeAccion) + "\n";
            seCambio = true; //Ya qu existio error
            break;
        } else {
            if (val1 > val2) {//Cambio
                nuevoNodo->nodoHojaSiguiente = nodoActual;
                if (nodoAnterior == NULL) {
                    paginaDondeSeColocaraElNodoHoja->listaDeNodosHoja->nodoHoja = nuevoNodo;
                } else {
                    nodoAnterior->nodoHojaSiguiente = nuevoNodo;
                }
                //Valor ingresado correctamente
                seCambio = true;
                paginaDondeSeColocaraElNodoHoja->listaDeNodosHoja->cantidad++;
                break;
            } else {
                nodoAnterior = nodoActual;
                nodoActual = nodoActual->nodoHojaSiguiente;
            }
        }

    }
    if (!seCambio) {//Es ultimo o antepenultimo elemento de la lista
        val1 = convertirACodigoComparable(nodoActual->clave, tablaParaElemento);
        val2 = convertirACodigoComparable(nuevoNodo->clave, tablaParaElemento);
        if (val1 == val2) {//Se compara si ya existe el elemento
            cout << "No se puede INGRESAR VALOR CLAVE ES REPETIDO x2" << endl;
            acciones += "No se puede INGRESAR VALOR CLAVE ES REPETIDO en fila:" + to_string(numeroDeInicioDeAccion) + "\n";
        } else {
            if (val1 > val2) {//Ultimo nodo
                nuevoNodo->nodoHojaSiguiente = nodoActual;
                if (nodoAnterior == NULL) {
                    paginaDondeSeColocaraElNodoHoja->listaDeNodosHoja->nodoHoja = nuevoNodo;
                } else {
                    nodoAnterior->nodoHojaSiguiente = nuevoNodo;
                }
            } else {
                nodoActual->nodoHojaSiguiente = nuevoNodo;
            }
            //Valor ingresado correctamente
            paginaDondeSeColocaraElNodoHoja->listaDeNodosHoja->cantidad++;
        }

    }

}

Pagina* crearPagina(NodoHoja *nodoHoja, NodoInterno *nodoInterno, Pagina *padre) {
    Pagina* pagina = new Pagina;
    if (nodoHoja == NULL) {
        pagina->tipo = "interna";
        pagina->listaDeNodosHoja = NULL;
        pagina->listaDeNodosInternos = crearListaDeNodosInternos(nodoInterno);
    } else {
        pagina->tipo = "hoja";
        pagina->listaDeNodosHoja = crearListaDeNodosHoja(nodoHoja);
        pagina->listaDeNodosInternos = NULL;
    }
    pagina->padre = padre;
    pagina->paginaSiguiente = NULL;
    pagina->paginaAnterior =NULL;

}

Arbol* crearArbol(NodoHoja *nodoHoja, NodoInterno *nodoInterno, Pagina *padre) {
    Arbol* arbol = new Arbol;
    arbol->paginaRaiz = crearPagina(nodoHoja, nodoInterno, padre);
    return arbol;
}

/*------------------------------Acciones para lista de tablas--------------------------------------------*/
Lista* crearLista() {
    //Lista* lista = (Lista *) malloc(sizeof (Lista));
    Lista *lista = new Lista;
    lista->PrimerNodo = NULL;
    return lista;
}

void anadirTablaA_Lista(Lista* lista, Nodo* nodo) {
    if (lista->PrimerNodo == NULL) {
        lista->PrimerNodo = nodo;
    } else {
        Nodo* ultimoNodo = lista->PrimerNodo;
        while (ultimoNodo->siguiente != NULL) {
            ultimoNodo = ultimoNodo->siguiente;
        }
        ultimoNodo->siguiente = nodo;
    }
}

/*------------------------------Acciones para Nodos(Tipos de tabla)-------------------------*/
Nodo* crearNodo() {
    //Nodo* nodo = (Nodo*) malloc(sizeof (Nodo));
    Nodo *nodo = new Nodo;
    nodo->siguiente = NULL;
    nodo->atributoPrimero = NULL;
    nodo->arbol = NULL;
    //nodo->tabla=NULL;
    //nodo->clave=NULL;
    nodo->llavesForaneas = NULL;
    return nodo;
}

void anadirAtributoA_Nodo(Atributo* atributo, Nodo* nodo) {
    if (nodo->atributoPrimero == NULL) {
        nodo->atributoPrimero = atributo;
    } else {
        Atributo* atributoNext = nodo->atributoPrimero;
        while (atributoNext->atributoSiguiente != NULL) {
            atributoNext = atributoNext->atributoSiguiente;
        }
        atributoNext->atributoSiguiente = atributo;
    }
}

/*--------------------------------Acciones para atributos----------------------------------------*/
Atributo* crearAtributo(string tipo, string id) {
    //Atributo* atributo = (Atributo*) malloc(sizeof (Atributo));
    Atributo *atributo = new Atributo;
    atributo->tipo = tipo;
    atributo->id = id;
    atributo->atributoSiguiente = NULL;
    return atributo;
}

ElementoDeTabla* crearElementoTabla(string atributo, string valorDeAtributo) {
    //Atributo* atributo = (Atributo*) malloc(sizeof (Atributo));
    ElementoDeTabla *elementoTabla = new ElementoDeTabla;
    elementoTabla->atributo = atributo;
    elementoTabla->valorDeAtributo = valorDeAtributo;
    elementoTabla->valorSiguiente = NULL;
    return elementoTabla;
}
//-----------------------------------------------------Metodos auxiliares-----------------------------

bool buscarAtributo(string idClave) {//Busca un atributo para comparar con la clave de una estructura---->.xml
    Atributo* atributo = nodoActual->atributoPrimero; //Nodo q se esta creando por estructura.xml
    if (atributo != NULL) {
        while (atributo->atributoSiguiente != NULL) {
            if (atributo->id.compare(idClave) == 0) {
                return true;
            } else {
                atributo = atributo->atributoSiguiente;
            }
        }
        if (atributo->id.compare(idClave) == 0) {
            return true;
        }
    }
    return false;
}

Nodo* buscarTabla(string nombreDeTabla) {//Busca la tabla especificada para ver si es posible la creacion de un elemento--->.dat
    Nodo* tabla = listaDeTablas->PrimerNodo;
    if (tabla == NULL) {
        return NULL;
    } else {
        while (tabla->siguiente != NULL) {
            if (tabla->tabla.compare(nombreDeTabla) == 0) {
                return tabla;
            }
            tabla = tabla->siguiente;
        }
        if (tabla->tabla.compare(nombreDeTabla) == 0) {//Por si solo existe una tabla
            return tabla;
        }
    }
    return NULL; //No se encontro la tabla
}

void concatenarElementoDeTabla(ElementoDeTabla *nuevoElemento) {//Concatena los atributos de entrada.txt para luego anadirlo al arbol
    ElementoDeTabla* elementoNext = elementoDeTabla;
    while (elementoNext->valorSiguiente != NULL) {
        elementoNext = elementoNext->valorSiguiente;
    }
    elementoNext->valorSiguiente = nuevoElemento;
}

string buscarLlavePrimaria(string clave, ElementoDeTabla *elemento) {
    ElementoDeTabla *elementoNext = elemento;
    while (elementoNext->valorSiguiente != NULL) {
        if (elementoNext->atributo == clave) {
            return elementoNext->valorDeAtributo;
        }
        elementoNext = elementoNext->valorSiguiente;
    }
    if (elementoNext->atributo == clave) {
        return elementoNext->valorDeAtributo;
    }
    return "";
}

bool buscarAtributoEnTabla(Nodo* tabla, string atributoTexto, string tipo) {//Busca que exista el atributo en la tabla especifica y que coincida con el tipo para crear el atributo
    if (tabla == NULL) {
        return false;
    } else {
        Atributo* atributo = tabla->atributoPrimero;
        while (atributo->atributoSiguiente != NULL) {
            if (atributo->id.compare(atributoTexto) == 0) {
                if (atributo->tipo.compare("char") == 0) {
                    return true;
                } else if (atributo->tipo.compare(tipo) == 0) {
                    return true;
                }
            }
            atributo = atributo->atributoSiguiente;
        }
        if (atributo->id.compare(atributoTexto) == 0) {
            if (atributo->tipo.compare("char") == 0) {
                return true;
            } else if (atributo->tipo.compare(tipo) == 0) {
                return true;
            }
        }
        return false;
    }
}

Atributo* buscarAtributoEnTablaSinTipo(Nodo* tabla, string atributoTexto) {
    if (tabla == NULL) {
        return NULL;
    } else {
        Atributo* atributo = tabla->atributoPrimero;
        while (atributo->atributoSiguiente != NULL) {
            if (atributo->id.compare(atributoTexto) == 0) {
                return atributo;
            }
            atributo = atributo->atributoSiguiente;
        }
        if (atributo->id.compare(atributoTexto) == 0) {
            return atributo;
        }
        return NULL;
    }
}
//-----------------------------------------------------------ESTRUCTURA.XML-----------------------------------------

void analizarEstructura() {

    //Variables y constantes para lectura de archivo
    const string ficheroXml = "estructura.xml";
    char letra;
    string cadenaFormada = "";
    ifstream fichero;

    fichero.open(ficheroXml.c_str());
    if (!fichero.fail()) {
        fichero.get(letra);
        while (!fichero.eof()) {
            if (letra == '\n') {//Se ha formado una linea
                numeroDeFila++;
                analisisDeLineaParaEstructura(cadenaFormada);
                cadenaFormada = "";
            } else if (letra == '\t' || letra == '\r') {
                //Ignoramos
            } else {
                cadenaFormada += string(1, letra);
            }
            fichero.get(letra);
        }
        fichero.close();
    }
    cout << cadenaFormada << endl;

}

void analisisDeLineaParaEstructura(string linea) {
    if (linea.compare("<estructura>") == 0) {
        numeroDeInicioDeAccion = numeroDeFila;
        //Se crea el nodo
        nodoActual = crearNodo();
        cout << "Inicio de estructura" << endl;
    } else if (linea.compare("</estructura>") == 0) {
        //No existen errores si llego hasta aca
        //Se anade el nodo a lista
        if (laClaveExiste && !existeErrorEnLlavesForaneas) {
            cout << "Fin de estructura" << endl;
            laClaveExiste = false;
            existeErrorEnLlavesForaneas = false;
            if (buscarTabla(nodoActual->tabla) == NULL) {
                acciones += "Tabla creada:" + nodoActual->tabla + " fila:" + to_string(numeroDeInicioDeAccion) + "\n";
                anadirTablaA_Lista(listaDeTablas, nodoActual);
            } else {
                acciones += "La tabla ya existe y no se creo:" + nodoActual->tabla + " fila:" + to_string(numeroDeInicioDeAccion) + "\n";
            }

        } else {//Error ya que la clave no se creo
            laClaveExiste = false;
            existeErrorEnLlavesForaneas = false;
            acciones += "ERROR Tabla NO creada:" + nodoActual->tabla + " fila:" + to_string(numeroDeInicioDeAccion) + "\n";
        }
    } else if (linea.compare("") == 0) {
        // "LINEA VACIA" no se realizan acciones
    } else {
        analisisDeAtributo(linea);
    }

}

void analisisDeAtributo(string linea) {
    //estructura = <id1>tipo</id2>
    ListaDeNombreDeTabla *listaDeNombresDeTabla = NULL;
    string estructuraFormada = "";
    string id1 = "";
    //string id2 = "";
    string tipo = "";
    //Para llaves foraneas
    int contadorDeSignosMenores = 0;
    string nombreDeTablasParaLlavesForaneas; //Para crearlas se debe verificar que la tablas existan
    bool esTipo = false;
    bool esSegundoId = false;
    for (int i = 0; i < linea.size(); i++) {
        char caracter = linea.at(i);
        switch (caracter) {
            case '<':
                contadorDeSignosMenores++;
                estructuraFormada += caracter;
                break;
            case '>':
                estructuraFormada += caracter;
                esTipo = true;
                if (contadorDeSignosMenores >= 3) {
                    if (contadorDeSignosMenores == 3) {
                        listaDeNombresDeTabla = crearListaDeNombresDeTabla(crearNombreDeTabla(nombreDeTablasParaLlavesForaneas));
                    } else {
                        anadirNombreDeTabla(listaDeNombresDeTabla, crearNombreDeTabla(nombreDeTablasParaLlavesForaneas));
                    }
                    nombreDeTablasParaLlavesForaneas = "";
                }
                break;
            case '/':
                estructuraFormada += caracter;
                esTipo = false;
                esSegundoId = true;
                break;
            default://Es una letra
                estructuraFormada += caracter;
                if (contadorDeSignosMenores >= 3) {
                    nombreDeTablasParaLlavesForaneas += caracter;
                } else if (esTipo) {
                    tipo += caracter;
                } else if (esSegundoId) {
                    id1 += caracter;
                }
                break;
        }
    }
    //Verificar que esas tablas existan y el atributo mencionado exista en la tabla

    //For para esto y una variable de error 

    bool seEncontro = false;
    if (listaDeNombresDeTabla != NULL) {//Verificando que las tablas existan para las llaves foraneas
        NombreDeTabla *nombre = listaDeNombresDeTabla->primerNombre;
        if (nombre != NULL) {
            while (nombre->siguiente != NULL) {
                if (!buscarAtributoEnTabla(buscarTabla(nombre->nombreTabla), id1, tipo)) {
                    existeErrorEnLlavesForaneas = true;
                    seEncontro = true;
                    break;
                }
                nombre = nombre->siguiente;
            }
            if (!seEncontro) {
                if (!buscarAtributoEnTabla(buscarTabla(nombre->nombreTabla), id1, tipo)) {
                    existeErrorEnLlavesForaneas = true;
                }
            }
        }
        //Si no ha existido error en las llaves foraneas las llena
        Nodo* nodoAc = nodoActual;
        if (!existeErrorEnLlavesForaneas) {
            if (nodoActual->llavesForaneas == NULL) {
                nodoActual->llavesForaneas = crearListaDeLlavesForaneas();
            }
            NombreDeTabla *nombre2 = listaDeNombresDeTabla->primerNombre;
            while (nombre2->siguiente != NULL) {
                anadirLlaveForanea(nodoActual->llavesForaneas, crearLlaveForanea(buscarTabla(nombre2->nombreTabla), id1));
                nombre2 = nombre2->siguiente;
            }
            anadirLlaveForanea(nodoActual->llavesForaneas, crearLlaveForanea(buscarTabla(nombre2->nombreTabla), id1));
        }
    }





    if (id1.compare("tabla") == 0) {//Es tabla,solo se le actualiza la tabla al nodo actual
        cout << "ES TABLA" << endl;
        nodoActual->tabla = tipo;
    } else if (id1.compare("clave") == 0) {//Es clave
        cout << "ES CLAVE" << endl;
        //Buscar si se ha definido anteriormente la clave
        //Buscar un atributo en el nodo tal que su id coincida con tipo de clave
        if (buscarAtributo(tipo)) {
            nodoActual->clave = tipo; //Se le asigna la clave a la tabla
            laClaveExiste = true;
        }
    } else {//Es un atributo cualquiera
        cout << "ES ATRIBUTO" << endl;
        Atributo *nuevoAtributo = crearAtributo(tipo, id1);
        anadirAtributoA_Nodo(crearAtributo(tipo, id1), nodoActual);
    }


}

//---------------------------------------------------------------------ENTRADA.DAT-------------------------------------------------

void analizarEntrada() {
    //Variables y constantes para lectura de archivo
    const string ficheroXml = "entrada.txt";
    char letra;
    string cadenaFormada = "";
    ifstream fichero;

    fichero.open(ficheroXml.c_str());
    if (!fichero.fail()) {
        fichero.get(letra);
        while (!fichero.eof()) {
            if (letra == '\n') {//Se ha formado una linea
                numeroDeFila++;
                cout << "NUMERO DE FILA------------------------------->" << numeroDeFila << endl;
                analisisDeLineaParaEntrada(cadenaFormada);
                cadenaFormada = "";
            } else if (letra == '\t' || letra == '\r') {
                //Ignoramos
            } else {
                cadenaFormada += string(1, letra);
            }
            fichero.get(letra);
        }
        fichero.close();
    }
    cout << cadenaFormada << endl;
}

void analisisDeLineaParaEntrada(string linea) {
    string atributo = "";
    string valorDeAtributo = "";
    bool esValorDeAtributo = false;
    bool seLlenoAtributo = false;

    if (linea.compare("") == 0) {
        // "LINEA VACIA" no se realizan acciones
    } else {
        int numeroDeSignoMenor = 0;
        int numeroDeDiagonal = 0;

        for (int i = 0; i < linea.size(); i++) {//S busca llenar los valores de TipoDeAtribiuto y su valor
            char caracter = linea.at(i);
            switch (caracter) {
                case '<':
                    numeroDeSignoMenor++;
                    break;
                case '>':
                    //estructuraFormada += caracter;
                    esValorDeAtributo = true;
                    break;
                case '/':
                    //estructuraFormada += caracter;
                    esValorDeAtributo = false;
                    seLlenoAtributo = true;
                    numeroDeDiagonal++;
                default://Es una letra
                    //estructuraFormada += caracter;
                    if (esValorDeAtributo) {
                        valorDeAtributo += caracter;
                    } else if (!seLlenoAtributo) {
                        atributo += caracter;
                    }
                    break;

            }
        }
        if (numeroDeSignoMenor == 1 && numeroDeDiagonal == 0) {//Es inicio de Introduccion de datos
            //Se tiene el nombre de la tabla asi q se busca,de no encontrarla se manda un error
            numeroDeInicioDeAccion = numeroDeFila;
            cout << "Tabla:" << atributo << " ";
            tablaParaElemento = buscarTabla(atributo);
            if (tablaParaElemento == NULL) {//Si no se encuentra la tabla
                existeErrorAlCrearElementoDeTabla = true;
            }
        } else if (numeroDeSignoMenor == 1 && numeroDeDiagonal == 1) {//Es fin de Introduccion de datos
            //Si no hubo errores se asigna al arbol, de lo contrario se manda error
            if (!existeErrorAlCrearElementoDeTabla) {//Los atributos y tabla existen
                //Verificar si trae el dato obligatorio, clave
                if (buscarLlavePrimaria(tablaParaElemento->clave, elementoDeTabla).compare("") != 0) {//Se crea el elemento tabla y se llama al arbol
                    cout << "CREADO;;;;;;;;;;;;;" << endl;
                    insertarEnArbol(elementoDeTabla); //Se incerta el elemento en el arbol de la tabla
                    /*
                 Acciones del arbol
                     * 
                     * 
                     * 
                     * 
                     * 
                     * 
                     * 
                     * 
                     * 
                     * 
                     * 
                     */


                } else {
                    acciones += "No se ha creado el elemento ya que no contiene la clave elemento en fila:" + to_string(numeroDeInicioDeAccion) + "\n";
                    //mensaje para accion de que no cuenta con la clave
                }
                existeErrorAlCrearElementoDeTabla = false;
                elementoDeTabla = NULL; //Al terminar de agregar al arbol se hace nulo
                tablaParaElemento = NULL;
            } else {
                existeErrorAlCrearElementoDeTabla = false; //Se reinicia para la proxima tabla
                elementoDeTabla = NULL;
                tablaParaElemento = NULL;
                acciones += "No se ha creado el elemento ya que no existe la tabla o por problemas de atributos mal especificados fila:" + to_string(numeroDeInicioDeAccion) + "\n";
            }
            cout << "Tabla Fin:" << atributo << endl;
        } else {//Es atributo y su valor
            //Buscar si el atributo pertenecen a la tabla
            //Si pertenece anadirlo a atributo
            cout << "Atributo:" << atributo << " ValorAtributo:" << valorDeAtributo << endl;
            if (tablaParaElemento != NULL) {
                string tipo = "int";
                for (int i = 0; i < valorDeAtributo.size(); i++) {
                    if (!isdigit(valorDeAtributo.at(i))) {
                        tipo = "char";
                    }
                }
                cout << "TIPO:" << tipo << endl;
                if (buscarAtributoEnTabla(tablaParaElemento, atributo, tipo)) {//Se busca si el atributo introducido es parte de la tabla
                    if (elementoDeTabla == NULL) {
                        elementoDeTabla = crearElementoTabla(atributo, valorDeAtributo); //Creamos el primer elemto tabla
                    } else {
                        ElementoDeTabla *nuevoElemento = crearElementoTabla(atributo, valorDeAtributo);
                        concatenarElementoDeTabla(nuevoElemento);
                        cout << "SE CONCATENO" << endl;
                    }
                } else {
                    existeErrorAlCrearElementoDeTabla = true;
                }

            }

            /*            
                        if (primerAtributoDeElementoTabla == NULL) {
                            buscarAtributo(atributo);
                        }*/
        }

    }



}

//----------------------------------------------------------Reportes----------------------------------------------------------

void analizarReportes() {
    //Variables y constantes para lectura de archivo
    const string ficheroXml = "reportes.txt";
    char letra;
    string cadenaFormada = "";
    ifstream fichero;
    Pagina * paginaRaiz = listaDeTablas->PrimerNodo->arbol->paginaRaiz;


    fichero.open(ficheroXml.c_str());
    if (!fichero.fail()) {
        fichero.get(letra);
        while (!fichero.eof()) {
            if (letra == '\n') {//Se ha formado una linea
                numeroDeFila++;
                analisisDeLineaParaReporte(cadenaFormada);
                cadenaFormada = "";
            } else if (letra == '\t' || letra == '\r') {
                //Ignoramos
            } else {
                cadenaFormada += string(1, letra);
            }
            fichero.get(letra);
        }
        fichero.close();
    }
    cout << cadenaFormada << endl;
}

void analisisDeLineaParaReporte(string cadenaFormada) {
    if (cadenaFormada.compare("<reporte>") == 0) {//Es inicio de reporte
        cout << "********************--------------------*********************-----Reportes****************************-------------------------" << endl;

    } else if (cadenaFormada.compare("</reporte>") == 0) {//Es fin de reporte

    } else if (cadenaFormada.compare("") == 0) {//Linea vacia por lo tanto no se realizan acciones 

    } else {//Linea de acciones
        analisisDeReporte(cadenaFormada);
    }
}

void analisisDeReporte(string linea) {
    ListaDeNombreDeTabla *listaDeNombresTablas = NULL;
    string nombreDeAtributo = "";
    string nombreDeTabla = "";
    int numeroDeSignoMenor = 0;
    int numeroDeSignoMayor = 0;
    int numeroDeComa = 0;
    for (int i = 0; i < linea.size(); i++) {
        char caracter = linea.at(i);
        switch (caracter) {
            case '<':
                numeroDeSignoMenor++;
                break;
            case '>':
                //No se concatena
                //Pero si se anade a lista de tablas, si numeroDeSignoMenor==1
                if (numeroDeSignoMenor == 1) {
                    numeroDeSignoMayor++;
                    if (numeroDeComa == 0) {//Solo hay una tabla
                        listaDeNombresTablas = crearListaDeNombresDeTabla(crearNombreDeTabla(nombreDeTabla));
                    } else {
                        anadirNombreDeTabla(listaDeNombresTablas, crearNombreDeTabla(nombreDeTabla));
                    }
                }
                break;
            case '/':
                //No se realizan acciones
                break;
            case ',':
                if (numeroDeSignoMenor == 1) {
                    numeroDeComa++;
                    if (numeroDeComa == 1) {//existiran mas tablas
                        listaDeNombresTablas = crearListaDeNombresDeTabla(crearNombreDeTabla(nombreDeTabla));
                    } else {//enesima tabla
                        anadirNombreDeTabla(listaDeNombresTablas, crearNombreDeTabla(nombreDeTabla));
                    }
                    nombreDeTabla = "";
                }
                break;
            default://Es una letra
                if (numeroDeSignoMayor == 1 && numeroDeSignoMenor == 1) {
                    nombreDeAtributo += caracter;
                } else if (numeroDeSignoMenor == 1) {//Solo nombres de tablas
                    nombreDeTabla += caracter;
                }
                break;
        }

    }

    NombreDeTabla* nmTabla = listaDeNombresTablas->primerNombre;
    Nodo * tabla = buscarTabla(nmTabla->nombreTabla);
    Atributo *atributo = buscarAtributoEnTablaSinTipo(tabla, nombreDeAtributo);
    cout << "********************--------------------*********************-----Reportes****************************-------------------------" << endl;
    cout << "NOMBRE DE TABLA:" << nmTabla->nombreTabla << endl;
    cout << "ATRIBUTO:" << nombreDeAtributo << endl;

    mostrarDatosDePagina(buscarPaginaMasALaIzquierda(listaDeTablas->PrimerNodo->arbol->paginaRaiz));

    if (atributo != NULL && nmTabla != NULL) {//Existe la tabla y el atributo
        //Obtener todos los elementos de la tabla
        //Ordenarlos segun el atributo que nos indica el reporte
        Pagina *primerPagina = buscarPaginaMasALaIzquierda(tabla->arbol->paginaRaiz);

    } else {
        //Indicar que no es posible crear el reporte
    }
}

void mostrarDatosDePagina(Pagina* primerPagina) {
    Pagina *raiz = listaDeTablas->PrimerNodo->arbol->paginaRaiz;
    string nombreDeTabla = listaDeTablas->PrimerNodo->tabla;
    Pagina *paginaMasALAIzquierda = buscarPaginaMasALaIzquierda(listaDeTablas->PrimerNodo->arbol->paginaRaiz);
    while (primerPagina->paginaSiguiente != NULL) {
        NodoHoja *nodoHoja = primerPagina->listaDeNodosHoja->nodoHoja;
        while (nodoHoja->nodoHojaSiguiente != NULL) {
            ElementoDeTabla *elemento = nodoHoja->elementoDeTabla;
            cout << "Nuevo ELEMENETO DE TABLA" << endl;
            while (elemento != NULL) {
                cout << "Atributo:" << elemento->atributo << endl;
                cout << "ValorDeAtributo:" << elemento->valorDeAtributo << endl;
                elemento = elemento->valorSiguiente;
            }
            //Para el ultimo elemento
            //cout << "Atributo:" << elemento->atributo << endl;
            //cout << "ValorDeAtributo:" << elemento->valorDeAtributo << endl;
            nodoHoja = nodoHoja->nodoHojaSiguiente;
        }
        //Para el ultimo nodoHoja
        ElementoDeTabla *elemento = nodoHoja->elementoDeTabla;
        cout << "Nuevo ELEMENETO DE TABLA" << endl;
        while (elemento != NULL) {
            cout << "Atributo:" << elemento->atributo << endl;
            cout << "ValorDeAtributo:" << elemento->valorDeAtributo << endl;
            elemento = elemento->valorSiguiente;
        }
        //cout << "Atributo:" << elemento->atributo << endl;
        //cout << "ValorDeAtributo:" << elemento->valorDeAtributo << endl;
        //nodoHoja = nodoHoja->nodoHojaSiguiente;
        primerPagina = primerPagina->paginaSiguiente;
    }

    //Para cuando solo sea un pagina o es la ultima pagina
    NodoHoja *nodoHoja = primerPagina->listaDeNodosHoja->nodoHoja;
    while (nodoHoja->nodoHojaSiguiente != NULL) {
        ElementoDeTabla *elemento = nodoHoja->elementoDeTabla;
        cout << "Nuevo ELEMENETO DE TABLA" << endl;
        while (elemento != NULL) {
            cout << "Atributo:" << elemento->atributo << endl;
            cout << "ValorDeAtributo:" << elemento->valorDeAtributo << endl;
            elemento = elemento->valorSiguiente;
        }
        //Ultimo elemento de nodo hoja
        //cout << "Atributo:" << elemento->atributo << endl;
        //cout << "ValorDeAtributo:" << elemento->valorDeAtributo << endl;
        nodoHoja = nodoHoja->nodoHojaSiguiente;
    }
    //Para el ultimo nodoHoja
    ElementoDeTabla *elemento = nodoHoja->elementoDeTabla;
    cout << "Nuevo ELEMENETO DE TABLA" << endl;
    while (elemento != NULL) {
        cout << "Atributo:" << elemento->atributo << endl;
        cout << "ValorDeAtributo:" << elemento->valorDeAtributo << endl;
        elemento = elemento->valorSiguiente;
    }
    //cout << "Atributo:" << elemento->atributo << endl;
    //cout << "ValorDeAtributo:" << elemento->valorDeAtributo << endl;
    //nodoHoja = nodoHoja->nodoHojaSiguiente;
    //primerPagina = primerPagina->paginaSiguiente;


}

Pagina* buscarPaginaMasALaIzquierda(Pagina* partida) {
    if (partida->tipo.compare("hoja") == 0) {//Solo existe la pagina raiz
        return partida;
    } else {
        partida = partida->listaDeNodosInternos->nodoInterno->hijoIzquierdo;
        return buscarPaginaMasALaIzquierda(partida);
    }
}

//-----------------------------------------------------ESCRITURA DE ARCHIVOS-----------------------------------------

void escribirEnArchivo(char* direccion, string mensaje) {
    ofstream file;
    file.open(direccion);
    file << mensaje << endl;
    file.close();
}

void anadirTextoA_Archivo(string fichero, string unString) {
    fstream fileAdd;
    fileAdd.open(fichero.c_str(), ios::app);
    fileAdd << unString << endl;
    fileAdd.close();
}

//----------------------------------------------------------ARBOLES-------------------------------------------------------------

NodoHoja* buscarNodoHoja(Pagina* paginaDeNodo, int posicion) {
    int contador = 1;
    NodoHoja * nodo = paginaDeNodo->listaDeNodosHoja->nodoHoja;
    while (nodo != NULL) {
        if (contador == posicion) {
            return nodo;
        }
        nodo = nodo->nodoHojaSiguiente;
        contador++;
    }
    return nodo;
}

NodoInterno* buscarNodoInterno(Pagina* paginaDeNodo, int posicion) {
    int contador = 1;
    NodoInterno * nodo = paginaDeNodo->listaDeNodosInternos->nodoInterno;
    while (nodo != NULL) {
        if (contador == posicion) {
            return nodo;
        }
        nodo = nodo->nodoInternoSiguiente;
        contador++;
    }
    return nodo;
}

void insertarEnArbol(ElementoDeTabla *elementoDeTabla) {
    acciones += "Se ha creado ele elemento de fila:" + to_string(numeroDeInicioDeAccion) + "\n";
    if (tablaParaElemento->arbol == NULL) {//No se cuente con arbol, por lo tanto es el primer dato
        tablaParaElemento->arbol = crearArbol(crearNodoHoja(elementoDeTabla, tablaParaElemento), NULL, NULL); //NULL1=nodoInterno, NULL2 padre
        imprimirElementosDeTbla(tablaParaElemento);
    } else {//Ya se cuenta con el arbol asi que se ingresar el enesimo dato
        cout << "NUEVO ELEMNTO #2" << endl << endl << endl;
        Pagina* paginaRaiz = tablaParaElemento->arbol->paginaRaiz;
        if (paginaRaiz->tipo.compare("hoja") == 0) {
            anadirNodoHojaA_Arbol(crearNodoHoja(elementoDeTabla, tablaParaElemento), paginaRaiz);
            imprimirElementosDeTbla(tablaParaElemento);
            //Verificar que la pagina siga cumpliendo con su tamano
            if (paginaRaiz->listaDeNodosHoja->cantidad == 5) {
                //Restructurar el arbol
                //Conseguir los 5 nodos
                NodoHoja* nodo1 = crearNodoHoja(buscarNodoHoja(paginaRaiz, 1)->elementoDeTabla, tablaParaElemento);
                NodoHoja* nodo2 = crearNodoHoja(buscarNodoHoja(paginaRaiz, 2)->elementoDeTabla, tablaParaElemento);
                NodoHoja* nodo3 = crearNodoHoja(buscarNodoHoja(paginaRaiz, 3)->elementoDeTabla, tablaParaElemento);
                NodoHoja* nodo4 = crearNodoHoja(buscarNodoHoja(paginaRaiz, 4)->elementoDeTabla, tablaParaElemento);
                NodoHoja* nodo5 = crearNodoHoja(buscarNodoHoja(paginaRaiz, 5)->elementoDeTabla, tablaParaElemento);
                //Se toman los primeros dos elementos del arbol para crear la pagina  Izquierdo
                Pagina *paginaIzquierda = crearPagina(nodo1, NULL, NULL);
                anadirNodoHojaA_Arbol(nodo2, paginaIzquierda);
                //Se toman los siguientes tres elementos del arbol para crear el nodoHojaDerecho
                Pagina *paginaDerecha = crearPagina(nodo3, NULL, NULL);
                anadirNodoHojaA_Arbol(nodo4, paginaDerecha);
                anadirNodoHojaA_Arbol(nodo5, paginaDerecha);
                //Se anade la pagina Siguiente
                paginaIzquierda->paginaSiguiente = paginaDerecha;
                paginaDerecha->paginaAnterior=paginaIzquierda;
                //Tomar el tercer elemento de la pagina y con la clave de este crear un nodo pagina anadiendo sus hijos Izquierdo y derecho
                NodoInterno* nodoInternoRaiz = crearNodoInterno(paginaIzquierda, paginaDerecha, nodo3->clave);
                Pagina *nuevaPaginaRaiz = crearPagina(NULL, nodoInternoRaiz, NULL);
                nuevaPaginaRaiz->tipo = "raiz";
                paginaIzquierda->padre = nuevaPaginaRaiz;
                paginaDerecha->padre = nuevaPaginaRaiz;
                //Cambiar la raiz del arbol
                tablaParaElemento->arbol->paginaRaiz = nuevaPaginaRaiz;
                //Eliminar paginaRaiz ya que se han tomado todos los elementos de alli 
                paginaIzquierda->paginaSiguiente = paginaDerecha;
                paginaDerecha->paginaSiguiente = NULL;
                //
                delete paginaRaiz;
                //cout << "LLEGO A 5" << endl;
                //cout << "---------------------------------------------------------------------------------->" << nuevaPaginaRaiz->listaDeNodosInternos->nodoInterno->hijoIzquierdo->listaDeNodosHoja->nodoHoja->clave << endl;
                //cout << "Llego a 5" << endl;
            }
        } else {//Toca moverse por indices y llegar a la hoja
            NodoInterno *nodoInterno = paginaRaiz->listaDeNodosInternos->nodoInterno;
            cout << "ANTES DE INICIAR BUSQUEDA DE PAGINA DE INSSERCION RAIZ EN SU PRIEMER ELMENTO ES:" << nodoInterno->clave << endl;
            Pagina* paginaDeInsercion = buscarPaginaDeInsercion(nodoInterno, elementoDeTabla->valorDeAtributo); //Pagina hoja
            cout << elementoDeTabla->valorDeAtributo << endl;
            //cout << "Clave antes:" << paginaDeInsercion->listaDeNodosHoja->nodoHoja->clave << endl;
            restructurarArbol(paginaDeInsercion, elementoDeTabla);
            //cout << "Clave despues:" << paginaDeInsercion->listaDeNodosHoja->nodoHoja->clave << endl;

        }

    }
}

//Componer esta madre, como anadirNodoHoja

void anadirNodoInterno(Pagina* pagina, NodoInterno* nuevoNodo) {
    bool seCambio = false;
    NodoInterno* nodoAnterior = NULL;
    NodoInterno *nodoActual = pagina->listaDeNodosInternos->nodoInterno;
    long val1 = convertirACodigoComparable(nodoActual->clave, tablaParaElemento);
    long val2 = convertirACodigoComparable(nuevoNodo->clave, tablaParaElemento);
    while (nodoActual->nodoInternoSiguiente != NULL) {//Cambio
        val1 = convertirACodigoComparable(nodoActual->clave, tablaParaElemento);
        val2 = convertirACodigoComparable(nuevoNodo->clave, tablaParaElemento);
        if (val1> val2) {
            nuevoNodo->nodoInternoSiguiente = nodoActual; //Se cambia
            nodoActual->hijoIzquierdo = nuevoNodo->hijoDerecho; //Se cambian los apuntadores
            if (nodoAnterior == NULL) {//SI era primero de lista
                pagina->listaDeNodosInternos->nodoInterno = nuevoNodo;
            } else {//Si no lo era
                nodoAnterior->nodoInternoSiguiente = nuevoNodo;
                nodoAnterior->hijoDerecho = nuevoNodo->hijoIzquierdo; //Se cambia el apuntador
            }
            //Valor ingresado correctamente
            seCambio = true;
            pagina->listaDeNodosInternos->cantidad++;
            break;
        } else {
            nodoAnterior = nodoActual;
            nodoActual = nodoActual->nodoInternoSiguiente;
        }
    }
    if (!seCambio) {
        val1 = convertirACodigoComparable(nodoActual->clave, tablaParaElemento);
        val2 = convertirACodigoComparable(nuevoNodo->clave, tablaParaElemento);
        if (val1 >val2) {
            nuevoNodo->nodoInternoSiguiente = nodoActual; //Se cambia
            nodoActual->hijoIzquierdo = nuevoNodo->hijoDerecho; //Se cambian los apuntadores
            if (nodoAnterior == NULL) {//SI era primero de lista
                pagina->listaDeNodosInternos->nodoInterno = nuevoNodo;
            } else {//Si no lo era
                nodoAnterior->nodoInternoSiguiente = nuevoNodo;
                nodoAnterior->hijoDerecho = nuevoNodo->hijoIzquierdo; //Se cambia el apuntador
            }
            //Valor ingresado correctamente
        } else {//Se ingresara en la ultima posicion
            nodoActual->nodoInternoSiguiente = nuevoNodo;
            nodoActual->hijoDerecho = nuevoNodo->hijoIzquierdo;
        }
        pagina->listaDeNodosInternos->cantidad++;
    }
}

void restructurarArbol(Pagina *pagina, ElementoDeTabla *elemento) {
    if (pagina->tipo.compare("hoja") == 0) {
        anadirNodoHojaA_Arbol(crearNodoHoja(elemento, tablaParaElemento), pagina); //Si no lo anadio su cantidad permanecera igual por lo que el metodo termina
        if (pagina->listaDeNodosHoja->cantidad == 5) {
            Pagina *paginaPadre = pagina->padre; //Padre para reestructurarlo
            //Nodos
            NodoHoja* nodo1 = crearNodoHoja(buscarNodoHoja(pagina, 1)->elementoDeTabla, tablaParaElemento);
            NodoHoja* nodo2 = crearNodoHoja(buscarNodoHoja(pagina, 2)->elementoDeTabla, tablaParaElemento);
            NodoHoja* nodo3 = crearNodoHoja(buscarNodoHoja(pagina, 3)->elementoDeTabla, tablaParaElemento);
            NodoHoja* nodo4 = crearNodoHoja(buscarNodoHoja(pagina, 4)->elementoDeTabla, tablaParaElemento);
            NodoHoja* nodo5 = crearNodoHoja(buscarNodoHoja(pagina, 5)->elementoDeTabla, tablaParaElemento);
            //Se toman los primeros dos elementos del arbol para crear la pagina  Izquierdo
            Pagina *paginaIzquierda = crearPagina(nodo1, NULL, paginaPadre);
            anadirNodoHojaA_Arbol(nodo2, paginaIzquierda);
            //Se toman los siguientes tres elementos del arbol para crear el nodoHojaDerecho
            Pagina *paginaDerecha = crearPagina(nodo3, NULL, paginaPadre);
            anadirNodoHojaA_Arbol(nodo4, paginaDerecha);
            anadirNodoHojaA_Arbol(nodo5, paginaDerecha);
            //Se anade la pagina Siguiente
            paginaIzquierda->paginaSiguiente = paginaDerecha;
            //Pagina derecha debe apuntar a la anterior pagina siguiente
            paginaDerecha->paginaSiguiente = pagina->paginaSiguiente;
            paginaDerecha->paginaAnterior=paginaIzquierda;
            //Se anade al padre
            if(pagina->paginaAnterior!=NULL){//Tenia pagina anterior
                pagina->paginaAnterior->paginaSiguiente=paginaIzquierda;
                paginaIzquierda->paginaAnterior=pagina->paginaAnterior;
            }
            if(pagina->paginaSiguiente!=NULL){
                pagina->paginaSiguiente->paginaAnterior=paginaDerecha;
            }



            anadirNodoInterno(paginaPadre, crearNodoInterno(paginaIzquierda, paginaDerecha, nodo3->clave));
            delete pagina;
            //Luego se manda a llamar al metodo para verificar q el tamano no este excedido
            restructurarArbol(paginaPadre, NULL);
        }
    } else if (pagina->tipo.compare("interna") == 0) {
        //imprimirElementosDeTbla(tablaParaElemento);
        if (pagina->listaDeNodosInternos->cantidad == 5) {
            Pagina *paginaPadre = pagina->padre; //Padre para reestructurarlo

            //Se divide

            NodoInterno *nodo1 = crearNodoInterno(buscarNodoInterno(pagina, 1)->hijoIzquierdo, buscarNodoInterno(pagina, 1)->hijoDerecho, buscarNodoInterno(pagina, 1)->clave);
            NodoInterno *nodo2 = crearNodoInterno(buscarNodoInterno(pagina, 2)->hijoIzquierdo, buscarNodoInterno(pagina, 2)->hijoDerecho, buscarNodoInterno(pagina, 2)->clave);
            NodoInterno *nodo4 = crearNodoInterno(buscarNodoInterno(pagina, 4)->hijoIzquierdo, buscarNodoInterno(pagina, 4)->hijoDerecho, buscarNodoInterno(pagina, 4)->clave);
            NodoInterno *nodo5 = crearNodoInterno(buscarNodoInterno(pagina, 5)->hijoIzquierdo, buscarNodoInterno(pagina, 5)->hijoDerecho, buscarNodoInterno(pagina, 5)->clave);

            //Pagina izquierda
            Pagina *paginaIzquierda = crearPagina(NULL, nodo1, paginaPadre);
            anadirNodoInterno(paginaIzquierda, nodo2);
            //Pagina derechanodo3
            Pagina *paginaDerecha = crearPagina(NULL, nodo4, paginaPadre);
            anadirNodoInterno(paginaDerecha, nodo5);
            //Nodo que subira
            NodoInterno *nodo3 = crearNodoInterno(paginaIzquierda, paginaDerecha, buscarNodoInterno(pagina, 3)->clave);
            //NOdo 3 no tiene padre
            anadirNodoInterno(paginaPadre, nodo3);
            nodo1->hijoDerecho->padre = paginaIzquierda;
            nodo1->hijoIzquierdo->padre = paginaIzquierda;

            nodo2->hijoDerecho->padre = paginaIzquierda;
            nodo2->hijoIzquierdo->padre = paginaIzquierda;



            nodo4->hijoDerecho->padre = paginaDerecha;
            nodo4->hijoIzquierdo->padre = paginaDerecha;

            nodo5->hijoDerecho->padre = paginaDerecha;
            nodo5->hijoIzquierdo->padre = paginaDerecha;


            delete pagina;
            restructurarArbol(paginaPadre, NULL);



            //Luego se manda a llamar al metodo para verificar que el tamano no este excedido

        }
    } else {//De tipo raiz
        //imprimirElementosDeTbla(tablaParaElemento);
        if (pagina->listaDeNodosInternos->cantidad == 5) {
            //Se divide
            NodoInterno *nodo1 = crearNodoInterno(buscarNodoInterno(pagina, 1)->hijoIzquierdo, buscarNodoInterno(pagina, 1)->hijoDerecho, buscarNodoInterno(pagina, 1)->clave);
            NodoInterno *nodo2 = crearNodoInterno(buscarNodoInterno(pagina, 2)->hijoIzquierdo, buscarNodoInterno(pagina, 2)->hijoDerecho, buscarNodoInterno(pagina, 2)->clave);
            NodoInterno *nodo4 = crearNodoInterno(buscarNodoInterno(pagina, 4)->hijoIzquierdo, buscarNodoInterno(pagina, 4)->hijoDerecho, buscarNodoInterno(pagina, 4)->clave);
            NodoInterno *nodo5 = crearNodoInterno(buscarNodoInterno(pagina, 5)->hijoIzquierdo, buscarNodoInterno(pagina, 5)->hijoDerecho, buscarNodoInterno(pagina, 5)->clave);
            //Se cambia la raiz y termina el metodo

            //Pagina izquierda
            Pagina *paginaIzquierda = crearPagina(NULL, nodo1, NULL);
            anadirNodoInterno(paginaIzquierda, nodo2);

            //Pagina derechanodo3
            Pagina *paginaDerecha = crearPagina(NULL, nodo4, NULL);
            anadirNodoInterno(paginaDerecha, nodo5);

            //Nodo que subira
            NodoInterno *nodo3 = crearNodoInterno(paginaIzquierda, paginaDerecha, buscarNodoInterno(pagina, 3)->clave);
            //
            Pagina *nuevaPaginaRaiz = crearPagina(NULL, nodo3, NULL);
            nuevaPaginaRaiz->tipo = "raiz";
            paginaIzquierda->padre = nuevaPaginaRaiz;
            paginaDerecha->padre = nuevaPaginaRaiz;
            //Cambiar la raiz del arbol
            tablaParaElemento->arbol->paginaRaiz = nuevaPaginaRaiz;

            nodo1->hijoDerecho->padre = paginaIzquierda;
            nodo1->hijoIzquierdo->padre = paginaIzquierda;

            nodo2->hijoDerecho->padre = paginaIzquierda;
            nodo2->hijoIzquierdo->padre = paginaIzquierda;



            nodo4->hijoDerecho->padre = paginaDerecha;
            nodo4->hijoIzquierdo->padre = paginaDerecha;

            nodo5->hijoDerecho->padre = paginaDerecha;
            nodo5->hijoIzquierdo->padre = paginaDerecha;



            delete pagina;
        }
    }
}

Pagina *buscarPaginaDeInsercion(NodoInterno *partida, string claveBuscada) {
    Pagina* posiblePaginaHoja = NULL;
    long val1 = convertirACodigoComparable(partida->clave, tablaParaElemento);
    long val2 = convertirACodigoComparable(claveBuscada, tablaParaElemento);
    while (partida->nodoInternoSiguiente != NULL) {
        val1 = convertirACodigoComparable(partida->clave, tablaParaElemento);
        val2 = convertirACodigoComparable(claveBuscada, tablaParaElemento);
        if (val1 > val2) {
            posiblePaginaHoja = partida->hijoIzquierdo;
            if (posiblePaginaHoja->tipo.compare("hoja") == 0) {
                return posiblePaginaHoja;
            } else {
                return buscarPaginaDeInsercion(posiblePaginaHoja->listaDeNodosInternos->nodoInterno, claveBuscada);
            }
        } else if (val1 <= val2) {//Pasamos al siguiente elemento q  no sera nulo
            partida = partida->nodoInternoSiguiente;
        }
    }
    //Llegamos al final del arreglo, queda el ultimo elemento que apunta a null
    val1 = convertirACodigoComparable(partida->clave, tablaParaElemento);
    val2 = convertirACodigoComparable(claveBuscada, tablaParaElemento);
    if (val1 > val2) {
        posiblePaginaHoja = partida->hijoIzquierdo;

    } else if (val1 <= val2) {//Pasamos al siguiente elemento q  no sera nulo
        posiblePaginaHoja = partida->hijoDerecho;
    }
    //Verifico si es hoja
    if (posiblePaginaHoja->tipo.compare("hoja") == 0) {
        return posiblePaginaHoja;
    } else {
        return buscarPaginaDeInsercion(posiblePaginaHoja->listaDeNodosInternos->nodoInterno, claveBuscada);
    }

}

void eliminarDatoDeArbol(Nodo* tabla, Pagina *pagina) {

}

