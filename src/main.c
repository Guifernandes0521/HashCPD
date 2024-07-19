#include "hashing.h"
// still need to implement time counting

int hash_function(int key, int size){
     return key % size;
}

int main() {
    int hash_sizes[4] = {997, 1999, 3989, 7993};
    char buff[200]; 

    // looping through different hash table sizes
    for (int i = 0; i < 4; i++) {
        int max_list_size = 0;
        int sum_list_size = 0;
        int hash_occupation = 0;
        int max_miss = 0;
        int avg_miss = 0;
        // average list size = sum_list_size / hash_occupation

        Player ** hash_table = create_hash_table(hash_sizes[i]);

        if(hash_table == NULL) {
            printf("problem creating hash_table\n");
        }

        FILE * players_csv = fopen("players.csv", "r");

        if (players_csv == NULL) {
            printf("players file not found\n");
            return 1;
        }

        // gettting rid of first line
        fgets(buff, sizeof(buff), players_csv);

        while (fgets(buff, sizeof(buff), players_csv) != NULL) {
            Player *new_player = (Player *)malloc(sizeof(Player));

            char *field = strtok(buff, ",");
            new_player->id = atoi(field);

            field = strtok(NULL, ",");
            new_player->name = malloc(strlen(field) + 1); // +1 for the null terminator
            strcpy(new_player->name, field);

            field = strtok(NULL, "\"\n");
            new_player->position = malloc(strlen(field) + 1); // +1 for the null terminator
            strcpy(new_player->position, field);

            new_player->next = NULL;

            insert(hash_table, new_player, new_player->id, hash_sizes[i], *hash_function);
        }
        // get hash stats here
        // max_list_size, avg_list_size, occupation, construction_time
        
        // Printing hash table
        char file_name[50];
        // Using sprintf to format string similar to f-strings in Python
        sprintf(file_name, "player_hash_%d.txt", hash_sizes[i]);

        FILE * hash_result = fopen(file_name, "w");

        print_hash_file(hash_table, hash_sizes[i], hash_result);

        fclose(hash_result);
        
        // Consultas
        FILE * consultas_csv = fopen("consultas.csv", "r");

        if (consultas_csv == NULL) {
            printf("search file not found");
            return 1;
        }

        // oppening file for search stats
        FILE * search_stats = fopen("search_stats.txt", "w");

        if (search_stats == NULL) {
            printf("search_stats file not found");
            return 1;
        }

        int search_id;
        // needs to do this one time to get the stats, and other to write to the file, or it'll take too long.
        while (fgets(buff, sizeof(buff), consultas_csv) != NULL) {
            search_id = atoi(buff);
            // need to get search stats here ()
            // auxiliary player struct for query
            Player * searched_player = search(hash_table, search_id, hash_sizes[i], *hash_function);
            if (searched_player == NULL) {
                printf("missed");
            }
            else{
                printf("found");
            }
        }
        // print searching time, searches per player, max "", avg """

        fclose(players_csv);
        fclose(consultas_csv);
        delete_hash(hash_table,hash_sizes[i]);
    }    
    return 0;
}

