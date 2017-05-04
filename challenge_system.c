#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>
#include <string.h>
#include <assert.h>


#include "challenge_system.h"

#define ROW_LENGTH 51


/* open the data base file and take the imformation from it*/
Result create_system(char *init_file, ChallengeRoomSystem **sys){
    char buffer[ROW_LENGTH];
    
    FILE* file= fopen(init_file,"r");
    if (file == NULL){
        return NULL_PARAMETER;
    }
    fgets(buffer,ROW_LENGTH,file);
    
    return OK;
}
