#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>
#include <string.h>
#include <assert.h>


#include "challenge.h"

/************************************************************************
 * insialise challege struct to 0                                       *
 ***********************************************************************/
Result init_challenge(Challenge *challenge, int id, char *name, Level level){
    if ((challenge == NULL) || (name == NULL)){
        return NULL_PARAMETER;
    }
    
    char* name_copy = malloc((strlen(name)+1)*sizeof(char));
    if (name_copy == NULL){
        return MEMORY_PROBLEM;
    }
    
    memcpy(name_copy, name, (strlen(name)+1));
    challenge->id = id;
    challenge->level = level;
    challenge->name = name_copy;
    challenge->best_time=0;
    challenge->num_visits=0;
    return OK;
}

/***********************************************************************
 * reset challege struct to 0                                          *
 **********************************************************************/
Result reset_challenge(Challenge *challenge){
    if (challenge == NULL){
        return NULL_PARAMETER;
    } else {
        free(challenge->name);
        memset(challenge, 0, sizeof(*challenge));
    }
    return OK;
}

/************************************************************************
 * changing the name of challenge                                       *
 ***********************************************************************/
Result change_name(Challenge *challenge, char *name){
    if ((challenge == NULL) || (name == NULL)){
        return NULL_PARAMETER;
    }
    
    char* name_copy = malloc((1+strlen(name))*sizeof(char));
    if (name_copy == NULL){
        return MEMORY_PROBLEM;
    }
    memcpy(name_copy, name, (1+strlen(name)));
    free(challenge->name);
    challenge->name = name_copy;
    return OK;
}

/************************************************************************
 * set best time for challenge                                          *
 ***********************************************************************/
Result set_best_time_of_challenge(Challenge *challenge, int time){
    if (challenge == NULL){
        return NULL_PARAMETER;
    }
    if (time < 0){
        return ILLEGAL_PARAMETER;
    }
    if (time< challenge->best_time){
        challenge->best_time=time;
    }
    return OK;
}

/************************************************************************
 * return the best time for a challenge                                 *
 ***********************************************************************/
Result best_time_of_challenge(Challenge *challenge, int *time){
    if (challenge == NULL){
        return NULL_PARAMETER;
    }
    *time=challenge->best_time;
    
    return OK;
}

/************************************************************************
 * add +1 to visitors count                                             *
 ***********************************************************************/
Result inc_num_visits(Challenge *challenge){
    if (challenge == NULL){
        return NULL_PARAMETER;
    }
    challenge->num_visits++;
    return OK;
}

/************************************************************************
 * return visits amount                                                 *
 ***********************************************************************/
Result num_visits(Challenge *challenge, int *visits){
    if (challenge == NULL){
        return NULL_PARAMETER;
    }
    *visits=challenge->num_visits;
    
    return OK;
}
