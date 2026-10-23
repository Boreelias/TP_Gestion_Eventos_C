#include "estructuras.h"
#include "compra.h"
#include "evento.h"
#include "fecha.h"
#include "util.h"
#include "ubicacion.h"
#include "establecimiento.h"

void generarCodigoCompra(const char *idEvento, const char *nombreEvento, char *codigoDestino)
{
    FILE *archComp;
    int cantCompras;
    int correlativo;
    int sumaDigitos;
    char letras[4];
    int j;
    char letraShift;
    const char *pNombre;
    const char *pId;
    tEvento eve;
    int dia, mes, anio, hora, min;

    cantCompras = 0;
    sumaDigitos = 0;

    archComp = fopen("Compras.bin", "rb");
    if(archComp != NULL)
    {
        fseek(archComp, 0, SEEK_END);
        cantCompras = (int)(ftell(archComp) / sizeof(tCompra));
        fclose(archComp);
    }
    cantCompras++;

    correlativo = ((cantCompras - 1) % 999) + 1;

    if(obtenerEventoPorId(idEvento, &eve))
    {
        convertirTimestampAFecha(eve.fechaHora, &dia, &mes, &anio, &hora, &min);
        sumaDigitos = dia + mes;
    }

    pId = idEvento;
    while(*pId != '\0')
    {
        sumaDigitos += (int)(*pId);
        pId++;
    }
    sumaDigitos = sumaDigitos % 1000;

    j = 0;
    pNombre = nombreEvento;
    while(*pNombre != '\0' && j < 3)
    {
        if((*pNombre >= 'A' && *pNombre <= 'Z') || (*pNombre >= 'a' && *pNombre <= 'z'))
        {
            letraShift = *pNombre + 5;
            if((*pNombre <= 'Z' && letraShift > 'Z') || (*pNombre <= 'z' && letraShift > 'z'))
            {
                letraShift -= 26;
            }
            *(letras + j) = letraShift;
            j++;
        }
        pNombre++;
    }
    while(j < 3)
    {
        *(letras + j) = 'X';
        j++;
    }
    *(letras + 3) = '\0';

    sprintf(codigoDestino, "%03d%s%03d", sumaDigitos, letras, correlativo);
}

const tSector *seleccionarSector(const tEstablecimiento *est)
{
    unsigned idSector;
    const tSector *pSector;
    const tSector *pFinSector;
    const tSector *sectorElegido;

    sectorElegido = NULL;
    pSector = est->sectores;
    pFinSector = est->sectores + MAX_SECTORES;

    printf("Ingrese el ID del sector para la compra: ");
    if(scanf("%u", &idSector) != 1)
    {
        vaciarBufferTeclado();
        return NULL;
    }
    vaciarBufferTeclado();

    while(pSector < pFinSector)
    {
        if(pSector->id == idSector)
        {
            sectorElegido = pSector;
            break;
        }
        pSector++;
    }

    return sectorElegido;
}

int cargarUbicacionesCompra(tCompra *nuevaCompra, const tEvento *eve, const tSector *sectorElegido, int cantEntradas)
{
    int i;
    int filaIngresada;
    int butacaIngresada;
    int compraValida;
    tEntrada *pEntrada;
    tEntrada *pCheck;
    int butacaDuplicada;

    compraValida = 1;
    pEntrada = nuevaCompra->Entradas;
    i = 0;

    while(i < cantEntradas && compraValida)
    {
        printf("\n--- Seleccion de Ubicacion #%d ---\n", i + 1);
        printf("Ingrese Fila (1 a %d): ", sectorElegido->filas);
        if(scanf("%d", &filaIngresada) != 1)
        {
            vaciarBufferTeclado();
            printf(">> ERROR: Entrada invalida para la fila.\n");
            compraValida = 0;
            break;
        }
        vaciarBufferTeclado();

        printf("Ingrese Butaca (1 a %d): ", sectorElegido->columnas);
        if(scanf("%d", &butacaIngresada) != 1)
        {
            vaciarBufferTeclado();
            printf(">> ERROR: Entrada invalida para la butaca.\n");
            compraValida = 0;
            break;
        }
        vaciarBufferTeclado();

        if(filaIngresada < 1 || filaIngresada > sectorElegido->filas ||
            butacaIngresada < 1 || butacaIngresada > sectorElegido->columnas)
        {
            printf(">> ERROR: La ubicacion ingresada no existe en el sector.\n");
            compraValida = 0;
        }
        else if(estaUbicacionOcupadaPorEvento(eve->id, sectorElegido->id, filaIngresada, butacaIngresada))
        {
            printf(">> ERROR: La ubicacion [Fila %d, Butaca %d] ya se encuentra OCUPADA.\n", filaIngresada, butacaIngresada);
            compraValida = 0;
        }
        else
        {
            butacaDuplicada = 0;
            pCheck = nuevaCompra->Entradas;
            while(pCheck < pEntrada)
            {
                if(pCheck->fila == filaIngresada && pCheck->butaca == butacaIngresada)
                {
                    butacaDuplicada = 1;
                    break;
                }
                pCheck++;
            }

            if(butacaDuplicada)
            {
                printf(">> ERROR: Ya seleccionaste esta ubicacion en la presente compra.\n");
                compraValida = 0;
            }
            else
            {
                pEntrada->sector = (int)sectorElegido->id;
                pEntrada->fila = filaIngresada;
                pEntrada->butaca = butacaIngresada;
                pEntrada->precio = sectorElegido->precio;
                strcpy(pEntrada->idCompra, nuevaCompra->id);
                strcpy(pEntrada->estado, "VIGENTE");

                nuevaCompra->ImporteTotal += sectorElegido->precio;
                pEntrada++;
                i++;
            }
        }
    }

    return compraValida;
}

int guardarCompraYEntradas(const tCompra *nuevaCompra)
{
    FILE *archComp;
    FILE *archEnt;
    const tEntrada *pEntrada;
    int i;
    int exito;

    exito = 1;
    archComp = fopen("Compras.bin", "ab");
    archEnt = fopen("Entradas.bin", "ab");

    if(archComp == NULL || archEnt == NULL)
    {
        if(archComp != NULL) fclose(archComp);
        if(archEnt != NULL) fclose(archEnt);
        return 0;
    }

    if(fwrite(nuevaCompra, sizeof(tCompra), 1, archComp) != 1)
    {
        exito = 0;
    }
    else
    {
        pEntrada = nuevaCompra->Entradas;
        i = 0;
        while(i < nuevaCompra->cantEntradas && exito)
        {
            if(fwrite(pEntrada, sizeof(tEntrada), 1, archEnt) != 1)
            {
                exito = 0;
            }
            pEntrada++;
            i++;
        }
    }

    fclose(archComp);
    fclose(archEnt);
    return exito;
}

void procesarCompraEntradas(const char *idUsuario)
{
    char idEvento[50];
    tEvento eve;
    tEstablecimiento *est;
    const tSector *sectorElegido;
    int cantEntradas;
    tCompra nuevaCompra;
    const tSector *pSector;
    const tSector *pFinSector;

    sectorElegido = NULL;

    limpiarPantalla();
    printf("====================================================\n");
    printf("                 COMPRA DE ENTRADAS                 \n");
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
        printf("ERROR: No se encontro el establecimiento asociado.\n");
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

    sectorElegido = seleccionarSector(est);
    if(sectorElegido == NULL)
    {
        printf("ERROR: El sector seleccionado no existe o es invalido.\n");
        free(est);
        pausar();
        return;
    }

    mostrarMatrizUbicaciones(eve.id, sectorElegido);

    printf("Cuantas entradas desea comprar? (Maximo %d): ", MAX_ENTRADAS_POR_COMPRA);
    if(scanf("%d", &cantEntradas) != 1 || cantEntradas <= 0 || cantEntradas > MAX_ENTRADAS_POR_COMPRA)
    {
        vaciarBufferTeclado();
        printf("ERROR: Cantidad de entradas invalida o supera el maximo permitido.\n");
        free(est);
        pausar();
        return;
    }
    vaciarBufferTeclado();

    nuevaCompra.cantEntradas = cantEntradas;
    nuevaCompra.ImporteTotal = 0;
    strcpy(nuevaCompra.IdUsuario, idUsuario);
    strcpy(nuevaCompra.IdEvento, eve.id);
    generarCodigoCompra(eve.id, eve.nombre, nuevaCompra.id);

    if(!cargarUbicacionesCompra(&nuevaCompra, &eve, sectorElegido, cantEntradas))
    {
        printf("\nLa compra fue cancelada debido a errores en las ubicaciones seleccionadas.\n");
        free(est);
        pausar();
        return;
    }

    nuevaCompra.fechaCompra = eve.fechaHora;

    if(!mostrarYConfirmarResumen(&nuevaCompra, &eve, est, sectorElegido))
    {
        printf("\nCompra cancelada por el usuario.\n");
        free(est);
        pausar();
        return;
    }

    if(!guardarCompraYEntradas(&nuevaCompra))
    {
        printf("ERROR CRITICO: No se pudieron guardar los registros en los archivos binarios.\n");
        free(est);
        pausar();
        return;
    }

    free(est);
    printf("\n¡COMPRA REALIZADA CON EXITO! Codigo asignado: %s\n", nuevaCompra.id);
    pausar();
}

int mostrarYConfirmarResumen(const tCompra *nuevaCompra, const tEvento *eve, const tEstablecimiento *est, const tSector *sectorElegido)
{
    int confirmaCompra;

    confirmaCompra = 0;
    limpiarPantalla();
    printf("====================================================\n");
    printf("                 RESUMEN DE COMPRA                  \n");
    printf("====================================================\n");
    printf("Codigo de Compra   : %s\n", nuevaCompra->id);
    printf("Evento             : %s (%s)\n", eve->nombre, eve->id);
    printf("Establecimiento    : %s\n", est->nombre);
    printf("Sector             : %s\n", sectorElegido->nombre);
    printf("Cantidad Entradas  : %d\n", nuevaCompra->cantEntradas);
    printf("Precio Unitario    : $%.2f\n", sectorElegido->precio);
    printf("Importe Total      : $%.2f\n", nuevaCompra->ImporteTotal);
    printf("====================================================\n");

    printf("Desea confirmar la compra? (1: Si / 0: No): ");
    if(scanf("%d", &confirmaCompra) != 1)
    {
        vaciarBufferTeclado();
        confirmaCompra = 0;
    }
    vaciarBufferTeclado();

    return confirmaCompra;
}

void consultarEntradasUsuario(const char *idUsuario)
{
    FILE *archEnt;
    FILE *archComp;
    tEntrada ent;
    tCompra comp;
    tEvento eve;
    tEstablecimiento *est;
    int hayEntradas;
    char strFecha[15];
    char strHora[10];
    const tSector *pSector;
    const tSector *pFinSector;
    char nombreSector[30];
    int encontradaCompra;

    hayEntradas = 0;
    archEnt = fopen("Entradas.bin", "rb");

    limpiarPantalla();
    printf("====================================================\n");
    printf("                 MIS ENTRADAS COMPRADAS             \n");
    printf("====================================================\n");

    if(archEnt == NULL)
    {
        printf("No hay entradas registradas en el sistema.\n\n");
        pausar();
        return;
    }

    est = (tEstablecimiento*)malloc(sizeof(tEstablecimiento));
    if(est == NULL)
    {
        printf("ERROR: Memoria insuficiente.\n");
        fclose(archEnt);
        pausar();
        return;
    }

    while(fread(&ent, sizeof(tEntrada), 1, archEnt) == 1)
    {
        encontradaCompra = 0;
        archComp = fopen("Compras.bin", "rb");
        if(archComp != NULL)
        {
            while(!encontradaCompra && fread(&comp, sizeof(tCompra), 1, archComp) == 1)
            {
                if(strcmp(comp.id, ent.idCompra) == 0)
                {
                    encontradaCompra = 1;
                }
            }
            fclose(archComp);
        }

        if(encontradaCompra && strcmp(comp.IdUsuario, idUsuario) == 0)
        {
            if(obtenerEventoPorId(comp.IdEvento, &eve))
            {
                formatearFecha(eve.fechaHora, strFecha);
                formatearHora(eve.fechaHora, strHora);
                obtenerEstablecimientoPorId(eve.idEstablecimiento, est);

                strcpy(nombreSector, "Desconocido");
                pSector = est->sectores;
                pFinSector = est->sectores + MAX_SECTORES;
                while(pSector < pFinSector)
                {
                    if(pSector->id == (unsigned)ent.sector)
                    {
                        strcpy(nombreSector, pSector->nombre);
                        break;
                    }
                    pSector++;
                }

                printf("Codigo Compra   : %s\n", ent.idCompra);
                printf("Evento          : %s (%s)\n", eve.nombre, eve.id);
                printf("Fecha y Hora    : %s %s\n", strFecha, strHora);
                printf("Establecimiento : %s\n", est->nombre);
                printf("Sector          : %s (ID: %u)\n", nombreSector, ent.sector);
                printf("Ubicacion       : Fila %d, Butaca %d\n", ent.fila, ent.butaca);
                printf("Precio          : $%.2f\n", ent.precio);
                printf("Estado Entrada  : %s\n", ent.estado);
                printf("Estado Evento   : %s\n", eve.estado == 1 ? "DISPONIBLE" : "NO DISPONIBLE");
                printf("----------------------------------------------------\n");
                hayEntradas = 1;
            }
        }
    }

    free(est);
    fclose(archEnt);

    if(!hayEntradas)
    {
        printf("No posees entradas registradas a tu nombre.\n");
    }

    printf("\n");
    pausar();
}

void cancelarEntradasUsuario(const char *idUsuario)
{
    char idCompraBuscado[50];
    FILE *archComp;
    FILE *archEnt;
    tCompra comp;
    tEntrada ent;
    tEvento eve;
    int encontradaComp;
    int encontradaEnt;
    long long diferenciaTiempo;
    long long tiempoActual;
    int confirmacion;
    long offsetEntrada;

    encontradaComp = 0;
    encontradaEnt = 0;

    limpiarPantalla();
    printf("====================================================\n");
    printf("                CANCELACION DE ENTRADAS             \n");
    printf("====================================================\n");

    printf("Ingrese el Codigo de Compra a cancelar (9 caracteres): ");
    if(scanf("%49s", idCompraBuscado) != 1)
    {
        vaciarBufferTeclado();
        printf("Error en la lectura del codigo.\n");
        pausar();
        return;
    }
    vaciarBufferTeclado();

    archComp = fopen("Compras.bin", "rb");
    if(archComp == NULL)
    {
        printf("ERROR: No se pudo abrir el archivo de compras.\n");
        pausar();
        return;
    }

    while(!encontradaComp && fread(&comp, sizeof(tCompra), 1, archComp) == 1)
    {
        if(strcmp(comp.id, idCompraBuscado) == 0)
        {
            encontradaComp = 1;
        }
    }
    fclose(archComp);

    if(!encontradaComp)
    {
        printf("ERROR: No se encontro ninguna compra con el codigo '%s'.\n", idCompraBuscado);
        pausar();
        return;
    }

    if(strcmp(comp.IdUsuario, idUsuario) != 0)
    {
        printf("ERROR: La compra ingresada no pertenece a su usuario.\n");
        pausar();
        return;
    }

    if(!obtenerEventoPorId(comp.IdEvento, &eve))
    {
        printf("ERROR: No se encontro el evento asociado a la compra.\n");
        pausar();
        return;
    }

    tiempoActual = (long long)time(NULL);
    diferenciaTiempo = (long long)eve.fechaHora - tiempoActual;

    if(diferenciaTiempo <= 172800)
    {
        printf("ERROR: No es posible cancelar la compra.\n");
        printf("La cancelacion debe realizarse con mas de 48 horas de anticipacion al evento[cite: 3].\n");
        pausar();
        return;
    }

    printf("\n--- DATOS DE LA COMPRA A CANCELAR ---\n");
    printf("Codigo de Compra : %s\n", comp.id);
    printf("Evento           : %s\n", eve.nombre);
    printf("Cantidad Entradas: %d\n", comp.cantEntradas);
    printf("Importe Total    : $%.2f\n", comp.ImporteTotal);
    printf("----------------------------------------------------\n");
    printf("Desea confirmar la cancelacion? (1: Si / 0: No): ");
    if(scanf("%d", &confirmacion) != 1 || confirmacion != 1)
    {
        vaciarBufferTeclado();
        printf("\nOperacion cancelada por el usuario.\n");
        pausar();
        return;
    }
    vaciarBufferTeclado();

    archEnt = fopen("Entradas.bin", "r+b");
    if(archEnt == NULL)
    {
        printf("ERROR: No se pudo abrir el archivo de entradas para actualizar.\n");
        pausar();
        return;
    }

    while(fread(&ent, sizeof(tEntrada), 1, archEnt) == 1)
    {
        if(strcmp(ent.idCompra, comp.id) == 0)
        {
            if(strcmp(ent.estado, "VIGENTE") == 0)
            {
                strcpy(ent.estado, "CANCELADA");
                offsetEntrada = ftell(archEnt) - (long)sizeof(tEntrada);
                fseek(archEnt, offsetEntrada, SEEK_SET);
                fwrite(&ent, sizeof(tEntrada), 1, archEnt);
                fseek(archEnt, offsetEntrada + (long)sizeof(tEntrada), SEEK_SET);
                encontradaEnt = 1;
            }
        }
    }

    fclose(archEnt);

    if(encontradaEnt)
    {
        printf("\n¡COMPRA Y ENTRADAS CANCELADAS EXITOSAMENTE!\n");
        printf("Las ubicaciones asociadas han vuelto a estar disponibles[cite: 3].\n");
    }
    else
    {
        printf("\nAviso: Las entradas de esta compra ya se encontraban canceladas o no se encontraron.\n");
    }

    pausar();
}
