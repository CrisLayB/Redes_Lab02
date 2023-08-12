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
./hamming/hamming
```

Ejemplo:

```
./hamming/hamming
```

```
Input: 1011001
Output: 10101001110
r: 4
```

### Java - Receptor

```
javac hamming/Hamming.java
java hamming/Hamming
```

Ejemplo:

```
Input: 10101001110
Output: Todo bien 10101001110
CORRECTO
```