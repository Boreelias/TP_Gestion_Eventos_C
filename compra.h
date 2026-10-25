#ifndef COMPRA_H_INCLUDED
#define COMPRA_H_INCLUDED
#include "estructuras.h"

void generarCodigoCompra(const char *idEvento, const char *nombreEvento, char *codigoDestino);
int mostrarYConfirmarResumen(const tCompra *nuevaCompra, const tEvento *eve, const tEstablecimiento *est, const tSector *sectorElegido);
int cargarUbicacionesCompra(tCompra *nuevaCompra, const tEvento *eve, const tSector *sectorElegido, int cantEntradas);
void generarCodigoCompra(const char *idEvento, const char *nombreEvento, char *codigoDestino);
const tSector *seleccionarSector(const tEstablecimiento *est);
int guardarCompraYEntradas(const tCompra *nuevaCompra);
void procesarCompraEntradas(const char *idUsuario);
void consultarEntradasUsuario(const char *idUsuario);
void cancelarEntradasUsuario(const char *idUsuario);

#endif // COMPRA_H_INCLUDED
