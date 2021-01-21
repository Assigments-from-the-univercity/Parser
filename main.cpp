#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>

using namespace std;

enum LexemeType{
    LEFT_BRACKET, RIGHT_BRACKET, LEFT_CURV_BRACKET, RIGHT_CURV_BRACKET,
    OP_PLUS, OP_MINUS, OP_MUL, OP_DIV, OP_EAQUAL, OP_PP, OP_MM, OP_REMINDER,
    NUMBER, NUMBER_DOUBLE, NUMBER_INT,
    SEMICOLON, COMA,
    FOR_LOOP, IF_CASE,
    LESS, MORE, NOT_LESS, NOT_MORE, IS_SAME,
    COUT, COUT_PRINT, COUT_ENDL,
    UNDEF_VALUE,
    QUOTES,
    EOFile
};
struct Lexeme{
    LexemeType type;
    string value;

    Lexeme(string value,  LexemeType type){
        this->value = value;
        this->type = type;
    }

    Lexeme(char value,  LexemeType type){
        this->value = value;
        this->type = type;
    }

    Lexeme(LexemeType type){
        this->value = "";
        this->type = type;
    }

    Lexeme(){};

    /*double getVelue(){
        //double i = value.toS
    }*/
};
struct LexemeBuffer{
    vector < Lexeme > lextems;
    int pos = 0;

    LexemeBuffer(vector < Lexeme > lextems){
        this->lextems = lextems;
        this->pos = 0;
    }

    LexemeBuffer(vector < Lexeme > lextems, int pos){
        this->lextems = lextems;
        this->pos = pos;
    }

    LexemeBuffer(){}

    Lexeme next(){
        return lextems[pos++];
    }

    void back(){
        pos--;
    }

    Lexeme getLextem(){
        return lextems[pos];
    }

    void getOut(){
        for (int i = 0; i < lextems.size(); i++){
            cout << lextems[i].value << " " << lextems[i].type << endl;
        }
    }

    int getSize(){
        return lextems.size();
    }
};
struct Number{
    string name;
    string value;
    LexemeType type;

    Number(){};

    Number(string name, string value){
        this->name = name;
        this->value = value;
        this->type = NUMBER_DOUBLE;
    }

    Number(string name, string value, LexemeType type){
        this->name = name;
        this->value = value;
        this->type =  type;
    }
};

double Parse(string str);
vector < Lexeme > Convert(ifstream &fin);
double Factor(LexemeBuffer &buffer, vector < Number > &numberSet);
double MultDiv(LexemeBuffer &lexemes, vector < Number > &numberSet);
double PlusMinus(LexemeBuffer &lexemes, vector < Number > &numberSet);
double Expression(LexemeBuffer &lexemes, vector < Number > &numberSet);
double Calculate(LexemeBuffer &lexemes, vector < Number > numberSet);
void StartWork(LexemeBuffer &lexemes, vector < Number > &numberSet);
void StartWork(LexemeBuffer &lexemes, vector < Number > &numberSet, bool IsOneLine);
void CreateDoubles(LexemeBuffer &lexemes, vector < Number > &numberSet);
void CreateInt(LexemeBuffer &lexemes, vector < Number > &numberSet);
string FindNumberInNumberSet(vector < Number > numberSet, string name);
void Addition(LexemeBuffer &lexemes, vector < Number > &numberSet, Lexeme lexeme, LexemeType lType);
void Eaquation(LexemeBuffer &lexemes, vector < Number > &numberSet);
void ChangeNumberInNumberSet(vector < Number > &numberSet, double value, string name);
void ForLoop(LexemeBuffer &lexemes, vector < Number > &numberSet);
void IfCase(LexemeBuffer &lexemes, vector < Number > &numberSet);
void Cout(LexemeBuffer &lexemes, vector < Number > &numberSet);

int main() {
    ifstream fin("in.txt");
    LexemeBuffer lexBuffer(Convert(fin));
    vector < Number > numberSet;
    StartWork(lexBuffer, numberSet);
}

double Parse(string str){
    stringstream ss;
    ss << str;
    double value;
    ss >> value;
    return value;
}

vector < Lexeme > Convert(ifstream &fin){
    LexemeBuffer lexBuffer;
    while (!fin.eof()) {
        char c;
        c = ' ';
        fin >> c;
        switch (c) {
            case '(':
                lexBuffer.lextems.push_back(Lexeme(LEFT_BRACKET)); lexBuffer.pos++; break;
            case ')':
                lexBuffer.lextems.push_back(Lexeme(RIGHT_BRACKET)); lexBuffer.pos++; break;
            case '{':
                lexBuffer.lextems.push_back(Lexeme(LEFT_CURV_BRACKET)); lexBuffer.pos++; break;
            case '}':
                lexBuffer.lextems.push_back(Lexeme(RIGHT_CURV_BRACKET)); lexBuffer.pos++; break;
            case '+':
                if (lexBuffer.lextems[lexBuffer.lextems.size()-1].type == OP_PLUS){
                    lexBuffer.lextems[lexBuffer.lextems.size()-1].type = OP_PP;
                    break;
                }
                lexBuffer.lextems.push_back(Lexeme(OP_PLUS)); lexBuffer.pos++; break;
            case '-':
                if (lexBuffer.lextems[lexBuffer.lextems.size()-1].type == OP_MINUS){
                    lexBuffer.lextems[lexBuffer.lextems.size()-1].type = OP_MM;
                    break;
                }
                lexBuffer.lextems.push_back(Lexeme(OP_MINUS)); lexBuffer.pos++; break;
            case '*':
                lexBuffer.lextems.push_back(Lexeme(OP_MUL)); lexBuffer.pos++; break;
            case '/':
                if (lexBuffer.lextems[lexBuffer.lextems.size()-1].type == OP_DIV){
                    string str;
                    getline(fin,str);
                    lexBuffer.lextems.pop_back();
                    break;
                }
                lexBuffer.lextems.push_back(Lexeme(OP_DIV)); lexBuffer.pos++; break;
            case '=':
                if (lexBuffer.lextems[lexBuffer.lextems.size()-1].type == LESS){
                    lexBuffer.lextems[lexBuffer.lextems.size()-1].type = NOT_MORE;
                    break;
                }
                if (lexBuffer.lextems[lexBuffer.lextems.size()-1].type == MORE){
                    lexBuffer.lextems[lexBuffer.lextems.size()-1].type = NOT_LESS;
                    break;
                }
                if (lexBuffer.lextems[lexBuffer.lextems.size()-1].type == OP_EAQUAL){
                    lexBuffer.lextems[lexBuffer.lextems.size()-1].type = IS_SAME;
                    break;
                }
                lexBuffer.lextems.push_back(Lexeme(OP_EAQUAL)); lexBuffer.pos++; break;
            case '%':
                lexBuffer.lextems.push_back(Lexeme(OP_REMINDER)); lexBuffer.pos++; break;
            case ';':
                lexBuffer.lextems.push_back(Lexeme(SEMICOLON)); lexBuffer.pos++; break;
            case ',':
                lexBuffer.lextems.push_back(Lexeme(COMA)); lexBuffer.pos++; break;
            case '<':
                if (lexBuffer.lextems[lexBuffer.lextems.size()-1].type == LESS){
                    lexBuffer.lextems[lexBuffer.lextems.size()-1].type = COUT_PRINT;
                    break;
                }
                lexBuffer.lextems.push_back(Lexeme(LESS)); lexBuffer.pos++; break;
            case '>':
                lexBuffer.lextems.push_back(Lexeme(MORE)); lexBuffer.pos++; break;
            case '"':
                if (c == '\"'){
                    string str;
                    fin.get(c);
                    while (c != '\"'){
                        str += c;
                        if (! fin.get(c)){
                            throw runtime_error("Invalid input at char " + lexBuffer.pos);
                        }
                    }
                    lexBuffer.lextems.push_back(Lexeme(str, QUOTES));
                }
                lexBuffer.pos++; break;
            default:
                if (c >= '0' && c <= '9') {
                    string number;
                    while (c >= '0' && c <= '9') {
                        number += c;
                        lexBuffer.pos++;
                        if (fin.eof()){
                            break;
                        }
                        //fin >> c;
                        fin.get(c);
                    }
                    //long pos = fin.tellp();
                    int posit = fin.tellg();
                    fin.seekg(posit - 1);
                    lexBuffer.lextems.push_back(Lexeme(number, NUMBER));
                }
                else if (c >= 'A' && c <= 'Z' ||
                         c >= 'a' && c <= 'z') {
                    string numStr;
                    while (c >= 'A' && c <= 'Z' ||
                           c >= 'a' && c <= 'z') {
                        numStr += c;
                        lexBuffer.pos++;
                        if (fin.eof()) {
                            break;
                        }
                        //fin >> c;
                        fin.get(c);
                    }
                    if (numStr == "double") {
                        lexBuffer.lextems.push_back(Lexeme(NUMBER_DOUBLE)); lexBuffer.pos++;
                    }
                    else if (numStr == "int") {
                        lexBuffer.lextems.push_back(Lexeme(NUMBER_INT)); lexBuffer.pos++;
                    }
                    else if (numStr == "if") {
                        lexBuffer.lextems.push_back(Lexeme(IF_CASE)); lexBuffer.pos++;
                    }
                    else if (numStr == "for") {
                        lexBuffer.lextems.push_back(Lexeme(FOR_LOOP)); lexBuffer.pos++;
                    }
                    else if (numStr == "cout") {
                        lexBuffer.lextems.push_back(Lexeme(COUT)); lexBuffer.pos++;
                    }
                    else if (numStr == "<<") {
                        lexBuffer.lextems.push_back(Lexeme(COUT_PRINT)); lexBuffer.pos++;
                    }
                    else if (numStr == "endl") {
                        lexBuffer.lextems.push_back(Lexeme(COUT_ENDL)); lexBuffer.pos++;
                    }
                    else {
                        lexBuffer.lextems.push_back(Lexeme(numStr, UNDEF_VALUE)); lexBuffer.pos++;
                        //throw runtime_error("Invalid input at char " + lexBuffer.pos);
                        //cout << "Invalid input";
                        //system("stop");
                    }
                    int posit = fin.tellg();
                    fin.seekg(posit - 1);
//                    cout << "Введіть значення для змінної " << numStr << ": ";
//                    cin >> numStr;
//                    lexBuffer.lextems.push_back(Lexeme(numStr, LexemeType::NUMBER));
                }
                else {
                    if (c == ' ' || c == '\n'){
                        lexBuffer.pos++;
                    }
                    else {
                        throw runtime_error("Invalid input at char " + lexBuffer.pos);
                    }
                }
        }
    }
    lexBuffer.lextems.push_back(Lexeme(LexemeType::EOFile));
    return lexBuffer.lextems;
}

double Calculate(LexemeBuffer &lexemes, vector < Number > numberSet){
    Expression(lexemes, numberSet);
}

double Expression(LexemeBuffer &lexemes, vector < Number > &numberSet){
    Lexeme lexeme = lexemes.next();
    if (lexeme.type == EOFile ||
        lexeme.type == SEMICOLON ||
        lexeme.type == COMA ||
        lexeme.type == LESS ||
        lexeme.type == MORE ||
        lexeme.type == OP_EAQUAL ||
        lexeme.type == COUT_PRINT){
        return 0;
    }
    else if (lexemes.getSize() == 1){
        Lexeme lexValue1 = lexeme;
        double value1;
        if (lexValue1.type == UNDEF_VALUE) {
            value1 = Parse(FindNumberInNumberSet(numberSet, lexValue1.value));
        }
        else{
            value1 = Parse(lexValue1.value);
        }
        return value1;
    }
    else {
        lexemes.back();
        return PlusMinus(lexemes, numberSet);
    }
}

double PlusMinus(LexemeBuffer &lexemes, vector < Number > &numberSet){
    double value = MultDiv(lexemes, numberSet);
    while (true){
        Lexeme lexeme = lexemes.next();
        switch (lexeme.type) {
            case OP_PLUS:
                value += MultDiv(lexemes, numberSet);
                break;
            case OP_MINUS:
                value -= MultDiv(lexemes, numberSet);
                break;
            case OP_REMINDER:
//                int val = value;
//                val %= (int)MultDiv(lexemes, numberSet);
                value = (int)value % (int)MultDiv(lexemes, numberSet);
                break;
            default:
                lexemes.back();
                return value;
        }
    }
}

double MultDiv(LexemeBuffer &lexemes, vector < Number > &numberSet){
    double value = Factor(lexemes, numberSet);
    while (true){
        Lexeme lexeme = lexemes.next();
        switch (lexeme.type) {
            case OP_MUL:
                value *= Factor(lexemes, numberSet);
                break;
            case OP_DIV:
                value /= Factor(lexemes, numberSet);
                break;
            default:
                lexemes.back();
                return value;
        }
    }
}

double Factor(LexemeBuffer &lexemes, vector < Number > &numberSet){
    Lexeme lexeme = lexemes.next();
    double value;
    string str;
    Lexeme lexeme1;
    switch (lexeme.type) {
        case NUMBER:
            return Parse(lexeme.value);
        case UNDEF_VALUE:
            str = FindNumberInNumberSet(numberSet, lexeme.value);
            if (str != "is not exist"){
                return Parse(str);
            }
            else {
                throw runtime_error("Unexpected token: " + lexeme.value
                                    + " at position: " + to_string(lexemes.pos));
            }
        case OP_MINUS:
            lexeme1 = lexemes.next();
            if (lexeme1.type == NUMBER){
                return Parse(lexeme1.value) * -1;
            }
            else {
                throw runtime_error("Unexpected token: " + lexeme.value
                                    + " at position: " + to_string(lexemes.pos));
            }
        case LEFT_BRACKET:
            value  = Expression(lexemes, numberSet); //TODO create "double value" right here
            lexeme = lexemes.next();
            if (lexeme.type != RIGHT_BRACKET){
                throw runtime_error("Unexpected token: " + lexeme.value
                                    + " at position: " + to_string(lexemes.pos));
            }
            return value;
        default:
            throw runtime_error("Unexpected token: " + lexeme.value
                                + " at position: " + to_string(lexemes.pos));
    }
}

void StartWork(LexemeBuffer &lexemes, vector < Number > &numberSet){
    StartWork(lexemes, numberSet, false);
}

void StartWork(LexemeBuffer &lexemes, vector < Number > &numberSet, bool IsOneLine){
    Lexeme lexeme = lexemes.next();
    while (!(lexeme.type == EOFile ||
           lexeme.type == RIGHT_CURV_BRACKET ||
           lexeme.type == SEMICOLON)) {
        switch (lexeme.type) {
            case NUMBER_DOUBLE:
                CreateDoubles(lexemes, numberSet); break;
            case NUMBER_INT:
                CreateInt(lexemes, numberSet); break;
            case UNDEF_VALUE:
                lexemes.back();
                Eaquation(lexemes, numberSet);
                lexeme = lexemes.next();
                /*if (lexeme.type == SEMICOLON){
                    lexeme = lexemes.next();
                }
                else {
                    throw runtime_error("Unexpected token: " + lexeme.value
                                        + " at position: " + to_string(lexemes.pos));
                }*/
                break;
            case FOR_LOOP:
                ForLoop(lexemes, numberSet); break;
            case IF_CASE:
                IfCase(lexemes, numberSet); break;
            case COUT:
                Cout(lexemes, numberSet); break;
                //TODO next case;
            default:
                throw runtime_error("Unexpected token: " + lexeme.value
                                    + " at position: " + to_string(lexemes.pos));
        }
        if (lexemes.getLextem().type == EOFile){
            break;
        }
        if (IsOneLine == true){
            lexemes.back();
            lexeme = lexemes.next();
            if (lexeme.type == SEMICOLON){
                break;
            }
        }
        lexeme = lexemes.next();
    }
    return;
}

void Cout(LexemeBuffer &lexemes, vector < Number > &numberSet){
    Lexeme lexeme = lexemes.next();
    while (lexeme.type == COUT_PRINT){
        lexeme = lexemes.next();
        if (lexeme.type == COUT_ENDL){
            cout << endl;
        }
        else if(lexeme.type == QUOTES){
            cout << lexeme.value;
        }
        else {
            lexemes.back();
            double expr = Expression(lexemes, numberSet);
            cout << expr;
        }
        lexeme = lexemes.next();
    }
    if (lexeme.type != SEMICOLON){
        throw runtime_error("Unexpected token: " + lexeme.value
                            + " at position: " + to_string(lexemes.pos));
    }
}

void IfCase(LexemeBuffer &lexemes, vector < Number > &numberSet){
    Lexeme lexeme = lexemes.next();
    if (lexeme.type == LEFT_BRACKET){
        LexemeBuffer forLoopBuffer1;
        lexeme = lexemes.next();
        while (!(lexeme.type == LESS ||
               lexeme.type == NOT_LESS ||
               lexeme.type == NOT_MORE ||
               lexeme.type == MORE ||
               lexeme.type == IS_SAME)){
            forLoopBuffer1.lextems.push_back(lexeme);
            lexeme = lexemes.next();
        }
        double value1 = Calculate(forLoopBuffer1, numberSet);


        /*
        Lexeme lexValue1 = lexemes.next();
        double value1;
        if (lexValue1.type == UNDEF_VALUE) {
            value1 = Parse(FindNumberInNumberSet(numberSet, lexValue1.value));
        }
        else{
            value1 = Parse(lexValue1.value);
        }
        */

        Lexeme lexeme_compare = lexeme;


        LexemeBuffer forLoopBuffer2;
        lexeme = lexemes.next();
        while (lexeme.type != RIGHT_BRACKET){
            forLoopBuffer2.lextems.push_back(lexeme);
            lexeme = lexemes.next();
        }
        double value2 = Calculate(forLoopBuffer2, numberSet);

        //double value2 = Expression(lexemes, numberSet);
        /*
        Lexeme lexValue2 = lexemes.next();
        double value2;
        if (lexValue2.type == UNDEF_VALUE) {
            value2 = Parse(FindNumberInNumberSet(numberSet, lexValue2.value));
        }
        else{
            value2 = Parse(lexValue2.value);
        }*/
        //lexeme = lexemes.next();
        if (lexeme.type == RIGHT_BRACKET){
            //lexeme = lexemes.next();
            //if (lexeme.type == LEFT_CURV_BRACKET) {
                bool ifCase = true;
                switch (lexeme_compare.type) {
                    case LESS:
                        if (value1 < value2) {
                            StartWork(lexemes, numberSet, true);
                        }
                        else {
                            ifCase = false;
                        }
                        break;
                    case MORE:
                        if (value1 > value2) {
                            StartWork(lexemes, numberSet, true);
                        }
                        else {
                            ifCase = false;
                        }
                        break;
                    case IS_SAME:
                        if (value1 == value2) {
                            StartWork(lexemes, numberSet, true);
                        }
                        else {
                            ifCase = false;
                        }
                        break;
                    case NOT_MORE:
                        if (value1 <= value2) {
                            StartWork(lexemes, numberSet, true);
                        }
                        else {
                            ifCase = false;
                        }
                        break;
                    case NOT_LESS:
                        if (value1 >= value2) {
                            StartWork(lexemes, numberSet, true);
                        }
                        else {
                            ifCase = false;
                        }
                        break;
                    default:
                        throw runtime_error("Unexpected token: " + lexeme.value
                                                + " at position: " + to_string(lexemes.pos));
                    }
                    if (ifCase == false){
                        lexeme = lexemes.next();
                        while (lexeme.type != SEMICOLON){
                            lexeme = lexemes.next();
                        }
                    }
            } else{
                throw runtime_error("Unexpected token: " + lexeme.value
                                    + " at position: " + to_string(lexemes.pos));
            }

    }
    else {
        throw runtime_error("Unexpected token: " + lexeme.value
                            + " at position: " + to_string(lexemes.pos));
    }
}

void ForLoop(LexemeBuffer &lexemes, vector < Number > &numberSet){
    Lexeme lexeme = lexemes.next();
    if (lexeme.type == LEFT_BRACKET){
        //lexeme = lexemes.next();
        StartWork(lexemes, numberSet, true);
        lexemes.back();
        lexeme = lexemes.next();
        if (lexeme.type == SEMICOLON){
            Lexeme lexValue1 = lexemes.next();
            double value1;
            //double value1 = Expression(lexemes, numberSet);
            if (lexValue1.type == UNDEF_VALUE) {
                value1 = Parse(FindNumberInNumberSet(numberSet, lexValue1.value));
            }
            else{
                value1 = Parse(lexValue1.value);
            }
            Lexeme lexeme_compare = lexemes.next();
            //double value2 = Expression(lexemes, numberSet);
            Lexeme lexValue2 = lexemes.next();
            double value2;
            if (lexValue2.type == UNDEF_VALUE) {
                value2 = Parse(FindNumberInNumberSet(numberSet, lexValue2.value));
            }
            else{
                value2 = Parse(lexValue2.value);
            }
            lexeme = lexemes.next();
            if (lexeme.type == SEMICOLON){
                LexemeBuffer forLoopBuffer;
                lexeme = lexemes.next();
                while (lexeme.type != RIGHT_BRACKET){
                    forLoopBuffer.lextems.push_back(lexeme);
                    lexeme = lexemes.next();
                }
                lexeme = lexemes.next();
                if (lexeme.type == LEFT_CURV_BRACKET){
                    bool loop = true;
                    int posCurr = lexemes.pos;
                    while (loop) {
                        switch (lexeme_compare.type) {
                            case LESS:
                                if (value1 < value2) {
                                    lexemes.pos = posCurr;
                                    StartWork(lexemes, numberSet);
                                }
                                else {
                                    loop = false;
                                }
                                break;
                            case MORE:
                                if (value1 > value2) {
                                    lexemes.pos = posCurr;
                                    StartWork(lexemes, numberSet);
                                }
                                else {
                                    loop = false;
                                }
                                break;
                            case IS_SAME:
                                if (value1 = value2) {
                                    lexemes.pos = posCurr;
                                    StartWork(lexemes, numberSet);
                                }
                                else {
                                    loop = false;
                                }
                                break;
                            case NOT_MORE:
                                if (value1 <= value2) {
                                    lexemes.pos = posCurr;
                                    StartWork(lexemes, numberSet);
                                }
                                else {
                                    loop = false;
                                }
                                break;
                            case NOT_LESS:
                                if (value1 >= value2) {
                                    lexemes.pos = posCurr;
                                    StartWork(lexemes, numberSet);
                                }
                                else {
                                    loop = false;
                                }
                                break;
                            default:
                                throw runtime_error("Unexpected token: " + lexeme.value
                                                    + " at position: " + to_string(lexemes.pos));
                        }
                        if (loop) {
                            forLoopBuffer.pos = 0;
                            Eaquation(forLoopBuffer, numberSet);
                            value1 = Parse(FindNumberInNumberSet(numberSet, lexValue1.value));
                            if (lexValue2.type == UNDEF_VALUE) {
                                value2 = Parse(FindNumberInNumberSet(numberSet, lexValue2.value));
                            }
                        }
                    }
                } else{
                    throw runtime_error("Unexpected token: " + lexeme.value
                                        + " at position: " + to_string(lexemes.pos));
                }
            }
        }
    }
    else {
        throw runtime_error("Unexpected token: " + lexeme.value
                            + " at position: " + to_string(lexemes.pos));
    }
}

void Eaquation(LexemeBuffer &lexemes, vector < Number > &numberSet){
    //lexemes.pos = 0;
    Lexeme lexeme = lexemes.next();
    Lexeme lexeme1 = lexemes.next();
    if (lexeme1.type == OP_EAQUAL){
        double value = Expression(lexemes, numberSet);
        if (lexeme.type == NUMBER_INT){
            int v = value;
            value = v;
        }
        ChangeNumberInNumberSet(numberSet, value, lexeme.value);
    }
    else if (lexeme.type == OP_PP){
        double value = Parse(FindNumberInNumberSet(numberSet,lexeme1.value));
        value++;
        ChangeNumberInNumberSet(numberSet, value, lexeme1.value);
    }
    else if (lexeme.type == OP_MM){
        double value = Parse(FindNumberInNumberSet(numberSet,lexeme1.value));
        value--;
        ChangeNumberInNumberSet(numberSet, value, lexeme.value);
    }
    else {
        throw runtime_error("Unexpected token: " + lexeme.value
                            + " at position: " + to_string(lexemes.pos));
    }
}

void CreateDoubles(LexemeBuffer &lexemes, vector < Number > &numberSet){
    Lexeme lexeme = lexemes.next();
    while (lexeme.type != SEMICOLON){
        if (lexeme.type == UNDEF_VALUE){
            if (FindNumberInNumberSet(numberSet, lexeme.value) == "is not exist"){
                //Number number();
                Lexeme lexeme1 = lexemes.next();
                if (lexeme1.type == OP_EAQUAL){
                    Addition(lexemes, numberSet, lexeme, NUMBER_DOUBLE);
                    lexeme = lexemes.next();
                }
                else {
                    throw runtime_error("Unexpected token: " + lexeme.value
                                        + " at position: " + to_string(lexemes.pos));
                }
            } else {
                throw runtime_error("Unexpected token: " + lexeme.value
                                    + " at position: " + to_string(lexemes.pos));
            }
        }
        else if (lexeme.type == COMA) {
            lexeme = lexemes.next();
            continue;
        }
        else{
            throw runtime_error("Unexpected token: " + lexeme.value
                                + " at position: " + to_string(lexemes.pos));
        }
    }
}

void CreateInt(LexemeBuffer &lexemes, vector < Number > &numberSet){
    Lexeme lexeme = lexemes.next();
    while (lexeme.type != SEMICOLON){
        if (lexeme.type == UNDEF_VALUE){
            if (FindNumberInNumberSet(numberSet, lexeme.value) == "is not exist"){
                //Number number();
                Lexeme lexeme1 = lexemes.next();
                if (lexeme1.type == OP_EAQUAL){
                    Addition(lexemes, numberSet, lexeme, NUMBER_INT);
                    lexeme = lexemes.next();
                }
                else {
                    throw runtime_error("Unexpected token: " + lexeme.value
                                        + " at position: " + to_string(lexemes.pos));
                }
            } else {
                throw runtime_error("Unexpected token: " + lexeme.value
                                    + " at position: " + to_string(lexemes.pos));
            }
        }
        else if (lexeme.type == COMA) {
            lexeme = lexemes.next();
            continue;
        }
        else{
            throw runtime_error("Unexpected token: " + lexeme.value
                                + " at position: " + to_string(lexemes.pos));
        }
    }
}

string FindNumberInNumberSet(vector < Number > numberSet, string name){
    for (int i=0; i < numberSet.size(); i++){
        if (numberSet[i].name == name){
            return numberSet[i].value;
        }
    }
    return "is not exist";
}

void ChangeNumberInNumberSet(vector < Number > &numberSet, double value, string name){
    for (int i = 0; i < numberSet.size(); i++){
        if (numberSet[i].name == name){
            if (numberSet[i].type == NUMBER_INT){
                int v = value;
                value = v;
            }
            numberSet[i].value = to_string(value);
            break;
        }
    }
}

void Addition(LexemeBuffer &lexemes, vector < Number > &numberSet, Lexeme lexeme, LexemeType lType){
    double value;
    value = Expression(lexemes, numberSet);
    if (lType == NUMBER_INT){
        int v = value;
        value = v;
    }
    Number number(lexeme.value, to_string(value), lType);
    numberSet.push_back(number);
}