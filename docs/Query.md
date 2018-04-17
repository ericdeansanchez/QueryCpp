# Synopsis of Query


| Class | Description |
|---|---|
| [```QueryBase```](https://github.com/ericdeansanchez/QueryCpp/new/master#queryresult) | Abstract Base Class for the query classes. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L129) |
|[```WordQuery```](https://github.com/ericdeansanchez/QueryCpp/new/master#wordquery) | Class derived from ```QueryBase``` that looks for given word. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L145)|
| [```NotQuery```](https://github.com/ericdeansanchez/QueryCpp/new/master#notquery)| Class derived from ```QueryBase``` that represents the set of lines in which its ```Query``` operand does not appear. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L270) |
| [```BinaryQuery```](https://github.com/ericdeansanchez/QueryCpp/new/master#binaryquery) | Abstract base class derived from ```QueryBase``` that represents queries with two ```Query``` operands. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L194)|
|[```OrQuery```](https://github.com/ericdeansanchez/QueryCpp/new/master#orquery) | Class derived from ```BinaryQuery``` that returns the **union** of the line numbers in which its two operands appear. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L246) |
|[```AndQuery```](https://github.com/ericdeansanchez/QueryCpp/new/master#andquery) | Class derived from ```BinaryQuery``` that returns the **intersection** of the line numbers in which its two operands appear. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L218) |

## Classes

### TextQuery

Class that read a given file and build a lookup map. This class has a query operation that takes
a ```std::string``` argument and returns a ```QueryResult``` representing the lines on which that
the particular string appears.

### QueryBase

### WordQuery

### BinaryQuery

### AndQuery

### OrQuery

### NotQuery

### QueryResult

Class that holds the results of a ```query``` operation.

### Query

Interface class that points to an object of a type derived from ```QueryBase```.

### Operations

| Name | Description |
|------|-------------|
| Query q(s) | Binds the ```Query``` ```q``` to a new ```WordQuery```.|
| q1 & q2 | Returns ```Query``` bound to a new ```AndQuery``` object holding q1 and q2 |
| q1 | q2 | Returns ```Query``` bound to a new ```OrQuery``` object holding q1 or q2 |
| ~q1 | Returns a ```Query``` bound to a new ```NotQuery``` object holding q |
