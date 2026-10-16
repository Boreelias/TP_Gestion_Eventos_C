#include "estructuras.h"
#include "usuario.h"

int main()
{
    int perfil = 0;
    char idUsuario[9];

    do
    {
        if(iniciarSistema(&perfil, idUsuario))
        {
            switch(perfil)
            {
                case 1: //MENU ADMINISTRADOR
                    printf("\n[ACA VA EL MENU DE ADMINISTRADOR]\n");
                    system("pause");
                    break;

                case 2: //MENU USUARIO
                    ejecutarMenuUsuario(idUsuario);
                    break;
            }
        }
    }while(perfil != 0);

    system("cls");
    printf("=====SISTEMA FINALIZADO=====");

    return 0;
}
