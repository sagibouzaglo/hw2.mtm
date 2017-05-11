#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>
#include <string.h>
#include <assert.h>


#include "challenge_system.h"
#include "challenge_room_system_fields.h"

#define ROW_LENGTH 51
#define NOT_FOUND -1

static Result reset_all_rooms(ChallengeRoomSystem *sys);


/************************************************************************
 * open the data base file and take the imformation from it             *
 ***********************************************************************/
Result create_system(char *init_file, ChallengeRoomSystem **sys){

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
    Result checking_problems;
    int num_of_challenge = 0;
    int IDchallenge=0;
    Level level_chalenge=NULL;

    fscanf(input, "%d" ,&num_of_challenge);
    ((*sys)->SysChallenges)=malloc(sizeof(Challenge)*num_of_challenge);
    if(((*sys)->SysChallenges)== NULL){
        free((*sys)->name);
        return MEMORY_PROBLEM;
    }
    ((*sys)->Sysnum_of_challenges) = num_of_challenge;

    for(int i=0 ; i<num_of_challenge ; ++i ){
        fscanf(input , "%s %d %d" , buffer , &IDchallenge , &level_chalenge);

        checking_problems=init_challenge(*(((*sys)->SysChallenges)+i), IDchallenge, buffer, level_chalenge);
        if (checking_problems != OK){
            free((*sys)->name);
            free((*sys)->SysChallenges);
            return checking_problems;
        }
    }
    int num_of_room=0;
    int challenges_in_room=0;
    int IDs_challenge=0;

    fscanf(input, "%d" ,&num_of_room);
    (*sys)-> = num_of_room;
    (*sys)->SysRooms=malloc(sizeof(ChallengeRoom)*num_of_room);
    if ((*sys)->SysRooms == NULL) {
        free((*sys)->name);
        free((*sys)->SysChallenges);
        return MEMORY_PROBLEM;
    }

    for(int i=0 ; i <num_of_room ; i++ ){
        fscanf(input , "%s %d" , buffer , &challenges_in_room );
        checking_problems=init_room(*(((*sys)->SysRooms)+i),buffer,challenges_in_room);
        if(checking_problems!=OK){
            free((*sys)->name);
            free((*sys)->SysChallenges);
            free((*sys)->SysRooms);
            return checking_problems;
        }


        for(int j=0; j<challenges_in_room ;++j){
            fscanf(input , "%d" , &IDs_challenge);
            for(int k=0 ; k<num_of_challenge ; ++k ){

                if((*sys)->(SysChallenges+k)->id == challenges_in_room){
                    checking_problems=init_challenge_activity(*(((*sys)->(SysRooms+i)))->challenges,*(((*sys)->SysChallenges)+k));/*???? <-- ??*/
                    if (checking_problems!=OK){
                        free((*sys)->name);
                        free((*sys)->SysChallenges);
                        free((*sys)->SysRooms);
                        return checking_problems;
                    }
                }
            }
        }
    }
    fclose(input);
    return OK;
}

Result destroy_system(ChallengeRoomSystem *sys, int destroy_time,
                      char **most_popular_challenge_p, char **challenge_best_time) {
    if (destroy_time < (sys->Systime))return ILLEGAL_TIME;
    all_visitors_quit(sys, destroy_time);
    int best_time = 0;
    most_popular_challenge(sys, most_popular_challenge_p);
    if (*most_popular_challenge_p == NULL) {
        *challenge_best_time = NULL;
    } else {
        best_time_of_system_challenge(sys, (*(((sys)->SysChallenges)))->name,
                                      &best_time);
        int challenge_time = 0;
        int j = NOT_FOUND;
        for (int i = 1; i < (sys->Sysnum_of_challenges); ++i) {
            best_time_of_system_challenge(sys,
                                          (*(((sys)->SysChallenges) + i))->name,
                                          &challenge_time);
            if(challenge_time<best_time){
                best_time = challenge_time;
                j=i;
            }
            if(challenge_time==best_time){
                if (strcmp(((*(sys->SysChallenges + j))->name),
                           ((*(sys->SysChallenges + i))->name)) > 0) {
                    best_time = challenge_time;
                    j=i;
                }
            }

            *challenge_best_time = malloc(
                    sizeof(char) *
                    strlen((*((sys)->SysChallenges) + j)->name + 1));
            if (*challenge_best_time == NULL) return MEMORY_PROBLEM;

            strcpy(*challenge_best_time,(((*(sys)->SysChallenges)+j)->name));
        }

    }
    Result check = reset_all_rooms(sys);
    if (check != OK) return check;
    return OK;
}


/************************************************************************
 *                    *
 ***********************************************************************/
Result visitor_arrive(ChallengeRoomSystem *sys, char *room_name, char *visitor_name, int visitor_id, Level level, int start_time){
    if (sys == NULL){
        return NULL_PARAMETER;
    }
    if ((visitor_name == NULL)||(room_name == NULL)){
        return ILLEGAL_PARAMETER;
    }
    Visitor visitor;
    /* need to use system_room_of_visitor */
    
    
    return OK;
} //not finished

/************************************************************************
 * visitor is exiting the room and saving his best time.                *
 * alse the system realese memory and change the linked list            *
 ***********************************************************************/
Result visitor_quit(ChallengeRoomSystem *sys, int visitor_id, int quit_time){
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
} // not finished

/************************************************************************
 * using "visitor quit" to exit all visitors from the system and        *
 * release the memory                                                   *
 ***********************************************************************/
Result all_visitors_quit(ChallengeRoomSystem *sys, int quit_time){
    if (((quit_time<0)||(quit_time<=(sys->Systime)))&&(sys!=NULL)){
        return ILLEGAL_TIME;
    }
    if (sys == NULL){
        return NULL_PARAMETER;
    }
    Result check;
    while (sys->linked_list != NULL){
        sys->linked_list=sys->linked_list->next;
        check =visitor_quit(sys,sys->linked_list->visitor->visitor_id,quit_time);
        if (check != OK){
            return check;
        }

    }
    return OK;
}

/************************************************************************
 * using "room_of_visitor" to find the room the visitr is staying in    *
 ***********************************************************************/
Result system_room_of_visitor(ChallengeRoomSystem *sys, char *visitor_name, char **room_name){
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

/************************************************************************
 * finding the relevant challenge and change it's name                  *
 ***********************************************************************/
Result change_challenge_name(ChallengeRoomSystem *sys, int challenge_id, char *new_name){
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

/************************************************************************
 * finding the relevant room and change it's name                       *
 ***********************************************************************/
Result change_system_room_name(ChallengeRoomSystem *sys, char *current_name, char *new_name){
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

/************************************************************************
 *                    *
 ***********************************************************************/
Result best_time_of_system_challenge(ChallengeRoomSystem *sys, char *challenge_name, int *time){

    if (sys == NULL || challenge_name == NULL) return NULL_PARAMETER;
    int j = NOT_FOUND;
    for (int i = 0; i < (sys->Sysnum_of_challenges); ++i) {
        if (strcmp(challenge_name, ((*(sys->SysChallenges + i))->name)) == 0){
            j = i;
            break;
        }
    }
    if (j == NOT_FOUND)return ILLEGAL_PARAMETER;
    return best_time_of_challenge(((*(sys->SysChallenges + j))),time);

}

/************************************************************************
 *                           *
 ***********************************************************************/
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
/************************************************************************
 *                           *
 ***********************************************************************/
static Result reset_all_rooms(ChallengeRoomSystem *sys){
    if(sys == NULL) return NULL_PARAMETER;
    Result check;
    for(int i =0; i<((sys)->Sys_num_of_rooms) ; ++i){
        check=reset_room(*(((sys)->SysRooms)+i));
        if(check != OK) return check;
        }
    return OK;
    }




