#include "hashing.h"
// still need to implement loop that tests different sizes
// still need to implement time counting
// still need to implement a safer way of reading strings 
#define HASH_SIZE 997

int hash_function(int key, int size){
     return key % size;
}

int main() {
    //int hash_sizes[4] = {997, 1999, 3989, 7993};
    char buff[200]; 

    // HASHING
    
    Player ** hash_table = create_hash_table(HASH_SIZE);

    FILE * players_csv = fopen("players.csv", "r");

    if (players_csv == NULL) {
        printf("players file not found\n");
        return 1;
    }

    // gettting rid of first line
    fgets(buff, sizeof(buff), players_csv);

    while (fgets(buff, sizeof(buff), players_csv) != NULL) {
        Player * new_player = (Player *)malloc(sizeof(Player));

        char *field = strtok(buff, ",");
        new_player->id = atoi(field);
        field = strtok(NULL, ",");
        strcpy(new_player->name, field);
        field = strtok(NULL, "\"\n");
        strcpy(new_player->position, field);
        new_player->next = NULL;

        insert(hash_table, new_player, new_player->id, HASH_SIZE, *hash_function);
    }

    // Printing hash table
    FILE * hash_result = fopen("player_hash.txt", "w");

    print_hash_file(hash_table, HASH_SIZE, hash_result);

    fclose(hash_result);
    
    // Consultas
    FILE * consultas_csv = fopen("consultas.csv", "r");

    int search_id;
    if (consultas_csv == NULL) {
        printf("searches file not found");
        return 1;
    }
    while (fgets(buff, sizeof(buff), consultas_csv) != NULL) {
        search_id = atoi(buff);
        Player * searched_player = search(hash_table, search_id, HASH_SIZE, *hash_function);
        if (searched_player == NULL) {
            // do something about the player not being in the hash
        }
        else{
            // do something else about the player being in the hash
        }
    }

    fclose(players_csv);
    fclose(consultas_csv);
    delete_hash(hash_table,HASH_SIZE);
    
    return 0;
}

