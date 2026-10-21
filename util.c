#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void vaciarBufferTeclado(void)
{
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void pausar(void)
{
    printf("Presione ENTER para continuar...");
    getchar();
}

void limpiarPantalla(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
