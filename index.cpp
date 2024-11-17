#include "instruction.hpp"


int main(void){
    std::string file;
    std::cout << "Specify the file(.bcalc) to be interpreted : ";
    std::cin >> file;
    Interpreter Main = Interpreter(file);
    while(Main.wereError()){Main.interpret();}
    return 0;
}
