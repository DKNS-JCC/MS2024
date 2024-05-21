#include <stdlib.h>
#include "alumno.h"
#include <stdio.h>
#include <string.h>
#include "dispersion.h"

int funcionHash(tAlumno *reg, int nCubos)
{
    if (reg == NULL)
        return -1;
    if (nCubos <= 0)
        return -1;
    return atoi(reg->dni) % nCubos; // Calcula la posición del cubo en el que se almacenará el registro
}

void mostrarReg(tAlumno *reg)
{
    if (reg == NULL)
        return;
    printf("DNI: %s\n", reg->dni);
    printf("Nombre: %s\n", reg->nombre);
    printf("Primer apellido: %s\n", reg->ape1);
    printf("Segundo apellido: %s\n", reg->ape2);
    printf("Provincia: %s\n", reg->provincia);
}
int cmpClave(tAlumno *reg1, tAlumno *reg2)
{
    if (reg1 == NULL || reg2 == NULL)
        return -1;
    return strcmp(reg1->dni, reg2->dni);
}

int buscar(char *fichero, char *dni)
{
    tPosicion pos;
    tAlumno reg;
    int salida;

    FILE *fHash = fopen(fichero, "rb");
    if (fHash == NULL)
    {
        return -1;
    }
    strcpy(reg.dni, dni);
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
int modificar(char *fichero, char *dni, char *provincia)
{
    tPosicion pos;
    tAlumno reg;

    int salida;

    FILE *fHash = fopen(fichero, "rb+");
    if (fHash == NULL)
    {
        return -1;
    }

    strcpy(reg.dni, dni);
    salida = busquedaHash(fHash, &reg, &pos);
    if (salida == -1)
    {
        puts("No se ha encontrado el registro");
    }
    else
    {
        strcpy(reg.provincia, provincia);
        fseek(fHash, sizeof(regConfig) + pos.cubo * sizeof(tipoCubo), SEEK_SET);
        fwrite(&reg, sizeof(tAlumno), 1, fHash);
        puts("\nRegistro modificado\n");
    }
    fclose(fHash);
    return salida;
}
