# Ejemplos de Analizador Léxico y Sintáctico

Este directorio contiene ejemplos básicos para un curso de compiladores, mostrando cómo crear un analizador léxico y sintáctico en C++.

## Archivos incluidos

- **archivo.txt**: Archivo de ejemplo con código fuente a analizar
- **main.cpp**: Implementación básica de un analizador léxico y sintáctico

## Cómo compilar y ejecutar

### En Linux/Mac:
```bash
g++ -o main main.cpp -std=c++11
./main
```

### En Windows (MinGW):
```bash
g++ -o main.exe main.cpp -std=c++11
main.exe
```

## Descripción del programa

El programa implementa:

1. **Analizador Léxico**: Convierte el código fuente en tokens
   - Reconoce palabras clave (int, float, if, while, for, return)
   - Identifica identificadores (nombres de variables)
   - Reconoce números (enteros y decimales)
   - Detecta operadores (+, -, *, /, =, <, >)
   - Detecta delimitadores ((, ), {, }, ;, ,)

2. **Analizador Sintáctico**: Verifica la estructura del código
   - Analiza declaraciones de variables
   - Verifica expresiones simples
   - Detecta errores sintácticos básicos

## Personalización

Puedes modificar el archivo `archivo.txt` con tu propio código para analizar. El analizador reconocerá:
- Declaraciones de variables con tipos int y float
- Operaciones aritméticas básicas
- Estructuras de control (if, while)

## Notas para estudiantes

Este es un ejemplo básico y simplificado. Para un compilador completo, necesitarás:
- Manejo de más tipos de datos
- Tablas de símbolos
- Árboles de sintaxis abstracta (AST)
- Generación de código intermedio
- Optimización de código
- Manejo de errores más robusto
