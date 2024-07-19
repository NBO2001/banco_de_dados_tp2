#ifndef BPTREE_HPP
#define BPTREE_HPP

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

// Descrição geral da Árvore B+ com declaração dos métodos de inserção, criação e armazenamento em memória secundária.
template <typename T, size_t M>
class BPTree
{
    private:
        struct Node;
        Node *root;
        void split_node(Node *curr, stack<Node *>& parents);
        void update_children_pointer(Node *curr, Node *left);
        void destroy_tree(Node *node)
        {
            if(node == nullptr)
                return;
            for(const auto &child : node->children)
                destroy_tree(child);
            delete node;
        }

    public:
        BPTree()
        {
            this->root = nullptr;
        }

        ~BPTree()
        {
            this->destroy_tree(this->root);
        }

        void insert(T key, uint64_t data);
        uint64_t search(T key);
        void print(void);
        void serialize(const char* filename);
        void deserialize(const char* filename);
};


// Descrição geral de um nó da árvore, contento os campos de chaves, filhos (nós intermediários) ou dados (nós folha),
// e ponteiro pra o próximo nó, pra ajudar na leitura sequencial 
template <typename T, size_t M>
struct BPTree<T, M>::Node
{
    vector<T> keys;
    vector<Node *> children;
    vector<uint64_t> data;
    Node *next;

    Node()
    {
        this->keys.reserve(M+1);
        this->children.reserve(M+1);
        this->data.reserve(M+1);
        this->next = nullptr;
    }

    bool is_leaf(void);
    size_t find_pos(T key);
};


// Se o array de filhos estiver vazio, significa que é nó filho. Dessa forma um campo booleano na estrutura do nó é economizado
template<typename T, size_t M>
bool BPTree<T, M>::Node::is_leaf(void)
{
    return this->children.empty();
}


// Função de busca binária adaptada para saber qual a posição correta de uma chave em um nó OU qual posição correta para inseri-lo,
// para manter a ordenação intacta. O controle da ordenação é feito pelas inserções.
template<typename T, size_t M>
size_t BPTree<T, M>::Node::find_pos(T key)
{
    int low = 0;
    int high = this->keys.size() - 1;
    size_t mid = low + (high - low) / 2;

    while(low <= high)
    {
        if(key > this->keys[mid])
            low = mid + 1;
        else if(key < this->keys[mid])
            high = mid - 1;
        else
            return mid;
        mid = low + (high - low) / 2;
    }
    return mid;
}


// Método de inserir uma chave na árvore
template<typename T, size_t M>
void BPTree<T, M>::insert(T key, uint64_t data)
{
    if(this->root == nullptr)
        this->root = new Node();

    Node *node = this->root;
    stack<Node *> parents;

    // Caso não seja folha, encontra a folha correta para inserir, enquanto empilha os pais para possível operação de split
    while(node->is_leaf() == false)
    {
        parents.push(node);
        size_t pos = node->find_pos(key);
        node = node->children[pos];
    }

    // Insere ordenadamente. Em caso de overflow do nó, a operação de split é iniciada, com a chave central indo para o nó pai
    size_t pos = node->find_pos(key);
    node->keys.insert(node->keys.begin() + pos, key);
    node->data.insert(node->data.begin() + pos, data);
    if(node->keys.size() > M)
        this->split_node(node, parents);
}


// Função auxiliar para atualizar o ponteiro na liked list formada pelos nós folha
template<typename T, size_t M>
void BPTree<T, M>::update_children_pointer(Node *curr, Node *left)
{
    Node *node = this->root;
    while(node->is_leaf() == false)
        node = node->children[0];

    if(node == curr)
        return;

    while(node->next != curr)
        node = node->next;
    node->next = left;
}


// Método de split de nó
template<typename T, size_t M>
void BPTree<T, M>::split_node(Node *curr, stack<Node *>& parents)
{
    size_t mid = M / 2;
    Node *left = new Node();
    Node *right = new Node();

    // Os dados do nó com overflow são distribuídos por dois novos nós (left e right). Uma cópia da chave central é mantida no nó da
    // esquerda para manter a propriedade de que todos os dados estão presentes nas folhas
    right->keys.assign(curr->keys.begin() + mid + 1, curr->keys.end());
    if(curr->is_leaf() == false)
    {
        left->keys.assign(curr->keys.begin(), curr->keys.begin() + mid);
        left->children.assign(curr->children.begin(), curr->children.begin() + mid + 1);
        right->children.assign(curr->children.begin() + mid + 1, curr->children.end());
    }
    else
    {
        left->keys.assign(curr->keys.begin(), curr->keys.begin() + mid + 1);
        left->data.assign(curr->data.begin(), curr->data.begin() + mid + 1);
        right->data.assign(curr->data.begin() + mid + 1, curr->data.end());

        // Em caso de um nó folha, após a redistribuição dos dados, há uma atualização de ponteiro na linked list        
        if(parents.empty() == false)
        {
            Node *parent = parents.top();
            size_t pos = parent->find_pos(curr->keys[0]);
            if(parent->children[pos] != curr)
            {
                int i = 0;
                while(parent->children[i] != curr)
                    i++;
                pos = i;
            }
   
            if(pos > 0)
                parent->children[pos-1]->next = left;
            else
                this->update_children_pointer(curr, left);
        }
        left->next = right;
        right->next = curr->next;
    }

    // Caso o nó splitado seja uma raíz, uma nova raíz é criada, aumentando a altura da árvore
    if(parents.empty())
    {
        this->root = new Node();
        this->root->keys.push_back(curr->keys[mid]);
        this->root->children.push_back(left);
        this->root->children.push_back(right);
        delete curr;
    }
    // Caso contrário, a chave central é inserida no nó pai ao da operação e, se necessário, o split também é feito nele de modo recurssivo
    else
    {
        Node *parent = parents.top();
        parents.pop();
        size_t pos = parent->find_pos(curr->keys[mid]);
        if(parent->children[pos] != curr)
        {
            int i = 0;
            while(parent->children[i] != curr)
                i++;
            pos = i;
        }

        parent->keys.insert(parent->keys.begin() + pos, curr->keys[mid]);
        parent->children[pos] = left;
        parent->children.insert(parent->children.begin() + pos + 1, right);
        //delete curr;
        if(parent->keys.size() > M)
            this->split_node(parent, parents);
    }
}


// Operação de busca binária em cada nó (bloco). Caso o dado de interesse não esteja presente, o offset 0 é retornado.
template<typename T, size_t M>
uint64_t BPTree<T, M>::search(T key)
{
    Node *node = this->root;
    if(node == nullptr)
        return 0;

    size_t pos, blk_quant = 1;
    while(node->is_leaf() == false)
    {
        blk_quant++;
        pos = node->find_pos(key);
        node = node->children[pos];
    }

    printf("%zu BLOCOS LIDOS\n", blk_quant);
    pos = node->find_pos(key);
    if(node->keys[pos] == key)
        return node->data[pos];
    return 0;
}


// Função para armazenar os nós da árvore em memória secundária.
template <typename T, size_t M>
void BPTree<T, M>::serialize(const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to open file for writing\n");
        return;
    }

    Node *node = this->root;
    while(node->is_leaf() == false)
        node = node->children[0];

    int i = 0;
    for(node; node != nullptr; node = node->next)
    {
        for(int j = 0; j < node->keys.size(); j++)
        {
            if constexpr(is_same_v<T, string>)
            {
                node->keys[j].resize(300, '\0');
                fwrite(node->keys[j].c_str(), 300, 1, file);
            }
            else
                fwrite(&node->keys[j], sizeof(T), 1, file);
            fwrite(&node->data[j], sizeof(uint64_t), 1, file);
        }
        printf("ESCREVENDO BLOCO %d\n", ++i);
    }
    printf("QUANTIDADE DE BLOCOS ESCRITOS: %d\n", i);
    fclose(file);
}


// Função para carregar os nós da árvore para memória principal a partir do arquivo de índice
template <typename T, size_t M>
void BPTree<T, M>::deserialize(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to open file for reading\n");
        return;
    }

    uint64_t offset;
    if constexpr(is_same_v<T, string>)
    {
        uint8_t buff[300];
        while(fread(buff, 300, 1, file))
        {
            fread(&offset, sizeof(offset), 1, file);
            string key(reinterpret_cast<char *>(buff));
            this->insert(key, offset);
        }
    }
    else
    {
        T key;
        while(fread(&key, sizeof(T), 1, file))
        {
            fread(&offset, sizeof(offset), 1, file);
            this->insert(key, offset);
        }
    }
    fclose(file);
}


template <typename T, size_t M>
void BPTree<T, M>::print(void)
{
    Node *node = this->root;
    if(node == nullptr)
        return;

    while(node->is_leaf() == false)
        node = node->children[0];

    for(node; node != nullptr; node = node->next)
    {
        for(const auto &key : node->keys)
            cout << key << endl;
    }
}

#endif