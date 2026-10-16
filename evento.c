#include "estructuras.h"
#include "evento.h"

void buscarNombreEstablecimiento(unsigned idBuscado, char *nomEstablecimiento)
{
    FILE *archEst = fopen("Establecimientos.bin","rb");
    if(archEst == NULL)
    {
        printf("Fallo la apertura del archivo!");
        system("pause");
        exit(-1);
        strcpy(nomEstablecimiento, "Desconocido");
    }

    tEstablecimiento est;
    int encontrado = 0;

        fread(&est, sizeof(tEstablecimiento),1,archEst);
        while(!feof(archEst) && !encontrado)
        {
            if(est.id == idBuscado)
            {
                strcpy(nomEstablecimiento, est.nombre);
                encontrado = 1;
            }
            else
                fread(&est, sizeof(tEstablecimiento),1,archEst);
        }

        if(!encontrado)
            strcpy(nomEstablecimiento, "No encontrado");

        fclose(archEst);
}

void consultarEventosDisponibles()
{
    FILE *archEve = fopen("Eventos.bin","rb");
    if(archEve == NULL)
    {
        printf("No se pudo abrir el archivo!");
        system("pause");
        exit(-1);
    }

    tEvento eve;

    int hayEven = 0;

    system("cls");
    printf("====================================================\n");
    printf("                 EVENTOS DISPONIBLES                \n");
    printf("====================================================\n");

    fread(&eve, sizeof(tEvento),1,archEve);
    while(!feof(archEve))
    {
        if(eve.estado == 1)
        {
            char nombreEst[30];
            buscarNombreEstablecimiento(eve.idEstablecimiento, nombreEst);

            printf("Identificador: %s\n", eve.id);
            printf("Nombre: %s\n", eve.nombre);
            printf("Tipo de evento: %s\n", eve.tipoEvento);
            printf("Fecha/hora: %llu\n", eve.fechaHora);
            printf("Establecimiento: %s\n", nombreEst);
            printf("..........................................\n\n");
            hayEven = 1;
        }
        fread(&eve, sizeof(tEvento), 1, archEve);
    }

    if(!hayEven)
        printf("No hay eventos disponobles!");
    fclose(archEve);
    system("pause");
}
