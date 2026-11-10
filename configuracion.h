
#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include "estructuras.h"

/* Lee los datos del archivo y los almacena en Configuracion.
   Retorna 1 si la lectura fue exitosa y 0 si ocurrió un error. */
int cargarConfiguracion(Configuracion *config, const char *nombreArchivo);

/* Libera la memoria dinámica reservada para plantas y zombies. */
void liberarConfiguracion(Configuracion *config);

#endif
