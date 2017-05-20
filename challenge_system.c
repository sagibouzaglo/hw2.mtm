#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include "challenge_system.h"

#define ROW_LENGTH 51
#define NOT_FOUND -1
#define CHECK_NULL(ptr) if(ptr==NULL){\
                            return NULL_PARAMETER;\
                            }
#define CHECK_MEMORY(ptr) if(ptr==NULL){\
                                return MEMORY_PROBLEM;\
                                }
#define CHECK(ptr,file)  if(!ptr) {\
                            fclose(file);\
                            return MEMORY_PROBLEM;\
                            }
#define CHECK_AND_2FREE(ptr,ptr2,ptr3,file) if(! ptr) {\
                            fclose(file);\
                            free(ptr2);\
                            free(ptr3);\
                            return MEMORY_PROBLEM;\
                            }
#define CHECK_AND_FREE(ptr,ptr2,file) if(! ptr) {\
                            fclose(file);\
                            free(ptr2);\
                            return MEMORY_PROBLEM;\
                            }
#define CHECK_RESULT_AND_2FREE(Result,ptr2,ptr3,file) if(Result != OK) {\
                            fclose(file);\
                            free(ptr2);\
                            free(ptr3);\
                            return Result;\
                            }
#define CHECK_RESULT_AND_3FREE(Result,ptr1,ptr2,ptr3,file) if(Result != OK) {\
                            fclose(file);\
                            free(ptr1);\
                            free(ptr2);\
                            free(ptr3);\
                            return Result;\
                            }
static  Result reset_all_challenges(ChallengeRoomSystem *sys);

static Result create_all_rooms(ChallengeRoomSystem *sys, FILE* input);

static Result create_all_challenges(ChallengeRoomSystem *sys,FILE* input);

static Result find_visitor(ChallengeRoomSystem *sys,int visitor_id,
                                                            Node *visitor);
static Result reset_all_rooms(ChallengeRoomSystem *sys);


/************************************************************************
 * open the data base file and take the imformation from it             *
 * 25 lines                                                             *
 ***********************************************************************/
Result create_system(char *init_file, ChallengeRoomSystem **sys){
    *sys=malloc(sizeof(ChallengeRoomSystem));
    CHECK_MEMORY(*sys);
    ((*sys)->Systime)=0;
    (*sys)->linked_list = NULL;
    (*sys)->SysChallenges=NULL;
    (*sys)->SysRooms=NULL;
    (*sys)->Sysnum_of_challenges=0;
    (*sys)->Sys_num_of_rooms=0;
    char buffer[ROW_LENGTH];
    FILE *input;
    input =fopen(init_file, "r");
    CHECK_NULL(input);
    fscanf(input, "%s" ,buffer);
    (*sys)->name = malloc(sizeof(char) * strlen(buffer)+1);
    CHECK ((*sys)->name ,input);
    strcpy((*sys)->name, buffer);
    Result checking_problems=create_all_challenges(*sys , input);
    CHECK_RESULT_AND_2FREE(checking_problems,((*sys)->name),
                                    ((*sys)->SysChallenges),input);
    checking_problems=create_all_rooms(*sys, input);
    CHECK_RESULT_AND_3FREE(checking_problems,((*sys)->name),
                            ((*sys)->SysChallenges),((*sys)->SysRooms),input);

    fclose(input);
    return OK;
}

/************************************************************************
 *             *
 * 44 lines                                                             *
 ***********************************************************************/
Result destroy_system(ChallengeRoomSystem *sys, int destroy_time,
                                        char **most_popular_challenge_p,
                                                char **challenge_best_time) {
    if (destroy_time < ((sys)->Systime)){
        return ILLEGAL_TIME;
    }
    all_visitors_quit(sys, destroy_time);
    int best_time=sys->Systime;
    most_popular_challenge(sys, most_popular_challenge_p);
    if (*most_popular_challenge_p == NULL) {
        *challenge_best_time = NULL;
    } else {
        best_time_of_system_challenge(sys,((*(sys->SysChallenges ))->name),
                                      &best_time);
        int challenge_time = best_time;
        int j = 0;
        for (int i = 0; i < (sys->Sysnum_of_challenges); ++i) {
            best_time_of_system_challenge(sys,
                                          ((*(sys->SysChallenges + i))->name),
                                          &challenge_time);
            if((challenge_time < best_time) && (challenge_time != 0)){
                best_time = challenge_time;
                j=i;
            } else if(challenge_time==best_time){
                    if (strcmp(((*(sys->SysChallenges + j))->name),
                               ((*(sys->SysChallenges + i))->name)) > 0) {
                        best_time = challenge_time;
                        j=i;
                    }
            }
        }
        *challenge_best_time = malloc(sizeof(char) *
                                   (strlen((*(sys->SysChallenges+j))->name)+1));
        if (*challenge_best_time == NULL){
            return MEMORY_PROBLEM;
        }
        strcpy(*challenge_best_time,((*(sys->SysChallenges + j))->name));
    }
    Result check = reset_all_rooms(sys);
    if (check != OK) return check;
    check = reset_all_challenges(sys);
    if (check != OK) return check;
    free(sys->SysChallenges);
    free(sys->SysRooms);
    free(sys->name);
    free(sys);
    return OK;
}

/************************************************************************
 *                    *
 * 50 lines
 ***********************************************************************/
Result visitor_arrive(ChallengeRoomSystem *sys, char *room_name,
                                char *visitor_name, int visitor_id,
                                        Level level, int start_time){
    CHECK_NULL(sys);
    if ((visitor_name == NULL)||(room_name == NULL)){
        return ILLEGAL_PARAMETER;
    }
    if(start_time < sys->Systime){
        return ILLEGAL_TIME;
    }
    Node first_visitor = sys->linked_list;
    Node node3 = NULL;
    find_visitor(sys,visitor_id,&node3);
    if (node3 != NULL) {
        return ALREADY_IN_ROOM;
    }
    ChallengeRoom *room = NULL;
    for (int i=0 ; i<(sys->Sys_num_of_rooms) ; ++i){
        if(strcmp(((*((sys->SysRooms) + i))->name),room_name)== 0){
            room =(*((sys->SysRooms) + i));
            break;
        }
    }
    if(room == NULL){
        return ILLEGAL_PARAMETER;
    }
    Visitor *visitor=malloc(sizeof(Visitor));
    CHECK_MEMORY(visitor);
    Result checking_problems= init_visitor(visitor,visitor_name,visitor_id);
    if (checking_problems != OK){
        reset_visitor(visitor);
        free(visitor);
        return checking_problems;
    }
    checking_problems=visitor_enter_room(room,visitor,level,start_time);
    if (checking_problems != OK){
        reset_visitor(visitor);
        free(visitor);
        return checking_problems;
    }
    Node node2 = malloc(sizeof(*node2));
    if(!node2) {
        reset_visitor(visitor);
        free(visitor);
        return MEMORY_PROBLEM;
    }
    node2->visitor=visitor;
    node2->next=NULL;
    node2->next = first_visitor;
    first_visitor=node2;
    sys->linked_list=first_visitor;
    sys->Systime=start_time;
    return  OK;
}

/************************************************************************
 * visitor is exiting the room and saving his best time.                *
 * alse the system realese memory and change the linked list            *
 *  33 lines                                                            *
 ***********************************************************************/
Result visitor_quit(ChallengeRoomSystem *sys, int visitor_id, int quit_time){
    CHECK_NULL(sys);
    if (((quit_time < 0)||(quit_time < (sys->Systime)))){
        return ILLEGAL_TIME;
    }
    Node tmp_node1 = sys -> linked_list;
    Node previous = NULL;
    while(tmp_node1 != NULL) {
        if((tmp_node1->visitor->visitor_id) != visitor_id) {
            previous = tmp_node1;
            tmp_node1 = tmp_node1->next;
        }
        else{
            if(previous != NULL){
                previous->next = tmp_node1->next;
            }
            else {
                sys->linked_list=tmp_node1->next;
            }

            Result checking_problems = visitor_quit_room(tmp_node1->visitor,
                                                                    quit_time);
            if(checking_problems != OK)return checking_problems;
            checking_problems = reset_visitor(tmp_node1->visitor);
            if(checking_problems != OK)return checking_problems;
            free(tmp_node1->visitor);
            free(tmp_node1);
            sys->Systime=quit_time;
            return OK;
        }

    }
    free(tmp_node1);
    return NOT_IN_ROOM;
}

/************************************************************************
 * using "visitor quit" to exit all visitors from the system and        *
 * release the memory                                                   *
 * 17 lines                                                             *
 ***********************************************************************/
Result all_visitors_quit(ChallengeRoomSystem *sys, int quit_time){
    CHECK_NULL(sys);
    if (((quit_time<0)||(quit_time<(sys->Systime))) ){
        return ILLEGAL_TIME;
    }
    Result check;
    Node tmp_node1 = sys->linked_list;
    Node previous = NULL;
    CHECK_NULL(tmp_node1);
    while (tmp_node1 != NULL){
        previous=tmp_node1;
        tmp_node1=previous->next;
        check =visitor_quit(sys,previous->visitor->visitor_id,quit_time);
        if (check != OK){
            return check;
        }
    }
    return OK;
}

/************************************************************************
 * using "room_of_visitor" to find the room the visitr is staying in    *
 * 23 lines                                                             *
 ***********************************************************************/
Result system_room_of_visitor(ChallengeRoomSystem *sys, char *visitor_name,
                                                            char **room_name) {
    CHECK_NULL(sys);
    if ((visitor_name == NULL) || (room_name == NULL)) {
        return ILLEGAL_PARAMETER;
    }
    Node tmp_node1 = sys->linked_list;
    Node previous = NULL;
    if (tmp_node1 == NULL) {
        free(tmp_node1);
        return NOT_IN_ROOM;
    }
    while (tmp_node1 != NULL) {
        if (strcmp(tmp_node1->visitor->visitor_name, visitor_name)!=0) {
            previous = tmp_node1;
            tmp_node1 = previous->next;
        } else {
            *room_name = malloc(sizeof(char)*
                                    strlen(*tmp_node1->visitor->room_name)+1);
            CHECK_NULL(*room_name);
            strcpy(*room_name,*tmp_node1->visitor->room_name);
            return OK;
        }
    }
    return NOT_IN_ROOM;
}

/************************************************************************
 * finding the relevant challenge and change it's name                  *
 * 11 lines                                                             *
 ***********************************************************************/
Result change_challenge_name(ChallengeRoomSystem *sys, int challenge_id,
                                                                char *new_name){
    CHECK_NULL(sys);
    CHECK_NULL(new_name);
    int i=0;
    while ((i<(sys->Sysnum_of_challenges)) &&
                        (challenge_id != ((*((sys->SysChallenges)+i))->id))){
        i++;
    }
        if (i>=sys->Sysnum_of_challenges){
            return ILLEGAL_PARAMETER;
        }
    return change_name((*((sys->SysChallenges)+i)),new_name);
}

/************************************************************************
 * finding the relevant room and change it's name                       *
 * 11 lines                                                             *
 ***********************************************************************/
Result change_system_room_name(ChallengeRoomSystem *sys, char *current_name,
                                                                char *new_name){
    CHECK_NULL(sys);
    CHECK_NULL(new_name);
    int i=0;
    while ((i<(sys->Sys_num_of_rooms) &&
                    (strcmp(current_name,((*((sys->SysRooms)+i))->name))!=0))){
        i++;
    }
    if (i>=(sys->Sys_num_of_rooms)){
        return ILLEGAL_PARAMETER;
    }
    return change_room_name((*((sys->SysRooms)+i)), new_name);
}

/************************************************************************
 *                    *
 * 16 lines                                                             *
 ***********************************************************************/
Result best_time_of_system_challenge(ChallengeRoomSystem *sys,
                                            char *challenge_name, int *time){
    CHECK_NULL(sys);
    CHECK_NULL(challenge_name);
    int j=0;
    int not_found=NOT_FOUND;
    for (int i = 0; i < (sys->Sysnum_of_challenges); i++) {
        if (strcmp(challenge_name,((*(sys->SysChallenges + i))->name)) == 0){
            j = i;
            not_found = 1;
            break;
        }
    }
    if (not_found == NOT_FOUND){
        return ILLEGAL_PARAMETER;
    }
    best_time_of_challenge((*(sys->SysChallenges + j)),time);
    return OK;
}

/************************************************************************
 *                           *
 * 27 lines                                                             *
 ***********************************************************************/
Result most_popular_challenge(ChallengeRoomSystem *sys, char **challenge_name){
    CHECK_NULL(sys);
    int max_num_of_visitor_in_challenge = 0;
    int visits=0;
    int j = 0;
    for(int i=0; i<((sys)->Sysnum_of_challenges);++i){
        num_visits((*(sys->SysChallenges+i)),&visits);
        if(visits > max_num_of_visitor_in_challenge){
            max_num_of_visitor_in_challenge = visits;
            j=i;
        }
        else if(visits == max_num_of_visitor_in_challenge){
            if(strcmp(((*(sys->SysChallenges+j))->name),
                                    ((*(sys->SysChallenges+i))->name)) > 0 ) {
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
            sizeof(char) * (strlen((*(sys->SysChallenges+j))->name) + 1));
    if (*challenge_name == NULL) return MEMORY_PROBLEM;
    strcpy(*challenge_name,((*(sys->SysChallenges+j))->name));
    return OK;
}

/************************************************************************
 *                           *
 * 10 lines                                                             *
 ***********************************************************************/
static Result reset_all_rooms(ChallengeRoomSystem *sys){
    CHECK_NULL(sys);
    Result check;
    for(int i =0; i<((sys)->Sys_num_of_rooms) ; ++i){
        check=reset_room(*(((sys)->SysRooms)+i));
        if(check != OK){
            return check;
        }
        free((*(((sys)->SysRooms)+i)));
    }
    return OK;
}

/************************************************************************
 *                           *
 * 11 lines                                                              *
 ***********************************************************************/
static Result reset_all_challenges(ChallengeRoomSystem *sys){
    CHECK_NULL(sys);
    Result check;
    for(int i =0; i<((sys)->Sysnum_of_challenges) ; ++i){
        check=reset_challenge(*(((sys)->SysChallenges)+i));
        if(check != OK){
            return check;
        }
        free((*(((sys)->SysChallenges)+i)));
    }
    return OK;
}
/************************************************************************
 *                           *
 * 33 lines                                                             *
 ***********************************************************************/
static Result create_all_rooms(ChallengeRoomSystem *sys, FILE* input){
    int num_of_room=0;
    int challenges_in_room=0;
    int IDs_challenge=0;
    char buffer[ROW_LENGTH];
    fscanf(input, "%d" ,&num_of_room);
    (sys->Sys_num_of_rooms) = num_of_room;
    sys->SysRooms=malloc(sizeof(ChallengeRoom*) * num_of_room);
    CHECK_AND_2FREE(((sys)->SysRooms),((sys)->name),(sys->SysChallenges),input);
    for(int i=0 ; i <num_of_room ; i++ ){
        *(sys->SysRooms + i) = malloc(sizeof(ChallengeRoom));
        CHECK_AND_2FREE(*(((sys)->SysRooms)+i),((sys)->name),
                                                (*(sys)->SysChallenges),input);
        fscanf(input , "%s %d" , buffer , &challenges_in_room );
        Result checking_problems=init_room(*(((sys)->SysRooms)+i),
                                                    buffer,challenges_in_room);
        CHECK_RESULT_AND_3FREE(checking_problems,((sys)->name),
                                            (*(sys)->SysChallenges),
                                                (*(sys)->SysRooms),input);
        for(int j=0; j<challenges_in_room ;++j){
            fscanf(input , "%d" , &IDs_challenge);
            for(int k=0 ; k<sys->Sysnum_of_challenges ; ++k ){
                if((*((sys)->SysChallenges+k))->id == IDs_challenge){
                    checking_problems=init_challenge_activity(
                                        ((*((sys)->SysRooms+i))->challenges +j),
                                                   (*((sys)->SysChallenges+k)));
                    CHECK_RESULT_AND_3FREE(checking_problems,((sys)->name),
                                           (*(sys)->SysChallenges),
                                            (*(sys)->SysRooms),input);
                }
            }
        }
    }
    return  OK;
}

/************************************************************************
 *                           *
 * 21 lines                                                             *
 ***********************************************************************/
static Result create_all_challenges(ChallengeRoomSystem *sys,FILE* input){
    char buffer[ROW_LENGTH];
    int num_of_challenge = 0;
    int id_challenge=0;
    int level_challenge = 0;
    fscanf(input, "%d" ,&num_of_challenge);
    ((sys)->SysChallenges)=malloc(sizeof(Challenge*)*num_of_challenge);
    CHECK_AND_FREE(((sys)->SysChallenges),((sys)->name),input);
    ((sys)->Sysnum_of_challenges) = num_of_challenge;
    Result checking_problems;
    for(int i=0 ; i<num_of_challenge ; ++i ){
        *(((sys)->SysChallenges)+i)=malloc(sizeof(Challenge));
        CHECK_AND_FREE( *(((sys)->SysChallenges)+i),((sys)->name),input);
        fscanf(input , "%s %d %d" , buffer , &id_challenge , &level_challenge);
        --level_challenge;
        checking_problems=init_challenge(*(((sys)->SysChallenges)+i),
                                            id_challenge, buffer,
                                                (Level)level_challenge);
        CHECK_RESULT_AND_2FREE(checking_problems,((sys)->name),
                                            *((sys)->SysChallenges),input);
    }
    return OK;
}

/************************************************************************
 *                           *
 * 17 lines                                                             *
 ***********************************************************************/
static Result find_visitor(ChallengeRoomSystem *sys,int visitor_id, Node *nod2){
    CHECK_NULL (sys);
    Node ptr = sys->linked_list;
    if (ptr == NULL){
        return NOT_IN_ROOM;
    }
    while(ptr != NULL) {
        if (ptr->visitor != NULL && ptr->visitor->visitor_id == visitor_id ){
            *nod2=ptr;
            return OK;
        }
        if(ptr->next!=NULL) {
            ptr = ptr->next;
        } else {
            return NOT_IN_ROOM;
        }
    }
    return NOT_IN_ROOM;
}
