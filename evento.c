#include "ubicacion.h"
#include "estructuras.h"
#include "evento.h"
#include "establecimiento.h"
#include "fecha.h"
#include "util.h"
#include "compra.h"

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

    eve.estado = 1;

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

int buscarEvento(FILE *archEve, const char *idBuscado, tEvento *eve)
{
    rewind(archEve);
    while(fread(eve, sizeof(tEvento), 1, archEve) == 1)
    {
        if(strcmp(eve->id, idBuscado) == 0)
        {
            fseek(archEve, -(long)sizeof(tEvento), SEEK_CUR);
            return 1;
        }
    }
    return 0;
}

int pedirNuevoEstado(void)
{
    int nuevoEstado;

    printf("\nSeleccione el nuevo estado:\n");
    printf("1. Disponible\n");
    printf("2. Cancelado\n");
    printf("3. Finalizado\n");
    printf("Opcion: ");

    if(scanf("%d", &nuevoEstado) != 1 || nuevoEstado < 1 || nuevoEstado > 3)
    {
        vaciarBufferTeclado();
    }
    vaciarBufferTeclado();
    return nuevoEstado;
}

void modificarEstadoEvento(void)
{
    char idBuscado[50];
    tEvento eve;
    FILE *archEve;
    int nuevoEstado;

    limpiarPantalla();
    printf("====================================================\n");
    printf("             GESTION DE ESTADO DE EVENTOS           \n");
    printf("====================================================\n");

    printf("Ingrese el ID del evento a modificar: ");
    if(scanf("%49s", idBuscado) != 1)
    {
        vaciarBufferTeclado();
        printf("Error en la lectura del ID.\n");
        pausar();
        return;
    }
    vaciarBufferTeclado();

    archEve = fopen("Eventos.bin", "r+b");
    if(archEve == NULL)
    {
        printf("ERROR: No se pudo abrir el archivo de eventos.\n");
        pausar();
        return;
    }

    if(!buscarEvento(archEve, idBuscado, &eve))
    {
        printf("ERROR: No se encontro el evento con ID '%s'.\n", idBuscado);
        fclose(archEve);
        pausar();
        return;
    }

    printf("\nEvento Encontrado : %s (%s)\n", eve.nombre, eve.id);
    if(eve.estado == 1) printf("Estado Actual     : Disponible (1)\n"); else if(eve.estado == -1) printf("Estado Actual     : Cancelado (-1)\n"); else printf("Estado Actual     : Finalizado (2)\n");
    printf("----------------------------------------------------\n");

    nuevoEstado = pedirNuevoEstado();
    if(nuevoEstado == 0)
    {
        printf("ERROR: Opcion de estado invalida.\n");
        fclose(archEve);
        pausar();
        return;
    }

    eve.estado = nuevoEstado;
    fwrite(&eve, sizeof(tEvento), 1, archEve);
    fclose(archEve);

    if(nuevoEstado == 2 || nuevoEstado == 3)
    {
        actualizarEntradasDeEvento(eve.id, nuevoEstado);
    }

    printf("\n¡ESTADO DEL EVENTO ACTUALIZADO EXITOSAMENTE!\n");
    pausar();
}




void consultarVentasEvento(void)
{
    char idEvento[50];
    tEvento eve;
    FILE *archComp;
    tCompra comp;
    int entradasVendidas;
    int entradasCanceladas;
    float recaudacionVigente;
    int i;
    const tEntrada *pEntrada;

    entradasVendidas = 0;
    entradasCanceladas = 0;
    recaudacionVigente = 0.0f;

    limpiarPantalla();
    printf("====================================================\n");
    printf("                 CONSULTA DE VENTAS                 \n");
    printf("====================================================\n");

    printf("Ingrese el ID del evento a consultar: ");
    if(scanf("%49s", idEvento) != 1)
    {
        vaciarBufferTeclado();
        pausar();
        return;
    }
    vaciarBufferTeclado();

    if(!obtenerEventoPorId(idEvento, &eve))
    {
        printf("ERROR: No se encontro el evento con ID '%s'.\n", idEvento);
        pausar();
        return;
    }

    archComp = fopen("Compras.bin", "rb");
    if(archComp != NULL)
    {
        while(fread(&comp, sizeof(tCompra), 1, archComp) == 1)
        {
            if(strcmp(comp.IdEvento, idEvento) == 0)
            {
                pEntrada = comp.Entradas;
                for(i = 0; i < comp.cantEntradas; i++)
                {
                    if(strcmp(pEntrada->estado, "CANCELADA") == 0)
                    {
                        entradasCanceladas++;
                    }
                    else
                    {
                        entradasVendidas++;
                        recaudacionVigente += pEntrada->precio;
                    }
                    pEntrada++;
                }
            }
        }
        fclose(archComp);
    }

    printf("\n--- REPORTE DE VENTAS: %s (%s) ---\n", eve.nombre, eve.id);
    printf("  Cantidad de entradas vendidas vigentes : %d\n", entradasVendidas);
    printf("  Cantidad de entradas canceladas        : %d\n", entradasCanceladas);
    printf("  Recaudacion total vigente              : $%.2f\n", recaudacionVigente);
    printf("----------------------------------------------------\n\n");
    pausar();
}




void busquedaEficienteEvento(void)
{
    int opcion;
    char idBuscado[50];
    FILE *archEve;
    int inicio, fin, medio;
    int totalRegistros;
    tEvento eve;
    int encontrado;
    char strFecha[15];
    char strHora[10];
    char nombreEst[30];

    encontrado = 0;

    limpiarPantalla();
    printf("====================================================\n");
    printf("             BUSQUEDA EFICIENTE DE EVENTOS          \n");
    printf("====================================================\n");
    printf("1. Buscar evento por ID\n");
    printf("2. Consultar eventos de un estadio/teatro\n");
    printf("Opcion: ");
    if(scanf("%d", &opcion) != 1)
    {
        vaciarBufferTeclado();
        pausar();
        return;
    }
    vaciarBufferTeclado();

    if(opcion == 1)
    {
        printf("Ingrese el ID del evento: ");
        if(scanf("%49s", idBuscado) != 1)
        {
            vaciarBufferTeclado();
            pausar();
            return;
        }
        vaciarBufferTeclado();

        archEve = fopen("Eventos.bin", "rb");
        if(archEve == NULL)
        {
            printf("No existen eventos registrados.\n");
            pausar();
            return;
        }

        fseek(archEve, 0, SEEK_END);
        totalRegistros = (int)(ftell(archEve) / sizeof(tEvento));

        inicio = 0;
        fin = totalRegistros - 1;

        while(inicio <= fin && !encontrado)
        {
            int cmp;
            medio = inicio + (fin - inicio) / 2;
            fseek(archEve, medio * sizeof(tEvento), SEEK_SET);
            fread(&eve, sizeof(tEvento), 1, archEve);

            cmp = strcmp(eve.id, idBuscado);
            if(cmp == 0)
            {
                encontrado = 1;
            }
            else if(cmp < 0)
            {
                inicio = medio + 1;
            }
            else
            {
                fin = medio - 1;
            }
        }
        fclose(archEve);

        if(encontrado)
        {
            buscarNombreEstablecimiento(eve.idEstablecimiento, nombreEst);
            formatearFecha(eve.fechaHora, strFecha);
            formatearHora(eve.fechaHora, strHora);

            printf("\n--- EVENTO ENCONTRADO ---\n");
            printf("ID              : %s\n", eve.id);
            printf("Nombre          : %s\n", eve.nombre);
            printf("Tipo            : %s\n", eve.tipoEvento);
            printf("Fecha           : %s\n", strFecha);
            printf("Hora            : %s\n", strHora);
            printf("Establecimiento : %s\n", nombreEst);
            printf("Estado          : %d\n", eve.estado);
        }
        else
        {
            printf("No se encontro ningun evento con ID '%s'.\n", idBuscado);
        }
    }
    else if(opcion == 2)
    {
        unsigned idEst;
        int hay;
        hay = 0;

        printf("Ingrese el ID del establecimiento: ");
        if(scanf("%u", &idEst) != 1)
        {
            vaciarBufferTeclado();
            pausar();
            return;
        }
        vaciarBufferTeclado();

        archEve = fopen("Eventos.bin", "rb");
        if(archEve != NULL)
        {
            printf("\nEventos para el establecimiento ID %u:\n", idEst);
            while(fread(&eve, sizeof(tEvento), 1, archEve) == 1)
            {
                if(eve.idEstablecimiento == idEst)
                {
                    formatearFecha(eve.fechaHora, strFecha);
                    formatearHora(eve.fechaHora, strHora);
                    printf(" - [%s] %s | %s %s (Estado: %d)\n", eve.id, eve.nombre, strFecha, strHora, eve.estado);
                    hay = 1;
                }
            }
            fclose(archEve);
        }
        if(!hay)
            printf("No hay eventos programados para ese establecimiento.\n");
    }

    printf("\n");
    pausar();
}




int criterioVendidas(const tEntrada *e)
{
    return (strcmp(e->estado, "CANCELADA") != 0);
}

int criterioCanceladas(const tEntrada *e)
{
    return (strcmp(e->estado, "CANCELADA") == 0);
}

int contarEntradas(const char *idEvento, int (*criterio)(const tEntrada*))
{
    FILE *archComp;
    tCompra comp;
    int cant;
    int i;
    const tEntrada *pEntrada;

    cant = 0;
    archComp = fopen("Compras.bin", "rb");
    if(archComp == NULL)
        return 0;

    while(fread(&comp, sizeof(tCompra), 1, archComp) == 1)
    {
        if(strcmp(comp.IdEvento, idEvento) == 0)
        {
            pEntrada = comp.Entradas;
            for(i = 0; i < comp.cantEntradas; i++)
            {
                if(criterio(pEntrada))
                    cant++;
                pEntrada++;
            }
        }
    }

    fclose(archComp);
    return cant;
}

void conteoEntradasCriterio(void)
{
    char idEvento[50];
    tEvento eve;
    int v, c;

    limpiarPantalla();
    printf("====================================================\n");
    printf("               CONTEO DE ENTRADAS                   \n");
    printf("====================================================\n");

    printf("Ingrese el ID del evento: ");
    if(scanf("%49s", idEvento) != 1)
    {
        vaciarBufferTeclado();
        pausar();
        return;
    }
    vaciarBufferTeclado();

    if(!obtenerEventoPorId(idEvento, &eve))
    {
        printf("ERROR: No se encontro el evento con ID '%s'.\n", idEvento);
        pausar();
        return;
    }

    v = contarEntradas(idEvento, criterioVendidas);
    c = contarEntradas(idEvento, criterioCanceladas);

    printf("\n--- RESULTADOS DEL CONTEO: %s ---\n", eve.nombre);
    printf("  Entradas Vendidas  : %d\n", v);
    printf("  Entradas Canceladas: %d\n", c);
    printf("  Total emitidas     : %d\n\n", v + c);
    pausar();
}




void reporteOcupacionEvento(void)
{
    char idEvento[50];
    tEvento eve;
    tEstablecimiento *est;
    int capTotal;
    int ocupadas;
    int disponibles;
    float porcentaje;
    int s, f, c;
    const tSector *pSector;

    capTotal = 0;
    ocupadas = 0;
    disponibles = 0;
    porcentaje = 0.0f;

    limpiarPantalla();
    printf("====================================================\n");
    printf("                REPORTE DE OCUPACION                \n");
    printf("====================================================\n");

    printf("Ingrese el ID del evento: ");
    if(scanf("%49s", idEvento) != 1)
    {
        vaciarBufferTeclado();
        pausar();
        return;
    }
    vaciarBufferTeclado();

    if(!obtenerEventoPorId(idEvento, &eve))
    {
        printf("ERROR: No se encontro el evento con ID '%s'.\n", idEvento);
        pausar();
        return;
    }

    est = (tEstablecimiento*)malloc(sizeof(tEstablecimiento));
    if(est == NULL)
    {
        printf("ERROR: Memoria insuficiente.\n");
        pausar();
        return;
    }

    if(!obtenerEstablecimientoPorId(eve.idEstablecimiento, est))
    {
        printf("ERROR: No se encontro el establecimiento asociado.\n");
        free(est);
        pausar();
        return;
    }

    pSector = est->sectores;
    for(s = 0; s < MAX_SECTORES; s++)
    {
        if(pSector->id != 0 || pSector->capacidad > 0)
        {
            int fMax = pSector->filas;
            int cMax = pSector->columnas;

            for(f = 1; f <= fMax; f++)
            {
                for(c = 1; c <= cMax; c++)
                {
                    capTotal++;
                    if(estaUbicacionOcupadaPorEvento(eve.id, pSector->id, f, c))
                        ocupadas++;
                    else
                        disponibles++;
                }
            }
        }
        pSector++;
    }

    if(capTotal > 0)
        porcentaje = ((float)ocupadas / (float)capTotal) * 100.0f;

    printf("\n--- ESTADISTICAS DE OCUPACION: %s (%s) ---\n", eve.nombre, eve.id);
    printf("  Capacidad total efectiva : %d butacas\n", capTotal);
    printf("  Ubicaciones ocupadas     : %d\n", ocupadas);
    printf("  Ubicaciones disponibles  : %d\n", disponibles);
    printf("  Porcentaje de ocupacion  : %.2f%%\n", porcentaje);
    printf("----------------------------------------------------\n\n");

    free(est);
    pausar();
}
