#include "estructuras.h"
#include "evento.h"
#include "establecimiento.h"

int existeEvento(const char *idBuscado)
{
    tEvento eve;
    int contador = 0;

    FILE *archEve = fopen("Eventos.bin","rb");
    if(archEve == NULL)
    {
        printf("No se pudo abrir el archivo Eventos.bin\n");
        system("pause");
        exit(-1);
    }

    fread(&eve, sizeof(tEvento),1,archEve);
    while(!feof(archEve))
    {
        if(strcmpi(eve.id, idBuscado) == 0)
        {
            contador = 1;
            return contador;
        }

        fread(&eve, sizeof(tEvento),1,archEve);
    }

    return contador;
}

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

void altaEvento()
{
    char exceso[100];

    FILE *archEve = fopen("Eventos.bin","ab");
    if(archEve == NULL)
    {
        printf("No se pudo abrir el archivo!");
        system("pause");
        exit(-1);
    }

    tEvento eve;
    system("cls");

    printf("====================================================\n");
    printf("                  REGISTRAR EVENTOS                 \n");
    printf("====================================================\n");

    printf("Ingrese el ID del evento: ");
    scanf("%s", exceso);
    if(strlen(exceso) != 10)
    {
        printf("El ID %s se va de los limites\n", exceso);
        system("pause");
        return;
    }
    strcpy(eve.id, exceso);

    if(existeEvento(eve.id))
    {
        printf("El evento %s ya existe!\n", eve.id);
        system("pause");
        return;
    }

    printf("Ingrese el nombre del evento: ");
    scanf("%s", exceso);
    if(strlen(exceso) > 30)
    {
        printf("El nombre %s se va de los limites\n", exceso);
        system("pause");
        return;
    }
    strcpy(eve.nombre, exceso);

    printf("Ingrese el tipo de evento: ");
    scanf("%s", exceso);
    if(strlen(exceso) > 50)
    {
        printf("El tipo de evento %s se va de los limites\n", exceso);
        system("pause");
        return;
    }
    strcpy(eve.tipoEvento, exceso);


    printf("Ingrese el ID establecimiento: ");
    scanf("%u", &eve.idEstablecimiento);

    if(!existeEstablecimiento(eve.idEstablecimiento))
    {
        printf("ERROR!\nEl establecimiento con el ID %d no existe en el sistema.\nDebe registrar el establecimiento previamente\n", eve.idEstablecimiento);
        system("pause");
        return;
    }

    printf("Ingrese la fecha y hora: ");
    int leido = scanf("%llu", &eve.fechaHora);

    if(leido != 1)
    {
        printf("Error!\nLa fecha esta mal ingresada\n");
        system("pause");
        return;
    }

    if(!validarFecha(eve.fechaHora))
    {
        printf("ERROR!\La fecha %llu no es valida\n", eve.fechaHora);
        system("pause");
        return;
    }

    printf("Ingrese el estado del evento: ");
    scanf("%d", &eve.estado);

    fwrite(&eve, sizeof(tEvento),1,archEve);

    fclose(archEve);

    printf("\nEvento registrado!");
    system("pause");
}
