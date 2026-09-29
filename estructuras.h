#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#define MAX_SECTORES 15
#define MAX_FIL 30
#define MAX_COL 45

typedef struct {
    int fila;
    int columna;
    int disponibilidad;
} tUbicacion;

typedef struct {
    unsigned id;
    char nombre[15];
    unsigned capacidad;
    float Precio;
    int filas;
    int columnas;
    tUbicacion Ubicaciones[MAX_FIL][MAX_COL];
} tSector;

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
    tSector sectores[MAX_SECTORES];
}tEstablecimiento;

#endif // ESTRUCTURAS_H_INCLUDED
