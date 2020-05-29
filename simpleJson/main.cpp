//
//  main.cpp
//  simpleJson
//
//  Created by brainelectronics on 29.04.20.
//  Copyright (c) 2020 brainelectronics. All rights reserved.
//

#include "main.h"
#include "jsonHelper.h"

int main(int argc, const char * argv[])
{
    // allocate head and tmp node in the heap
    struct Node* head = getNode();
    // struct Node* tmp = getNode();



    // create some basic json

    // add an unsigned char (0...255) Node to the linked list
    uint8_t abcU8 = 129;
    addU8IntegerNode(&head, "uint8Node", &abcU8);

    // add a signed char (-128...127) Node to the linked list
    int8_t abcS8 = -123;
    addS8IntegerNode(&head, "int8Node", &abcS8);

    // add an unsigned int (0...65536) Node to the linked list
    uint16_t abcU16 = 32800;
    addU16IntegerNode(&head, "uint16Node", &abcU16);

    // add a signed int (-32768...32767) Node to the linked list
    int16_t abcS16 = -32760;
    addS16IntegerNode(&head, "int16Node", &abcS16);

    // add an unsigned long int (0...4294967295) Node to the linked list
    uint32_t abcU32 = 2147483648;   // maximum seems int32_t limits
    addU32IntegerNode(&head, "uint32Node", &abcU32);

    // add a signed long int (-2147483648...2147483647) Node to the linked list
    int32_t abcS32 = -2147483648;
    addS32IntegerNode(&head, "int32Node", &abcS32);

    // add a float Node to the linked list
    float abcF = 1.23456;
    addFloatIntegerNode(&head, "floatNode", &abcF);



    // add an integer Node to the linked list
    int abc = 123;
    addIntegerNode(&head, "intNode", &abc);



    // add a string Node to the linked list
    char* pSomeString = (char *) calloc(255, sizeof(char));
    sprintf(pSomeString, "asdf bla bla %d", 1234);
    addStringNode(&head, "strNode", pSomeString);



    // add array Node to the linked list
    int arr2[] = { 11, 22, 33, 44, 55, 66, 77 };
    int numOfElements = sizeof(arr2) / sizeof(arr2[0]);
    addArrayNode(&head, "arrayNode", arr2, numOfElements);



    // add a nested Node list to the linked list
    struct Node* nestedHead = addNestedNode(&head, "nodeNode");

    // append a nested integer Node to the first sub Node (optional)
    int bcd = 234;
    addIntegerNode(&nestedHead, "intNestedNode", &bcd);

    // append a nested string Node to the first sub Node (optional)
    char* pNestedString = (char *) calloc(255, sizeof(char));
    sprintf(pNestedString, "Hello World! @ %d", 9876);
    addStringNode(&nestedHead, "strNestedNode", pNestedString);

    // append a nested array Node to the first sub Node (optional)
    int nestedArray[] = { 123, 456, 789 };
    int nestedArrayElements = sizeof(nestedArray) / sizeof(nestedArray[0]);
    addArrayNode(&nestedHead, "arrayNestedNode", nestedArray, nestedArrayElements);

    // append a nested nested Node to the first sub Node (optional)
    struct Node* nestedNestedHead = addNestedNode(&nestedHead, "nodeNestedNode");
    // append a nested integer Node to the first sub Node (optional)
    int cde = 345;
    addIntegerNode(&nestedNestedHead, "intNestedNestedNode", &cde);

    // append a nested integer Node to the first sub sub Node (optional)
    int bcde = 2345;
    addIntegerNode(&nestedHead, "intNestedNode2", &bcde);



    // add an integer Node to the linked list
    int abcd = 1234;
    addIntegerNode(&head, "intNode2", &abcd);



    // print the content of the linked list (JSON) so far
    printf("Finished JSON creation\n");
    printJson(&head, PRETTY);
    printf("\n");

    /*
    // overwrite an existing integer Node with string
    char* pOverwrittenIntString = (char *) calloc(255, sizeof(char));
    sprintf(pOverwrittenIntString, "This integer Node is now a string Node");
    addStringNode(&head, "intNode2", pOverwrittenIntString);

    // print the content of the updated linked list (JSON)
    printf("After overwriting intNode2 with string value\n");
    printJson(&head, PRETTY);
    printf("\n");



    // search for node in the linked list
    printf("\n\n\nSearching for Node\n");
    tmp = searchForKeyString(&head, "nodeNode");
    if (tmp != NULL)
    {
        printf("SUCCESS\nBEGIN OF \"%s\" CONTENT\n", tmp->keyStr);
        if (tmp->type != aNode) {
            // if found Node is an integer, string or array Node
            printNodeContent(tmp);
        }
        else if (tmp->type == aNode)
        {
            // if it's a nested Node, take head of Node's content
            tmp = (Node*)tmp->ptr;

            printJson(&tmp, PRETTY);
        }
        printf("\nEND OF CONTENT\n");
    }
    else
    {
        printf("No matching node found\n");
    }



    printf("Updating some values of the JSON\n");

    // update an existing Node or create a new Node
    int newAbc = 246;
    addIntegerNode(&head, "intNode", &newAbc);

    // add a string Node to the linked list
    char* pNewSomeString = (char *) calloc(255, sizeof(char));
    sprintf(pNewSomeString, "qwertz bla bla %d", 2345);
    addStringNode(&head, "strNode", pNewSomeString);

    // append a nested array Node to the first sub Node (optional)
    int newArray[] = { 111, 222, 333, 444, 555, 666, 777, 888 };
    int newArrayElements = sizeof(newArray) / sizeof(newArray[0]);
    addArrayNode(&head, "arrayNode", newArray, newArrayElements);

    // search and get nested Node's content head
    // or create new nested Node if None has been found
    struct Node* foundNestedHead = getNestedNodeHead(&head, "strNode", true);
    // add a new string and a new integer Node
    char* pNewNestedSomeString = (char *) calloc(255, sizeof(char));
    sprintf(pNewNestedSomeString, "Darlings bum bum %d.%d.%d", 26, 3, 18);
    addStringNode(&foundNestedHead, "newStrNestedNode", pNewNestedSomeString);
    int newInt = 999;
    addIntegerNode(&head, "intNestedNode", &newInt);

    // print the content of the updated linked list (JSON)
    printf("Finished JSON update\n");
    printJson(&head, PRETTY);
    printf("\n");



    printf("Clearing nested Node's content\n");

    // clear nested Node's content
    struct Node* clearedNestedHead = clearNestedNodeContent(&head, "nodeNode");

    // print the content of the updated linked list (JSON)
    printf("After cleaning\n");
    printJson(&head, PRETTY);
    printf("\n");



    // search and get nested Node's content head
    // or create new nested Node if None has been found
    clearedNestedHead = getNestedNodeHead(&head, "nodeNode", true);
    // add some new string and integer Node to clear nested Node
    // update existing string Node
    char* pStrNestedSomeString = (char *) calloc(255, sizeof(char));
    sprintf(pStrNestedSomeString, "qwertz");
    addStringNode(&clearedNestedHead, "newStrNestedNode", pStrNestedSomeString);
    addIntegerNode(&clearedNestedHead, "intNode", &newAbc);

    // print the content of the updated linked list (JSON)
    printf("After adding one new nested string Node to nestedNode\n");
    printJson(&head, PRETTY);
    printf("\n");
    */



    return 0;
}
