#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "dispersion.h"

int main()
{ regConfig reg;
  int error;
  char fEntrada[20],fSalida[20];
	#ifdef ALUM 
 		strcpy(fEntrada,"alumnos.dat");
 		strcpy(fSalida,"alumnosC.hash");
	  	reg.nCubos=15;
	#endif  	  	
	#ifdef ASIG
 		strcpy(fEntrada,"asignaturas.dat");
 		strcpy(fSalida,"asignaturasC.hash");
	  	reg.nCubos=10;
	#endif  	

  	//reg.nCubos=15;
  	reg.nCubosDes=4;	// Número de cubos area de desborde
  	reg.nCuboDesAct = reg.nCubos;	
	reg.densidadMax=80;
	reg.densidadMin=40;
	reg.numReg=0;
	reg.numRegDes=0;

	error = creaHash(fEntrada,fSalida,&reg);

  	switch (error) {
  	case -1:
  	case -2:	
  	case -5 : {printf("Error %d en la función creaHash\n", error);
  		  return -1;  }
  	case -3: printf("Error (%d): Se supera densidad máxima de ocupación\n", error);break;
  	case -4: printf("Error (%d): No se alcaza densidad mínima de ocupación\n", error);break;
  	}
  	leeHash(fSalida); 	
}
