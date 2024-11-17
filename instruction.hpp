#include "util.hpp"

class InstructionStorage{
    Instruction* store[6];
public:
    InstructionStorage(double& a, size_t& i, double& u, size_t& lineNumber);
    Instruction* operator[] (char n){
        switch(n){
            case 'p':
                return store[0];
            case 's':
                return store[1];
            case 'l':
                return store[2];
            case 'g':
                return store[3];
            case 'i':
                return store[4];
            default:
                return store[5];
        }
    }
    ~InstructionStorage(){
        for (int i = 0; i < 6; i++){
            delete this->store[i];
        }
    }
};

class Interpreter{
    double a;
    size_t i;
    double u;
    char status[3];
    size_t lineNumber;
    Code code;
    InstructionStorage instructions;
public:
    Interpreter(std::string file) : a(0), i(0), u(0), lineNumber(1), code(file), instructions(this->a, this->i, this->u, this->lineNumber){
        code.getStatus(this->status);
        status[2] = '\0';
    }
    void interpret(){
        if (lineNumber == this->code.getSize() + 1){
            this->status[0] = 'e'; 
            return;
        }
        this->status[0] = 't';
        std::string current = this->code[lineNumber];
        code.getStatus(this->status);
        if (status[0] == 'g'){
            return;
        }
        size_t index = 0;
        size_t check = 0;

        while(current[index] != ' '){
            if (current[index] < 48 || current[index] > 57) {
                this->status[1] = '1';
                return;
            }
            check *= 10;
            check += current[index]-48;
            index++;
        }
        index++;

        if (check % 10){
            this->status[1] = '2';
            return;
        }
        if ((check / 10) != lineNumber){
            this->status[1] = '3';
            return;
        }

        if(this->instructions[current[index]]->task(current, this->status, index)){
            lineNumber++;
        };
    }
    bool wereError(){
        switch(this->status[0]){
            case 'f':
                std::cout << "File ";
                switch(this->status[1]){
                    case '0':
                        std::cout << "Read successfully!" << std::endl;
                        std::cout << "------------------------------------------" << std::endl;
                        return true;
                    case '1':
                        std::cout << "error: File not the correct format" << std::endl;
                        break;
                    case '2':
                        std::cout << "error: File not found" << std::endl;
                        break;
                }
                return false;
            case 'e':
                std::cout << "------------------------------------------" << std::endl;
                std::cout << "Code ran successfully!" << std::endl;
                return false;
            default:
                if (this->status[1] == '0') {return true;}
                std::cout << "------------------------------------------" << std::endl;
                switch(this->status[0]){
                    case 't':
                        std::cout << "Syntax Error {at line " << this->lineNumber*10 << "} : ";
                        switch(this->status[1]){
                            case '1':
                                std::cout << "Invalid format for line number";
                                break;
                            case '2':
                                std::cout << "Line number doesnt end with zero";
                                break;
                            case '3':
                                std::cout << "Line number doesnt match the current line";
                                break;
                            case '4':
                                std::cout << "Non existent instruction";
                                break;
                            case '5':
                                std::cout << "Instruction name not written correctly";
                                break;
                            case '6':
                                std::cout << "Code not spaced properly";
                                break;
                        }
                        break;
                    case 'g':
                        std::cout << "Goto Error {at line " << this->lineNumber*10 << "} : ";
                        switch(this->status[1]){
                            case '1':
                                std::cout << "Jump to line that is not recorded";
                                break;
                            case '2':
                                std::cout << "Invalid format for line number argument";
                                break;
                            case '3':   
                                std::cout << "Line number argument doesnt end with 0";
                                break;
                            case '4':
                                std::cout << "No closing ; at the end of line";
                        }
                        break;
                    case 'p':
                        std::cout << "Print Error {at line " << this->lineNumber*10 << "} : ";
                        switch(this->status[1]){
                            case '1':
                                std::cout << "Non existent variable given as argument";
                                break;
                            case '2':
                                std::cout << "Number not formated properly";
                                break;
                            case '3':
                                std::cout << "No closing ; at the end of line";
                                break;
                        }
                        break;
                    case 'l':
                        std::cout << "Let Error {at line " << this->lineNumber*10 << "} : ";
                        switch(this->status[1]){
                            case '1':
                                std::cout << "Non existent variable used for assignment";
                                break;
                            case '2':
                                std::cout << "No assignment operator next to variable";
                                break;
                            case '3':
                                std::cout << "Operation not formated correctly";
                                break;
                            case '4':
                                std::cout << "No closing ; at the end of line";
                                break;
                        }
                        break;
                    case 'i':
                        std::cout << "if Error {at line " << this->lineNumber*10 << "} : ";
                        switch(this->status[1]){
                            case '1':
                                std::cout << "Condition not formated properly";
                                break;
                            case '2':
                                std::cout << "Nested instruction not spaced properly";
                                break;
                            case '3':
                                std::cout << "No nested instruction at the correct place";
                                break;
                            case '4':
                                std::cout << "Non existent nested intruction used";
                                break;
                            case '5':
                                std::cout << "Nested instruction isn't properly formated";
                                break;
                        }
                        break;
                    case 's':
                        std::cout << "Scan Error {at line " << this->lineNumber*10 << "} : ";
                        switch(this->status[1]){
                            case '1':
                                std::cout << "Non existent variable given as argument";
                                break;
                            case '2':
                                std::cout << "Non number input recorded";
                                break;
                            case '3':
                                std::cout << "No closing ; at the end of line";
                        }
                        break;
                    default:
                        std::cout << "Error {at line " << this->lineNumber*10 << "} : Something happened lol";
                }
                return false;
        }
    }
};


class Let : public Instruction{
    double& a;
    size_t& i;
    double& u;
public:
    Let(double& a, size_t& i, double& u): a(a), i(i), u(u) {}
    bool task(std::string code, char* status, size_t currentPos){
        currentPos++;
        char test[] = "et";
        for (size_t i = 0; i < 2; i++){
            if (code[currentPos] != test[i]){
                status[1] = '5';
                return false;
            }
            currentPos++;
        }
        if (code[currentPos] != ' ') {
            status[1] = '6';
            return false;
        }
        currentPos++;
        if (code[currentPos] == ' ') {
            status[1] = '6';
            return false;
        }
        status[0] = 'l';
        switch(code[currentPos]){
            case 'a':
                currentPos++;
                if(code[currentPos] == '='){
                    if(performOperation(a, currentPos, code, a, u, i)){
                        break;
                    }
                    else{
                        status[1] = '3';
                        return false;
                    }
                }
                else{
                    status[2] = '2';
                    return false;
                }
                break;
            case 'u':
                currentPos++;
                if(code[currentPos] == '='){
                    if(performOperation(u, currentPos, code, a, u, i)){
                        break;
                    }
                    else{
                        status[1] = '3';
                        return false;
                    }
                }
                else{
                    status[2] = '2';
                    return false;
                }
                break;
            case 'i':
                currentPos++;
                if(code[currentPos] == '='){
                    if(performOperation(i, currentPos, code, a, u, i)){
                        break;
                    }
                    else{
                        status[1] = '3';
                        return false;
                    }
                }
                else{
                    status[2] = '2';
                    return false;
                }
                break;
            default:
                status[1] = '1';
                return false;
        }
        currentPos++;
        if (code[currentPos] == ';'){
            return true;
        }
        else{
            status[1] = '4';
            return false;
        }
    }
};

class Print : public Instruction{
    double& a;
    size_t& i;
    double& u;
public:
    Print(double& a, size_t& i, double& u): a(a), i(i), u(u) {}
    bool task(std::string code, char* status, size_t currentPos){
        currentPos++;
        char test[] = "rint";
        for (size_t i = 0; i < 4; i++){
            if (code[currentPos] != test[i]){
                status[1] = '5';
                return false;
            }
            currentPos++;
        }
        if (code[currentPos] != ' ') {
            status[1] = '6';
            return false;
        }
        currentPos++;
        if (code[currentPos] == ' ') {
            status[1] = '6';
            return false;
        }
        status[0] = 'p';
        switch(code[currentPos]){
            case 'a':
                std::cout << this->a << std::endl;
                break;
            case 'u':
                std::cout << this->u << std::endl;
                break;
            case 'i':
                std::cout << this->i << std::endl;
                break;
            default:
                if ((code[currentPos] < 48 || code[currentPos] > 57) && code[currentPos] != '-'){
                    status[1] = '1';
                    return false;
                }
                std::string toDouble;
                if(stodPREP(code, currentPos, toDouble)) {std::cout << toDouble << std::endl;}
                else{
                    status[1] = '2';
                    return false;
                }
        }
        currentPos++;
        if (code[currentPos] == ';'){
            return true;
        }
        else{
            status[1] = '3';
            return false;
        }
    }
};

class If : public Instruction{
    double& a;
    size_t& i;
    double& u;
    size_t& lineNumber;
public:
    If(double& a, size_t& i, double& u, size_t& lineNumber): a(a), i(i), u(u), lineNumber(lineNumber) {}
    bool task(std::string code, char* status, size_t currentPos){
        currentPos++;
        if (code[currentPos] != 'f'){
            status[1] = '5';
            return false;
        }
        currentPos++;
        if (code[currentPos] != ' ') {
            status[1] = '6';
            return false;
        }
        currentPos++;
        if (code[currentPos] == ' ') {
            status[1] = '6';
            return false;
        }
        status[0] = 'i';
        InstructionStorage ifTask = InstructionStorage(a, i, u, lineNumber);
        double value;
        currentPos--;
        if (performOperation(value, currentPos, code, a, u, i)){
            if (value != 0){
                currentPos++;
                if (code[currentPos] != ' '){
                    status[1] = '2';
                    return false;
                }
                currentPos++;
                if (code[currentPos] == ' '){
                    status[1] = '3';
                    return false;
                }
                if (ifTask[code[currentPos]]->task(code, status, currentPos)){
                    return true;
                }
                else{
                    if(status[0] == 't'){
                        status[0] = 'i';
                        status[1] = '4';
                        return false;
                    }
                    else{
                        status[0] = 'i';
                        status[1] = '5';
                        return false;
                    }
                }

            }
            else{
                return true;
            }
        }
        else{
            status[1] = '1';
            return false;
        }
    }
};

class Goto : public Instruction{
    size_t& lineNumber;
public:
    Goto(size_t& lineNumber): lineNumber(lineNumber) {}
    bool task(std::string code, char* status, size_t currentPos){
        currentPos++;
        char test[] = "oto";
        for (size_t i = 0; i < 3; i++){
            if (code[currentPos] != test[i]){
                status[1] = '5';
                return false;
            }
            currentPos++;
        }
        if (code[currentPos] != ' ') {
            status[1] = '6';
            return false;
        }
        currentPos++;
        if (code[currentPos] == ' ') {
            status[1] = '6';
            return false;
        }
        status[0] = 'g';
        size_t check = 0;
        while(code[currentPos] != ' ' && code[currentPos] != '\0' && code[currentPos] != ';'){
            if (code[currentPos] < 48 || code[currentPos] > 57){
                status[1] = '2';
                return false;
            }
            check *= 10;
            check += code[currentPos] - 48;
            currentPos++;
        }

        if(check % 10){
            status[1] = '3';
            return false;
        }
        this->lineNumber = (check/10) - 1;
        if (code[currentPos] == ';'){
            return true;
        }
        else{
            status[1] = '4';
            return false;
        }
    }
};

class Scan : public Instruction{
    double& a;
    size_t& i;
    double& u;
public:
    Scan(double& a, size_t& i, double& u): a(a), i(i), u(u) {}
    bool task(std::string code, char* status, size_t currentPos){
        currentPos++;
        char test[] = "can";
        for (size_t i = 0; i < 3; i++){
            if (code[currentPos] != test[i]){
                status[1] = '5';
                return false;
            }
            currentPos++;
        }
        if (code[currentPos] != ' ') {
            status[1] = '6';
            return false;
        }
        currentPos++;
        if (code[currentPos] == ' ') {
            status[1] = '6';
            return false;
        }
        status[0] = 's';
        std::string recorder;
        std::string result;
        size_t i = 0;
        switch(code[currentPos]){
            case 'a':
                std::cout << "Scan: ";
                std::cin >> recorder;
                if (stodPREP(recorder, i, result)){
                    this->a = std::stod(result);
                }
                else{
                    status[1] = '2';
                    return false;
                }
                break;
            case 'u':
                std::cout << "Scan: ";
                std::cin >> recorder;
                if (stodPREP(recorder, i, result)){
                    this->u = std::stod(result);
                }
                else{
                    status[1] = '2';
                    return false;
                }
                break;
            case 'i':
                std::cout << "Scan: ";
                std::cin >> recorder;
                if (stodPREP(recorder, i, result)){
                    this->i = std::stod(result);
                }
                else{
                    status[1] = '2';
                    return false;
                }
                break;
            default:
                status[1] = '1';
                return false;
        }
        currentPos++;
        if (code[currentPos] == ';'){
            return true;
        }
        else{
            status[1] = '3';
            return false;
        }
    }
};

class Error : public Instruction{
public:
    Error(){}
    bool task(std::string code, char* status, size_t currentPos){
        status[0] = 't';
        status[1] = '4';
        return false;
    }
};

InstructionStorage::InstructionStorage(double& a, size_t& i, double& u, size_t& lineNumber){
        this->store[0] = new Print(a, i, u);
        this->store[1] = new Scan(a, i, u);
        this->store[2] = new Let(a, i, u); 
        this->store[3] = new Goto(lineNumber);
        this->store[4] = new If(a, i, u, lineNumber);
        this->store[5] = new Error();
    }