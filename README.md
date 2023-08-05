# Esquemas de detección y corrección de errores

Universidad del Valle de Guatemala

Facultad de Ingeniería

Departamento de Ciencias de la Computación

Redes - Sección 10

Laboratorio 2

## => Código Hamming

### C++ - Emisor

```
g++ hamming/hamming.cpp -o hamming/hamming
./hamming/hamming plot_bits
```

Ejemplo:

```
./hamming/hamming 1011001
Input: 1011001
Output: 10101001110
r: 4
```

### Java - Receptor

```
javac hamming/Hamming.java
java hamming/Hamming plot_bits
```

Ejemplo:

```
java hamming/Hamming 10101001110 10101001110
Input: 10101001110
Output: Todo bien 10101001110
CORRECTO
```

En caso de error (Donde se agregara 1 en el 4to caracter):

```
java hamming/Hamming 10111001110 10101001110
Input: 10111001110
Errores encontrados
10111001110
   |
Trama de bits Arreglado: 
Output: 10101001110
CORRECTO
```

En caso de error donde es imposible arreglar los bits:

```
java hamming/Hamming 10111000101 10101001110
Input: 10111000101
Este error no es posible de corregir
Output: -1
INCORRECTO
```

En caso de error (Donde hay más de dos bits erroneos es poco probable que se arregle la cadena de bits)

```
java hamming/Hamming 10111001101 10101001110
Input: 10111001101
Errores encontrados
10111001101
|
Trama de bits Arreglado:
Output: 00111001101
INCORRECTO
```