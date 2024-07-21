#include "hashing.h"

// implement the desired hash
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
        double constructing_time = 0, searching_time = 0;
        
        // stats variables
        float occupied_ratio;
        int  max_list_size, average_list_size;
        
        // creating hash table 
        Player ** hash_table = create_hash_table(hash_sizes[i]);
        if(hash_table == NULL) {
            printf("problem creating hash_table\n");
        }
        // opening players file
        FILE * players_csv = fopen("players.csv", "r");
        if (players_csv == NULL) {
            printf("players file not found or problem when oppening\n");
            return 1;
        }

        // gettting rid of first line
        fgets(buff, sizeof(buff), players_csv);
        // loop for constructing the hash
        while (fgets(buff, sizeof(buff), players_csv) != NULL) {
            // creates new player
            Player *new_player = (Player *)malloc(sizeof(Player));
            // gets id
            char *field = strtok(buff, ",");
            new_player->id = atoi(field);
            // gets name and allocates memory for name in struct
            field = strtok(NULL, ",");
            new_player->name = malloc(strlen(field) + 1); // +1 for the null terminator
            strcpy(new_player->name, field);
            // gets position and allocates memory for it
            field = strtok(NULL, "\"\n");
            new_player->position = malloc(strlen(field) + 1); // +1 for the null terminator
            strcpy(new_player->position, field);

            new_player->next = NULL;

            // in each insertion counts the time and sums it to constructing_time
            start = clock();
            insert(hash_table, new_player, new_player->id, hash_sizes[i], *hash_function);
            end = clock();
            constructing_time += ((double) (end - start)) / CLOCKS_PER_SEC;
        }
        fclose(players_csv);
        // get hash stats part 1
        hash_stats(hash_table, hash_sizes[i], &occupied_ratio, &max_list_size, &average_list_size);

        // Using sprintf to concat the file name and size of hash
        char file_name[50];
        sprintf(file_name, "experimento%d.txt", hash_sizes[i]);
                
        FILE * hash_stats = fopen(file_name, "a");
        if (hash_stats == NULL) {
            printf("problem oppening hash_stats\n");
            return 1;
        }

        // writing hash stats part 1
        fprintf(hash_stats, "PARTE1: ESTATISTICAS DA TABELA HASH\n");
        // putting time in milliseconds
        constructing_time = constructing_time * 1000;
        fprintf(hash_stats, "TEMPO DE CONSTRUCAO DA TABELA %.2f\n", constructing_time);
        fprintf(hash_stats, "TAXA DE OCUPACAO %.2f\n", occupied_ratio);
        fprintf(hash_stats, "TAMANHO MAXIMO DE LISTA %d\n", max_list_size);
        fprintf(hash_stats, "TAMANHO MEDIO DE LISTA %d\n\n", average_list_size);
        
        // starting Consultas
        FILE * consultas_csv = fopen("consultas.csv", "r");
        if (consultas_csv == NULL) {
            printf("consultas file not found or problem when oppening\n");
            return 1;
        }

        // Search results will be written to an auxiliary file, and that wirtten back to main file when search time is set.
        FILE* aux_file = fopen(".aux_search_results.txt", "w");
        if (aux_file == NULL) {
            printf("problem oppening aux_file\n");
            return 1;
        }

        // variables for search stats
        int max_tests = 0, tests_acumulator = 0, found = 0;
        fprintf(hash_stats, "PARTE 2: ESTATISTICAS DAS CONSULTAS\n");

        // loop for getting search results that will be written to aux_file
        while (fgets(buff, sizeof(buff), consultas_csv) != NULL) {
            int num_testes;
            int search_id = atoi(buff);
            //search time starts here
            start = clock();
            Player * searched_player = search(hash_table, search_id, hash_sizes[i], &num_testes, *hash_function);
            end = clock();
            searching_time += ((double) (end - start)) / CLOCKS_PER_SEC;


            if (searched_player == NULL) {
                fprintf(aux_file, "%d NAO ENCONTRADO %d\n", search_id, num_testes);
            }
            else{
                // write search on file
                fprintf(aux_file, "%d %s %d\n", search_id, searched_player->name, num_testes);
                tests_acumulator += num_testes;
                found++;
                if (num_testes > max_tests) {
                    max_tests = num_testes;
                }
            }
        }
        fclose(consultas_csv);
        fclose(aux_file);
        
        // writes time to search_stasts file
        searching_time = searching_time * 1000;
        fprintf(hash_stats, "TEMPO PARA REALIZACAO DE TODAS CONSULTAS %.2f\n", searching_time);

        // loop that moves all the searches from auxiliary file to search_stats file
        aux_file = fopen(".aux_search_results.txt", "r");
        if (aux_file == NULL) {
            perror("Failed to open auxiliary file for reading");
            return 1; // Or handle the error as appropriate
        }
        char line_buffer[1024]; // Buffer to store lines read from the auxiliary file
        // Read each line from the auxiliary file and write it to the search_stats file
        while (fgets(line_buffer, sizeof(line_buffer), aux_file) != NULL) {
            fputs(line_buffer, hash_stats);
        }
        // Close the auxiliary file after copying its contents
        fclose(aux_file);
        // delete the auxiliary file if it's no longer needed
        //remove(".aux_search_results.txt");
        fclose(hash_stats);
        delete_hash(hash_table,hash_sizes[i]);
    }    
    return 0;
}

