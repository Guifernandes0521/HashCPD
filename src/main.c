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
        // time variables
        clock_t start, end;
        double constructing_time, searching_time = 0;
        
        // stats variables
        float occupied_ratio;
        int  max_list_size, average_list_size;
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

        // starting time count
        start = clock();
        // loop for constructing the hash
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
        // finish time count
        end = clock();
        // calculating time
        constructing_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        // get hash stats here
        hash_stats(hash_table, hash_sizes[i], &occupied_ratio, &max_list_size, &average_list_size);

        // print first part of stats on file

        // Printing hash table
        char file_name[50];
        // Using sprintf to format string similar to f-strings in Python
        sprintf(file_name, "hash_stats_%d.txt", hash_sizes[i]);

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
        int max_testes;
        // needs to do this one time to get the stats, and other to write to the file, or it'll take too long.
        while (fgets(buff, sizeof(buff), consultas_csv) != NULL) {
            int num_testes;
            search_id = atoi(buff);
            //search time starts here
            start = clock();
            
            Player * searched_player = search(hash_table, search_id, hash_sizes[i], &num_testes, *hash_function);
            
            end = clock();
            
            searching_time += ((double) (end - start)) / CLOCKS_PER_SEC;


            if (searched_player == NULL) {
                // write search on file
            }
            else{
                // write search on file
            }
        }
        // writes time to the file;
    


        fclose(players_csv);
        fclose(consultas_csv);
        fclose(search_stats);
        delete_hash(hash_table,hash_sizes[i]);
    }    
    return 0;
}

