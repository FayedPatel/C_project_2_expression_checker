#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int debugMode;
typedef struct MyStruct {
    char *dArray;
    int size;
    int top;

} Stack;

void initializeStack (Stack *stack) {
    stack->size = 2;
    stack->dArray = (char*) malloc ( sizeof (char) * stack->size);
    stack->top = 0;
}

void push (Stack *stack, char symbol) {

    // used for debugging
    int oldsize;
    int count = 0;  // keep track of number of values copied in array
    int i;

    // check if enough space and insert dynamic grow here
    if (stack->top >= stack->size) {
        char *tempArray = (char*) malloc ( sizeof (char) * stack->size + 2);

        // copy elements to bigger temp array
        for (i=0; i < stack->size; i++) {
            count = count + 1;
            tempArray[i] = stack->dArray[i];
        }

        // free up memory to avoid memory leak
        free (stack->dArray);
        // redirect the pointer
        stack->dArray = tempArray;
        // increase size of stack
        oldsize = stack->size;
        stack->size = stack->size + 2;

        if (debugMode) {
            printf("Size of array grew from %d, to %d\n", oldsize, stack->size );
            printf("The number of values copied from old array to new dynamic array is %d\n", count);
        }

    }

    // show the symbol that is being added onto the stack
    if (debugMode) {
        printf("%c has been pushed onto the stack!\n", symbol);
    }

    stack->dArray[stack->top] = symbol;
    stack->top = stack->top + 1;

}

void pop (Stack *stack) {

    // use for storing the pop value to show in debug mode
    char symbol;

    // check if stack is empty so you cant pop off anymore
    if (stack->top ==0) {
        printf ("Stack is empty, error\n");
        return;
    }

    if (debugMode) {
        symbol = stack->dArray[stack->top-1];
        printf("%c has been popped off the stack!\n", symbol);
    }

    stack->top = stack->top - 1;

}

// function to check if stack is empty or not
int isEmpty (Stack *stack) {
    if (stack->top == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

// this function will reset the stack back to normal state. removing all elements.
void reset (Stack *stack) {

    if (debugMode) {
        printf("Resetting stack....emptying stack....changing size back to normal....\n");
    }
    while ( isEmpty(stack) != 1) {
        pop(stack);
    }
    initializeStack(stack);
}

// grab the top symbol of the stack and return it
char getTop (Stack *stack) {

    // check if top is empty, return -1
    if (stack->top == 0) {
        return -1;
    }

    return ( stack->dArray[stack->top - 1]);
}

// takes in a closing symbol as parameter and will return the opposite symbol
char getCorrectSymbol (char symbol) {

    // open symbols
    if (symbol == '{' ) {
        return '}';
    }
    else if (symbol == '(' ) {
        return ')';
    }
    else if (symbol == '[' ) {
        return ']';
    }
    else if (symbol == '<' ) {
        return '>';
    }

        // closing symbol
    else if (symbol == '}' ) {
        return '{';
    }
    else if (symbol == ')' ) {
        return '(';
    }
    else if (symbol == ']' ) {
        return '[';
    }
    else if (symbol == '>') {
        return '<';
    }

    return 0;
}

void displayInput (char input[], int length) {

    int i;
    for (i=0; i < length; i++) {
        printf ("%c", input[i]);
    }
}

int main(int argc, char** argv) {

    //set debug mode to false
    debugMode = 0;
    int i;
    int j;

    // see if the command line was found
    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debugMode = 1;
        }
    }

    // create stack
    Stack stk;

    initializeStack(&stk);

    char userInput[302];
    int length = 0;           // length of user input
    char topSymbol;           // stores symbol at top of stack
    int errorLocation;        // use to store index position of where the error occur
    char errorSymbol;         // store error symbol
    char correctSymbol;       // gets opposite symbol of closing symbol....example } will get {
    char oppositeSymbol;      // same as above ^
    int errorOccured = 0;     // store the i position to line up ^ sign
    int garbageInput = 1;     // if user enters random input...like askdhflksdjfk

    printf("Enter a expression to see if it is balance or not\n");

    while (fgets (userInput, 302, stdin)) {

        length = strlen (userInput);

        // quit program
        if ((userInput[0] == 'q' || userInput[0] == 'Q') && length == 2 ) {
            printf ("You chose to quit the program. Exiting...\n");
            break;
        }

        // loop through entire user input for balance checker
        for (i =0; i < length; i ++ ) {

            // detect opening symbol. push on to the stack
            if (userInput[i] == '(' || userInput[i] == '{' || userInput[i] == '[' || userInput[i] == '<') {
                push(&stk, userInput[i]);
                topSymbol = getTop(&stk);
                garbageInput = 0;
            }

            if (userInput[i] == ')' || userInput[i] == '}' || userInput[i] == ']' || userInput[i] == '>') {

                garbageInput = 0;

                // get opposite symbol to do the comparisons
                oppositeSymbol = getCorrectSymbol(userInput[i]);

                // if stack is empty then automatic error..
                if (isEmpty (&stk) ) {
                    errorLocation = i;
                    errorSymbol = userInput[i];

                    correctSymbol = getCorrectSymbol(errorSymbol);

                    displayInput(userInput, length);

                    // loop through to line up ^ sign
                    for (j =0; j < errorLocation; j++) {
                        printf (" ");
                    }

                    printf("^ missing %c\n", correctSymbol);
                    errorOccured = 1;  // showed that an error occurred so entire expression is unbalance regardless
                    break;
                }


                    // if match with top of stack then we can pop off
                else if (topSymbol == oppositeSymbol) {
                    pop(&stk);
                    topSymbol = getTop(&stk);
                }

                    // expect different symbol message
                else if (topSymbol != oppositeSymbol) {
                    errorLocation = i; // mark ith position to line up ^ sign
                    correctSymbol = getCorrectSymbol(topSymbol);

                    displayInput(userInput, length);

                    // loop through to line up ^ sign
                    for (j =0; j < errorLocation; j++) {
                        printf (" ");
                    }

                    printf("^ expecting %c\n", correctSymbol);
                    errorOccured = 1; // mark this variable to true so we dont have to check anything else after
                    break;
                }
            }

        } // end of for loop

        // if user enters random things and not symbols we loop back
        if (garbageInput) {
            printf("Sorry that was not a valid expression for balance symbol checker. Please try again\n");
            continue;
        }

        // no errors occured
        if (errorOccured == 0) {

            displayInput(userInput, length);

            if (isEmpty(&stk)) {
                printf("Expression is balanced\n");
            }

                // missing a closing symbol
            else {
                topSymbol = getTop(&stk);
                oppositeSymbol = getCorrectSymbol(topSymbol);

                // loop through entire user input to the end
                // length +1 because we want to move one spot ahead of entire list to show missing closing symbol
                for (j = 0; j < length+1; j ++) {
                    printf (" ");
                }

                printf("^ missing %c\n", oppositeSymbol);
            }
        }

        // reset everything
        reset(&stk);
        errorOccured = 0;
        garbageInput = 1;

        printf("Enter another expression or press q/Q to quit\n");


    } // end of while loop

    return 0;

}