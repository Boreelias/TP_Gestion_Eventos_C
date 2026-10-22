#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED

#include <stdio.h>
#include <time.h>

int esBisiesto(int anio);
int esFechaValida(int dia, int mes, int anio, int hora, int min);
int parsearFechaCadena(const char *cadena, int *dia, int *mes, int *anio);
int parsearHoraCadena(const char *cadena, int *hora, int *min);
unsigned long long convertirFechaATimestamp(int dia, int mes, int anio, int hora, int min);
void convertirTimestampAFecha(unsigned long long ts, int *dia, int *mes, int *anio, int *hora, int *min);
void formatearFecha(unsigned long long ts, char *strFecha);
void formatearHora(unsigned long long ts, char *strHora);

#endif // FECHA_H_INCLUDED
