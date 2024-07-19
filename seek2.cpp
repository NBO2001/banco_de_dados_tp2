#include "./libs/BPtree/BPTree.hpp"
#include "./libs/Database/Database.h"
#include "./libs/Block/Block.h"
#include "./libs/Register/Register.h"

#define ORDER 13

using namespace std;

void block_print(Block *bk, string title)
{
    unsigned int offset = 0;

    Register *reg = new Register();

    for(int i = 0; i < bk->n_regs; i++)
    {
        uint16_t size_reg = 0;
        memcpy(&size_reg, bk->block + offset, sizeof(size_reg));
        offset += 2;

        reg->from_bytes((char *) bk->block+offset);
        offset += size_reg;
        if(reg->title == title)
        {
            reg->print();
            delete reg;
            return;
        }
    }
    delete reg;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "USAGE: seek2 <title>\n");
        return 1;
    }

    string TITLE = "\"" + string(argv[1]) + "\"";

    FILE *file = fopen("./downloads/db.bin", "rb");
    if(file == NULL)
    {
        fprintf(stderr, "Erro ao abrir arquivo de dados\n.");
        return 1;
    }

    Database dbFile = Database((char*) "./downloads/db.bin");
    Block *blk;

    BPTree<string, ORDER> bptree;
    bptree.deserialize("downloads/second_index.bin");
    uint64_t offset = bptree.search(TITLE);
    printf("Offset(%d)\n", (int) offset);
    blk = dbFile.read(offset);
    block_print(blk, TITLE);

    delete blk;

    return 0;
}