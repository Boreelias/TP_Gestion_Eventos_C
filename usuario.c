#include "usuario.h"
#include "estructuras.h"
#include "evento.h"


int iniciarSistema(int *perfil, char *idUsuario)
{
    system("cls");
    printf("=======GESTOR DE EVENTOS=======\n");
    printf("Seleccione el perfil: \n");
    printf("1. ADMINISTRADOR.\n");
    printf("2. USUARIO.\n");
    printf("0. SALIR.\n");
    printf("OPCION: ");
    scanf("%d", perfil);

    system("cls");
    switch(*perfil)
    {
        case 1: printf("\n-----Sesion iniciada como ADMINISTRADOR-----");
            return 1;

        case 2: printf("\n-----Sesion iniciada como USUARIO-----\n");
            char buffer[50];
                do
                {
                    printf("Ingrese el ID del usuario: ");
                    scanf("%s", buffer);

                    if(strlen(buffer) != 8)
                        printf("ERROR: El ID debe tener exactamente 8 caracteres\n");

                }while(strlen(buffer) != 8);

            strcpy(idUsuario, buffer);
            system("cls");
            printf("========BIENVENIDO========\n");
            printf("Sesion iniciada como: %s", idUsuario);
            return 1;

        case 0:
            return 0;

        default:
            printf("Opcion invalida!\nIntente nuevamente\n\n");
    }
    return 0;
}

void ejecutarMenuUsuario(const char *idUser)
{
    int opcionUser = 0;
    do
    {
        system("cls");
        printf("==================================================\n");
        printf("             MENU USUARIO (ID: %s)                \n", idUser);
        printf("==================================================\n");

        printf("1. Consultar eventos disponibles.\n");
        printf("0. Salir.\n");
        printf("Opcion: ");
        scanf("%d", &opcionUser);

        switch(opcionUser)
        {
        case 1:
            consultarEventosDisponibles();
            break;
        case 0:
            printf("Cerrando sesion...");
            break;
        default:
            printf("Numero ingresado incorrecto!\nIngrese nuevamente\n\n");
        }
    }while(opcionUser != 0);
}

void ejecutarMenuAdministrador()
{
    int opcionAdmin = 0;
    do
    {
        system("cls");
        printf("==================================================\n");
        printf("                MENU ADMINISTRADOR                \n");
        printf("==================================================\n");

        printf("1. Ingresar nuevo evento.\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcionAdmin);

        switch(opcionAdmin)
        {
        case 1:
            altaEvento();
            break;
        case 0:
            printf("Cerrando sesion...");
            break;
        default:
            printf("Numero ingresado incorrecto!\nIngrese nuevamente\n\n");
            system("pause");
        }
    }while(opcionAdmin != 0);
}
