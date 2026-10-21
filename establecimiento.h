#ifndef ESTABLECIMIENTO_H_INCLUDED
#define ESTABLECIMIENTO_H_INCLUDED

#include "estructuras.h"

int existeEstablecimiento(unsigned idBuscado);
int buscarNombreEstablecimiento(unsigned idBuscado, char *nomEstablecimiento);
int obtenerEstablecimientoPorId(unsigned idBuscado, tEstablecimiento *estDestino);
void consultarEstablecimiento(void);

#endif // ESTABLECIMIENTO_H_INCLUDED
