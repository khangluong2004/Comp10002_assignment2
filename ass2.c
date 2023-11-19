/* Program to generate text based on the context provided by input prompts.

  Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/,
  September 2023, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.
  All included code is (c) Copyright University of Melbourne, 2023.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Luong An Khang (ID: 1301266)
  Dated:     12/10/2023
*/

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>




/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message
#define NOSFMT "Number of statements: %d\n"                 // no. of statements
#define NOCFMT "Number of characters: %d\n"                 // no. of chars
#define NPSFMT "Number of states: %d\n"                     // no. of states
#define TFQFMT "Total frequency: %d\n"                      // total frequency
#define END_STR '\0' // End of string

#define CRTRNC '\r'                             // carriage return character
#define NEWLINE '\n'
  
#define TRUE 1
#define FALSE 0

#define INIT_ID 0  // The id of root/ initial state/ node
#define LEAF_FREQ 0  // The frequency of the leaf

#define ELLIPSES "..." // Ellipse
#define ELLIPSE_LEN 3  // Length of the ellipse "..."
#define CHAR_LIMIT 37  // Maximum char limit for stage 1

#define LEN_CHAR_STR 2 // Length of a string containing a single char


/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef struct state state_t;   // a state in an automaton
typedef struct node  node_t;    // a node in a linked list

struct node {                   // a node in a linked list of transitions has
    char*           str;        // ... a transition string
    state_t*        state;      // ... the state reached via the string, and
    node_t*         next;       // ... a link to the next node in the list.
};

typedef struct {                // a linked list consists of
    node_t*         head;       // ... a pointer to the first node and
    node_t*         tail;       // ... a pointer to the last node in the list.
} list_t;

struct state {                  // a state in an automaton is characterized by
    int             id;         // ... an identifier,
    int             freq;       // ... frequency of traversal,
    int             visited;    // ... visited status flag, and
    list_t*         outputs;    // ... a list of output states.
};

typedef struct {                // an automaton consists of
    state_t*        ini;        // ... the initial state, and
    int             nid;        // ... the identifier of the next new state.
} automaton_t;

/* USEFUL FUNCTIONS ----------------------------------------------------------*/
/* Stages and main functions */
/* Stage 0 */
automaton_t *stage_0(int *total_char);

/* Stage 1*/
void stage_1(automaton_t *automaton_ptr);

void check_and_replay(state_t **cur_state_ptr, int *cur_position_ptr, 
    int num_char_line, char c, int *finish_line_ptr);

void continue_after_replay(state_t **cur_state_ptr, 
    int *cur_position_ptr, int num_char_line);
void finish_transition_str(state_t **cur_state_ptr, int *cur_position, 
    int *remain);
void generate_to_limit(int remain, state_t **cur_state_ptr);

/* Stage 2 */
void stage_2(automaton_t *automaton_ptr, int total_char);
automaton_t *compress_automaton(automaton_t *automaton_ptr, int steps, 
    int *frequency);
int compress_states(state_t *parent, state_t *child, int *frequency);

/* Free automaton */
void free_automaton(automaton_t *automaton_ptr);
void free_states(state_t *root, automaton_t *automaton_ptr);

/* Support functions */

int mygetchar(void);            /* getchar() that skips carriage returns */
void print_ellipse(int char_limit);
void print_to_limit(char *str, int char_limit);

state_t *make_state(int id);
node_t *make_node(state_t *state);
char *make_str(char c);
char *dynamic_strcat(char *s1, char *s2);



/* Linked list functions */
list_t *make_empty_list();
int is_empty(list_t *list);
int is_single_node(list_t *list);

list_t *insert_node_head(list_t *list, node_t *node);
list_t *insert_str_head(list_t *list, char *str, state_t *state);
node_t *get_head(list_t *list);
list_t *remove_head(list_t *list);
list_t *free_list(list_t *list);

node_t *find_str(list_t *list, char search_char, int *cur_position);
node_t *find_max_node(list_t *list);
state_t *find_min(list_t *list);



/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(int argc, char *argv[]) {
    /* Message from Artem: The proposed in this skeleton file #define's,
    typedef's, and struct's are the subsets of those from my sample solution
    to this assignment. You can decide to use them in your program, or if
    you find them confusing, you can remove them and implement your solution
    from scratch. I will share my sample solution with you at the end of
    the subject. */

    /* Stage 0*/
    /* Creating automaton and store total number of char 
    (or total frequency on all nodes) for stage 2 */
    printf(SDELIM, 0);
    int total_char = 0;
    automaton_t *automaton_ptr = stage_0(&total_char);

    /* Stage 1 */
    /* Processing prompts given the automaton */
    printf(SDELIM, 1);
    stage_1(automaton_ptr);

    /* Stage 2*/
    /* Compress the automaton and process the prompt on new automaton*/
    printf(SDELIM, 2);
    stage_2(automaton_ptr, total_char);

    /* Free all the automaton states (and the automaton itself) */
    free_automaton(automaton_ptr);
        
    return EXIT_SUCCESS;        /* algorithms are fun!!! */ 
}


/* PROPERTIES SECTION -------------------------------------------------------*/
/* This section is the explanation of the properties resulted from compression
for arc strings that will be used in later program, to make it simpler to code 
and faster to run */

/* PROPERTIES OF THE OUTGOING ARCS FROM A NODE/ STATE */
/* By the compression algorithm, all transition, outgoing arcs/strings from the 
same state: 
1. Will only differ by the last character. 
2. Have the same length.
3. Each compression between parent node x and child node y will increase
the length of outgoing string of parent node x by 1.


This is proved by noting that:
1. The arc string only has more than 1 character after compression (since
originally, 1 arc string contains 1 single character).

Due to the depth-first traversal, for any compression between parent node x
and child node y, the parent node x compression will happen before any
compression on the child node y.

So, considering any current compression between parent node x
and child node y, the child node y cannot undergo compression before, thus 
the outgoing string of y will have length of exactly 1. 

So, after compression, all new outgoing arcs = outgoing arc of x + 
the different single character in the outgoing string of y.
Thus, the difference between the outgoing arcs
would lie only in the last characters (as they share all the rest from the 
single outgoing arc from x). This holds for all compression. 

Another consequence of this characteristic of compression is that:

1. After compression, all outgoing strings from x will increase by 1 in 
length compared to before (since all outgoing strings from y has length of 1).
2. All outgoing strings will also have the same length of each other (and
equal to the length of the outgoing string of x plus 1).

*/


/* MAIN STAGES */
/* STAGE 0 ------------------------------------------------------------------*/
/* Input the scanned statements and generate the automaton character by 
character. As input format is guaranteed, each statement will be finished
with a newline, and process is finished when an empty line is read. 
Return the pointer handle of the automaton */
automaton_t
*stage_0(int *total_char){
    automaton_t *automaton_ptr = (automaton_t *) malloc(sizeof(automaton_t));
    assert(automaton_ptr != NULL);
    
    /* Create the initial state */
    state_t *init_ptr = make_state(INIT_ID);

    automaton_ptr->ini = init_ptr;
    automaton_ptr->nid = 1; /* Next id after 0 */

    int num_statements = 0; /* Statement counter */
    int num_chars = 0; /* Characters counter */
    int new_line = TRUE; /* Flag to check for the start of a newline */

    /* The position to check the char inside the transition link */
    int cur_position = 0;


    char c;
    state_t *cur_state_ptr = init_ptr;

    while ((c = mygetchar()) != EOF){
        /* Check for empty line which triggers the end of stage 0.
        If non-empty line, reset cur_state_ptr and new_line flag */
        if (new_line == TRUE){
            if (c == NEWLINE){
                break;
            } else {
                new_line = FALSE;
                cur_state_ptr = init_ptr;
            }
        }

        /* If c is a '\n', move to newline, update statements counter,
        reset cur_state_ptr and skip the iteration */
        if (c == NEWLINE){
            new_line = TRUE;
            num_statements++;
            cur_state_ptr = NULL;
            continue;
        }

        /* Increment character counter */
        num_chars++;
        
        /* Look for the child node with transition str containing the scanned
        character at the cur_position (0 for stage 0) */
        node_t *found_node = find_str(cur_state_ptr->outputs, 
            c, &cur_position);

        if (found_node != NULL){
            /* Str already exists, so update frequency
            and shift cur_state to the next state */
            cur_state_ptr->freq++;
            cur_state_ptr = found_node->state; 

        } else {
            /* Create a new state from scanned character and add
            it to the current state outputs */
            char *insert_str = make_str(c);
            state_t *insert_state = make_state(automaton_ptr->nid);

            cur_state_ptr->outputs = insert_str_head(cur_state_ptr->outputs,
                insert_str, insert_state);

            /* Increment the next id in automaton */
            automaton_ptr->nid++;

            /* Increment the frequency of cur_state_ptr, and
            shift cur_state to the insert_state */
            cur_state_ptr->freq++;
            cur_state_ptr = insert_state;
        }
    }

    /* Finish processing stage 0, output summary.
    The nid is used as a state counter. */
    printf(NOSFMT, num_statements);
    printf(NOCFMT, num_chars);
    printf(NPSFMT, automaton_ptr->nid);

    /* Return values */
    *total_char = num_chars;
    return automaton_ptr;
}


/* STAGE 1 ---------------------------------------------------------*/
/* Scan the input prompts, and process to generate following characters
if replay possible */
void
stage_1(automaton_t *automaton_ptr){
    int new_line = TRUE; /* Flag for whether the char is start of a new line */
    int finish_line = FALSE; /* Finish processing line flag */
    int finish_file = FALSE; /* End of file flag */

    int num_char_line = 0; /* Counter to check for the char limit */
    int cur_position = 0; /* Set position in the transition string */
    

    state_t *cur_state_ptr = automaton_ptr->ini;

    /* Guard against EOF to re-use the function after stage 2 compression */
    while (!finish_file){
        char c = mygetchar();

        /* Check for empty line which triggers the end of stage 1.
        Else, reset all flags and cur_state_ptr to init state pointer */
        if (new_line == TRUE){
            if (c == NEWLINE){
                break;
            } else {
                new_line = FALSE;
                cur_state_ptr = automaton_ptr->ini;
            }
        } 

        /* Check if end of line or end of file detected to start generating */
        if (c == NEWLINE || c == EOF){
            /* Set flags accordingly */
            new_line = TRUE;

            if (c == EOF){
                finish_file = TRUE;
            }

            /* Replay not possible or all of char limit is used 
            (as signaled by the finish_line flag), so reset flags, 
            cur_position, put new line and terminate for this line */
            if (finish_line == TRUE){
                finish_line = FALSE;
                cur_position = 0;
                num_char_line = 0;

                putchar(NEWLINE);
                continue;
            }

            /* Replay possible, so generate corresponding outputs */
            continue_after_replay(&cur_state_ptr, &cur_position, 
                num_char_line);
            
            /* Reset num_char_line and cur_position */
            cur_position = 0;
            num_char_line = 0;

            continue;
        }

        if (finish_line == TRUE){
            /* Finished with this line (reach char limit or can't replay), 
            so continue scanning until '\n' */
            continue;
        }

        /* Check if there is enough space in char limit to print new char */
        if (num_char_line >= CHAR_LIMIT){
            finish_line = TRUE;
            continue;
        }

        /* Print out the character */
        putchar(c);
        /* Increment char counter */
        num_char_line++;

        /* Find the char and replay, if possible.
        If impossible, turn the finish_line flag on to continue consuming
        the line without processing */
        check_and_replay(&cur_state_ptr, &cur_position, 
            num_char_line, c, &finish_line);
    }
}

/* Check if the input string is in the automaton and "replay" it (by printing
the char and continue down the automaton). Only check if still in char limit.
If can't find, print the ELLIPSE up to 37 characters limit, then set
finish_line flag. 

Note: 
Due to the Properties section noted above, the finding of matched string
could be carried out char by char, as:

1. All arc/ string with more than 1 char coming from the same state will 
differ ONLY by the last char. 
2. It's obvious that char by char works if the arc contains 1 char, so the 
explanation is for arc with more than 1 char.

If the current character is in the middle of an arc/ string, that must 
mean all the characters before the current char is matched by at least 1 string
, thus all string (based on 1) must match as they all share the same pre-fix 
string before the last character (since the current char is on the arc/string,
all the characters before it, if on the same arc, must not be the last char).

So, we only need to check if there exist a string containing the current 
character at the current position. The string that contains it will 
automatically match all the prefix and is the correct path to follow.
*/
void
check_and_replay(state_t **cur_state_ptr, int *cur_position_ptr, 
    int num_char_line, char c, int *finish_line_ptr){

    /* Find the string in outputs */
    node_t *found_node = find_str((*cur_state_ptr)->outputs, 
        c, cur_position_ptr);

    if (found_node == NULL){
        /* Can't find char in automaton, so set finish_line flag and print the 
        ELLIPSE up to the char limit */
        *finish_line_ptr = TRUE;

        int remain = CHAR_LIMIT - num_char_line;
        print_ellipse(remain);

    } else {
        /* Check if the full length of current transition link is checked
        (when cur_position reset to 0) before moving on to next state */
        if (*cur_position_ptr == 0){
            *cur_state_ptr = found_node->state;
        }

    }
}

/* If replay possible, the function prints out the ELLIPSE and start
generating up to the char limit of the line. Only called if still
within the char limit. */
void
continue_after_replay(state_t **cur_state_ptr, int *cur_position_ptr, 
    int num_char_line){

    /* Find the remaining characters left until limit reached */
    int remain = CHAR_LIMIT - num_char_line;

    /* Print the ellipse. If the remain is only enough for the ellipse,
    return early (as there is no more space for generating) */
    if (remain <= ELLIPSE_LEN){
        print_ellipse(remain);
        putchar(NEWLINE);
        return;
    } else {
        print_ellipse(ELLIPSE_LEN);
    }

    remain -= ELLIPSE_LEN;

    /* Print out the rest of the transition string if not finished,
    shown by the checking cur_position (position in the transition string that
    is compared with the current scanned char) not reset to 0 */
    if (*cur_position_ptr != 0){
        finish_transition_str(cur_state_ptr, cur_position_ptr, 
            &remain);
    }

    /* Generating the following strings up to char limit */
    generate_to_limit(remain, cur_state_ptr);
    
    /* Put newline char */
    putchar(NEWLINE);

    return;
}

/* Function used to print out all the remaining of the transition_str on arc
if the last char in prompts is in the middle of a transition string/ arc.

From the PROPERTIES section, we could guarantee that all prefix of the 
string are the same, and should match the searched string upto cur_position 
(so no need for checking if this condition is valid by re-iterating from the 
start of the string). Otherwise, the processing has been terminated since
no matched string has been found previously.

The path to follow during generation (for the rest of the transition str) 
would be the maximum node (max frequency and max ASCII). */
void
finish_transition_str(state_t **cur_state_ptr, int *cur_position, int *remain){
    /* Find the max node, take the string, and print the rest of the string
    up to char limit */
    node_t *next_node = find_max_node((*cur_state_ptr)->outputs);
    print_to_limit(next_node->str + *cur_position, *remain);
    
    /* Reduce remain chars by the string length. If the string is truncated,
    the remain is set to 0, and will be checked to stop generation */
    int str_length = strlen(next_node->str + *cur_position);
    if (str_length > *remain){
        *remain = 0;
    } else {
        *remain -= str_length;
    }

    /* Finish transition string, so shift cur_state */
    *cur_state_ptr = next_node->state;

    /* Reset cur_position, since all of the string is printed out */
    *cur_position = 0;
}


/* Used when replay is possible. The function continues 
generating up to the char limit (after printing the ellipse
and finish any transition string that the cur char is in the middle of). */
void
generate_to_limit(int remain, state_t **cur_state_ptr){
    /* Keep a counter of the number of characters used */
    int used_char = 0;
    while (used_char < remain){
        node_t *next_node = find_max_node((*cur_state_ptr)->outputs);
        
        /* Reach the end of automation, so stop generating */
        if (next_node == NULL){
            return;
        }

        /* Print out the transition string and continue 
        if there are remaining places under CHAR_LIMIT */
        print_to_limit(next_node->str, remain - used_char);

        /* Add the entire length, since if the str is truncated, the 
        used_char will be increased above remain to end loop */
        used_char += strlen(next_node->str);

        *cur_state_ptr = next_node->state;
    }

}



/* STAGE 2 ---------------------------------------------------------*/

/* Function reads from stdin the required number of compression steps.
Traverse the automaton depth-first, prioritize ASCII minimum node and 
compress any 2 states x->y if satisfied the conditions outlined in
the question (x: 1 single outgoing arc, y: 1 or more outgoing arcs).

Print out the summary of the compressed automaton (total frequency and states), 
perform prompt processing on the automaton and print finishing lines. 

Search directly for the minimum node during depth-first search
as allowed for simplicity. 

Use automaton.nid as a states counter (since no more state needs to be 
generated).

Note that: From stage 0, total char = total frequency stored on all nodes
(since the total frequency is guaranteed to increase by 1 when 1 char/arc
is added to the automaton, which increase the frequency of the state before
the arc by 1).
The new frequency = Total intial frequency - Total frequency of removed states. 
*/

void
stage_2(automaton_t *automaton_ptr, int total_char){    
    int steps;

    scanf("%d", &steps);

    /* Use mygetchar to consume the new line char (either \n or \r\n) */
    mygetchar();

    /* Compress the automaton, and keeping track of the frequency 
    of the removed states */
    int removed_frequency = 0;
    automaton_ptr = compress_automaton(automaton_ptr, steps, 
        &removed_frequency);

    /* Calculate the new total frequency, as explained above */
    int total_freq = total_char - removed_frequency;

    /* Print the summary of number of states and 
    total frequency of all states */
    printf(NPSFMT, automaton_ptr->nid);
    printf(TFQFMT, total_freq);

    /* Apply stage 1 process of prompts on the compressed automaton */
    printf(MDELIM);
    stage_1(automaton_ptr);
    printf(THEEND);
}



/* Compress the automaton with `steps` compression stages or until there is
no more possible compression. Compression is detected using depth-first
search and criteria listed in lectures.

Return the pointer to the compressed automaton (same as the input).

- For the depth-first search: 

A stack is used, where: 
1. Any new node is inserted at head, and its children (unvisited and 
with min ASCII) will be checked and added (if any). 

2. The node is removed if all its children are visited or it has no children.
The search continues until the stack is empty (no more available compression)
or the number of compressions steps required is satisfied.

- For the compression stages:

Since the compression only affect the cur_node and the nodes below/after it 
(in depth-first search order), the path from the root to the node before 
cur_node (if any) will remain unchanged. 

Thus, there is no need to start searching from the root. Since if we restart
from the root, the same path will be traversed to reach the node before
cur_node. It's more efficient to restart searching from 
the node before cur_node.

Except if the cur_node is the root node (no before node), in which we will 
recheck from the cur_node (which is the root). */

automaton_t
*compress_automaton(automaton_t *automaton_ptr, int steps, 
    int *removed_frequency){

    /* Number of states after all compressions.
    Note: The nid property in automaton_t is the next id, and also
    the counter for all possible states. If more steps required than
    available states, set target_states to 0 and do all possible compress */
    int target_states = 0;
    if (steps < automaton_ptr->nid){
        target_states = automaton_ptr->nid - steps;
    }

    /* Set up the stack for depth-first search and add the init node */
    list_t *stack = make_empty_list();
    node_t *init_node = make_node(automaton_ptr->ini);
    stack = insert_node_head(stack, init_node);

    /* Continue searching until all `steps` finished or no more possible
    compression */
    while(!is_empty(stack) && (automaton_ptr->nid > target_states)){
        node_t *cur_node = get_head(stack);
        cur_node->state->visited = TRUE;

        /* Find the next smallest ASCII state that is unvisited */
        state_t *next_state = find_min(cur_node->state->outputs);

        /* If nothing found, as all the head node's children are checked
        or it has no children, then remove the head */
        if (next_state == NULL){
            stack = remove_head(stack);
            continue;
        }

        /* Check if the current state and its min child is compress-able */
        int compressable = compress_states(cur_node->state, next_state, 
            removed_frequency);
        
        if (compressable == FALSE){
            /* Can't compress, so add the next_node as head, and continue 
            searching */
            node_t *next_node = make_node(next_state);
            stack = insert_node_head(stack, next_node);

        } else {
            /* Else, the compress function compresses the 2 nodes */

            /* Update the total states. Reduce 1 as 1 state is removed
            during compression */
            automaton_ptr->nid--;

            /* As explained above, restart compression from the parent node of
            cur_node, by reset cur_node visited state to unvisited,
            and remove cur_node from stack (if not root node).  */

            cur_node->state->visited = FALSE;

            if (cur_node->state->id != INIT_ID){
                stack = remove_head(stack);
            }             
        }
    }

    /* Finish compression, so free stack (the init_node will be
    part of the stack or already freed before) */
    stack = free_list(stack);

    return automaton_ptr;
}


/* Given `parent` and `child` are 2 consecutive states (`parent` -> `child`), 
the function check and compress `parent` and `child`, if possible.
The `parent` and `child` states corresponds to x and y states specified in 
the question.
`parent` and `child` must not be NULL. `parent->outputs` must not be 
NULL, since `child` must be in the list.

Return FALSE (0) if not able to compress,
TRUE (1) if possible and compressed */

int
compress_states(state_t *parent, state_t *child, 
    int *removed_frequency){

    assert(parent != NULL && child != NULL);
    assert(parent->outputs != NULL);

    /* Check validity */
    /* Single output of parent */
    if (!is_single_node(parent->outputs)){
        return FALSE;
    }

    /* Non-empty child-outputs */
    if (is_empty(child->outputs)){
        return FALSE;
    }

    /* If valid, compress parent and child nodes */

    /* Update the removed_frequency by the frequency of child */
    *removed_frequency = *removed_frequency + child->freq;

    /* Traverse through the outputs of child, and add
    the outgoing str/arc of parent to all str/arc of child's outputs. 
    Free the previous string of child in the process as well.
    The new outputs arcs will be transferred to the parent nodes. */
    char *parent_str = parent->outputs->head->str;

    node_t *cur_node = child->outputs->head;
    while (cur_node != NULL){
        char *pre_str = cur_node->str; 
        cur_node->str = dynamic_strcat(parent_str, pre_str);

        free(pre_str);

        cur_node = cur_node->next;
    }

    /* Free current parent->outputs (freeing the nodes, not the child state)
    Note: parent_str is inside parent->outputs, so will be freed as well */
    parent->outputs = free_list(parent->outputs);

    /* Assign the new outgoing arcs created in child->outputs to parent */
    parent->outputs = child->outputs;

    /* Free child */
    free(child);

    return TRUE;
}


/* FREE AUTOMATON ---------------------------------------------------------*/
/* Free automaton */
void
free_automaton(automaton_t *automaton_ptr){
    if (automaton_ptr == NULL){
        return;
    }

    /* Free all the states */
    state_t *root_state = automaton_ptr->ini;
    free_states(root_state, automaton_ptr);

    /* Check if all states are freed by checking
    the nid counter */
    assert(automaton_ptr->nid == 0);

    /* Free the automaton */
    free(automaton_ptr);
    return;
}

/* Free the input state and all of its descendants (eg: all nodes
linked to the input states, its children nodes, grandchildren and so on).
Update the automaton counter to validate at the end that all states
are freed */
void
free_states(state_t *root, automaton_t *automaton_ptr){
    /* Base case */
    if (root == NULL){
        return;
    }

    /* Recursive case */
    /* Traverse the outputs and call free_states on the children */
    node_t *cur_node = root->outputs->head;

    while (cur_node != NULL){
        free_states(cur_node->state, automaton_ptr);
        cur_node = cur_node->next;
    }

    /* Free the linked list outputs itself */
    free_list(root->outputs);

    /* Free the root state */
    free(root);

    /* Update automaton states counter (nid) */
    automaton_ptr->nid--;
    
    return;
}



/* USEFUL FUNCTIONS ---------------------------------------------------------*/

/* An improved version of getchar(); skips carriage return characters.
NB: Adapted version of the mygetchar() function by Alistair Moffat */
int mygetchar() {
    int c;
    while ((c=getchar())==CRTRNC);
    return c;
}

/* Create an empty state struct (in heap) with the given id.
The outputs list is created as an empty list.
Return the handle pointer of the new state. */
state_t
*make_state(int id){
    state_t *new = (state_t *) malloc(sizeof(state_t));
    assert(new != NULL);

    new->id = id;
    new->freq = 0;
    new->visited = FALSE;
    new->outputs = make_empty_list();

    return new;
}

/* Make a node with given state and NULL pointer placed for 
all pointers, including the str.
Return the handle pointer of the new node. */
node_t
*make_node(state_t *state){
    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    assert(new_node != NULL);

    new_node->state = state;
    new_node->str = NULL;
    new_node->next = NULL;

    return new_node;
}

/* Create a string (in heap) containing a single character.
Return the handle pointer of new string. */
char
*make_str(char c){
    char *new = (char *) malloc(LEN_CHAR_STR * sizeof(char));
    assert(new != NULL);

    new[0] = c;
    new[1] = END_STR;
    return new;
}

/* Combine 2 strings to create a new string (in heap).
Return the pointer handle of the new string. */
char
*dynamic_strcat(char *s1, char *s2){
    assert(s1 != NULL && s2 != NULL);

    int len1 = strlen(s1);
    int len2 = strlen(s2);
    /* Form the new_len (Add 1 for the END_STR character) */
    int new_len = len1 + len2 + 1;

    char *new_str = (char *) malloc(new_len);
    assert(new_str != NULL);

    /* Copy characters from s1 */
    for (int i=0; i < len1; i++){
        new_str[i] = s1[i];
    }

    /* Copy characters from s2 */
    for (int i=0; i < len2; i++){
        new_str[len1 + i] = s2[i];
    }

    new_str[new_len - 1] = END_STR;

    return new_str;
}

/* Print the ellipse up to `char_limit` */
void
print_ellipse(int char_limit){
    char *ellipse = ELLIPSES;
    print_to_limit(ellipse, char_limit);
}

/* Print the `str` up to `char_limit` */
void
print_to_limit(char *str, int char_limit){
    int length_str = strlen(str);

    for (int i=0; i < char_limit && i < length_str; i++){
        putchar(str[i]);
    }
}

/* LINKED LIST FUNCTIONS ----------------------------------------------------*/
/* Adapted from Alistair Moffat's linked list code in 2023 COMP10002 lecture 7
(Wrote those myself to better suit the assignment, but acknowledge Alistair 
just in case) */

/* Make an empty list.
Return the empty list pointer handle. */
list_t
*make_empty_list(){
    list_t *list = (list_t *) malloc(sizeof(list_t));
    assert(list != NULL);
    list->head = list->tail = NULL;
    return list;
}

/* Check empty list.
Return 1 if empty, 0 if not. */
int
is_empty(list_t *list){
    return (list->head == NULL);
}

/* Check if the list has 1 single node.
Return 1 if single node, 0 if not. */
int
is_single_node(list_t *list){
    /* Check non-existing and empty list */
    if (list == NULL || is_empty(list)){
        return FALSE;
    }

    /* Check that there is no second item */
    if (list->head->next == NULL){
        return TRUE;
    }

    return FALSE;
}

/* Used to insert a node to the start/head of the list.
list must not be NULL.
Return the same list input pointer if successful. */
list_t 
*insert_node_head(list_t *list, node_t *node){
    assert(list != NULL);

    node->next = list->head;
    list->head = node;
    
    /* Handle empty linked list */
    if (list->tail == NULL){
        list->tail = node;
    }

    return list;
}

/* Making a node, given a transition string and the state, 
and inserting that node to the linked list.
list must not be NULL.
Return the same list input pointer if successful. */
list_t
*insert_str_head(list_t *list, char *str, state_t *state){
    assert(list != NULL);

    node_t *new = make_node(state);
    assert(new != NULL);
    new->str = str;

    return insert_node_head(list, new);
}



/* Get head (without removing it).
list must not be NULL.
Return NULL if the list is empty (so list->head == NULL) */
node_t
*get_head(list_t *list){
    assert(list != NULL);

    return list->head;
}

/* Remove a node from head and free the node.
list must not be NULL and not be empty. 
Return the pointer handle of the list */
list_t
*remove_head(list_t *list){
    assert(list != NULL && list->head != NULL);
    
    node_t *old_head = list->head;
    list->head = list->head->next;

    free(old_head->str);
    free(old_head);

    if (list->head == NULL){
        list->tail = NULL;
    }

    return list;
}

/* Free the linked list after use. 
Note: Only free the node and str, but NOT the state.

Always return NULL value to be applied to the list pointer */
list_t
*free_list(list_t *list){
    if (list == NULL){
        return NULL;
    }

    node_t *prev;
    node_t *cur_node = list->head;
    while(cur_node != NULL){
        prev = cur_node;
        cur_node = cur_node->next;
        free(prev->str);
        free(prev);
    }
    
    free(list);
    return NULL;
}



/* Find the node containing the string that stores the search_char at 
cur_position in the string.

If found, update the cur_position to point to the next character in
the string (to be checked). Reset cur_position to 0 if increment pass
the length of the string.

Return the node pointer if found, or NULL if not */
node_t
*find_str(list_t *list, char search_char, int *cur_position){
    if (list == NULL){
        return NULL;
    }

    node_t *cur_node = list->head;
    while (cur_node != NULL){
        if (cur_node->str[*cur_position] == search_char){
            /* Increment cur_position up to, but not more than,
            the length of the string - 1 (by taking mod) */
            int str_length = strlen(cur_node->str);
            *cur_position = (*cur_position + 1) % str_length;

            return cur_node;
        }

        cur_node = cur_node->next;
    }

    return NULL;
}

/* Find the node with highest frequency and highest ASCII.
As proved in the PROPERTIES section, if the transition string differs,
the only difference will lie in the last character, so only the 
last character will be compared for string comparison. 

Moreover, since the length of all outgoing strings are equal, the last position
can be precomputed using the length of any string (after checking the list
is not empty).

Return the node pointer if found, or NULL if empty list or
NULL pointer. */
node_t
*find_max_node(list_t *list){
    if (list == NULL){
        return NULL;
    }

    /* Check empty list */
    if (is_empty(list)){
        return NULL;
    }

    /* Find the position of last char */
    int last_char_post = strlen(list->head->str) - 1;

    /* Initialized max_freq and max_node to that of the head state/ node */
    state_t *head_state = list->head->state;

    int max_freq = head_state->freq;
    node_t *max_node = list->head;


    /* Start checking from the next node from head */
    node_t *cur_node = list->head->next;
    while (cur_node != NULL){
        int cur_freq = cur_node->state->freq;
        char *cur_str = cur_node->str;

        /* Update the node with highest freq, then highest ASCII str */
        if (cur_freq > max_freq){
            max_freq = cur_freq;
            max_node = cur_node;
        } else if (cur_freq == max_freq){
            /* Only need to use last character for comparison (noted above) */
            if (cur_str[last_char_post] > (max_node->str)[last_char_post]){
                max_node = cur_node;
            }
        }

        cur_node = cur_node->next;
    }

    return max_node;
}


/* Find the node with lowest ASCII and is unvisited.

As noted in the PROPERTIES section function,
the difference between outgoing transition strings from the same node
only lies in the last character, thus for efficiency, only the last
character is compared for str comparison.

Moreover, since the length of all outgoing strings are equal, the last position
can be precomputed using the length of any string (after checking the list
is not empty).

Return the state of the node.
Return NULL if the list is empty or all nodes are visited. */
state_t
*find_min(list_t *list){
    assert(list != NULL);

    /* Check empty list */
    if (is_empty(list)){
        return NULL;
    }

    /* Calculate last character position, based on the head node string */
    int last_char_post = strlen(list->head->str) - 1;

    /* Set up minimum node */
    char *min_str = NULL;
    node_t *min_node = NULL;

    node_t *cur_node = list->head;
    while (cur_node != NULL){
        /* If already visited, skip the node */
        if (cur_node->state->visited == TRUE){
            cur_node = cur_node->next;
            continue;
        }

        /* Set min_node to the first unvisited node */
        if (min_str == NULL){
            min_str = cur_node->str;
            min_node = cur_node;
            cur_node = cur_node->next;
            continue;
        }

        /* Update min_node */
        if ((cur_node->str)[last_char_post] < min_str[last_char_post]){
            min_str = cur_node->str;
            min_node = cur_node;
        }
        cur_node = cur_node->next;
    }

    /* All node is visited, so return NULL */
    if (min_node == NULL){
        return NULL;
    }

    return min_node->state;
}

/* THE END ------------------------------------------------------------------*/
