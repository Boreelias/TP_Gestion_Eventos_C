#include "estructuras.h"
#include "usuario.h"
#include "util.h"

int main(void)
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
                    ejecutarMenuAdministrador();
                    break;

                case 2: //MENU USUARIO
                    ejecutarMenuUsuario(idUsuario);
                    break;
            }
        }
    }while(perfil != 0);

    limpiarPantalla();
    printf("=====SISTEMA FINALIZADO=====\n");

    return 0;
}
