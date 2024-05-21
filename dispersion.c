#include "dispersion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Lee el contenido del fichero hash organizado mediante el método de DISPERSIÓN según los criterios
// especificados en la práctica. Se leen todos los cubos completos tengan registros asignados o no. La
// salida que produce esta función permite visualizar el método de DISPERSIÓN

int leeHash(char *fichHash)
{
   FILE *f;
   tipoCubo cubo;
   regConfig regC;
   int j, nCubo = 0, densidadOcupacion;

   if ((f = fopen(fichHash, "rb")) == NULL)
      return -2;
   fread(&regC, sizeof(regConfig), 1, f);
   fread(&cubo, sizeof(cubo), 1, f);
   while (!feof(f))
   {
      for (j = 0; j < C; j++)
      {
         if (j == 0)
            printf("Cubo %2d (%2d reg. ASIGNADOS)", nCubo, cubo.numRegAsignados);
         else if ((j == 1) && cubo.desbordado)
            printf("DESBORDADO\t\t");
         else
            printf("\t\t\t");
         if (j < cubo.numRegAsignados)
            mostrarReg(&(cubo.reg[j]));
         else
            printf("\n");
      }
      nCubo++;
      fread(&cubo, sizeof(cubo), 1, f);
   }
   fclose(f);
   printf("ORGANIZACIÓN MÉTODO DISPERSIÓN:\n\t %d CUBOS y %d CUBOS DESBORDE con capacidad %d\n",
          regC.nCubos, regC.nCubosDes, C);
   printf("\t Contiene %d registros de los cuales se han desbordado %d:\n", regC.numReg, regC.numRegDes);

   densidadOcupacion = 100 * regC.numReg / ((regC.nCubos + regC.nCubosDes) * C);
   printf("Densidad ACTUAL de ocupación: %d \n\t(MÍNIMA permitida %d  MÁXIMA permitida %d)\n",
          densidadOcupacion, regC.densidadMin, regC.densidadMax);

   if (densidadOcupacion > regC.densidadMax)
   {
      printf("No se respeta la densidad máxima de ocupacion\n");
   }

   if (densidadOcupacion < regC.densidadMin)
   {
      printf("No se respeta la densidad mínima de ocupacion\n");
   }
   return 0;
}
// Crea un fichero hash vacío con la estructura necesaria para almacenar registros de tipo tipoReg.
int creaHvacio(char *fichHash, regConfig *reg)
{
   FILE *f;
   tipoCubo cubo;
   int i;
   int numCubos = reg->nCubos + reg->nCubosDes; // Numero total de cubos
   reg->numReg = 0;                             // Inicializamos el numero de registros
   reg->numRegDes = 0;                          // Inicializamos el numero de registros desbordados
   reg->nCuboDesAct = reg->nCubos;              // Inicializamos el numero de cubo desborde actual (Ultimo cubo de la zona de cubos)

   memset(&cubo, 0, sizeof(tipoCubo)); // Inicializamos el cubo a 0

   if ((f = fopen(fichHash, "wb")) == NULL)
      return -2;

   for (i = 0; i <= numCubos; i++)
   {
      fwrite(&cubo, sizeof(tipoCubo), 1, f); // Escribimos el cubo en el fichero
   }

   fclose(f);
   return 0;
}
// Inserta un registro en el fichero hash. Si el cubo en el que se debe insertar el registro está lleno
int desborde(FILE *fHash, tipoReg *reg, regConfig *regC)
{
   tipoCubo cubo;

   if (fseek(fHash, sizeof(regConfig) + regC->nCuboDesAct * sizeof(tipoCubo), SEEK_SET) != 0)
   {
      perror("Error posicionando el archivo");
      return -2;
   }
   if (fread(&cubo, sizeof(tipoCubo), 1, fHash) != 1)
   {
      perror("Error leyendo el cubo");
      return -2;
   }

   cubo.reg[cubo.numRegAsignados] = *reg; // Asignamos el registro al cubo
   cubo.numRegAsignados++;                // Incrementamos el numero de registros asignados

   if (cubo.numRegAsignados == C) // Si el cubo esta lleno
   {
      regC->nCuboDesAct++; // Incrementamos el numero de cubo desborde actual

      if (regC->nCuboDesAct >= regC->nCubos + regC->nCubosDes) // Si se sale del numero de cubos desborde crea un nuevo cubo desborde
      {
         if (fseek(fHash, sizeof(regConfig) + regC->nCuboDesAct * sizeof(tipoCubo), SEEK_SET) != 0)
         {
            perror("Error posicionando el archivo");
            return -2;
         }
         {
            perror("Error posicionando el archivo");
            return -2;
         }

         memset(&cubo, 0, sizeof(tipoCubo)); // Inicializamos el cubo a 0

         if (fwrite(&cubo, sizeof(tipoCubo), 1, fHash) != 1)
         {
            perror("Error escribiendo el cubo");
            return -2;
         }
      }

      return 0;
   }
   return 0;
}
// Crea un fichero hash a partir de un fichero de registros de tipo tipoReg. La función crea un fichero hash
int creaHash(char *fichEntrada, char *fichHash, regConfig *regC)
{
   FILE *fEntrada, *fHash;
   tipoReg regtemp;
   int hash = 0;
   tipoCubo cubo;
   int density;

   if (creaHvacio(fichHash, regC) != 0)
      return -5;

   fEntrada = fopen(fichEntrada, "r");
   if (fEntrada == NULL)
      return -1;

   fHash = fopen(fichHash, "rb+");
   if (fHash == NULL)
      return -2;

   while (fread(&regtemp, sizeof(tipoReg), 1, fEntrada) == 1)
   {
      hash = funcionHash(&regtemp, regC->nCubos);                          // Calculamos la posicion del cubo
      fseek(fHash, sizeof(regConfig) + hash * sizeof(tipoCubo), SEEK_SET); // Nos posicionamos en el cubo correspondiente
      fread(&cubo, sizeof(tipoCubo), 1, fHash);                            // Leemos el cubo

      if (cubo.numRegAsignados < C) // Cubo no lleno
      {
         cubo.reg[cubo.numRegAsignados] = regtemp; // Asignamos el registro al cubo
         cubo.numRegAsignados++;                   // Incrementamos el numero de registros asignados
         regC->numReg++;

         fseek(fHash, -sizeof(tipoCubo), SEEK_CUR); // Nos posicionamos en el cubo (inicio) poner el menos porque soy escoria
         fwrite(&cubo, sizeof(tipoCubo), 1, fHash); // Escribimos el cubo
      }
      else
      {
         cubo.desbordado = 1; // Marcamos el cubo como desbordado
         regC->numRegDes++;   // Incrementamos el numero de registros desbordados
         regC->numReg++;      // Incrementamos el numero de cubo desborde actual

         fseek(fHash, -sizeof(tipoCubo), SEEK_CUR); // Nos posicionamos en el cubo
         fwrite(&cubo, sizeof(tipoCubo), 1, fHash); // Escribimos el cubo

         if (desborde(fHash, &regtemp, regC) != 0)
            return -5;
      }
   }

   fseek(fHash, 0, SEEK_SET);                 // Nos posicionamos al principio del fichero
   fwrite(regC, sizeof(regConfig), 1, fHash); // Escribimos la configuracion del fichero

   fclose(fEntrada);
   fclose(fHash);

   density = 100 * (regC->numReg / ((regC->nCubos + regC->nCubosDes) * C)); // Calculamos la densidad de ocupacion

   if (density > regC->densidadMax) // Densidad superada
   {
      return -3;
   }
   else if (density < regC->densidadMin) // Densidad no alcanzada
   {
      return -4;
   }
   else
   {
      return 0;
   }
}

// Parte 2. Funciones genercias para el acceso a los ficheros
int busquedaHash(FILE *fHash, tipoReg *reg, tPosicion *posicion)
{
   tipoCubo cubo;
   regConfig regC;
   fread(&regC, sizeof(regConfig), 1, fHash);

   int hash = funcionHash(reg, regC.nCubos); // Calculamos la posicion del cubo

   fseek(fHash, sizeof(regConfig) + hash * sizeof(tipoCubo), SEEK_SET); // Nos posicionamos en el cubo correspondiente
   fread(&cubo, sizeof(tipoCubo), 1, fHash);

   for (int i = 0; i < cubo.numRegAsignados; i++)
   {
      if (cmpClave(reg, &cubo.reg[i]) == 0) // Si encontramos el registro
      {
         posicion->cubo = hash;  // Guardamos la posicion del cubo
         posicion->cuboDes = -1; // No esta desbordado
         posicion->posReg = i;   // Guardamos la posicion del registro
         *reg = cubo.reg[i];     // Guardamos el registro
         return 0;
      }
   }
   if (cubo.desbordado == 1) // Si el cubo esta desbordado
   {
      int cubosDes = regC.nCubos;
      while (1)
      {
         fseek(fHash, sizeof(regConfig) + cubosDes * sizeof(tipoCubo), SEEK_SET); // Nos posicionamos en el cubo desborde
         if (fread(&cubo, sizeof(tipoCubo), 1, fHash) != 1)
         {
            perror("Error leyendo el cubo");
            return -2;
         }
         for (int i = 0; i < cubo.numRegAsignados; i++)
         {
            if (cmpClave(reg, &cubo.reg[i]) == 0) // Si encontramos el registro
            {
               posicion->cubo = hash;        // Guardamos la posicion del cubo
               posicion->cuboDes = cubosDes; // Guardamos la posicion del cubo desborde
               posicion->posReg = i;         // Guardamos la posicion del registro
               *reg = cubo.reg[i];           // Guardamos el registro
               return 0;
            }
         }
         cubosDes++;
         if (cubosDes >= regC.nCubos + regC.nCubosDes) // Si no hay mas cubos desborde
         {
            break;
         }
      }
      return 1;
   }
}

int modificarReg(FILE *fHash, tipoReg *reg, tPosicion *posicion)
{
   regConfig regC;
   tipoCubo cubo;

   if (posicion->posReg == -1) // Si no se ha encontrado el registro
   {
      fseek(fHash, sizeof(regConfig) + posicion->cubo * sizeof(tipoCubo), SEEK_SET); // Nos posicionamos en el cubo
      fread(&cubo, sizeof(tipoCubo), 1, fHash);
   }
   else
   {
      fseek(fHash, sizeof(regConfig) + posicion->cuboDes * sizeof(tipoCubo), SEEK_SET); // Nos posicionamos en el cubo de desborde
      fread(&cubo, sizeof(tipoCubo), 1, fHash);
   }

   cubo.reg[posicion->posReg] = *reg; // Modificamos el registro

   fseek(fHash, -sizeof(tipoCubo), SEEK_SET); // Nos posicionamos en el cubo
   fwrite(&cubo, sizeof(tipoCubo), 1, fHash); // Escribimos el cubo

   return 0;
}
