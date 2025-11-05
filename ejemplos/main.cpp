#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>

using namespace std;

// Tipos de tokens
enum TipoToken {
    PALABRA_CLAVE,
    IDENTIFICADOR,
    NUMERO,
    OPERADOR,
    DELIMITADOR,
    DESCONOCIDO,
    FIN_ARCHIVO
};

// Estructura para representar un token
struct Token {
    TipoToken tipo;
    string valor;
    int linea;
};

// Clase Analizador Léxico
class AnalizadorLexico {
private:
    string codigo;
    size_t posicion;
    int linea;
    
    bool esPalabraClave(const string& palabra) {
        return (palabra == "int" || palabra == "float" || palabra == "if" || 
                palabra == "while" || palabra == "for" || palabra == "return");
    }
    
    bool esOperador(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/' || 
                c == '=' || c == '<' || c == '>');
    }
    
    bool esDelimitador(char c) {
        return (c == '(' || c == ')' || c == '{' || c == '}' || 
                c == ';' || c == ',');
    }
    
public:
    AnalizadorLexico(const string& cod) : codigo(cod), posicion(0), linea(1) {}
    
    Token siguienteToken() {
        Token token;
        token.linea = linea;
        
        // Saltar espacios en blanco
        while (posicion < codigo.length() && isspace(codigo[posicion])) {
            if (codigo[posicion] == '\n') linea++;
            posicion++;
        }
        
        // Fin del archivo
        if (posicion >= codigo.length()) {
            token.tipo = FIN_ARCHIVO;
            token.valor = "";
            return token;
        }
        
        char actual = codigo[posicion];
        
        // Identificadores y palabras clave
        if (isalpha(actual)) {
            string palabra = "";
            while (posicion < codigo.length() && (isalnum(codigo[posicion]) || codigo[posicion] == '_')) {
                palabra += codigo[posicion++];
            }
            
            if (esPalabraClave(palabra)) {
                token.tipo = PALABRA_CLAVE;
            } else {
                token.tipo = IDENTIFICADOR;
            }
            token.valor = palabra;
            return token;
        }
        
        // Números
        if (isdigit(actual)) {
            string numero = "";
            while (posicion < codigo.length() && (isdigit(codigo[posicion]) || codigo[posicion] == '.')) {
                numero += codigo[posicion++];
            }
            token.tipo = NUMERO;
            token.valor = numero;
            return token;
        }
        
        // Operadores
        if (esOperador(actual)) {
            token.tipo = OPERADOR;
            token.valor = string(1, actual);
            posicion++;
            return token;
        }
        
        // Delimitadores
        if (esDelimitador(actual)) {
            token.tipo = DELIMITADOR;
            token.valor = string(1, actual);
            posicion++;
            return token;
        }
        
        // Token desconocido
        token.tipo = DESCONOCIDO;
        token.valor = string(1, actual);
        posicion++;
        return token;
    }
};

// Clase Analizador Sintáctico
class AnalizadorSintactico {
private:
    vector<Token> tokens;
    size_t posicion;
    
    Token tokenActual() {
        if (posicion < tokens.size()) {
            return tokens[posicion];
        }
        Token token;
        token.tipo = FIN_ARCHIVO;
        return token;
    }
    
    void avanzar() {
        if (posicion < tokens.size()) {
            posicion++;
        }
    }
    
    bool verificar(TipoToken tipo) {
        return tokenActual().tipo == tipo;
    }
    
    void esperarToken(TipoToken tipo, const string& mensaje) {
        if (!verificar(tipo)) {
            cout << "Error sintáctico en línea " << tokenActual().linea 
                 << ": " << mensaje << endl;
            return;
        }
        avanzar();
    }
    
    // Analiza una declaración
    void analizarDeclaracion() {
        if (verificar(PALABRA_CLAVE)) {
            cout << "Declaración encontrada: " << tokenActual().valor << endl;
            avanzar();
            esperarToken(IDENTIFICADOR, "Se esperaba un identificador");
            
            if (verificar(OPERADOR) && tokenActual().valor == "=") {
                avanzar();
                analizarExpresion();
            }
            
            esperarToken(DELIMITADOR, "Se esperaba ';'");
        }
    }
    
    // Analiza una expresión simple
    void analizarExpresion() {
        if (verificar(NUMERO) || verificar(IDENTIFICADOR)) {
            avanzar();
            
            while (verificar(OPERADOR)) {
                avanzar();
                if (verificar(NUMERO) || verificar(IDENTIFICADOR)) {
                    avanzar();
                }
            }
        }
    }
    
public:
    AnalizadorSintactico(const vector<Token>& toks) : tokens(toks), posicion(0) {}
    
    void analizar() {
        cout << "\n=== Análisis Sintáctico ===" << endl;
        while (!verificar(FIN_ARCHIVO)) {
            if (verificar(PALABRA_CLAVE)) {
                analizarDeclaracion();
            } else {
                avanzar();
            }
        }
        cout << "Análisis sintáctico completado." << endl;
    }
};

int main() {
    cout << "=== Analizador Léxico y Sintáctico ===" << endl;
    cout << "Curso de Compiladores - Ejemplo Básico\n" << endl;
    
    // Leer archivo de entrada
    ifstream archivo("ejemplos/archivo.txt");
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo 'ejemplos/archivo.txt'" << endl;
        cout << "Asegúrate de que el archivo existe en la carpeta 'ejemplos/'" << endl;
        return 1;
    }
    
    string codigo((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
    archivo.close();
    
    cout << "Código fuente a analizar:" << endl;
    cout << "------------------------" << endl;
    cout << codigo << endl;
    cout << "------------------------\n" << endl;
    
    // Análisis Léxico
    AnalizadorLexico lexico(codigo);
    vector<Token> tokens;
    
    cout << "=== Análisis Léxico ===" << endl;
    cout << "Tokens encontrados:" << endl;
    
    Token token;
    do {
        token = lexico.siguienteToken();
        tokens.push_back(token);
        
        if (token.tipo != FIN_ARCHIVO) {
            cout << "Línea " << token.linea << " - ";
            switch (token.tipo) {
                case PALABRA_CLAVE:
                    cout << "PALABRA_CLAVE: " << token.valor << endl;
                    break;
                case IDENTIFICADOR:
                    cout << "IDENTIFICADOR: " << token.valor << endl;
                    break;
                case NUMERO:
                    cout << "NUMERO: " << token.valor << endl;
                    break;
                case OPERADOR:
                    cout << "OPERADOR: " << token.valor << endl;
                    break;
                case DELIMITADOR:
                    cout << "DELIMITADOR: " << token.valor << endl;
                    break;
                case DESCONOCIDO:
                    cout << "DESCONOCIDO: " << token.valor << endl;
                    break;
                default:
                    break;
            }
        }
    } while (token.tipo != FIN_ARCHIVO);
    
    // Análisis Sintáctico
    AnalizadorSintactico sintactico(tokens);
    sintactico.analizar();
    
    cout << "\n=== Análisis Completado ===" << endl;
    
    return 0;
}
