//
//  jsonHelper.cpp
//  simpleJson
//
//  Created by Jonas Scharpf on 19.05.20.
//  Copyright (c) 2020 Jonas Scharpf. All rights reserved.
//

#include "jsonHelper.h"


// Utility function to create a node in memory
struct Node* getNode()
{
    // calloc space for 1 Node and set 0 to all of them
    return ((struct Node*)calloc(1, sizeof(struct Node)));
}

// insert stuff into pointer of Node
void insertToPointer(struct Node* aNode, int key, void* ptr)
{
    if (aNode != NULL)
    {
        aNode->key = key;
        aNode->ptr = ptr;
    }
}

// check whether the linked list is empty
bool isEmpty(struct Node** head_ref)
{
    return *head_ref == NULL;
}

// get length of this linked list
int getLength(struct Node** head_ref)
{
    int length = 0;
    struct Node* current;
    
    // iterate over all nodes until the node is NULL
    for (current = *head_ref; current != NULL; current = current->next)
    {
        length++;
    }
    
    return length;
}

// display the list as JSON
void printJson(struct Node** head_ref, bool pretty, int indentationNumber)
{
    struct Node* ptr = *head_ref;
    
#ifdef DEBUG_PRINT
    // get total length
    // printf("Total length of this list: %d\n", getLength(head_ref));
#endif
    
    // if the list is empty or only 1 element (head itself) is the list
    // then it's an empty JSON
    if ((getLength(head_ref) <= 1) && (ptr != NULL) && (ptr->ptr == NULL)) {
        printf("{}");
        return;
    }
    
    if (pretty) {
        // print pretty indentation
        printf("{\n");
    }
    else {
        printf("{");
    }
    
    // start from the beginning until the next is NULL
    while(ptr != NULL) {
        printJsonChildContent(ptr, pretty, indentationNumber);
        
        // change to next Node
        ptr = ptr->next;
    }
    
    if (pretty) {
        // print pretty indentation
        printf("\n");
        // print n-1 indentation as closing bracket has same indentation as key
        for (int i = 0; i < indentationNumber-1; i++) {
            printf("  ");
        }
        printf("}");
    }
    else {
        printf("}");
    }
}

// display the content of an item in JSON format
void printJsonChildContent(struct Node* pNode, bool pretty, int indentationNumber)
{
    if (pNode != NULL) {
        if (pNode->ptr != NULL) {
            if (pretty) {
                for (int i = 0; i < indentationNumber; i++) {
                    printf("  ");
                }
                // print also space between key and value
                printf("\"%s\": ", pNode->keyStr);
            }
            else {
                printf("\"%s\":", pNode->keyStr);
            }
            
            // print actual content
            printNodeContent(pNode, pretty, indentationNumber);
            
            if (pNode->next != NULL) {
                if (pretty) {
                    // print pretty indentation
                    printf(",\n");
                }
                else {
                    printf(",");
                }
            }
        }
        else {
            // NULL pointer
            // printf("NULL pointer");
        }
    }
}

// Given a reference (pointer to pointer) to the head of a list
// and Node which is appended at the end
void appendNode(struct Node** head_ref, struct Node* new_node)
{
    struct Node* last = *head_ref;
    
    // 1. new_node is going to be the last node, so make next of it as NULL
    new_node->next = NULL;
    
    // 2. if the linked list is empty, then make new_node as head
    if (*head_ref == NULL) {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }
    
    // else traverse till the last node
    while (last->next != NULL) {
        last = last->next;
    }
    
    // change the next of last node to new_node
    last->next = new_node;
    
    // 4. make last node as previous of new_node
    new_node->prev = last;
    
    return;
}

// convert an array with n elements into a linked list
struct Node* arrayToList(int* pArr, int numberOfElements)
{
    struct Node* root = getNode();
    root->key = 0;
    root->type = aInt;
    root->ptr = pArr;
    
    for (int i = 1; i < numberOfElements; i++) {
        struct Node* asdfNode = getNode();
        asdfNode->key = i;
        asdfNode->type = aInt;
        asdfNode->ptr = (pArr + i);
        
        appendNode(&root, asdfNode);
    }
    
    return root;
}

// search for a Node matching the key (first match is returned)
struct Node* searchForKey(struct Node** head_ref, int key, bool deepSearch)
{
    // search for a Node by key only
    return searchForNode(head_ref, key, NULL, deepSearch);
}

// search for a Node matching the key string (first match is returned)
struct Node* searchForKeyString(struct Node** head_ref, const char* keyStr, bool deepSearch)
{
    return searchForNode(head_ref, 0, keyStr, deepSearch);
}

// find first node with given key and/or keyString and return it
struct Node* searchForNode(struct Node** head_ref, int key, const char* keyStr, bool deepSearch)
{
    // start from the first node
    struct Node* current = *head_ref;
    struct Node* tmp = NULL;
    
    // if list is empty
    if (current == NULL) {
        return NULL;
    }
    
    // start from the beginning until the next is NULL
    while (current != NULL) {
#ifdef DEBUG_PRINT
        printf("\n\n");
        printJson(&current);
#endif
        
        // only search by key if it is set
        if (key != 0) {
            if (current->key == key) {
                // return the current node if key integers match
                tmp = current;
#ifdef DEBUG_PRINT
                printf("Match by key\n");
#endif
                break;
            }
        }
        
        // only if a keyStr is given other than default value ""
        if ((keyStr != NULL) && (keyStr[0] != '\0') && (current->keyStr != NULL)) {
            if(strcmp(current->keyStr, keyStr) == 0) {
                // return the current node if key strings match
                tmp = current;
#ifdef DEBUG_PRINT
                printf("\nMatch by key string\n");
#endif
                break;
            }
        }
        
        // if this node contains Node(s) do a deep search
        if ((current->type == aNode) && (deepSearch == true)) {
#ifdef DEBUG_PRINT
            printf("\nEntering nested list");
#endif
            
            // typecast void pointer back to Node
            tmp = (Node*)current->ptr;
            tmp = searchForNode(&tmp, key, keyStr);
            
#ifdef DEBUG_PRINT
            printf("\n\tLeft nested list");
#endif
            
            if (tmp != NULL) {
                // some match was found
#ifdef DEBUG_PRINT
                printf(" with successful match");
#endif
                break;
            }
        }
        
        // move to next
        if (current != NULL) {
            current = current->next;
        }
    }
    
#ifdef DEBUG_PRINT
    printf("\n\nFound this Node by key \"%d\" or keyStr \"%s\"\n", key, keyStr);
    printf("BEGIN OF CONTENT\n");
    printJson(&tmp, PRETTY);
    printf("\nEND OF CONTENT\n");
#endif
    
    // if no matching data found, return the current NULL node
    return tmp;
}

// create a Node marked as integer
//struct Node* createIntegerNode(const char* keyStr, void* pVal)
struct Node* createIntegerNode(const char* keyStr, void* pVal, dataType type)
{
    // get a new Node
    struct Node* intNode = getNode();
    
    // intNode->key = 1234;
    strcpy(intNode->keyStr, keyStr);
    //    intNode->type = aInt;
    intNode->type = type;
    intNode->ptr = pVal;
    
    return intNode;
}

// add new Node or update existing Node in given linked list
void addIntegerNode(struct Node** head_ref, const char* keyStr, void* pVal)
{
    struct Node* pIntNode = NULL;
    
    // find already existing Node with this keyStr in head_ref
    // but do not enter nested Nodes
    pIntNode = searchForKeyString(head_ref, keyStr, false);
    
    if (pIntNode != NULL) {
        // update this Node's value
#ifdef DEBUG_PRINT
        printNodeUpdateInfo(pIntNode, keyStr, pVal, aInt, true);
#endif
        pIntNode->type = aInt;
        pIntNode->ptr = pVal;
    }
    else {
        // create new Node and append it
//        pIntNode = createIntegerNode(keyStr, pVal);
        pIntNode = createIntegerNode(keyStr, pVal, aInt);
        
        appendNode(head_ref, pIntNode);
        
#ifdef DEBUG_PRINT
        printNodeUpdateInfo(pIntNode, keyStr, pVal, aInt, false);
#endif
    }
}

// add new Node or update existing Node in given linked list
void addU8IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal)
{
    struct Node* pIntNode = NULL;
    
    // find already existing Node with this keyStr in head_ref
    // but do not enter nested Nodes
    pIntNode = searchForKeyString(head_ref, keyStr, false);
    
    if (pIntNode != NULL) {
        // update this Node's value
#ifdef DEBUG_PRINT
        printNodeUpdateInfo(pIntNode, keyStr, pVal, aInt, true);
#endif
        pIntNode->type = TYPE_UINT8;
        pIntNode->ptr = pVal;
    }
    else {
        // create new Node and append it
        // pIntNode = createIntegerNode(keyStr, pVal);
        pIntNode = createIntegerNode(keyStr, pVal, TYPE_UINT8);
        appendNode(head_ref, pIntNode);
        
#ifdef DEBUG_PRINT
        printNodeUpdateInfo(pIntNode, keyStr, pVal, aInt, false);
#endif
    }
}

// add new Node or update existing Node in given linked list
void addS8IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal)
{
    struct Node* pIntNode = NULL;
    
    // find already existing Node with this keyStr in head_ref
    // but do not enter nested Nodes
    pIntNode = searchForKeyString(head_ref, keyStr, false);
    
    if (pIntNode != NULL) {
        // update this Node's value
#ifdef DEBUG_PRINT
        printNodeUpdateInfo(pIntNode, keyStr, pVal, aInt, true);
#endif
        pIntNode->type = TYPE_INT8;
        pIntNode->ptr = pVal;
    }
    else {
        // create new Node and append it
        // pIntNode = createIntegerNode(keyStr, pVal);
        pIntNode = createIntegerNode(keyStr, pVal, TYPE_INT8);
        appendNode(head_ref, pIntNode);
        
#ifdef DEBUG_PRINT
        printNodeUpdateInfo(pIntNode, keyStr, pVal, aInt, false);
#endif
    }
}

// create a Node marked as string
struct Node* createStringNode(const char* keyStr, void* pVal)
{
    // get a new Node
    struct Node* stringNode = getNode();
    
    // stringNode->key = 1234;
    strcpy(stringNode->keyStr, keyStr);
    stringNode->type = aString;
    stringNode->ptr = pVal;
    
    return stringNode;
}

// add new Node or update existing Node in given linked list
void addStringNode(struct Node** head_ref, const char* keyStr, void* pVal)
{
    struct Node* pStrNode = NULL;
    
    // find already existing Node with this keyStr in head_ref
    // but do not enter nested Nodes
    pStrNode = searchForKeyString(head_ref, keyStr, false);
    
    if (pStrNode != NULL) {
        // update this Node's value
#ifdef DEBUG_PRINT
        printNodeUpdateInfo(pStrNode, keyStr, pVal, aString, true);
#endif
        // update type of Node as well as the pointer's content
        pStrNode->type = aString;
        pStrNode->ptr = pVal;
    }
    else {
        // create new Node and append it
        pStrNode = createStringNode(keyStr, pVal);
        appendNode(head_ref, pStrNode);
        
#ifdef DEBUG_PRINT
        printNodeUpdateInfo(pStrNode, keyStr, pVal, aString, false);
#endif
    }
}

// create a Node marked as array (only integer arrays are possible right now)
struct Node* createArrayNode(const char* keyStr, int* pVal, int numberOfElements)
{
    // get a new Node
    struct Node* arrayNode = getNode();
    
    // arrayNode->key = 1234;
    strcpy(arrayNode->keyStr, keyStr);
    arrayNode->type = aArray;
    arrayNode->ptr = arrayToList(pVal, numberOfElements);
    
    return arrayNode;
}

// add new Node or update existing Node in given linked list
void addArrayNode(struct Node** head_ref, const char* keyStr, void* pVal, int numberOfElements)
{
    struct Node* pArrayNode = NULL;
    
    // find already existing Node with this keyStr in head_ref
    // but do not enter nested Nodes
    pArrayNode = searchForKeyString(head_ref, keyStr, false);
    
    if (pArrayNode != NULL) {
        // update this Node's value
#ifdef DEBUG_PRINT
        printNodeUpdateInfo(pArrayNode, keyStr, arrayToList((int*)pVal, numberOfElements), aArray, true);
#endif
        // update type of Node as well as the pointer's content
        pArrayNode->type = aArray;
        pArrayNode->ptr = arrayToList((int*)pVal, numberOfElements);
    }
    else {
        // create new Node and append it
        pArrayNode = createArrayNode(keyStr, (int*)pVal, numberOfElements);
        appendNode(head_ref, pArrayNode);
        
#ifdef DEBUG_PRINT
        printNodeUpdateInfo(pArrayNode, keyStr, arrayToList((int*)pVal, numberOfElements), aArray, false);
#endif
    }
}

// create a Node marked as Node
struct Node* createNestedNode(const char* keyStr)
{
    // get a new Node (for the linked list)
    struct Node* nestedNode = getNode();
    
    // get a new Node (containing the nested stuff)
    struct Node* nestedNodeContent = getNode();
    
    // nestedNode->key = 1234;
    strcpy(nestedNode->keyStr, keyStr);
    nestedNode->type = aNode;
    nestedNode->ptr = nestedNodeContent;
    
    return nestedNode;
}

// add new Node or update existing Node Node in given linked list
struct Node* addNestedNode(struct Node** head_ref, const char* keyStr)
{
    struct Node* pNodeNode = NULL;
    
    pNodeNode = createNestedNode(keyStr);
    appendNode(head_ref, pNodeNode);
    
    // return head of nested Node
    return (Node*)pNodeNode->ptr;
}

// get content of nested Node
/*
 {
 "nestedNode": {
 "a": 1,
 "b": "asdf"
 },
 "c": 2
 }
 returns this if searched for "nestedNode"
 {
 "a": 1,
 "b": "asdf"
 }
 */
struct Node* getNestedNodeHead(struct Node** head_ref, const char* keyStr, bool createOnError)
{
    // find already existing Node with this keyStr in head_ref
    // but do not enter nested Nodes
    struct Node* nestedHead = NULL;
    nestedHead = searchForKeyString(head_ref, keyStr, false);
    
    if (nestedHead != NULL) {
        // if some nested Node was found
        if (nestedHead->type == aNode) {
            // if the found thing is a Node
            // return content of nested head
            nestedHead = (Node*)nestedHead->ptr;
        }
        else
        {
#ifdef DEBUG_PRINT
            printf("Found match is not a nested Node, but of type %d\n", nestedHead->type);
#endif
            // found Node is not a nested Node (int, str, array Node or similar)
            nestedHead = NULL;
        }
    }
    else
    {
        // no nested Node with this keyStr has been found
        nestedHead = NULL;
    }
    
    if ((createOnError == true) && (nestedHead == NULL)) {
#ifdef DEBUG_PRINT
        printf("Failed to find nested Node \"%s\", created new one\n", keyStr);
#endif
        nestedHead = addNestedNode(head_ref, keyStr);
    }
    
    // return content of nested head
    return nestedHead;
}

// clear content of nested Node
/*
 "nestedNode": {
 "a": 1,
 "b": "asdf"
 }
 becomes
 "nestedNode": {}
 */
struct Node* clearNestedNodeContent(struct Node** head_ref, const char* keyStr)
{
    // find already existing Node with this keyStr in head_ref
    // but do not enter nested Nodes
    struct Node* nestedHead = searchForKeyString(head_ref, keyStr, false);
    
    if (nestedHead != NULL) {
        // if some nested Node was found
        if (nestedHead->type == aNode) {
            // if the found thing is a Node
            
            // free old stuff of nested Node's content
            if (nestedHead->ptr != NULL) {
                free(nestedHead->ptr);
                
                // clear content of nested head
                nestedHead->ptr = getNode();
                
                nestedHead = (Node*)nestedHead->ptr;
            }
        }
        else
        {
            nestedHead = NULL;
        }
    }
    else
    {
        nestedHead = NULL;
    }
    
    // return content of nested head
    return nestedHead;
}

void printNodeUpdateInfo(struct Node* pNode, const char* keyStr, void* pVal, enum dataType thisType, bool isUpdate)
{
    if (isUpdate == true) {
        printf("Node \"%s\" already exists in this linked list, update value from ", keyStr);
        printNodeContent(pNode);
        printf(" to ");
    }
    else
    {
        printf("Creating new Node \"%s\" with value ", keyStr);
    }
    
    struct Node* tmp = getNode();
    tmp->ptr = pVal;
    tmp->type =thisType;
    
    //    printf("START 2\n");
    //    printf("val: %d\n", *(int*)tmp->ptr);
    //    printf("type: %d\n", tmp->type);
    //    printf("END 2\n");
    printNodeContent(tmp);
    
    free(tmp);
    
    printf("\n");
}

void printNodeContent(struct Node* pNode, bool pretty, int indentationNumber)
{
    // local copy of number of indentations
    int thisIndentation = indentationNumber;
    
    // switch based on type of pointer content
    switch (pNode->type) {
        case aInt:
        {
            // typecast void pointer back to integer
            printf("%d", *(int*)pNode->ptr);
            break;
        }
        case aString:
        {
            printf("\"%s\"", (char*)pNode->ptr);
            break;
        }
        case aArray:
        {
            // typecast void pointer back to Node
            struct Node* tmpNode = (Node*)pNode->ptr;
            
            printf("[");
            
            // start from the beginning until the next is NULL
            while(tmpNode != NULL)
            {
                if (tmpNode->ptr != NULL)
                {
                    // typecast void pointer back to integer
                    printf("%d", *(int*)tmpNode->ptr);
                }
                
                // change to next Node
                tmpNode = tmpNode->next;
                
                // if there is a next item, put a commata
                if (tmpNode != NULL)
                {
                    printf(", ");
                }
            }
            printf("]");
            break;
        }
        case aDataContainer:
        {
            // typecast void pointer back to DataContainer
            printf("%d", ((DataContainer*)pNode->ptr)->number);
            break;
        }
        case aNode:
        case aList:
        {
            // typecast void pointer back to Node
            struct Node* tmpNode = (Node*)pNode->ptr;
            
            // increase indentation by one for recursive call
            thisIndentation++;
            printJson(&tmpNode, pretty, thisIndentation);
            break;
        }
        case TYPE_UINT8:
        {
            // typecast void pointer back to integer
            printf("%d", *(uint8_t*)pNode->ptr);
            break;
        }
        case TYPE_INT8:
        {
            // typecast void pointer back to integer
            printf("%d", *(int8_t*)pNode->ptr);
            break;
        }
        default:
        {
            printf("No matching case");
            break;
        }
    }
}


void createSomeJson(struct Node **head_ref)
{
    // add an integer Node to the linked list
    int abc = 123;
    addIntegerNode(head_ref, "intNode", &abc);
    
    
    
    // add a string Node to the linked list
    char* pSomeString = (char *) calloc(255, sizeof(char));
    sprintf(pSomeString, "asdf bla bla %d", 1234);
    addStringNode(head_ref, "strNode", pSomeString);
    
    
    
    // add array Node to the linked list
    int arr2[] = { 11, 22, 33, 44, 55, 66, 77 };
    int numOfElements = sizeof(arr2) / sizeof(arr2[0]);
    addArrayNode(head_ref, "arrayNode", arr2, numOfElements);
    
    
    
    // add a nested Node list to the linked list
    struct Node* nestedHead = addNestedNode(head_ref, "nodeNode");
    
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
    addIntegerNode(head_ref, "intNode2", &abcd);
}
