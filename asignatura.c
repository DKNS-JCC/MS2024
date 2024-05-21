#include <stdlib.h>
#include "asignatura.h"
#include <stdio.h>
#include <string.h>
#include "dispersion.h"

int funcionHash(tAsignatura *reg, int nCubos)
{
    if (reg == NULL)
        return -1;
    if (nCubos <= 0)
        return -1;
    return reg->codigo % nCubos; // Calcula la posición del cubo en el que se almacenará el registro
}
void mostrarReg(tAsignatura *reg)
{
    if (reg == NULL)
        return;
    printf("%d | ", reg->codigo);
    printf("%s  ", reg->nombre);
    printf("%c  ", reg->curso);
    printf("%f | ", reg->creditosT);
    printf("%f  ", reg->creditosP);
    printf("%c  ", reg->tipo);
    printf("%c  ", reg->cuatrimestre);
    printf("%d  ", reg->numGrT);
    printf("/ %d\n", reg->numGrP);

}
int cmpClave(tAsignatura *reg1, tAsignatura *reg2)
{
    if (reg1 == NULL || reg2 == NULL)
        return -1;
    if (reg1->codigo == reg2->codigo)
        return 0;
    else
        return 1;
}

int buscar(char *fichero, int codigo)
{
    tPosicion pos;
    tAsignatura reg;

    int salida;

    FILE *fHash = fopen(fichero, "rb");
    if (fHash == NULL)
    {
        return -1;
    }

    codigo = reg.codigo;
    salida = busquedaHash(fHash, &reg, &pos);
    if (salida == -1)
    {
        puts("No se ha encontrado el registro");
    }
    else
    {
        puts("Registro encontrado");
        mostrarReg(&reg);
    }
    fclose(fHash);
    return salida;
}
int modificar(char *fichero, int codigo, float creditosT, float creditosP)
{
    tPosicion pos;
    tAsignatura reg;

    int salida;

    FILE *fHash = fopen(fichero, "rb+");
    if (fHash == NULL)
    {
        return -1;
    }

    codigo = reg.codigo;
    salida = busquedaHash(fHash, &reg, &pos);
    if (salida == -1)
    {
        puts("No se ha encontrado el registro");
    }
    else
    {
        reg.creditosT = creditosT;
        reg.creditosP = creditosP;
        fseek(fHash, sizeof(regConfig) + pos.cubo * sizeof(tipoCubo), SEEK_SET);
        fwrite(&reg, sizeof(tAsignatura), 1, fHash);
        puts("\nRegistro modificado\n");
    }
    fclose(fHash);
    return salida;
}
