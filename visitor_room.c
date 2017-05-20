#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include "visitor_room.h"

#define CHECK_NULL(ptr) if(ptr==NULL){\
                            return NULL_PARAMETER;\
                            }
#define CHECK_MEMORY(ptr) if(ptr==NULL){\
                                return MEMORY_PROBLEM;\
                                }

#define CHECK_MEMORY2(ptr1,ptr2) if(ptr1==NULL){\
                                free(ptr2);\
                                return MEMORY_PROBLEM;\
                                }

/************************************************************************
 * inisialise challenge activity to 0                                   *
 * 6 lines                                                              *
 ***********************************************************************/
Result init_challenge_activity(ChallengeActivity *activity,
                                                Challenge *challenge){
    CHECK_NULL(challenge);
    CHECK_NULL(activity);
    activity->challenge = challenge;
    activity->start_time = 0;
    activity->visitor = NULL; 
    return OK;
}

/************************************************************************
 * reset the challenge activity to 0                                    *
 * 5 lines                                                              *
 ***********************************************************************/
Result reset_challenge_activity(ChallengeActivity *activity){
    CHECK_NULL(activity);
    activity->challenge = NULL;
    activity->start_time = 0;
    activity->visitor = NULL;

    return OK;
}

/************************************************************************
 * inisialise visitor to 0                                              *
 * 11 lines                                                             *
 ***********************************************************************/
Result init_visitor(Visitor *visitor, char *name, int id){
    CHECK_NULL(visitor);
    CHECK_NULL(name);

    visitor->visitor_name = malloc(sizeof(char)*(strlen(name)+1));
    CHECK_MEMORY(visitor->visitor_name);
    visitor->room_name = malloc(sizeof(char*));
    CHECK_MEMORY(visitor->room_name);
    strcpy(visitor->visitor_name,name);
    visitor->visitor_id = id;
    visitor->current_challenge = NULL;
    *(visitor->room_name) = NULL;

    return OK;
}

/************************************************************************
 * reset visitor to 0                                                   *
 * 7 lines                                                              *
 ***********************************************************************/
Result reset_visitor(Visitor *visitor){
    CHECK_NULL(visitor);
    visitor->visitor_id = 0;
    visitor->current_challenge = NULL;
    free(visitor->visitor_name);
    visitor->room_name = NULL;
    free(visitor);
    return OK;
}

/************************************************************************
 * inisialise room to 0                                                 *
 * 11 lines                                                             *
 ***********************************************************************/
Result init_room(ChallengeRoom *room, char *name, int num_challenges){
    CHECK_NULL(room);
    CHECK_NULL(name);
    if (num_challenges < 1) {
        return ILLEGAL_PARAMETER;
    }
    room->name = malloc(sizeof(char)*(strlen(name)+1));
    CHECK_MEMORY(room->name);
    room->challenges = malloc(sizeof(ChallengeActivity)*(num_challenges));
    CHECK_MEMORY2(room->challenges,room->name);
    strcpy(room->name,name);

    room->num_of_challenges = num_challenges;
    return OK;
}

/************************************************************************
 * reset room to 0                                                      *
 * 8 lines                                                              *
 ***********************************************************************/
Result reset_room(ChallengeRoom *room){
    CHECK_NULL(room);
    for(int i =0; i< room->num_of_challenges ; ++i){
        reset_challenge_activity((room->challenges)+i);
    }
    free(room->name);
    free(room->challenges);
    room->name = NULL;
    room->num_of_challenges = 0;

    return OK;
}

/************************************************************************
 * return the number of free challenges in room of the given level      *
 * 11 lines                                                             *
 ***********************************************************************/
Result num_of_free_places_for_level(ChallengeRoom *room, Level level,
                                                            int *places){
   CHECK_NULL(room);
    int counter = 0;
    for(int i=0; i < (room->num_of_challenges) ; ++i){
        if ((room->challenges + i)->visitor == NULL){
            if(level!=All_Levels) {
                if(((room->challenges+i))->challenge->level==level){
                    ++counter;
                }
            }
            else {
                counter++;
            }
        }
    }
    *places=counter;
    return OK;
}

/************************************************************************
 * change the room name                                                 *
 * 6 lines                                                              *
 ***********************************************************************/
Result change_room_name(ChallengeRoom *room, char *new_name){
    CHECK_NULL(room);
    CHECK_NULL(new_name);
    room->name = malloc(sizeof(char)*(strlen(new_name)+1));
    CHECK_MEMORY(room->name);
    strcpy( room->name,new_name);
    return OK;
}

/************************************************************************
 * find the room of given visitor                                       *
 * 7 lines                                                              *
 ***********************************************************************/
Result room_of_visitor(Visitor *visitor, char **room_name){
    CHECK_NULL(visitor);
    CHECK_NULL(room_name);
    if(*(visitor->room_name) == NULL) return NOT_IN_ROOM;
    *room_name=malloc(sizeof(char)*(strlen(*visitor->room_name)+1));
    CHECK_MEMORY(visitor->room_name);
    strcpy(*room_name,*visitor->room_name);
    return OK;
}

/************************************************************************
 * enter visitor to a challenge (with the smaller lexicographic name)   *
 * of a chosen level. if possible                                       *
 * 34 lines                                                             *
 ***********************************************************************/
Result visitor_enter_room(ChallengeRoom *room, Visitor *visitor, Level level,
                                                                int start_time){
    CHECK_NULL(visitor);
    CHECK_NULL(room);
    int places=0;
    Result checking=num_of_free_places_for_level(room,level,&places);
    if(checking !=OK){
        return checking;
    }
    if(*visitor->room_name != NULL ){
        return ALREADY_IN_ROOM;
    }
    if(places < 1){
        return NO_AVAILABLE_CHALLENGES;
    }
    ChallengeActivity *ChallengeToVisitor = NULL;
    for(int i=0; i< (room->num_of_challenges) ; ++i){
        if((room->challenges+i)->visitor != NULL) {
            continue;
        }
        if(level!=All_Levels) {
            if((room -> challenges +i )->challenge->level==level){
                if(ChallengeToVisitor == NULL ||
                                strcmp((room->challenges+i)->challenge->name,
                                       ChallengeToVisitor->challenge->name)<0){
                    ChallengeToVisitor = (room->challenges + i);
                }
            }
        }
        else{
            if(ChallengeToVisitor == NULL ||
                                strcmp((room->challenges+i)->challenge->name,
                                       ChallengeToVisitor->challenge->name)<0){
                ChallengeToVisitor = (room->challenges + i);
            }
        }
    }
    if(ChallengeToVisitor == NULL)
        return NO_AVAILABLE_CHALLENGES;
    ChallengeToVisitor->visitor=visitor;
    ChallengeToVisitor->start_time=start_time;
    *(visitor->room_name)=room->name;
    visitor->current_challenge=ChallengeToVisitor;
    visitor->current_challenge->start_time=start_time;
    ChallengeToVisitor->challenge->num_visits++;

    return OK;
}

/************************************************************************
 * visitor leave the room and update all relevant info                  *
 * 12 lines                                                             *
 ***********************************************************************/
Result visitor_quit_room(Visitor *visitor, int quit_time){
    CHECK_NULL(visitor);
    if(*visitor->room_name == NULL ) return NOT_IN_ROOM;
    int total_time=(quit_time - (visitor->current_challenge->start_time));
    set_best_time_of_challenge(visitor->current_challenge->challenge,
                                                                total_time);
    Result checking =init_challenge_activity(visitor->current_challenge,
                                        visitor->current_challenge->challenge);
    if(checking != OK){
        return checking;
    }
    visitor->current_challenge->visitor=NULL;
    *visitor->room_name=NULL;
    return OK;
}

