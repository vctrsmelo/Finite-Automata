/*
 * MIT License
Copyright (c) 2020 Victor Melo
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRANSITIONS 255

struct Transition {
    char *value;
    struct State *nextState;
};

struct State {
    char* name;
    bool isEnd;
    struct Transition *transitions;
    int transitionsCount;
};

/*
 * ending at e7 = numerical constant
 * ending at e8 = identifier
 * ending at e10 = string
 */
struct State e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10;

// Init
void initStates();
void initAState(struct State* state, char* name, bool isEnd);
void addTransition(struct State *fromState, char *value, struct State *toState);

// Parsing
struct State* getFinalState(char *input, struct State *state);
bool isStringIn(char *input, struct State *state);

// Aux functions
char* getAllCharsExcept(char *exceptions);

int main() {
    initStates();

    printf("Automato to identify strings, identifiers or numerical constant in PASCAL.\n");
    printf("Developed by \033[1;31mVictor S. Melo (Github: @vctrsmelo)\033[0m\n");

    while(true) {
        printf("Type input [exit() to finish]: ");
        char buffer[100];
        if (fgets(buffer, sizeof buffer, stdin))
            buffer[strcspn(buffer, "\n")] = '\0';  // Lop off potential tailing \n

        if (strcmp(buffer, "exit()") == 0) //TODO: not working
            break;

        struct State* finalState = getFinalState(buffer, &e0);

        if (finalState == &e2 || finalState == &e4 || finalState == &e7)
            printf("YES! %s is a numerical constant in PASCAL\n\n", buffer);
        else if (finalState == &e8)
            printf("YES! %s is an identifier in PASCAL\n\n", buffer);
        else if (finalState == &e10)
            printf("YES! %s is a string in PASCAL\n\n", buffer);
        else
            printf("NO! %s is not recognized as identifier, string or numerical constant in PASCAL\n\n", buffer);
    }

}


void initStates() {
    initAState(&e0, "e0", false);
    initAState(&e1,"e1", false);
    initAState(&e2, "e2",true);  // e2 is final state
    initAState(&e3, "e3",false);
    initAState(&e4, "e4",true);  // e4 is final state
    initAState(&e5, "e5",false);
    initAState(&e6, "e6",false);
    initAState(&e7, "e7",true);  // e7 is final state
    initAState(&e8, "e8",true); // e8 is final state
    initAState(&e9, "e9",false);
    initAState(&e10, "e10",true);// e10 is final state

    // ------- Numerical Constants -------

    char* n_transition = "0123456789";
    char* s_transition = "+-";
    char* E_transition = "E";
    char* dot_transition = ".";

    addTransition(&e0, s_transition, &e1);
    addTransition(&e0, n_transition, &e2);

    addTransition(&e1, n_transition, &e2);

    addTransition(&e2, n_transition, &e2);
    addTransition(&e2, dot_transition, &e3);
    addTransition(&e2, E_transition, &e5);

    addTransition(&e3, n_transition, &e4);

    addTransition(&e4, n_transition, &e4);
    addTransition(&e4, E_transition, &e5);

    addTransition(&e5, s_transition, &e6);
    addTransition(&e5, n_transition, &e7);

    addTransition(&e6, n_transition, &e7);

    addTransition(&e7, n_transition, &e7);


    // ------- Identifiers -------
    char* underline_transition = "_";
    char* a_transition = "abcdefghijklmnopqrstuvwxyz";

    addTransition(&e0, a_transition, &e8);

    addTransition(&e8, a_transition, &e8);
    addTransition(&e8, n_transition, &e8);
    addTransition(&e8, underline_transition, &e8);


    // ------- Strings -------
    char* string_transition = "'";
    char* nabs_transition = getAllCharsExcept("'");

    addTransition(&e0,string_transition, &e9);
    addTransition(&e9, nabs_transition, &e9);
    addTransition(&e9, string_transition, &e10);
    addTransition(&e10, string_transition, &e9);

}

void initAState(struct State* state, char* name, bool isEnd) {
    struct State aux = {.name = name, .isEnd = isEnd, .transitionsCount = 0, .transitions = malloc(sizeof(struct Transition) * MAX_TRANSITIONS)};
    *state = aux;
}

void addTransition(struct State *fromState, char *value, struct State *toState) {
    struct Transition new_transition = {.value = value, .nextState = toState};

    int i = fromState->transitionsCount;
    fromState->transitions[i] = new_transition;
    fromState->transitionsCount++;

}

bool isStringIn(char *input, struct State *state) {
    return getFinalState(input, state)->isEnd;
}

struct State* getFinalState(char *input, struct State *state) {
    int input_index = 0;
    char c = input[input_index];
    struct State* current_state = state;

    while(c != '\0') {

        int i;
        bool transitioned = false;
        for(i = 0; i < current_state->transitionsCount; i++) {
            if (strchr(current_state->transitions[i].value, c) != NULL) {
                //character recognized
                current_state = current_state->transitions[i].nextState;
                transitioned = true;
                break;
            }
        }

        if(!transitioned) {
            // string not recognized
            return 0;
        }

        c = input[++input_index];
    }

    return current_state;
}

char* getAllCharsExcept(char *exceptions) {

    char* returned_chars = malloc(sizeof(char) * 126); // 126 is the number of valid chars in ASCII table

    int j = 0; // adding index

    int i;
    for(i = 1; i < 127; i++) {
        if (strchr(exceptions,(char) i) != NULL) {
            continue;
        }
        returned_chars[j++] = i;
    }

    returned_chars[j] = '\0';

    return returned_chars;
}
