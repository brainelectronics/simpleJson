//
//  jsonHelper.h
//  simpleJson
//
//  Created by Jonas Scharpf on 19.05.20.
//  Copyright (c) 2020 Jonas Scharpf. All rights reserved.
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

enum dataType
{
    aInt = 0,           //< void pointer contains an integer
    aString = 1,        //< void pointer contains a string
    aArray = 2,         //< void pointer contains an array of integers
    aDataContainer = 3, //< void pointer contains a Datacontainer
    aNode = 4,          //< void pointer contains a linked list
    aList = 5,          //< void pointer contains a linked list (alias for aNode)
    
    TYPE_UINT8,
    TYPE_INT8,
    TYPE_UINT16,
    TYPE_INT16,
    TYPE_UINT32,
    TYPE_INT32,
    TYPE_FLOAT,
    TYPE_NODE,
    TYPE_ARRAY,
    TYPE_CHAR,
    TYPE_DATACONTAINER
};

/*
 aUInt8 = 6,         //< void pointer contains an uint8_t
 aUInt16 = 7,        //< void pointer contains an uint16_t
 aUInt32 = 8,        //< void pointer contains an uint32_t
 aInt8 = 9,          //< void pointer contains an int8_t
 aInt16 = 10,        //< void pointer contains an int16_t
 aInt32 = 11,        //< void pointer contains an int32_t
 */

struct DataContainer {
    int number;         //<
    char character;     //<
    char string[50];    //<
    void *ptr;          //<
};

struct Node {
    char keyStr[50];    //< key as string
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
//struct Node* createIntegerNode(const char* keyStr, void* pVal);
struct Node* createIntegerNode(const char* keyStr, void* pVal, dataType type);

void addIntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addU8IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addS8IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
/*
 addIntegerU8
 addIntegerS8
 addIntegerU16
 addIntegerS16
 addIntegerU32
 addIntegerS32
 */

struct Node* createStringNode(const char* keyStr, void* pVal);
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
