//
//  jsonHelper.h
//  simpleJson
//
//  Created by brainelectronics on 19.05.20.
//  Copyright (c) 2020 brainelectronics. All rights reserved.
//

#ifndef __simpleJson__jsonHelper__
#define __simpleJson__jsonHelper__

#include <iostream>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

// uncomment to see further debug info print
//#define DEBUG_PRINT

// if PRETTY is used, the JSON will be printed with two spaces indentation
#define PRETTY true

// define length of key string
#define KEYSTR_LENGTH   50

enum dataType
{
    aInt = 0,           //< void pointer contains an integer
    aString = 1,        //< void pointer contains a string
    aArray = 2,         //< void pointer contains an array of integers
    aDataContainer = 3, //< void pointer contains a Datacontainer
    aNode = 4,          //< void pointer contains a linked list
    aList = 5,          //< void pointer contains a linked list (alias for aNode)

    TYPE_UINT8,         //< void pointer contains an unsigned char (0...255)
    TYPE_INT8,          //< void pointer contains a signed char (-128...127)
    TYPE_UINT16,        //< void pointer contains an unsigned int (0...65535)
    TYPE_INT16,         //< void pointer contains a signed int (-32768...32767 )
    TYPE_UINT32,        //< void pointer contains an unsigned long int (0...4294967295)
    TYPE_INT32,         //< void pointer contains a signed long int (-2147483648...2147483647)
    TYPE_FLOAT,         //< void pointer contains a float
    TYPE_NODE,          //< void pointer contains a linked list
    TYPE_ARRAY,         //< void pointer contains an array of integers
    TYPE_CHAR,          //< void pointer contains a char array/string
    TYPE_DATACONTAINER  //< void pointer contains a Datacontainer
};

struct DataContainer {
    int number;         //<
    char character;     //<
    char string[50];    //<
    void *ptr;          //<
};

struct Node {
    char keyStr[KEYSTR_LENGTH];    //< key as string
    int key;            //< unique key or similar, optional for JSON
    int type;           //< type of Node's ptr content, required for typecase

    void *ptr;          //< pointer to something, see enum dataType

    struct Node* next;  //< pointer to next Node in linked list
    struct Node* prev;  //< pointer to previous Node in linked list
};

// Node creation functions
struct Node* getNode();

// general insertion functions
void insertToPointer(struct Node* aNode, int key, void* ptr);
void appendNode(struct Node** head_ref, struct Node* new_node);

// general conversion functions
struct Node* arrayToList(int* pArr, int numberOfElements);

// general check and info functions
bool isEmpty(struct Node** head_ref);
int getLength(struct Node** head_ref);

// general debug print functions
void printNodeUpdateInfo(struct Node* pNode, const char* keyStr, void *pVal, enum dataType thisType, bool isUpdate);
void printNodeContent(struct Node* pNode, bool pretty = false, int indentationNumber = 0);

// JSON print functions
void printJson(struct Node** head_ref, bool pretty = false, int indentationNumber = 1);
void printJsonChildContent(struct Node* pNode, bool pretty = false, int indentationNumber = 1);

// searching functions
struct Node* searchForKey(struct Node** head_ref, int key = 0, bool deepSearch=true);
struct Node* searchForKeyString(struct Node** head_ref, const char* keyStr="", bool deepSearch=true);
struct Node* searchForNode(struct Node** head_ref, int key = 0, const char* keyStr="", bool deepSearch=true);

// insertion functions
struct Node* createGeneralNode(const char* keyStr, void* pVal, dataType type);
void addGeneralNode(struct Node** head_ref, const char* keyStr, void* pVal, dataType type);
void addU8IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addS8IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addU16IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addS16IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addU32IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addS32IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addIntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addFloatIntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addStringNode(struct Node** head_ref, const char* keyStr, void* pVal);

struct Node* createArrayNode(const char* keyStr, int* pVal, int numberOfElements);
void addArrayNode(struct Node** head_ref, const char* keyStr, void* pVal, int numberOfElements);
struct Node* createNestedNode(const char* keyStr);
struct Node* addNestedNode(struct Node** head_ref, const char* keyStr);

// nested Node functions
struct Node* getNestedNodeHead(struct Node** head_ref, const char* keyStr, bool createOnError = false);
struct Node* clearNestedNodeContent(struct Node** head_ref, const char* keyStr);

void createSomeJson(struct Node **head_ref);

#endif /* defined(__simpleJson__jsonHelper__) */
