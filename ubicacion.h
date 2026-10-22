#ifndef UBICACION_H_INCLUDED
#define UBICACION_H_INCLUDED

#include "estructuras.h"

int estaUbicacionOcupadaPorEvento(const char *idEvento, unsigned idSector, int fila, int butaca);
void mostrarMatrizUbicaciones(const char *idEvento, const tSector *sector);
void consultarUbicacionesYDisponibilidad(void);

#endif // UBICACION_H_INCLUDED
