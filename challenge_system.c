#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>
#include <string.h>
#include <assert.h>


#include "challenge_system.h"
#include "challenge_room_system_fields.h"

#define ROW_LENGTH 51


/* open the data base file and take the imformation from it*/
Result create_system(char *init_file, ChallengeRoomSystem **sys){
    char buffer[ROW_LENGTH];
    
    FILE* file= fopen(init_file,"r");
    if (file == NULL){
        return NULL_PARAMETER;
    }
    fgets(buffer,ROW_LENGTH,file);
    if (buffer == NULL){
        return NULL_PARAMETER;
    }
    sys->*name=malloc(sizeof(char*)*strlen(buffer));
    if (*sys->(*name) == NULL){
        return NULL_PARAMETER;
    }
    /* I'm not sure about the Syntax *sys->(*name) */
    strcpy(*sys->(*name),buffer);

    return OK;
}


Result destroy_system(ChallengeRoomSystem *sys, int destroy_time,
                      char **most_popular_challenge_p, char **challenge_best_time);


Result visitor_arrive(ChallengeRoomSystem *sys, char *room_name, char *visitor_name, int visitor_id, Level level, int start_time);


Result visitor_quit(ChallengeRoomSystem *sys, int visitor_id, int quit_time);


Result all_visitors_quit(ChallengeRoomSystem *sys, int quit_time);


Result system_room_of_visitor(ChallengeRoomSystem *sys, char *visitor_name, char **room_name);


Result change_challenge_name(ChallengeRoomSystem *sys, int challenge_id, char *new_name);


Result change_system_room_name(ChallengeRoomSystem *sys, char *current_name, char *new_name);


Result best_time_of_system_challenge(ChallengeRoomSystem *sys, char *challenge_name, int *time);


Result most_popular_challenge(ChallengeRoomSystem *sys, char **challenge_name);


#endif // CHALLENGE_SYSTEM_H_
