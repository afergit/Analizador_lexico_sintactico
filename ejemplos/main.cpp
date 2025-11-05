#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>

using namespace std;

// === Tipos de Tokens ===
enum TokenType {
    COUT, SHIFT, STRING,
    IF, ELSE, INT, FLOAT, CHAR,
    IDENTIFIER, NUMBER,
    ASSIGN, SEMICOLON,
    LPAREN, RPAREN, LBRACE, RBRACE,
    PLUS, MULT, REL_OP,
    UNKNOWN, END
};

string tokenTypeToString(TokenType type) {
    switch (type) {
        case IF: return "IF"; case ELSE: return "ELSE";
        case INT: return "INT"; case FLOAT: return "FLOAT"; case CHAR: return "CHAR";
        case IDENTIFIER: return "IDENTIFIER"; case NUMBER: return "NUMBER";
        case ASSIGN: return "ASSIGN"; case SEMICOLON: return "SEMICOLON";
        case LPAREN: return "IZQ_PAREN"; case RPAREN: return "DER_PAREN";
        case LBRACE: return "IZQ_BRACE"; case RBRACE: return "DER_BRACE";
        case PLUS: return "MAS"; case MULT: return "MULT"; case REL_OP: return "REL_OP";
        case UNKNOWN: return "UNKNOWN"; case END: return "END";
        case COUT: return "COUT"; case SHIFT: return "SHIFT"; case STRING: return "STRING";
        default: return "UNDEFINED";
    }
}

struct Token {
    TokenType type;
    string value;
};

// === Lexer ===
bool isLetter(char c) { return isalpha(c); }
bool isDigit(char c) { return isdigit(c); }
bool isRelOpStart(char c) { return c == '<' || c == '>' || c == '=' || c == '!'; }



Token getRelOp(ifstream &in, char first) {
    char second = in.peek();
    string op(1, first);
    if ((first == '=' || first == '!') && second == '=') {
        in.get();
        op += second;
    }
    return {REL_OP, op};
}

vector<Token> lexer(const string& filename) {
    ifstream in(filename);
    vector<Token> tokens;

    if (!in.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << filename << endl;
        return tokens;
    }

    char c;
    while (in.get(c)) {
        if (isspace(c)) continue;

        if (isLetter(c)) {
            string word(1, c);
            while (isLetter(in.peek()) || isDigit(in.peek())) {
                in.get(c);
                word += c;
            }

            if (word == "if") tokens.push_back({IF, word});
            else if (word == "else") tokens.push_back({ELSE, word});
            else if (word == "int") tokens.push_back({INT, word});
            else if (word == "float") tokens.push_back({FLOAT, word});
            else if (word == "char") tokens.push_back({CHAR, word});
            else if (word == "cout") tokens.push_back({COUT, word});
            else tokens.push_back({IDENTIFIER, word});
        }
        else if (isDigit(c)) {
            string num(1, c);
            while (isDigit(in.peek())) {
                in.get(c);
                num += c;
            }
            tokens.push_back({NUMBER, num});
        }
        else if (c == '=') {
            char next = in.peek();
            if (next == '=') {
                in.get();
                tokens.push_back({REL_OP, "=="});
            } else {
                tokens.push_back({ASSIGN, "="});
            }
        }
        else if (isRelOpStart(c)) {
            if (c == '<' && in.peek() == '<') { // consume el segundo '<'
                in.get(c);
                tokens.push_back({SHIFT, "<<"});
            } else {
                tokens.push_back(getRelOp(in, c));
            }
        }
        else if (c == '"') {
            string str;
            while (in.peek() != '"' && in.peek() != EOF) {
                in.get(c);
                str += c;
            }
            in.get(); // consumir la comilla final
            tokens.push_back({STRING, str});
        }

        else if (c == ';') tokens.push_back({SEMICOLON, ";"});
        else if (c == '(') tokens.push_back({LPAREN, "("});
        else if (c == ')') tokens.push_back({RPAREN, ")"});
        else if (c == '{') tokens.push_back({LBRACE, "{"});
        else if (c == '}') tokens.push_back({RBRACE, "}"});
        else if (c == '+') tokens.push_back({PLUS, "+"});
        else if (c == '*') tokens.push_back({MULT, "*"});
        else tokens.push_back({UNKNOWN, string(1, c)});
    }

    tokens.push_back({END, ""});
    return tokens;
}

// === Parser ===
class Parser {
    vector<Token> tokens;
    int pos;

public:
    Parser(const vector<Token>& t) : tokens(t), pos(0) {}

    Token current() { return tokens[pos]; }
    void advance() { if (pos < tokens.size()) pos++; }
    bool match(TokenType type) {
        if (current().type == type) {
            advance();
            return true;
        }
        return false;
    }


    bool parseAssignment() {
        if (match(IDENTIFIER)) {
            if (match(ASSIGN)) {
                if (parseExpression()) {
                    if (match(SEMICOLON)) {
                        cout << "Asignacion valida";
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool parseDeclaration() {
            if (match(INT) || match(FLOAT) || match(CHAR)) {
                if (match(IDENTIFIER)) {
                    if (match(ASSIGN)) {
                        if (parseExpression()) {
                            if (match(SEMICOLON)) {
                                //cout << "Declaracion valida\n";
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }

    bool parseExpression() {
        if (!parseTerm()) return false;
        while (match(PLUS)) {
            if (!parseTerm()) return false;
        }
        return true;
    }

    bool parseTerm() {
        if (!parseFactor()) return false;
        while (match(MULT)) {
            if (!parseFactor()) return false;
        }
        return true;
    }

    bool parseFactor() {
        if (match(LPAREN)) {
            if (parseExpression() && match(RPAREN)) return true;
            return false;
        }
        if (match(NUMBER) || match(IDENTIFIER)) return true;
        return false;
    }

    bool parseCondition() {
        if (parseExpression()) {
            if (match(REL_OP)) {
                return parseExpression();
            }
        }
            return false;
    }


    bool parseOutput() {
        if (match(COUT)) {
            while (match(SHIFT)) {
                if (match(STRING) || match(IDENTIFIER) || match(NUMBER)) {
                    continue;
                } else {
                    return false;
                }
            }
            if (match(SEMICOLON)) {
                //cout << "Instruccion de salida valida\n";
                return true;
            }
        }
        return false;
    }


    bool parseIfStatement() {
    if (match(IF)) {
        if (match(LPAREN)) {
            if (parseCondition() && match(RPAREN)) {
                if (match(LBRACE)) {
                    while (current().type != RBRACE && current().type != END) {
                        if (!parseDeclaration() && !parseAssignment() && !parseIfStatement()) {
                            return false;
                        }
                    }
                    if (!match(RBRACE)) return false;

                    if (match(ELSE)) {
                        if (match(LBRACE)) {
                            while (current().type != RBRACE && current().type != END) {
                                if (!parseDeclaration() && !parseAssignment() && !parseIfStatement()) {
                                    return false;
                                }
                            }
                            return match(RBRACE);
                        }
                        return false;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

    void parse() {
        while (current().type != END) {
            if (parseDeclaration()) {
                cout << "Declaracion valida\n";
            } else if (parseIfStatement()) {
                cout << "Condicional valida\n";
            } else if (parseOutput()) {
                cout << "Instruccion de salida valida\n";
            } else if(parseAssignment()){
                cout << "Asignacion valida\n";
            }
            else {
                cout << "Error de sintaxis\n";
                break;
            }
        }
    }

};

// === Main ===
int main() {
    string filename = "archivo.txt";
    vector<Token> tokens = lexer(filename);

    cout << "=== Tokens ===\n";
    for (const Token& token : tokens) {
        cout << "Token: " << token.value << " | Tipo: " << tokenTypeToString(token.type) << endl;
    }

    cout << "\n=== Analisis Sintactico ===\n";
    Parser parser(tokens);
    parser.parse();

    return 0;
}
