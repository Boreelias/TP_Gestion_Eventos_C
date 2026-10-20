#include "estructuras.h"
#include "establecimiento.h"

int existeEstablecimiento(unsigned idBuscado)
{
    tEstablecimiento est;
    int encontrado = 0;

    FILE *archEst = fopen("Establecimientos.bin","rb");
    if(archEst == NULL)
    {
        printf("El archivo Establecimientos.bin no existe\n\n");
        system("pause");
        exit(-1);
    }

    fread(&est, sizeof(tEstablecimiento),1,archEst);
    while(!feof(archEst))
    {
        if(est.id == idBuscado)
        {
            encontrado = 1;
            return encontrado;
        }
        fread(&est, sizeof(tEstablecimiento),1,archEst);
    }

    fclose(archEst);
    return encontrado;
}
