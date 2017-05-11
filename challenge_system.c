#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>
#include <string.h>
#include <assert.h>


#include "challenge_system.h"
#include "challenge_room_system_fields.h"

#define ROW_LENGTH 51



/* open the data base file and take the imformation from it*/
Result create_system(char *init_file, ChallengeRoomSystem **sys)
{

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
    ((*sys)->SysChallenges)=malloc(sizeof(Challenge)*num_of_challenge);
    if(((*sys)->SysChallenges)== NULL) return MEMORY_PROBLEM;
    ((*sys)->Sysnum_of_challenges) = num_of_challenge;

    for(int i=0 ; i<num_of_challenge ; ++i ){
        fscanf(input , "%s %d %d" , buffer , &IDchallenge , &level_chalenge);

        init_challenge(*(((*sys)->SysChallenges)+i), IDchallenge, buffer, level_chalenge);
    }
    int num_of_room=0;
    int challenges_in_room=0;
    int IDs_challenge=0;

    fscanf(input, "%d" ,&num_of_room);
    (*sys)->Sysnum_of_rooms = num_of_room;
    (*sys)->SysRooms=malloc(sizeof(ChallengeRoom)*num_of_room);
    if ((*sys)->SysRooms == NULL) return MEMORY_PROBLEM;

    for(int i=0 ; i <num_of_room ; i++ ){
        fscanf(input , "%s %d" , buffer , &challenges_in_room );
        init_room(*(((*sys)->SysRooms)+i),buffer,challenges_in_room);


        for(int j=0; j<challenges_in_room ;++j){
            fscanf(input , "%d" , &IDs_challenge);
            for(int k=0 ; k<num_of_challenge ; ++k ){

                if(((*sys)->SysChallenges)+k->id == challenges_in_room){
                    init_challenge_activity(,*(((*sys)->SysChallenges)+k));

                }
            }
        }
    }
    fclose(input);
    return OK;
}

Result destroy_system(ChallengeRoomSystem *sys, int destroy_time,
                      char **most_popular_challenge_p, char **challenge_best_time)
{

    if(destroy_time<((*sys)->Systime))return ILLEGAL_TIME;

    all_visitors_quit(sys,destroy_time);
    for(int i=0;i<(sys->Sysnum_of_rooms);++i){

    }
    most_popular_challenge(sys,destroy_time,most_popular_challenge_p);


}


Result visitor_arrive(ChallengeRoomSystem *sys, char *room_name, char *visitor_name, int visitor_id, Level level, int start_time)
{
    if (sys == NULL){
        return NULL_PARAMETER;
    }
    if ((visitor_name == NULL)||(room_name == NULL)){
        return ILLEGAL_PARAMETER;
    }
    Visitor visitor;
    /* need to use system_room_of_visitor */
    
    
    return OK;
}


Result visitor_quit(ChallengeRoomSystem *sys, int visitor_id, int quit_time)
{
    if (((quit_time<0)||(quit_time<=(sys->Systime)))&&(sys!=NULL)){
        return ILLEGAL_TIME;
    }
    if (sys == NULL){
        return NULL_PARAMETER;
    }
    Node tmp_node=malloc(sizeof(Node));
    if (tmp_node == NULL){
        return MEMORY_PROBLEM;
    }
    tmp_node->next = sys->linked_list;
    sys->linked_list=tmp_node;
    while((sys->linked_list->visitor->visitor_id)!= visitor_id){
        tmp_node->next=sys->linked_list->next;
        if (linked_list->next == NULL){
            return ILLEGAL_PARAMETER;
        }
    }
    visitor_quit_room(sys->linked_list->visitor, quit_time);
    sys->Systime=quit_time;
    return OK;
}


Result all_visitors_quit(ChallengeRoomSystem *sys, int quit_time);


Result system_room_of_visitor(ChallengeRoomSystem *sys, char *visitor_name, char **room_name)
{
    if (sys == NULL){
        return NULL_PARAMETER;
    }
    if ((visitor_name == NULL)||(room_name == NULL)){
        return ILLEGAL_PARAMETER;
    }
    Node tmp_node=malloc(sizeof(Node));
    if (tmp_node == NULL){
        return MEMORY_PROBLEM;
    }
    tmp_node->next = sys->linked_list;
    sys->linked_list=tmp_node;
    while(strcmp((sys->linked_list->visitor->visitor_name),visitor_name)!= 0){
        tmp_node->next=sys->linked_list->next;
        if (linked_list->next == NULL){
            return ILLEGAL_PARAMETER;
        }
    }
    if (room_of_visitor(tmp_node->visitor,room_name)==NOT_IN_ROOM){
        return NOT_IN_ROOM;
    }
    return OK;
}


Result change_challenge_name(ChallengeRoomSystem *sys, int challenge_id, char *new_name)
{
    if ((sys == NULL)||(new_name == NULL)){
        return NULL_PARAMETER;
    }
    int i=0;
    while ((i<(sys->Sysnum_of_challenges))&&(challenge_id != ((*((sys->SysChallenges)+i))->id))){
        i++;
    }
        if (i>=sys->Sysnum_of_challenges){
            return ILLEGAL_PARAMETER;
        }
    return change_name((*((sys->SysChallenges)+i)),new_name);
}


Result change_system_room_name(ChallengeRoomSystem *sys, char *current_name, char *new_name)
{
    if ((sys == NULL)||(new_name == NULL)){
        return NULL_PARAMETER;
    }
    int i=0;
    while ((i<(sys->Sysnum_of_rooms))&&(strcmp(current_name,((*((sys->SysRooms)+i))->name))!=0)){
        i++;
    }
    if (i>=sys->Sysnum_of_rooms){
        return ILLEGAL_PARAMETER;
    }
    return change_room_name((*((sys->SysRooms)+i)), new_name);
}


Result best_time_of_system_challenge(ChallengeRoomSystem *sys, char *challenge_name, int *time) {

    if (sys == NULL || challenge_name == NULL) return NULL_PARAMETER;
    int j = -1;/*dif*/
    for (int i = 0; i < (sys->Sysnum_of_challenges); ++i) {
        if (strcmp(challenge_name, ((*(sys->SysChallenges + i))->name)) == 0){
            j = i;
            break;
        }
    }
    if (j == -1)return ILLEGAL_PARAMETER;
    return best_time_of_challenge(((*(sys->SysChallenges + j))),time);

}

Result most_popular_challenge(ChallengeRoomSystem *sys, char **challenge_name){

    if(sys == NULL) return NULL_PARAMETER;

    int max_num_of_visitor_in_challenge=0;
    int visits=0;
    int j = 0;
    for(int i=0; i<((sys)->Sysnum_of_challenges);++i){
        num_visits((*(sys->SysChallenges+i)),&visits);
        if(visits > max_num_of_visitor_in_challenge){
            max_num_of_visitor_in_challenge = visits;
            j=i;
        }
        else if(visits == max_num_of_visitor_in_challenge){
            if(strcmp(((*(sys->SysChallenges+j))->name),((*(sys->SysChallenges+i))->name)) > 0 ) {
                max_num_of_visitor_in_challenge = visits;
                j=i;
            }
        }
    }
    if(max_num_of_visitor_in_challenge==0) {
        *challenge_name=NULL;
        return OK;
    }

    *challenge_name = malloc(
            sizeof(char) * strlen((*((sys)->SysChallenges) + j)->name + 1));
    if (*challenge_name == NULL) return MEMORY_PROBLEM;

    strcpy(*challenge_name,(((*(sys)->SysChallenges)+j)->name));
    return OK;
}


#endif // CHALLENGE_SYSTEM_H_

