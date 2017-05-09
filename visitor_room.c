#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>
#include <string.h>
#include <assert.h>


#include "visitor_room.h"

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

Result room_of_visitor(Visitor *visitor, char **room_name)
{
    if( (visitor == NULL) || (room_name == NULL) ) {
        return NULL_PARAMETER;
    }
    *room_name=malloc(sizeof(char)*(strlen(*visitor->room_name)+1));/* moved the D reference from "room name" to visitor - sagi*/
    if (visitor->room_name == NULL) { /* removed the D reference here, it gave us syntax error -sagi*/
        return MEMORY_PROBLEM;
    }
    if(*visitor->room_name == NULL) return NOT_IN_ROOM;
    return OK; /*if it doesn't enter all the "if"s we need a return - sagi*/
}

/* the challenge to be chosen is the lexicographically named smaller one that has
 the required level. assume all names are different. */
Result visitor_enter_room(ChallengeRoom *room, Visitor *visitor, Level level, int start_time)
{
    /* the challenge to be chosen is the lexicographically named smaller one that has
     the required level. assume all names are different. */
    if( (visitor == NULL) || (room == NULL) ) {
        return NULL_PARAMETER;
    }
    int places=0;
    num_of_free_places_for_level(room,level,&places);
    
    if(places < 1) return NO_AVAILABLE_CHALLENGES;
    
    if(visitor->room_name != NULL )
        return ALREADY_IN_ROOM;
    
    Challenge *ChallengeToVisitor = (room->challenges)->challenge; /*"Redefinition of 'challege',couldn't fix it for now... :(  -sagi*/
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

Result visitor_quit_room(Visitor *visitor, int quit_time){
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

/* IMPLEMENT HERE ALL WHAT IS NEEDED */
