#include "estructuras.h"
#include "evento.h"
#include "establecimiento.h"
#include "fecha.h"
#include "util.h"

int existeEvento(const char *idBuscado)
{
    tEvento eve;
    int encontrado = 0;
    FILE *archEve = fopen("Eventos.bin", "rb");

    if(archEve == NULL)
        return 0;

    while(!encontrado && fread(&eve, sizeof(tEvento), 1, archEve) == 1)
    {
        if(strcmp(eve.id, idBuscado) == 0)
            encontrado = 1;
    }

    fclose(archEve);
    return encontrado;
}

int obtenerEventoPorId(const char *idBuscado, tEvento *eveDestino)
{
    tEvento eve;
    int encontrado = 0;
    FILE *archEve = fopen("Eventos.bin", "rb");

    if(archEve == NULL)
        return 0;

    while(!encontrado && fread(&eve, sizeof(tEvento), 1, archEve) == 1)
    {
        if(strcmp(eve.id, idBuscado) == 0)
        {
            *eveDestino = eve;
            encontrado = 1;
        }
    }

    fclose(archEve);
    return encontrado;
}

int insertarEventoOrdenado(const tEvento *nuevo)
{
    tEvento actual;
    int insertado = 0;
    FILE *archOrig;
    FILE *archTemp;

    archOrig = fopen("Eventos.bin", "rb");
    archTemp = fopen("Eventos.tmp", "wb");

    if(archTemp == NULL)
    {
        if(archOrig != NULL)
            fclose(archOrig);
        return 0;
    }

    if(archOrig == NULL)
    {
        /* Si el archivo original no existe, simplemente escribimos el nuevo */
        if(fwrite(nuevo, sizeof(tEvento), 1, archTemp) != 1)
        {
            fclose(archTemp);
            remove("Eventos.tmp");
            return 0;
        }
        fclose(archTemp);
        rename("Eventos.tmp", "Eventos.bin");
        return 1;
    }

    /* Intercalacion ordenada por ID */
    while(fread(&actual, sizeof(tEvento), 1, archOrig) == 1)
    {
        if(!insertado && strcmp(nuevo->id, actual.id) < 0)
        {
            fwrite(nuevo, sizeof(tEvento), 1, archTemp);
            insertado = 1;
        }
        fwrite(&actual, sizeof(tEvento), 1, archTemp);
    }

    if(!insertado)
    {
        fwrite(nuevo, sizeof(tEvento), 1, archTemp);
    }

    fclose(archOrig);
    fclose(archTemp);

    remove("Eventos.bin");
    rename("Eventos.tmp", "Eventos.bin");

    return 1;
}

void consultarEventosDisponibles(void)
{
    tEvento eve;
    int hayEventos = 0;
    char strFecha[15];
    char strHora[10];
    char nombreEst[30];
    FILE *archEve = fopen("Eventos.bin", "rb");

    limpiarPantalla();
    printf("====================================================\n");
    printf("                 EVENTOS DISPONIBLES                \n");
    printf("====================================================\n");

    if(archEve == NULL)
    {
        printf("No hay eventos registrados en el sistema.\n\n");
        pausar();
        return;
    }

    while(fread(&eve, sizeof(tEvento), 1, archEve) == 1)
    {
        if(eve.estado == 1)
        {
            buscarNombreEstablecimiento(eve.idEstablecimiento, nombreEst);
            formatearFecha(eve.fechaHora, strFecha);
            formatearHora(eve.fechaHora, strHora);

            printf("Identificador   : %s\n", eve.id);
            printf("Nombre          : %s\n", eve.nombre);
            printf("Tipo de evento  : %s\n", eve.tipoEvento);
            printf("Fecha           : %s\n", strFecha);
            printf("Hora            : %s\n", strHora);
            printf("Establecimiento : %s\n", nombreEst);
            printf("----------------------------------------------------\n");
            hayEventos = 1;
        }
    }

    if(!hayEventos)
        printf("No hay eventos disponibles para la venta en este momento.\n");

    fclose(archEve);
    printf("\n");
    pausar();
}

void altaEvento(void)
{
    tEvento eve;
    char idTemp[50];
    char nomTemp[50];
    char tipoTemp[50];
    char fechaStr[50];
    char horaStr[50];
    int dia, mes, anio, hora, min;

    limpiarPantalla();
    printf("====================================================\n");
    printf("                  REGISTRAR EVENTO                  \n");
    printf("====================================================\n");

    /* 1. Ingreso y validacion de ID (maximo 9 caracteres + '\0') */
    printf("Ingrese el ID del evento (max 9 caracteres): ");
    if(scanf("%49s", idTemp) != 1)
    {
        vaciarBufferTeclado();
        printf("Error en la lectura del ID.\n");
        pausar();
        return;
    }
    vaciarBufferTeclado();

    if(strlen(idTemp) > 9)
    {
        printf("ERROR: El ID supera el limite permitido (max 9 caracteres).\n");
        pausar();
        return;
    }

    if(existeEvento(idTemp))
    {
        printf("ERROR: El evento con ID '%s' ya existe.\n", idTemp);
        pausar();
        return;
    }
    strcpy(eve.id, idTemp);

    /* 2. Ingreso de Nombre */
    printf("Ingrese el nombre del evento (max 29 caracteres): ");
    if(scanf(" %29[^\n]", nomTemp) != 1)
    {
        vaciarBufferTeclado();
        printf("Error en la lectura del nombre.\n");
        pausar();
        return;
    }
    vaciarBufferTeclado();
    strcpy(eve.nombre, nomTemp);

    /* 3. Ingreso de Tipo de Evento */
    printf("Ingrese el tipo de evento (recital, teatro, etc. max 29 caracteres): ");
    if(scanf(" %29[^\n]", tipoTemp) != 1)
    {
        vaciarBufferTeclado();
        printf("Error en la lectura del tipo de evento.\n");
        pausar();
        return;
    }
    vaciarBufferTeclado();
    strcpy(eve.tipoEvento, tipoTemp);

    /* 4. Ingreso de Establecimiento */
    printf("Ingrese el ID del establecimiento: ");
    if(scanf("%u", &eve.idEstablecimiento) != 1)
    {
        vaciarBufferTeclado();
        printf("ERROR: Debe ingresar un valor numerico.\n");
        pausar();
        return;
    }
    vaciarBufferTeclado();

    if(!existeEstablecimiento(eve.idEstablecimiento))
    {
        printf("ERROR: El establecimiento con ID %u no existe en el sistema.\n", eve.idEstablecimiento);
        printf("Debe registrar el establecimiento previamente.\n");
        pausar();
        return;
    }

    /* 5. Ingreso y validacion de Fecha y Hora tolerante a formatos */
    printf("Ingrese la fecha (ej: DD/MM/AAAA o DDMMAAAA): ");
    if(scanf("%49s", fechaStr) != 1)
    {
        vaciarBufferTeclado();
        printf("ERROR: Lectura de fecha invalida.\n");
        pausar();
        return;
    }
    vaciarBufferTeclado();

    if(!parsearFechaCadena(fechaStr, &dia, &mes, &anio))
    {
        printf("ERROR: Formato de fecha invalido. Use DD/MM/AAAA o DDMMAAAA.\n");
        pausar();
        return;
    }

    printf("Ingrese la hora (ej: HH:MM o HHMM): ");
    if(scanf("%49s", horaStr) != 1)
    {
        vaciarBufferTeclado();
        printf("ERROR: Lectura de hora invalida.\n");
        pausar();
        return;
    }
    vaciarBufferTeclado();

    if(!parsearHoraCadena(horaStr, &hora, &min))
    {
        printf("ERROR: Formato de hora invalido. Use HH:MM o HHMM.\n");
        pausar();
        return;
    }

    if(!esFechaValida(dia, mes, anio, hora, min))
    {
        printf("ERROR: La fecha y hora ingresada (%02d/%02d/%04d %02d:%02d) no es valida en el calendario.\n", dia, mes, anio, hora, min);
        pausar();
        return;
    }

    eve.fechaHora = convertirFechaATimestamp(dia, mes, anio, hora, min);

    /* 6. Estado inicial: 1 = Disponible por defecto */
    eve.estado = 1;

    /* 7. Insercion ordenada por ID en el archivo */
    if(insertarEventoOrdenado(&eve))
    {
        printf("\n====================================================\n");
        printf("           EVENTO REGISTRADO EXITOSAMENTE           \n");
        printf("====================================================\n");
    }
    else
    {
        printf("ERROR: No se pudo guardar el evento en el archivo.\n");
    }

    pausar();
}
