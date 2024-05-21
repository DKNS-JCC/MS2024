#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "dispersion.h"

int main()
{ int nCubo,nCuboDes, posReg, error,bytesDesp;
  tipoReg *reg;
  char dni[9],provincia[11];
  int salir=1,i;
 

	#ifdef ALUM 
		error = buscar("alumnosC.hash","8663873");
		error = modificar("alumnosC.hash","8663873","Sevilla");
		error = buscar("alumnosC.hash","8663873");
	#endif  	
	#ifdef ASIG
		error = buscar("asignaturasC.hash", 101116);
		error= modificar("asignaturasC.hash", 101116,4.5, 1.5);
		error = buscar("asignaturasC.hash", 101116);
	#endif
	if (error)  printf("Error %d en la función modificar\n", error);
}


