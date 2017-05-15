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

/************************************************************************
 * inisialise challenge activity to 0                                   *
 ***********************************************************************/
Result init_challenge_activity(ChallengeActivity *activity, Challenge *challenge){
    CHECK_NULL(challenge);
    CHECK_NULL(activity);
    activity->challenge = challenge;
    activity->start_time = 0;
    activity->visitor = NULL; 
    return OK;
}

/************************************************************************
 * reset the challenge activity to 0                                    *
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
 ***********************************************************************/
Result init_room(ChallengeRoom *room, char *name, int num_challenges){
    CHECK_NULL(room);
    CHECK_NULL(name);
    room->name = malloc(sizeof(char)*(strlen(name)+1));
    room->challenges = malloc(sizeof(ChallengeActivity)*(num_challenges));
    CHECK_MEMORY(room->name);
    CHECK_MEMORY(room->challenges);
    strcpy(room->name,name);
    if (num_challenges < 1) {
        return ILLEGAL_PARAMETER;
    }
    room->num_of_challenges = num_challenges;
    return OK;
}

/************************************************************************
 * reset room to 0                                                      *
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
 ***********************************************************************/
Result num_of_free_places_for_level(ChallengeRoom *room, Level level, int *places){
   CHECK_NULL(room);
    int counter = 0;
    for(int i=0; i < (room->num_of_challenges) ; ++i){
        if ((room->challenges + i)->visitor == NULL){
            if(level!=All_Levels) {
                if(room->challenges->challenge->level==level){
                    counter++;
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
 ***********************************************************************/
Result change_room_name(ChallengeRoom *room, char *new_name){
    CHECK_NULL(room);
    CHECK_NULL(new_name);
    room->name = malloc(sizeof(char)*(strlen(new_name)+1));
    CHECK_MEMORY(room->name);
    return OK;
}

/************************************************************************
 * find the room of given visitor                                       *
 ***********************************************************************/
Result room_of_visitor(Visitor *visitor, char **room_name){
    CHECK_NULL(visitor);
    CHECK_NULL(room_name);
    if(visitor->room_name == NULL) return NOT_IN_ROOM;
    *room_name=malloc(sizeof(char)*(strlen(*visitor->room_name)+1));
    CHECK_MEMORY(visitor->room_name);
    strcpy(*room_name,*visitor->room_name);
    return OK;
}

/************************************************************************
 * enter visitor to a challenge (with the smaller lexicographic name)   *
 * of a chosen level. if possible                                       *
 ***********************************************************************/
Result visitor_enter_room(ChallengeRoom *room, Visitor *visitor, Level level, int start_time){
    CHECK_NULL(visitor);
    CHECK_NULL(room);
    int places=0;
    num_of_free_places_for_level(room,level,&places);
    if(places < 1) return NO_AVAILABLE_CHALLENGES;
    if(visitor->room_name != NULL )
        return ALREADY_IN_ROOM;
    Challenge *ChallengeToVisitor = (room->challenges)->challenge; 
    int j=0;
    for(int i=0; i< (room->num_of_challenges) ; ++i){
        if(level!=All_Levels) {
            if((room -> challenges +i )->challenge->level==level){
                if(strcmp((room->challenges+i)->challenge->name,ChallengeToVisitor->name)<0){
                    *ChallengeToVisitor=*(room->challenges+i)->challenge;
                    j=i;
                }
            }
        }
        else{
            if(strcmp((room->challenges+i)->challenge->name,ChallengeToVisitor->name)<0){
                *ChallengeToVisitor=*(room->challenges+i)->challenge;
                j=i;
            }
        }
    }
    (room->challenges+j)->visitor=visitor;
    (room->challenges+j)->start_time=start_time;
    *(visitor->room_name)=room->name;
    visitor->current_challenge=(room->challenges+j);
    (room->challenges+j)->challenge->num_visits+=1;
    
    return OK;
}

/************************************************************************
 * visitor leave the room and update all relevant info                  *
 ***********************************************************************/
Result visitor_quit_room(Visitor *visitor, int quit_time){
    CHECK_NULL(visitor);
    int total_time=0;
    if(visitor->room_name == NULL ) return NOT_IN_ROOM;
    total_time=quit_time-(visitor->current_challenge->start_time);
    set_best_time_of_challenge(visitor->current_challenge->challenge,total_time);
    visitor->current_challenge->visitor=NULL;
    visitor->current_challenge->start_time=0;
    visitor->room_name=NULL;
    visitor->current_challenge=NULL;
    
    return OK;
}

