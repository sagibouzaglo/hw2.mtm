#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>
#include <string.h>
#include <assert.h>


#include "visitor_room.h"

/************************************************************************
 * inisialise challenge activity to 0                                   *
 ***********************************************************************/
Result init_challenge_activity(ChallengeActivity *activity, Challenge *challenge)
{
    if( (activity == NULL) || (challenge == NULL) ) {
        return NULL_PARAMETER;
    }
    activity->challenge = challenge;
    activity->start_time = 0;
    activity->visitor = NULL;

    return OK;
}

/************************************************************************
 * reset the challenge activity to 0                                    *
 ***********************************************************************/
Result reset_challenge_activity(ChallengeActivity *activity)
{
    if (activity == NULL) {
        return NULL_PARAMETER;
    }
    activity->challenge = NULL;
    activity->start_time = 0;
    activity->visitor = NULL;

    return OK;
}

/************************************************************************
 * inisialise visitor to 0                                              *
 ***********************************************************************/
Result init_visitor(Visitor *visitor, char *name, int id)
{
    if( (visitor == NULL) || (name == NULL) ) {
        return NULL_PARAMETER;
    }
    visitor->visitor_name = malloc(sizeof(char)*(strlen(name)+1));
    if (visitor->visitor_name == NULL) {
        return MEMORY_PROBLEM;
    }
    visitor->room_name = malloc(sizeof(char*));
    if (visitor->room_name == NULL) {
        return MEMORY_PROBLEM;
    }
    strcpy(visitor->visitor_name,name);
    visitor->visitor_id = id;
    visitor->current_challenge = NULL;
    *(visitor->room_name) = NULL;

    return OK;
}

/************************************************************************
 * reset visitor to 0                                                   *
 ***********************************************************************/
Result reset_visitor(Visitor *visitor)
{
    if(visitor == NULL) {
        return NULL_PARAMETER;
    }
    visitor->visitor_id = 0;
    visitor->current_challenge = NULL;
    free(visitor->visitor_name);
    visitor->room_name = NULL;

    return OK;
}

/************************************************************************
 * inisialise room to 0                                                 *
 ***********************************************************************/
Result init_room(ChallengeRoom *room, char *name, int num_challenges)
{
    if( (room == NULL) || (name == NULL) ) {
        return NULL_PARAMETER;
    }
    room->name = malloc(sizeof(char)*(strlen(name)+1));
    room->challenges = malloc(sizeof(ChallengeActivity)*(num_challenges));
    if ( (room->name == NULL) || (room->challenges == NULL) ) {
        return MEMORY_PROBLEM;
    }
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
Result reset_room(ChallengeRoom *room)
{
    if (room == NULL) {
        return NULL_PARAMETER;
    }
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
Result num_of_free_places_for_level(ChallengeRoom *room, Level level, int *places)
{
    if (room == NULL) {
        return NULL_PARAMETER;
    }
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
Result change_room_name(ChallengeRoom *room, char *new_name)
{
    if( (room == NULL) || (new_name == NULL) ) {
        return NULL_PARAMETER;
    }
    room->name = malloc(sizeof(char)*(strlen(new_name)+1));
    if (room->name == NULL) {
        return MEMORY_PROBLEM;
    }
    return OK;
}

/************************************************************************
 * find the room of given visitor                                       *
 ***********************************************************************/
Result room_of_visitor(Visitor *visitor, char **room_name)
{
    if( (visitor == NULL) || (room_name == NULL) ) {
        return NULL_PARAMETER;
    }
    if(*visitor->room_name == NULL) return NOT_IN_ROOM;
    *room_name=malloc(sizeof(char)*(strlen(*visitor->room_name)+1));
    if (visitor->room_name == NULL) return MEMORY_PROBLEM;
    
    strcpy(*room_name,*visitor->room_name); //was missing, i think it's ok now
    return OK;
}

/************************************************************************
 * enter visitor to a challenge (with the smaller lexicographic name)   *
 * of a chosen level. if possible                                       *
 ***********************************************************************/
Result visitor_enter_room(ChallengeRoom *room, Visitor *visitor, Level level, int start_time)
{
    if( (visitor == NULL) || (room == NULL) ) {
        return NULL_PARAMETER;
    }
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
Result visitor_quit_room(Visitor *visitor, int quit_time)
{
    if (visitor == NULL) return NULL_PARAMETER;
    
    int totaltime=0;
    if(visitor->room_name == NULL ) return NOT_IN_ROOM;
    totaltime=quit_time-(visitor->current_challenge->start_time);
    if (totaltime < visitor->current_challenge->challenge->best_time){
        visitor->current_challenge->challenge->best_time = totaltime;
    }
    visitor->current_challenge->visitor=NULL;
    visitor->current_challenge->start_time=0;
    visitor->room_name=NULL;
    visitor->current_challenge=NULL;
    
    return OK;
}

