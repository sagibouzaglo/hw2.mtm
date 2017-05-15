/* additional types that may be required to support a linled list of visitors */
/* Please notice that it's preferable not to use this file. */

#include "challenge_system.h"

typedef struct node{
    Visitor* visitor;
    struct node* next;
} *Node;
