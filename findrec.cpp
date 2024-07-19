#include <stdio.h>
#include <iostream>
#include <string.h>
#include "./libs/Database/Database.h"
#include "./libs/Block/Block.h"
#include "./libs/Register/Register.h"
#include "./libs/Hashing/Hashing.h"



int main(int argc, char* *argv){

    Database db = Database((char*)"./downloads/db.bin");
    Hashing hashing = Hashing(&db);

    if(argc < 1){
        std::cout << argv[0] << " <ID>\n";
        exit(1);
    }

    Register* reg;

    // Ler o id ;-;
    int id = atoi(argv[1]);

    // Procura no hash;-;
    reg = hashing.search(id);
    if(reg){
        // Mostra se existir ;-;
        reg->print();
        delete reg;
    }


}