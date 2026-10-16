#include "usuario.h"
#include "estructuras.h"


int iniciarSistema(int *perfil, char *idUsuario)
{

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
                        printf("ERROR: El ID debe tener exactamente 8 caracteres\nIngrese nuevamente: ");

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
