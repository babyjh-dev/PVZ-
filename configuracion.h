#ifndef configuracion_h
#define configuracion_h

#include "estructuras.h"

/* Lee los datos del archivo y los almacena en Configuracion
 Retorna 1 si la lectura fue exitosa y 0 si ocurrio un error*/
int cargarConfiguracion(Configuracion *config, const char *nombreArchivo);

// Libera la memoria dinamica reservada para plantas y zombies
void liberarConfiguracion(Configuracion *config);

#endif
