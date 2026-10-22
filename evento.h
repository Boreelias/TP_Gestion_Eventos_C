#ifndef EVENTO_H_INCLUDED
#define EVENTO_H_INCLUDED

#include "estructuras.h"

int existeEvento(const char *idBuscado);
int obtenerEventoPorId(const char *idBuscado, tEvento *eveDestino);
void consultarEventosDisponibles(void);
void altaEvento(void);
int insertarEventoOrdenado(const tEvento *nuevo);

#endif // EVENTO_H_INCLUDED
