#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char id[10];
    char nombre[30];
    char tipoEvento[30];
    unsigned idEstablecimiento;
    unsigned long long fechaHora;
    int estado;
}tEvento;


#endif // ESTRUCTURAS_H_INCLUDED
