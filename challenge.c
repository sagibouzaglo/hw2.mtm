#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "challenge_system.h"
#include "challenge.h"
#define CHECK_NULL(ptr) if(ptr==NULL){\
                            return NULL_PARAMETER;\
                            }
#define CHECK_MEMORY(ptr) if(ptr==NULL){\
                            return MEMORY_PROBLEM;\
                            }
/************************************************************************
 * insialise challege struct to 0                                       *
 * 12 lines                                                             *
 ***********************************************************************/
Result init_challenge(Challenge *challenge, int id, char *name, Level level){
    CHECK_NULL(name);
    CHECK_NULL(challenge);
    char* name_copy = malloc((strlen(name)+1)*sizeof(char));
    CHECK_MEMORY(name_copy);
    memcpy(name_copy, name, (strlen(name)+1));
    challenge->id = id;
    challenge->level = level;
    challenge->name = name_copy;
    challenge->best_time=0;
    challenge->num_visits=0;
    return OK;
}

/************************************************************************
 * reset challege struct to 0                                           *
 * 9 lines                                                              *
 ***********************************************************************/
Result reset_challenge(Challenge *challenge){
    CHECK_NULL(challenge);
    free(challenge->name);
    challenge->name=NULL;
    challenge->id=0;
    challenge->best_time=0;
    challenge->num_visits=0;
    challenge -> level = Easy;
    return OK;
}

/************************************************************************
 * changing the name of challenge                                       *
 * 9 lines                                                              *
 ***********************************************************************/
Result change_name(Challenge *challenge, char *name){
    CHECK_NULL(challenge);
    CHECK_NULL(name);
    char* name_copy = malloc((1+strlen(name))*sizeof(char));
    CHECK_MEMORY(name_copy);
    memcpy(name_copy, name, (1+strlen(name)));
    free(challenge->name);
    challenge->name = name_copy;
    return OK;
}

/************************************************************************
 * set best time for challenge                                          *
 * 7 lines                                                              *
 ***********************************************************************/
Result set_best_time_of_challenge(Challenge *challenge, int time){
    CHECK_NULL(challenge);
    if (time < 0 || (time > challenge->best_time && challenge->best_time!=0)){
        return ILLEGAL_PARAMETER;
    }
    challenge->best_time = time;
    return OK;
}

/************************************************************************
 * return the best time for a challenge                                 *
 * 4 lines                                                              *
 ***********************************************************************/
Result best_time_of_challenge(Challenge *challenge, int *time){
    CHECK_NULL(challenge);
    *time = challenge->best_time;
    return OK;
}

/************************************************************************
 * add +1 to visitors count                                             *
 * 4 lines                                                              *
 ***********************************************************************/
Result inc_num_visits(Challenge *challenge){
    CHECK_NULL(challenge);
    challenge->num_visits++;
    return OK;
}

/************************************************************************
 * return visits amount                                                 *
 * 4 lines                                                              *
 ***********************************************************************/
Result num_visits(Challenge *challenge, int *visits){
    CHECK_NULL(challenge);
    *visits=challenge->num_visits;
    return OK;
}
