# Generación de fichero por el método de dispersión

genAlumnosH: genFicheroHash.c dispersion.h dispersionAl.o alumno.o
	gcc -DALUM -g genFicheroHash.c dispersionAl.o alumno.o -o genAlumnosH
genAsignaturasH: genFicheroHash.c dispersion.h dispersionAs.o asignatura.o 
	gcc -DASIG -g genFicheroHash.c dispersionAs.o asignatura.o -o genAsignaturasH

# Modificación de registros 

modAlumnoH: modFicheroHash.c dispersionAl.o	alumno.o
	gcc -DALUM -g modFicheroHash.c dispersionAl.o alumno.o -o modAlumnoH

modAsignaturaH: modFicheroHash.c dispersionAs.o	asignatura.o
	gcc -DASIG -g modFicheroHash.c dispersionAs.o asignatura.o -o modAsignaturaH

# funciones genéricas de dispersión

dispersionAl.o: dispersion.h dispersion.c
	gcc -DALUM -c -g dispersion.c -o dispersionAl.o
#	gcc -g dispersion.c -o dispersionAl.o
dispersionAs.o: dispersion.h dispersion.c
	gcc -DASIG -c -g dispersion.c -o dispersionAs.o
	
# funciones especificas (dependen de tipoReg)

asignatura.o: asignatura.h asignatura.c
	gcc -DASIG -c -g asignatura.c
alumno.o: alumno.h alumno.c
	gcc -DALUM -c -g alumno.c
	
limpiar:
	rm *.o
