[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/OG1gh2Yo)

# Trabalho de Banco de Dados

## Sumário

1. [Introdução](#introducao)
2. [Estrutura do código](#estruturacodigo)
3. [Compilação](#compilacao)

## Integrantes

* Beatriz Braga Batista
* Felipe Fraxe
* Natanael Bezerra de Oliveira

<div id='introducao'/>

# Introdução

Optamos por adotar uma abordagem de registros não espalhados em nosso projeto. Além disso, para simplificar a implementação, decidimos não utilizar a técnica de rehashing. Nosso arquivo é composto por um número fixo de 280.000 blocos. Outra decisão de projeto foi remover caracteres especiais tanto dos títulos quanto dos **snnipts**, com o objetivo de reduzir o tamanho dos registros.

### Estrutura do **Registros**:

**offset**: 2 bytes. Aponta para o final do registo (Ou também pode ser usado para saber o tamanho do registo)

**id**: 4 bytes.

**title**: 300 bytes.

**year**: 2 bytes.

**autors**: 250 bytes.

**cited**: 4 bytes.

**year_updated**: 2 bytes

**month_updated**: 1 byte

**day_updated**: 1 byte

**hour_updated**: 1 byte

**minutes_updated**: 1 byte

 **seconds_updated**: 1 byte

**snippet_size**: 2 bytes. Tamanho do snippet.

**snippet**:  de 100 a 1024 bytes

Logo, os registros vão ficar entre 671 a 1595 bytes.

### Estrutura do **Bloco**:

**n_regs**: 2 bytes para o número de registros.

**size**: 2 bytes para o espaço ocupando no bloco.

**block**: 4092 bytes ( Porque é um bloco de 4096, mas estou ocupando 4 bytes para controles)

### Estrutura do **Database (File)**:

**n_blocks**: 4 bytes

Os bytes seguintes são os blocos.

<div id='estruturacodigo'/>

## Estrutura do Código

Nós decidimos estruturar nosso código em módulos, visando facilitar a implementação. Temos a seguinte estrutura:

- **downloads:** Pasta para armazenar o CSV de entrada e os arquivos binários resultantes:
  
  **db.bin:** Arquivo de dados.
  
  **primary_index.bin:** Arquivo de índice primário.
  
  **second_index.bin:** Arquivo de índice secundário.
  
- **libs:** Pasta que contém todas as bibliotecas do projeto.
  
  **libs/Register:** Módulo responsável por transformar os bytes do buffer em objetos de registro.
  
  **libs/Block:** Módulo para definição e manipulação de blocos de dados.
  
  **libs/Bucket:** Módulo para definição e manipulação de buckets.
  
  **libs/Hashing:** Módulo para definição e manipulação de funções de hashing, incluindo inserção e busca.
  
  **libs/Database:** Módulo para interface de manipulação do arquivo de dados.
  
  **libs/BPtree:** Módulo para implementação da árvore B+, incluindo funções de busca e inserção.

Todos os módulos são utilizados nos arquivos **upload.cpp**, **findrec.cpp**, **seek1.cpp** e **seek2.cpp**, portanto, as bibliotecas devem ser compiladas juntas.

<div id='compilacao'/>

## Compilação


Compilação e execução dos arquivos temporários:

1. Criando o banco de dados e inserindo os registros

```bash
g++ ./libs/*/*.*pp upload.cpp -o upload
./upload downloads/artigo.csv
```

2. Buscando um registro por ID

```bash
g++ ./libs/*/*.*pp findrec.cpp -o findrec
./findrec <ID>
```

3. Buscando no arquivo de índice primário.

```bash
g++ ./libs/*/*.*pp seek1.cpp -o seek1
./seek1 <ID>
```

4. Buscando no arquivo de índice primário.


```bash
g++ ./libs/*/*.*pp seek2.cpp -o seek2
./seek2 <Titulo>
```

Exemplo de uso

```bash
./seek2 "Natural user interface hardware."
```