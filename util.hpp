#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

bool isOperator(char chr){
    switch(chr){
        case '+':
            return true;
        case '-':
            return true;
        case '*':
            return true;
        case '/':
            return true;
        case '=':
            return true;
        case '>':
            return true;
        case '<':
            return true;
        default: 
            return false;
    }
}

bool stodPREP(std::string testString, size_t& index, std::string& destination){
    bool wasDot = false;
    if ((testString[index] < 48 || 57 < testString[index]) && testString[index] != '-'){
        return false;
    }
    destination += testString[index];
    index++;
    while (testString[index] != ' ' && testString[index] != '\0' && testString[index] != ';' && !(isOperator(testString[index]))){
        if (testString[index] != '.' && (testString[index] < 48 || 57 < testString[index])){
            return false;
        }
        if (testString[index] == '.'){
            if (wasDot){
                return false;
            }
            wasDot = true;
        }
        destination += testString[index];
        index++;
    };
    index--;
    return true;
}

template <typename T, typename C>
double operation(T numb1, char* operate, C numb2){
    switch(operate[0]){
        case '+':
            return (numb1 + numb2);
        case '-':
            return (numb1 - numb2);
        case '*':
            return (numb1 * numb2);
        case '/':
            return (numb1 / numb2);
        case '=':
            return (numb1 == numb2);
        case '!':
            return (numb1 != numb2);
        case '>':
            switch(operate[1]){
                case '=':
                    return (numb1 >= numb2);
                default:
                    return (numb1 > numb2);
            }
        case '<':
            switch(operate[1]){
                case '=':
                    return (numb1 <= numb2);
                default:
                    return (numb1 < numb2);
            }
        default:
            throw "error";
    }
}

template <typename T>
bool performOperation(T& destination, size_t& ind, std::string text, double a, double u, size_t i){
    double numb1;
    double numb2;
    char oper[3];
    std::string placeholder;
    ind++;
    switch(text[ind]){
        case 'a':
            numb1 = a;
            break;
        case 'u':
            numb1 = u;
            break;
        case 'i':
            numb1 = i;
            break;
        default:
            if (stodPREP(text, ind, placeholder)){
                numb1 = std::stod(placeholder);
                placeholder = "";
            }
            else{
                return false;
            }
    }
    ind++;
    if (isOperator(text[ind])){
        oper[0] = text[ind];
        ind++;
        if(text[ind] == '='){
            oper[1] = text[ind];
            switch(text[ind-1]){
                case '=':
                    break;
                case '<':
                    break;
                case '>':
                    break;
                default:
                    return false;
            }
            ind++;
        }
    }
    else{
        ind--;
        destination = numb1;
        return true;
    }
    switch(text[ind]){
        case 'a':
            numb2 = a;
            break;
        case 'u':
            numb2 = u;
            break;
        case 'i':
            numb2 = i;
            break;
        default:
            if (stodPREP(text, ind, placeholder)){
                numb2 = std::stod(placeholder);
            }
            else{
                return false;
            }
    }
    destination = operation(numb1, oper, numb2);
    return true;
}


class Instruction{
public:
    Instruction(){}
    virtual bool task(std::string code, char* status, size_t currentPos) = 0;
    virtual ~Instruction(){}
};

 
class Code{
    std::string* store;
    size_t size;
    char status[3];
public:
    Code(std::string file){
        this->store = new std::string[1];
        this->status[2] = '\0';
        this->status[0] = 'f';
        std::string test = "bcalc";
        int difference = file.size() - 5;
        for (int i = 4; i >= 0; i--){
            if (test[i] == file[i+difference]) {continue;}
            else{
                this->status[1] = '1';
                return;
            }
        }
        std::string st;
        std::string* temp;
        std::ifstream reading(file);

        if (reading){
            this->status[1] = '0';
            this->size = 1;
            std::getline(reading, this->store[0]);
            size_t i;

            while(std::getline(reading, st)){
                this->size++;
                temp = new std::string[size];
                for (i = 0; i < (this->size)-1; i++){
                    temp[i] = this->store[i];
                }
                delete[] this->store;
                this->store = temp;
                this->store[this->size-1] = st;
            }
            reading.close();
        }
        else{
            reading.close();
            this->status[1] = '2';
            return;
        }
    }
    size_t getSize() const {return this->size;}

    void getStatus(char* status){
        status[0] = this->status[0];
        status[1] = this->status[1];
    }

    std::string operator[](size_t lineNumber){
        if (lineNumber > this->size || lineNumber <= 0){
            this->status[0] = 'g';
            this->status[1] = '1';
            return "";
        }
        this->status[0] = 't';
        return this->store[lineNumber-1];
    }

    ~Code(){
        delete[] (this->store);
    }
};
