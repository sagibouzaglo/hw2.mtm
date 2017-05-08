#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>
#include <string.h>
#include <assert.h>


#include "challenge_system.h"
#include "challenge_room_system_fields.h"

#define ROW_LENGTH 51


/* open the data base file and take the imformation from it*/
Result create_system(char *init_file, ChallengeRoomSystem **sys) {

   if((*sys)== NULL) return NULL_PARAMETER;

    char buffer[ROW_LENGTH];

    FILE *input = fopen(init_file, "r");
    if (input == NULL) {
        return NULL_PARAMETER;
    }
    fscanf(input, "%s" ,buffer);
    if (buffer == NULL) {
        return NULL_PARAMETER;
    }
    (*sys)->name = malloc(sizeof(char *) * strlen(buffer));
    if ((*sys)->name == NULL) {
        return MEMORY_PROBLEM;
    }

    strcpy((*sys)->name, buffer);

    int num_of_challenge = 0;
    int IDchallenge=0;
    Level level_chalenge=NULL;

    fscanf(input, "%d" ,&num_of_challenge);

    for(int i=0 ; i<num_of_challenge ; ++i ){
        fscanf(input , "%s %d %d" , buffer , &IDchallenge , &level_chalenge);
        strcpy((*sys)->((*SysChallenges+i)->name),buffer);

        (*sys)->((*SysChallenges+i)->id)=IDchallenge;
        (*sys)->((*SysChallenges+i)->level)=level_chalenge;
    }
    int num_of_room=0;
    int challenges_in_room=0;
    int IDs_challenge=0;

    for(int i=0 ; i <num_of_room ; i++ ){
        fscanf(input , "%s %d" , buffer , &challenges_in_room );
        strcpy((*sys)->((*SysRooms+i)->name),buffer);
        (*sys)->((*SysRooms+i)->num_of_challenges)=challenges_in_room;
        int j=0;
        while (fscanf(input , "%d" , &IDs_challenge)!=1){
            for(int k=0 ; k<num_of_challenge ; ++k ){
                if((*sys)->((*SysChallenges+k)->id)==challenges_in_room){
                    (*sys)->((*SysRooms+i)->challenges+j)= (*sys)->(*SysChallenges+k);
                    j++;
                }
            }
        }
    }
    fclose(input);
    return OK;
}

Result destroy_system(ChallengeRoomSystem *sys, int destroy_time,
                      char **most_popular_challenge_p, char **challenge_best_time);


Result visitor_arrive(ChallengeRoomSystem *sys, char *room_name, char *visitor_name, int visitor_id, Level level, int start_time);


Result visitor_quit(ChallengeRoomSystem *sys, int visitor_id, int quit_time);


Result all_visitors_quit(ChallengeRoomSystem *sys, int quit_time);


Result system_room_of_visitor(ChallengeRoomSystem *sys, char *visitor_name, char **room_name);


Result change_challenge_name(ChallengeRoomSystem *sys, int challenge_id, char *new_name);


Result change_system_room_name(ChallengeRoomSystem *sys, char *current_name, char *new_name);


Result best_time_of_system_challenge(ChallengeRoomSystem *sys, char *challenge_name, int *time);


Result most_popular_challenge(ChallengeRoomSystem *sys, char **challenge_name);


#endif // CHALLENGE_SYSTEM_H_

