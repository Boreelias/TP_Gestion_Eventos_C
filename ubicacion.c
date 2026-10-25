#include "ubicacion.h"
#include "evento.h"
#include "establecimiento.h"
#include "util.h"

int estaUbicacionOcupadaPorEvento(const char *idEvento, unsigned idSector, int fila, int butaca)
{
    FILE *archEnt = fopen("Entradas.bin", "rb");
    tEntrada ent;
    FILE *archComp;
    tCompra comp;
    int ocupada = 0;

    if(archEnt == NULL)
        return 0;

    while(!ocupada && fread(&ent, sizeof(tEntrada), 1, archEnt) == 1)
    {
        /* Si coincide sector, fila y butaca y no esta cancelada */
        if(ent.sector == (int)idSector && ent.fila == fila && ent.butaca == butaca)
        {
            if(strcmp(ent.estado, "CANCELADA") != 0)
            {
                /* Verificar si esta entrada pertenece al evento consultado */
                archComp = fopen("Compras.bin", "rb");
                if(archComp != NULL)
                {
                    while(!ocupada && fread(&comp, sizeof(tCompra), 1, archComp) == 1)
                    {
                        if(strcmp(comp.id, ent.idCompra) == 0 && strcmp(comp.IdEvento, idEvento) == 0)
                        {
                            ocupada = 1;
                        }
                    }
                    fclose(archComp);
                }
            }
        }
    }

    fclose(archEnt);
    return ocupada;
}

void mostrarMatrizUbicaciones(const char *idEvento, const tSector *sector)
{
    int f, c;
    int ocupada;

    printf("\n--- DISTRIBUCION DE BUTACAS (Sector: %s) ---\n", sector->nombre);
    printf("Referencias: [ D ] = Disponible | [ X ] = Ocupada\n\n");

    /* Encabezado de columnas */
    printf("      ");
    for(c = 1; c <= sector->columnas; c++)
    {
        printf(" C%-2d ", c);
    }
    printf("\n");

    for(f = 1; f <= sector->filas; f++)
    {
        printf("F%-3d ", f);
        for(c = 1; c <= sector->columnas; c++)
        {
            ocupada = estaUbicacionOcupadaPorEvento(idEvento, sector->id, f, c);
            if(ocupada)
            {
                printf(" [X] ");
            }
            else
            {
                printf(" [D] ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void consultarUbicacionesYDisponibilidad(void)
{
    char idEvento[50];
    tEvento eve;
    tEstablecimiento *est;
    unsigned idSector;
    const tSector *sectorElegido = NULL;
    const tSector *pSector;
    const tSector *pFinSector;
    int i;
    int cantAValidar;
    int filaIngresada, butacaIngresada;

    limpiarPantalla();
    printf("====================================================\n");
    printf("        CONSULTA DE UBICACIONES Y DISPONIBILIDAD    \n");
    printf("====================================================\n");

    printf("Ingrese el ID del evento: ");
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
        printf("ERROR: No se encontro el establecimiento asociado al evento.\n");
        free(est);
        pausar();
        return;
    }

    printf("\nEvento: %s (%s) en %s\n", eve.nombre, eve.id, est->nombre);
    printf("----------------------------------------------------\n");
    printf("Sectores disponibles:\n");
    pSector = est->sectores;
    pFinSector = est->sectores + MAX_SECTORES;
    while(pSector < pFinSector)
    {
        if(pSector->id != 0 || pSector->capacidad > 0)
        {
            printf(" - ID: %u | Nombre: %-15s | Precio: $%.2f | (%d filas x %d butacas)\n",
                   pSector->id, pSector->nombre, pSector->precio,
                   pSector->filas, pSector->columnas);
        }
        pSector++;
    }
    printf("----------------------------------------------------\n");

    printf("Ingrese el ID del sector a consultar: ");
    if(scanf("%u", &idSector) != 1)
    {
        vaciarBufferTeclado();
        printf("ERROR: Debe ingresar un ID numerico valido.\n");
        free(est);
        pausar();
        return;
    }
    vaciarBufferTeclado();

    pSector = est->sectores;
    while(pSector < pFinSector && sectorElegido == NULL)
    {
        if(pSector->id == idSector)
        {
            sectorElegido = pSector;
        }
        pSector++;
    }

    if(sectorElegido == NULL)
    {
        printf("ERROR: El sector ID %u no existe en este establecimiento.\n", idSector);
        free(est);
        pausar();
        return;
    }

    
    mostrarMatrizUbicaciones(eve.id, sectorElegido);

    
    printf("Desea verificar la disponibilidad de ubicaciones? (1: Si / 0: No): ");
    if(scanf("%d", &cantAValidar) == 1 && cantAValidar == 1)
    {
        vaciarBufferTeclado();
        printf("\nCuantas ubicaciones desea validar?: ");
        if(scanf("%d", &cantAValidar) == 1 && cantAValidar > 0)
        {
            vaciarBufferTeclado();
            for(i = 0; i < cantAValidar; i++)
            {
                printf("\n--- Ubicacion #%d ---\n", i + 1);
                printf("Ingrese Fila (1 a %d): ", sectorElegido->filas);
                scanf("%d", &filaIngresada);
                printf("Ingrese Butaca/Columna (1 a %d): ", sectorElegido->columnas);
                scanf("%d", &butacaIngresada);
                vaciarBufferTeclado();

                /* Validar existencia */
                if(filaIngresada < 1 || filaIngresada > sectorElegido->filas ||
                   butacaIngresada < 1 || butacaIngresada > sectorElegido->columnas)
                {
                    printf(">> RESULTADO: La ubicacion [Fila %d, Butaca %d] NO EXISTE en el sector.\n",
                           filaIngresada, butacaIngresada);
                }
                else
                {
                    /* Validar disponibilidad */
                    if(estaUbicacionOcupadaPorEvento(eve.id, sectorElegido->id, filaIngresada, butacaIngresada))
                    {
                        printf(">> RESULTADO: La ubicacion [Fila %d, Butaca %d] esta OCUPADA.\n",
                               filaIngresada, butacaIngresada);
                    }
                    else
                    {
                        printf(">> RESULTADO: La ubicacion [Fila %d, Butaca %d] esta DISPONIBLE para compra.\n",
                               filaIngresada, butacaIngresada);
                    }
                }
            }
        }
    }
    vaciarBufferTeclado();

    free(est);
    printf("\n");
    pausar();
}
