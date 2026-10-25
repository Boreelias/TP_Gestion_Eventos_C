#include "usuario.h"
#include "estructuras.h"
#include "evento.h"
#include "establecimiento.h"
#include "ubicacion.h"
#include "util.h"
#include "compra.h"

int iniciarSistema(int *perfil, char *idUsuario)
{
    char buffer[50];
    int opcion = -1;

    limpiarPantalla();
    printf("==================================================\n");
    printf("                GESTOR DE EVENTOS                 \n");
    printf("==================================================\n");
    printf("Seleccione el perfil:\n");
    printf("1. ADMINISTRADOR\n");
    printf("2. USUARIO\n");
    printf("0. SALIR\n");
    printf("--------------------------------------------------\n");
    printf("OPCION: ");

    if(scanf("%d", &opcion) != 1)
    {
        vaciarBufferTeclado();
        printf("\nOpcion invalida!\n");
        pausar();
        return 0;
    }
    vaciarBufferTeclado();

    *perfil = opcion;
    limpiarPantalla();

    switch(*perfil)
    {
        case 1:
            printf("\n----- Sesion iniciada como ADMINISTRADOR -----\n\n");
            pausar();
            return 1;

        case 2:
            printf("\n----- Sesion iniciada como USUARIO -----\n");
            do
            {
                printf("Ingrese su ID de usuario (exactamente 8 caracteres): ");
                scanf("%49s", buffer);
                vaciarBufferTeclado();

                if(strlen(buffer) != 8)
                {
                    printf("ERROR: El ID de usuario debe tener exactamente 8 caracteres.\n\n");
                }
            } while(strlen(buffer) != 8);

            strcpy(idUsuario, buffer);
            limpiarPantalla();
            printf("==================================================\n");
            printf("                   BIENVENIDO                     \n");
            printf("==================================================\n");
            printf("Sesion iniciada como: %s\n\n", idUsuario);
            pausar();
            return 1;

        case 0:
            return 0;

        default:
            printf("Opcion invalida! Intente nuevamente.\n\n");
            pausar();
            return 0;
    }
}

void ejecutarMenuUsuario(const char *idUser)
{
    int opcionUser = 0;
    do
    {
        limpiarPantalla();
        printf("==================================================\n");
        printf("             MENU USUARIO (ID: %s)                \n", idUser);
        printf("==================================================\n");
        printf("1. Consultar eventos disponibles.\n");
        printf("2. Consultar establecimiento de un evento.\n");
        printf("3. Consultar ubicaciones y disponibilidad de un sector.\n");
        printf("4. Comprar entradas.\n");
        printf("5. Consultar entradas.\n");
        printf("6. Cancelar entradas.\n");
        printf("0. Cerrar sesion.\n");
        printf("--------------------------------------------------\n");
        printf("Opcion: ");

        if(scanf("%d", &opcionUser) != 1)
        {
            vaciarBufferTeclado();
            opcionUser = -1;
            continue;
        }
        vaciarBufferTeclado();

        switch(opcionUser)
        {
            case 1:
                consultarEventosDisponibles();
                break;
            case 2:
                consultarEstablecimiento();
                break;
            case 3:
                consultarUbicacionesYDisponibilidad();
                break;
            case 4:
                procesarCompraEntradas(idUser);
                break;
            case 5:
                consultarEntradasUsuario(idUser);
                break;
            case 6:
                cancelarEntradasUsuario(idUser);
                break;
            case 0:
                printf("\nCerrando sesion...\n");
                break;
            default:
                printf("\nNumero ingresado incorrecto! Ingrese nuevamente.\n\n");
                pausar();
        }
    } while(opcionUser != 0);
}

void ejecutarMenuAdministrador(void)
{
    int opcionAdmin = 0;
    do
    {
        limpiarPantalla();
        printf("==================================================\n");
        printf("                MENU ADMINISTRADOR                \n");
        printf("==================================================\n");
        printf("1. Registrar nuevo evento (Alta).\n");
        printf("2. Consultar establecimiento de un evento.\n");
        printf("3. Modificar estado del evento.\n");
        printf("4. Consultar ventas de un evento.\n");
        printf("5. Busqueda eficiente de eventos.\n");
        printf("6. Conteo de entradas por criterio.\n");
        printf("7. Reporte de ocupacion de un evento.\n");
        printf("0. Cerrar sesion.\n");
        printf("--------------------------------------------------\n");
        printf("Opcion: ");

        if(scanf("%d", &opcionAdmin) != 1)
        {
            vaciarBufferTeclado();
            opcionAdmin = -1;
            continue;
        }
        vaciarBufferTeclado();

        switch(opcionAdmin)
        {
            case 1:
                altaEvento();
                break;
            case 2:
                consultarEstablecimiento();
                break;
            case 3:
                modificarEstadoEvento();
                break;
            case 4:
                consultarVentasEvento();
                break;
            case 5:
                busquedaEficienteEvento();
                break;
            case 6:
                conteoEntradasCriterio();
                break;
            case 7:
                reporteOcupacionEvento();
                break;
            case 0:
                printf("\nCerrando sesion...\n");
                break;
            default:
                printf("\nNumero ingresado incorrecto! Ingrese nuevamente.\n\n");
                pausar();
        }
    } while(opcionAdmin != 0);
}
