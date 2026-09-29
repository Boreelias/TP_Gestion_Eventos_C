#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#define MAX_SECTORES 15

typedef struct
{
    char id[10];
    char nombre[30];
    char tipoEvento[30];
    unsigned idEstablecimiento;
    unsigned long long fechaHora;
    int estado;
}tEvento;

typedef struct
{
    unsigned id;
    char nombre[30];
    char direccion[30];
    unsigned capacidadTotal;
    tSector[MAX_SECTORES];
}tEstablecimiento;

#endif // ESTRUCTURAS_H_INCLUDED
