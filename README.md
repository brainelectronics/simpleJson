# Linked list based JSON encoder

This is a very simple JSON encoder written in C.

The code itself is not very complex and theirby easy to understand. It has been intentionally kept simple with a very open MIT license, see the [LICENSE.md](LICENSE.md), so that it can be taken apart and reused in other applications.

## Common API
### List operations

Check whether the linked list contains elements
```C
bool isEmpty(struct Node** head_ref);
```

Get length of linked list. Nested lists are counted as +1
```C
int getLength(struct Node** head_ref);
```

### Print functions

Print content of a Node.
```C
void printNodeContent(struct Node* pNode, bool pretty = false, int indentationNumber = 0);
```

Print complete linked list in JSON format. Use the `pretty` parameter to print the JSON in styled format (indentations).
```C
void printJson(struct Node** head_ref, bool pretty = false, int indentationNumber = 1);
void printJsonChildContent(struct Node* pNode, bool pretty = false, int indentationNumber = 1);
```

### Search functions

Search for an element in the linked list/JSON by key (number) or keyStr (name of element).
```C
struct Node* searchForKey(struct Node** head_ref, int key = 0, bool deepSearch=true);
struct Node* searchForKeyString(struct Node** head_ref, const char* keyStr="", bool deepSearch=true);
struct Node* searchForNode(struct Node** head_ref, int key = 0, const char* keyStr="", bool deepSearch=true);
```

### Add elements

The maximum number of characters for the key is limited to 50. This can be changed in the [jsonHelper.h](src/jsonHelper.h) file.

In case a element of this name already exists, the value will be overwritten. Thereby it is always safe to call add<Something>Node in case you are not sure whether this key already exists. If it does not yet exist, it will be created of course.

#### General

As only a void pointer is stored in the Node, a seperate element of the Node struct is set according to the type which is stored.

All elements like numbers, floats, strings, arrays or nested Nodes are represented by a data type, which is defined in the `dataType` enum.

The functions are just wrappers around the `addGeneralNode` function.

```C
struct Node* createGeneralNode(const char* keyStr, void* pVal, dataType type);
void addGeneralNode(struct Node** head_ref, const char* keyStr, void* pVal, dataType type);
```

#### Integers

```C
void addU8IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addS8IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addU16IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addS16IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addU32IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addS32IntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
void addIntegerNode(struct Node** head_ref, const char* keyStr, void* pVal);
```

Example:
```C
struct Node* head = getNode();

// add an unsigned int (0...65536) Node to the linked list
uint16_t abcU16 = 32800;
addU16IntegerNode(&head, "someKeyForThisUInt16", &abcU16);

// print the JSON with pretty indentation
printJson(&head, true);
```

```JSON
{
  "someKeyForThisUInt16": 32800,
}
```

#### Floats
```C
void addFloatNode(struct Node** head_ref, const char* keyStr, void* pVal);
```

Example:
```C
struct Node* head = getNode();

// add a float Node to the linked list
float abcF = 1.23456890;
addFloatNode(&head, "floatNode", &abcF);

// floats will be printed with a precision of 5 decimals
// print the JSON with pretty indentation
printJson(&head, true);
```

```JSON
{
  "floatNode": 1.23456,
}
```

#### Strings
```C
void addStringNode(struct Node** head_ref, const char* keyStr, void* pVal);
```

Example:
```C
struct Node* head = getNode();

// add a string Node to the linked list
char* pSomeString = (char *) calloc(255, sizeof(char));
sprintf(pSomeString, "asdf bla bla %d", 1234);
addStringNode(&head, "keyOfThisNode", pSomeString);

// print the JSON with pretty indentation
printJson(&head, true);
```

```JSON
{
  "keyOfThisNode": "asdf bla bla 1234",
}
```

#### Bools
```C
void addBoolNode(struct Node** head_ref, const char* keyStr, void* pVal);
```

Example:
```C
struct Node* head = getNode();

// add a boolean Node to the linked list
bool abcB = false;
addBoolNode(&head, "exampleOfBool", &abcB);

// print the JSON with pretty indentation
printJson(&head, true);
```

```JSON
{
  "exampleOfBool": false,
}
```

#### Array of integers
```C
struct Node* createArrayNode(const char* keyStr, int* pVal, int numberOfElements);
void addArrayNode(struct Node** head_ref, const char* keyStr, void* pVal, int numberOfElements);
```

Example:
```C
struct Node* head = getNode();

// add array Node to the linked list
int arr2[] = { 11, 22, 33, 44, 55, 66, 77 };
int numOfElements = sizeof(arr2) / sizeof(arr2[0]);
addArrayNode(&head, "arrayNode", arr2, numOfElements);

// print the JSON with pretty indentation
printJson(&head, true);
```

```JSON
{
  "arrayNode": [11, 22, 33, 44, 55, 66, 77],
}
```

#### Nested elements
```C
struct Node* createNestedNode(const char* keyStr);
struct Node* addNestedNode(struct Node** head_ref, const char* keyStr);

struct Node* getNestedNodeHead(struct Node** head_ref, const char* keyStr, bool createOnError = false);
struct Node* clearNestedNodeContent(struct Node** head_ref, const char* keyStr);
```

Example:
```C
struct Node* head = getNode();

// add a nested Node list to the linked list
struct Node* nestedHead = addNestedNode(&head, "nodeNode");

// append a nested integer Node to the first sub Node (optional)
int bcd = 234;
addIntegerNode(&nestedHead, "intNestedNode", &bcd);

// append a nested string Node to the first sub Node (optional)
char* pNestedString = (char *) calloc(255, sizeof(char));
sprintf(pNestedString, "Hello World! @ %d", 9876);
addStringNode(&nestedHead, "strNestedNode", pNestedString);

// append a nested nested Node to the first sub Node (optional)
struct Node* nestedNestedHead = addNestedNode(&nestedHead, "nodeNestedNode");
// append a nested integer Node to the first sub Node (optional)
float cde = 3.141592;
addFloatNode(&nestedNestedHead, "floatNestedNestedNode", &cde);

// print the JSON with pretty indentation
printJson(&head, true);
```

```JSON
{
  "nodeNode": {
    "intNestedNode": 234,
    "strNestedNode": "Hello World! @ 9876",
    "nodeNestedNode": {
      "floatNestedNestedNode": 3.15159
    }
  }
}
```

## Usecase

Use this library together with the [SerialDebugMonitor](https://github.com/brainelectronics/SerialDebugMonitor) and a microcontroller, to view the data in an easy and human readable way.

## Authors

* **brainelectronics** - *Initial Work* - [brainelectronics](https://github.com/brainelectronics/simpleJson)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
