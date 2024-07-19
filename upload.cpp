#include <stdio.h>
#include <iostream>
#include <string.h>
#include "./libs/Database/Database.h"
#include "./libs/Block/Block.h"
#include "./libs/Register/Register.h"
#include "./libs/Bucket/Bucket.h"
#include "./libs/Hashing/Hashing.h"
#include "./libs/BPtree/BPTree.hpp"


// 12 BYTES PARA CADA PAR chave/ponteiro
// 4096 (bloco) / 12 (par chave/ponteiro) = 340
#define ORDER_PRIMARY 340

// 308 BYTES PARA CADA PAR chave/ponteiro
// 4096 (bloco) / 308 (par chave/ponteiro) = 13
#define ORDER_SECOND 13

using namespace std;


typedef struct
{
    uint32_t num_blocks;
    uint16_t num_regs, block_occupation, reg_size;
    uint8_t reg[1595];
}
__attribute__((packed)) DBFile;


int main(int argc, char* *argv){

    if(argc < 1){
        std::cout << argv[0] << " <filename>\n";
        exit(1);
    }

    //Endereço do arquivo de dados
    const char* dbbin = "./downloads/db.bin";

    //Abre o csv
    FILE* fileCsv = fopen(argv[1], "r");
    // Abre o banco de dados ou criar caso não exista
    Database db = Database((char*)dbbin);
    
    //Criando os blocos no arquivo
    int buckets_tam = 4000;
    db.add_block(buckets_tam * BLOCKS_PER_BUCKET);

    Hashing hashing = Hashing(&db);
    Bucket bucket = Bucket(&db);

    // buffer para a leitura da linha
    char buffer[2024];
    Register reg;
    int tolRegisters = 0;

    // Para cada linha do arquivo
    while (fscanf(fileCsv,"%[^\n]%*c", buffer) > 0)
    {
        // Cria um registro com o buffer
        if(tolRegisters%200000==0) printf("Inserindo...\n");
        reg.from_buffer(buffer);
        tolRegisters += hashing.insert(reg.id, &reg); 
    }

    printf("Total de blocos(%d)\n", (buckets_tam*BLOCKS_PER_BUCKET));
    printf("Total de registros inseridos(%d)\n", tolRegisters);

    fclose(fileCsv);

    // Criando o índice

    /**
     * A aqui percorremos linermente o arquivo de dados inserindo
     * os index do registros e o id do bloco que ele está na árvore b+
    */

    printf("CRIANDO ARQUIVOS DE ÍNDICE\n");
    FILE *file = fopen("./downloads/db.bin", "rb");
    if(file == NULL)
    {
        fprintf(stderr, "Erro ao abrir arquivo de dados\n.");
        return 1;
    }

    BPTree<uint32_t, ORDER_PRIMARY> bptree_id;
    BPTree<string, ORDER_SECOND> bptree_title;
    unordered_map<string, int> map;
    DBFile dbf;
    uint8_t blk[4096];
    fread(&dbf.num_blocks, sizeof(dbf.num_blocks), 1, file);

    // Percorre o arquivo de dados e indexa em árvores b+
    for(int i = 0; i < dbf.num_blocks; i++)
    {
        fseek(file, (i * 4096) + 4, SEEK_SET);
        fread(blk, sizeof(blk), 1, file);
        memcpy(&dbf.num_regs, &blk[0], sizeof(dbf.num_regs));

        size_t k = 0;
        for(int j = 0; j < dbf.num_regs; j++)
        {
            memcpy(&dbf.reg_size, &blk[k+4], sizeof(dbf.reg_size));

            // Extrai o id de cada registro e indexa
            uint32_t id;
            memcpy(&id, &blk[k+6], sizeof(id));
            bptree_id.insert(id, i);

            // Extrai o title de cada registro e indexa
            uint8_t buff[300];
            memcpy(buff, &blk[k+10], 300);
            string title(reinterpret_cast<const char *>(buff));
            bptree_title.insert(title, i);
            k += dbf.reg_size + 2;
        }
    }
    bptree_id.print();

    printf("SALVANDO NÓS EM ARQUIVO\n");
    bptree_id.serialize("./downloads/primary_index.bin");
    bptree_title.serialize("./downloads/second_index.bin");
}