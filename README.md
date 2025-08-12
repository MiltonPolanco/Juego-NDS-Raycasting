# Juego-NDS-Raycasting

Juego de raycasting para Nintendo DS, desarrollado en C++ usando libnds y devkitARM.

## Características

- Motor de raycasting optimizado para Nintendo DS.
- Gráficos en modo 2D con paleta de colores.
- Minimapa en pantalla con indicador de dirección.
- Controles adaptados a la consola (flechas, A/B para strafe izquierda - derecha, L/R para velocidad lento - rapido).
- Objetivo: encontrar la sala dorada para ganar la partida.
- Pantalla de victoria.

## Controles

- **Flechas:** Moverse y girar.
- **A/B:** Strafe izquierda/derecha.
- **L/R:** Cambiar velocidad de movimiento.
- **START:** Comenzar juego o salir.

## Compilación y Ejecución

Este proyecto utiliza un archivo `Makefile` especialmente preparado para compilar juegos de Nintendo DS usando devkitARM y libnds. El Makefile detecta automáticamente los archivos fuente y organiza la compilación, enlazado y generación del archivo `.nds` listo para emulador o hardware real.

### Pasos para compilar

1. Instala [devkitPro](https://devkitpro.org/) y asegúrate de tener la variable de entorno `DEVKITARM` configurada.
2. Abre una terminal en la carpeta del proyecto.
3. Ejecuta:

    ```sh
    make
    ```

Durante la compilación, verás mensajes como estos en la consola:

```
main.cpp
linking Juego-NDS-Raycasting.elf
Nintendo DS rom tool 2.3.1 - Jan  8 2024
by Rafael Vuijk, Dave Murphy, Alexei Karpenko
built ... Juego-NDS-Raycasting.nds
```

Estos nombres y herramientas corresponden al proceso estándar de devkitPro y libnds para crear archivos `.nds`. El juego, su código y diseño son hechos por mí; los nombres y mensajes corresponden únicamente a las herramientas de desarrollo utilizadas.

El archivo resultante `Juego-NDS-Raycasting.nds` estará en el directorio del proyecto y puedes ejecutarlo en un emulador de Nintendo DS (de preferencia melonDS) o en hardware real.

## Estructura del Proyecto

- `source/`: Código fuente principal (`main.cpp`).
- `build/`: Archivos intermedios generados por la compilación.

## Créditos

- Milton Giovanni Polanco Serrano - 23471
- Nintendo DS rom tool 2.3.1 - Jan  8 2024 - by Rafael Vuijk, Dave Murphy, Alexei Karpenko

## Videos

- [Video prueba en emulador MelonDS](https://youtu.be/r1tPHpRhXZE)
- [Video prueba en DS XL (Mismo juego/.nds)](https://youtu.be/qY5BNL4acXc)