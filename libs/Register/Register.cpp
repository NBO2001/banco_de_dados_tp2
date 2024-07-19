#include "Register.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include "stdlib.h"

/**
 * Função para remover caracteres especiais, é uma função um pouco cara
 *  mas ajuda a diminuir o tamanho dos snnipets
*/
void Register::removeSpecialCharacters(unsigned char * string){

    int tamStr = strlen((char *)string);
    int indexCopia = 0 ;

    int i = 0 ;
    
    while(i < tamStr){
        
        if(string[i] == 195){

            if((string[i+1] >= 160 && string[i+1] <= 165) || (string[i+1] >= 128 && string[i+1] <= 133) ) string[indexCopia] = 97;
            else if((string[i+1] >= 168 && string[i+1] <= 171) || (string[i+1] >= 136 && string[i+1] <= 139) ) string[indexCopia] = 101;
            else if( (string[i+1] >= 172 && string[i+1] <= 174) || (string[i+1] >= 140 && string[i+1] <= 143)   ) string[indexCopia] = 105;
            else if( (string[i+1] >= 178 && string[i+1] <= 181) || (string[i+1] >= 146 && string[i+1] <= 150)   ) string[indexCopia] = 111;
            else if( (string[i+1] >= 185 && string[i+1] <= 188) || (string[i+1] >= 153 && string[i+1] <= 156)   ) string[indexCopia] = 117;
            else if( (string[i+1] == 167 || string[i+1] == 135)    ) string[indexCopia] = 99;
            else if( (string[i+1] == 177)    ) string[indexCopia] = 110;

            indexCopia++;
            i = i + 2;

        }else if(string[i] == 194) i = i + 2;
        else if(string[i] == 226){
            string[indexCopia] = 39;
            indexCopia++;
            i = i + 3;
        }else if( string[i] == 198 ){
            string[indexCopia] = 102;
            indexCopia++;
            i = i + 3;
        }
        else{
            string[indexCopia] = string[i];
            indexCopia++;
            i++;
        }
    }

    string[indexCopia] = '\0';


}

/***
 * Mostra um registro de forma legivel na tela
*/
void Register::print(){

    printf("Register: {\n\t\tid: %d, \n\t\ttitle: %s, \n\t\tyear: %hd, \n\t\tautors: %s, \n\t\tcited: %d, \n\t\tupdate: %d-%02d-%02d %02d:%02d:%02d,\n\t\tsnippet: %s \n}\n",
    id, title, year, autors, cited, 
    year_updated, month_updated, day_updated,
    hour_updated, minutes_updated, seconds_updated, snippet);

    // printf("Register: {\n\t\tid: %d, \n\t\ttitle: %s, \n\t\tyear: %hd, \n\t\tautors: %s, \n\t\tcited: %d, \n\t\tupdate: %d-%02d-%02d %02d:%02d:%02d\n}\n",
    // id, title, year, autors, cited, 
    // year_updated, month_updated, day_updated,
    // hour_updated, minutes_updated, seconds_updated);

}

void Register::from_buffer(char* buffer){
    char flag[2] = {0x22, 0x3b};
    std::pair<int, int> stack[7];
    int tan_stack = 0;

    int len = strlen(buffer);

    int start_field = 0;

    int i = 0;
    int tol_read;

    /**
     * Nessa parte vou procurar todas as aspas que são seguidas de virgula. 
     *  usei isso para delimitar onde começa e termina um campo do csv.
     *  Isso deixa o código mais lendo, mas é efinciente e não tão díficil de implementar
    */
    for (; i < len - 1; i++) {
        if (memcmp(buffer + i, flag, 2) == 0) {
            tol_read = (i + 1) - start_field;

            stack[tan_stack].first = start_field;
            stack[tan_stack].second = tol_read;
            tan_stack++;

            start_field = i + 2;
        }
    }

    tol_read = len - start_field;
    stack[tan_stack].first = start_field;
    stack[tan_stack].second = tol_read;
    tan_stack++;

    // Parsing the fields
    sscanf(buffer + stack[0].first, "\"%d\"", &id);

    memccpy(title, buffer + stack[1].first, 1, stack[1].second);
    title[stack[1].second] = '\0';

    sscanf(buffer + stack[2].first, "\"%hd\"", &year);

    memccpy(autors, buffer + stack[3].first, 1, stack[3].second);
    autors[stack[3].second] = '\0';

    sscanf(buffer + stack[4].first, "\"%d\"", &cited);

    sscanf(buffer + stack[5].first, "\"%hd-%hhd-%hhd %hhd:%hhd:%hhd\"",
           &year_updated, &month_updated, &day_updated,
           &hour_updated, &minutes_updated, &seconds_updated);

    if(stack[6].second > 1025){
        memccpy(snippet, buffer + stack[6].first, 1, 1000);
        snippet[1001] = '\0';
    }else{
        memccpy(snippet, buffer + stack[6].first, 1, stack[6].second);
        snippet[stack[6].second] = '\0';
    }

    // removeSpecialCharacters((unsigned char*) title);
    removeSpecialCharacters((unsigned char*) snippet);
    
}

/**
 * Copia um objeto registro para um vetor de bytes.
*/
char* Register::to_bytes(){

    unsigned short size = 569;
    size += strlen(snippet);

    char* content = (char*) malloc(size+2);

    int offset = 0;

    memcpy(content+offset, &size, 2);
    offset += 2;

    memcpy(content+offset, &id, 4 );
    offset += 4;

    memcpy(content+offset, title, 300);
    offset += 300;

    memcpy(content+offset, &year, 2);
    offset += 2;

    memcpy(content+offset, autors, 250);
    offset += 250;

    memcpy(content+offset, &cited, 4 );
    offset += 4;

    memcpy(content+offset, &year_updated, 2);
    offset += 2;

    memcpy(content+offset, &month_updated, 1);
    offset += 1;

    memcpy(content+offset, &day_updated, 1 );
    offset += 1;

    memcpy(content+offset, &hour_updated, 1);
    offset += 1;

    memcpy(content+offset, &minutes_updated, 1);
    offset += 1;

    memcpy(content+offset, &seconds_updated, 1);
    offset += 1;

    unsigned short snippet_size = (unsigned short) strlen(snippet);

    memcpy(content+offset, &snippet_size, 2 );
    offset += 2;

    memcpy(content+offset, snippet, snippet_size);
    // offset += snippet_size;

    
    return content;

}

/**
 * Cria um objeto registro a partir de um vetor de bytes 
*/
void Register::from_bytes(char* buffer){

    int offset = 0;

    memcpy(&id, buffer, 4);
    offset += 4;

    memcpy(title, buffer+offset, 300);
    offset += 300;

    memcpy(&year, buffer+offset, 2);
    offset += 2;

    memcpy(autors, buffer+offset, 250);
    offset += 250;

    memcpy(&cited,buffer+offset, 4 );
    offset += 4;

    memcpy(&year_updated,buffer+offset, 2);
    offset += 2;

    memcpy(&month_updated,buffer+offset, 1);
    offset += 1;

    memcpy(&day_updated, buffer+offset,1 );
    offset += 1;

    memcpy(&hour_updated, buffer+offset, 1);
    offset += 1;

    memcpy(&minutes_updated, buffer+offset, 1);
    offset += 1;

    memcpy(&seconds_updated, buffer+offset,1);
    offset += 1;

    unsigned short snippet_size;

    memcpy(&snippet_size, buffer+offset,2 );
    offset += 2;

    memcpy(snippet, buffer+offset, snippet_size);
    snippet[snippet_size] = '\0';
    offset += snippet_size;
    
}