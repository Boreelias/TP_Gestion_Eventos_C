#ifndef EVENTO_H_INCLUDED
#define EVENTO_H_INCLUDED

#include "estructuras.h"

int existeEvento(const char *idBuscado);
int obtenerEventoPorId(const char *idBuscado, tEvento *eveDestino);
void consultarEventosDisponibles(void);
void altaEvento(void);
int insertarEventoOrdenado(const tEvento *nuevo);
int buscarEvento(FILE *archEve, const char *idBuscado, tEvento *eve);
int pedirNuevoEstado(void);
void modificarEstadoEvento(void);

void consultarVentasEvento(void);
void busquedaEficienteEvento(void);
void conteoEntradasCriterio(void);
void reporteOcupacionEvento(void);
int criterioVendidas(const tEntrada *e);
int criterioCanceladas(const tEntrada *e);
int contarEntradas(const char *idEvento, int (*criterio)(const tEntrada*));

#endif // EVENTO_H_INCLUDED
