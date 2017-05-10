#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>
#include <string.h>
#include <assert.h>


#include "challenge_system.h"
#include "challenge_room_system_fields.h"

#define ROW_LENGTH 51

static Result popular_challenge(ChallengeRoomSystem *sys, int destroy_time,
char **most_popular_challenge_p, char **challenge_best_time);

/* open the data base file and take the imformation from it*/
Result create_system(char *init_file, ChallengeRoomSystem **sys) {

    if((*sys)== NULL) return NULL_PARAMETER;

    ((*sys)->(Systime)) = 0;
    char buffer[ROW_LENGTH];

    FILE *input = fopen(init_file, "r");
    if (input == NULL) {
        return MEMORY_PROBLEM;
    }
    fscanf(input, "%s" ,buffer);
    if (buffer == NULL) {
        return MEMORY_PROBLEM;
    }
    (*sys)->name = malloc(sizeof(char) * strlen(buffer)+1);
    if ((*sys)->name == NULL) {
        return MEMORY_PROBLEM;
    }

    strcpy((*sys)->name, buffer);

    int num_of_challenge = 0;
    int IDchallenge=0;
    Level level_chalenge=NULL;

    fscanf(input, "%d" ,&num_of_challenge);
    (*sys)->(SysChallenges)=malloc(sizeof(Challenge)*num_of_challenge);
    if((*sys)->(SysChallenges)== NULL) return MEMORY_PROBLEM;
    ((*sys)->(Sysnum_of_challenges)) = num_of_challenge;

    for(int i=0 ; i<num_of_challenge ; ++i ){
        fscanf(input , "%s %d %d" , buffer , &IDchallenge , &level_chalenge);

        init_challenge(*(((*sys)->SysChallenges)+i), IDchallenge, buffer, level_chalenge);
    }
    int num_of_room=0;
    int challenges_in_room=0;
    int IDs_challenge=0;
    fscanf(input, "%d" ,&num_of_room);

    (*sys)->SysRooms=malloc(sizeof(ChallengeRoom)*num_of_room);
    if ((*sys)->SysRooms == NULL) return MEMORY_PROBLEM;

    for(int i=0 ; i <num_of_room ; i++ ){
        fscanf(input , "%s %d" , buffer , &challenges_in_room );
        init_room(((*sys)->SysRooms)+i)),buffer,challenges_in_room);


        for(int j=0; j<challenges_in_room ;++j){
            fscanf(input , "%d" , &IDs_challenge);
            for(int k=0 ; k<num_of_challenge ; ++k ){

                if(((*sys)->SysChallenges)+k->id == challenges_in_room){
                    init_challenge_activity(((*sys)->SysChallenges)+k);

                }
            }
        }
    }
    fclose(input);
    return OK;
}

Result destroy_system(ChallengeRoomSystem *sys, int destroy_time,
                      char **most_popular_challenge_p, char **challenge_best_time){

    if(destroy_time<((*sys)->Systime))return ILLEGAL_TIME;

    all_visitors_quit(sys,destroy_time);


}


Result visitor_arrive(ChallengeRoomSystem *sys, char *room_name, char *visitor_name, int visitor_id, Level level, int start_time)
{
    
    return OK;
}


Result visitor_quit(ChallengeRoomSystem *sys, int visitor_id, int quit_time);


Result all_visitors_quit(ChallengeRoomSystem *sys, int quit_time);


Result system_room_of_visitor(ChallengeRoomSystem *sys, char *visitor_name, char **room_name);


Result change_challenge_name(ChallengeRoomSystem *sys, int challenge_id, char *new_name);


Result change_system_room_name(ChallengeRoomSystem *sys, char *current_name, char *new_name);


Result best_time_of_system_challenge(ChallengeRoomSystem *sys, char *challenge_name, int *time);


Result most_popular_challenge(ChallengeRoomSystem *sys, char **challenge_name);

static Result popular_challenge(ChallengeRoomSystem *sys, int destroy_time,
char **most_popular_challenge_p, char **challenge_best_time){


    int num_of_visitor_in_challenge=0;
    int j = 0;
    for(int i=0; i<((*sys)->Sysnum_of_challenges);++i){
        if(best_time_of_challenge(((*sys)->SysChallenges))+i,destroy_time) > num_of_visitor_in_challenge){
            num_of_visitor_in_challenge=best_time_of_challenge(((*sys)->SysChallenges))+i,destroy_time);
            j=i;
        }else if(best_time_of_challenge(((*sys)->SysChallenges))+i,destroy_time) == num_of_visitor_in_challenge){
            if(strcmp((((*sys)->SysChallenges))+j,){

            }
        }
    }
    if(num_of_visitor_in_challenge==0) {
        *most_popular_challenge_p=NULL;
    }
    else {
        *most_popular_challenge_p = malloc(
                sizeof(char) * strlen(((*sys)->SysChallenges) + j->name + 1));
        if (*most_popular_challenge_p == NULL) return MEMORY_PROBLEM;
    }

    strcpy(most_popular_challenge_p,((*sys)->SysChallenges)+j->name);
}
#endif // CHALLENGE_SYSTEM_H_

