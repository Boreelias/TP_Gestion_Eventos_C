#include "estructuras.h"
#include "establecimiento.h"
#include "evento.h"
#include "util.h"

int existeEstablecimiento(unsigned idBuscado)
{
    tEstablecimiento est;
    int encontrado = 0;
    FILE *archEst = fopen("Establecimientos.bin", "rb");

    if(archEst == NULL)
        return 0;

    while(!encontrado && fread(&est, sizeof(tEstablecimiento), 1, archEst) == 1)
    {
        if(est.id == idBuscado)
            encontrado = 1;
    }

    fclose(archEst);
    return encontrado;
}

int buscarNombreEstablecimiento(unsigned idBuscado, char *nomEstablecimiento)
{
    tEstablecimiento est;
    int encontrado = 0;
    FILE *archEst = fopen("Establecimientos.bin", "rb");

    if(archEst == NULL)
    {
        strcpy(nomEstablecimiento, "Desconocido");
        return 0;
    }

    while(!encontrado && fread(&est, sizeof(tEstablecimiento), 1, archEst) == 1)
    {
        if(est.id == idBuscado)
        {
            strcpy(nomEstablecimiento, est.nombre);
            encontrado = 1;
        }
    }

    if(!encontrado)
        strcpy(nomEstablecimiento, "No encontrado");

    fclose(archEst);
    return encontrado;
}

int obtenerEstablecimientoPorId(unsigned idBuscado, tEstablecimiento *estDestino)
{
    tEstablecimiento est;
    int encontrado = 0;
    FILE *archEst = fopen("Establecimientos.bin", "rb");

    if(archEst == NULL)
        return 0;

    while(!encontrado && fread(&est, sizeof(tEstablecimiento), 1, archEst) == 1)
    {
        if(est.id == idBuscado)
        {
            *estDestino = est;
            encontrado = 1;
        }
    }

    fclose(archEst);
    return encontrado;
}

void consultarEstablecimiento(void)
{
    char idEvento[50];
    tEvento eve;
    tEstablecimiento est;
    const tSector *pSector;
    const tSector *pFinSector;
    int haySectores;

    limpiarPantalla();
    printf("====================================================\n");
    printf("             CONSULTA DE ESTABLECIMIENTO            \n");
    printf("====================================================\n");

    printf("Ingrese el ID del evento a consultar: ");
    if(scanf("%49s", idEvento) != 1)
    {
        vaciarBufferTeclado();
        printf("Error en la lectura del ID.\n");
        pausar();
        return;
    }
    vaciarBufferTeclado();

    if(!obtenerEventoPorId(idEvento, &eve))
    {
        printf("ERROR: No se encontro ningun evento con ID '%s'.\n", idEvento);
        pausar();
        return;
    }

    if(!obtenerEstablecimientoPorId(eve.idEstablecimiento, &est))
    {
        printf("ERROR: No se encontro el establecimiento (ID: %u) asociado al evento.\n", eve.idEstablecimiento);
        pausar();
        return;
    }

    printf("\nEvento          : %s (%s)\n", eve.nombre, eve.id);
    printf("Establecimiento : %s (ID: %u)\n", est.nombre, est.id);
    printf("Direccion       : %s\n", est.direccion);
    printf("Capacidad Total : %u personas\n", est.capacidadTotal);
    printf("====================================================\n");
    printf("                      SECTORES                      \n");
    printf("====================================================\n");

    haySectores = 0;
    pSector = est.sectores;
    pFinSector = est.sectores + MAX_SECTORES;

    
    while(pSector < pFinSector)
    {
        if(pSector->id != 0 || pSector->capacidad > 0)
        {
            printf("Sector ID   : %u\n", pSector->id);
            printf("Nombre      : %s\n", pSector->nombre);
            printf("Capacidad   : %u\n", pSector->capacidad);
            printf("Precio      : $%.2f\n", pSector->precio);
            printf("Dimensiones : %d filas x %d columnas\n", pSector->filas, pSector->columnas);
            printf("----------------------------------------------------\n");
            haySectores = 1;
        }
        pSector++;
    }

    if(!haySectores)
    {
        printf("El establecimiento no posee sectores cargados actualmente.\n");
    }

    printf("\n");
    pausar();
}
