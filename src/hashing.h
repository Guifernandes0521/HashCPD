#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct PLAYER {
     int id;
     char *name;
     char *position;
     struct PLAYER * next;
} Player;

Player ** create_hash_table(int size);

void insert(Player ** hash_table, Player * jogador, int key, int size, int (*hash_function)(int key, int size));

Player * search(Player ** hash_table, int search_id, int size, int (*hash_function)(int key, int size));

void delete_linked_list(Player * jogador);

void delete_hash(Player ** hash_table, int size);

void print_hash_info(Player ** hash_table, int size, FILE * hash_txt);

Player ** create_hash_table(int size) {
     // cria vetor de ponteiros
     Player ** hash_table = (Player**)malloc((size_t)size * sizeof(Player*));
     
     if(hash_table == NULL) {
          return NULL;
     }

     for(int i = 0; i < size; i++) {
          hash_table[i] = NULL;
     }

     return hash_table;
}

void insert(Player **hash_table, Player * jogador, int key, int size, int (*hash_function)(int key, int size)) {
    int search_index = hash_function(key, size);
    // Check if the slot is empty
    if (hash_table[search_index] == NULL) {
        hash_table[search_index] = jogador; // Insert jogador directly into the hash table
    } 
    else {
        // Slot is not empty, find the end of the list and insert there
        Player * current_player = hash_table[search_index];
        while (current_player->next != NULL) {
            current_player = current_player->next;
        }
        current_player->next = jogador; // Insert jogador at the end of the list
    }
}

Player * search(Player ** hash_table, int search_id, int size, int (*hash_function)(int key, int size)){
     int search_index = hash_function(search_id, size);
     Player * current_player = hash_table[search_index];
     
     while (current_player != NULL) {
          if (current_player->id == search_id) {
               return current_player;
          }
          current_player = current_player->next;
     }
     return NULL;
}
// modify this gut to also free the string allocated inside of the struct
// maybe make this not recursive
void delete_linked_list(Player * jogador) {
     if (jogador == NULL) {
          return;
     }
     if (jogador->next != NULL) {
          delete_linked_list(jogador->next);
     }
     free(jogador->name);
     free(jogador->position);
     free(jogador);
}

void delete_hash(Player ** hash_table, int size) {
     for (int i = 0; i < size; i++) {
          delete_linked_list(hash_table[i]);
     }
     free(hash_table);
}

void print_hash_info(Player ** hash_table, int size, FILE * hash_txt) {
     for(int i = 0; i < size; i++) {
          fprintf(hash_txt ,"[%d]: ",i);
          if (hash_table[i] == NULL) {
               fprintf(hash_txt ,"empty\n");
          }
          else{
               Player * aux_player = hash_table[i];
               while (aux_player != NULL) {
                    fprintf(hash_txt,"%s ", aux_player->name);
                    
                    if (aux_player->next != NULL) {
                         fprintf(hash_txt ,"-> ");
                    }
                    else{
                         fprintf(hash_txt, "\n");
                    }
                    aux_player = aux_player->next;
               }
          }
     }
}
