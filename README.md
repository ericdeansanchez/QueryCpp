# QueryCpp

While honing my cpp skills, I came across some boiler plate within [C++ Primer (5th Edition)](https://www.amazon.com/Primer-5th-Stanley-B-Lippman/dp/0321714113/ref=sr_1_1?ie=UTF8&qid=1523940885&sr=8-1&keywords=c%2B%2B+primer) that wasn't entirely _fleshed out_. There were also a couple "bonus" problems this text proposes one solves; the solutions are contained here. 

The code takes a file as input, performs a query for a given word or words, returns the number of times the queried-for string
appears within the file and can display the search results (if any).

| Class | Description |
|---|---|
| ```QueryBase``` | Abstract Base Class for the query classes. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L129) |
|```WordQuery``` | Class derived from ```QueryBase``` that looks for given word. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L145)|
| ```NotQuery```| Class derived from ```QueryBase``` that represents the set of lines in which its ```Query``` operand does not appear. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L270) |
| ```BinaryQuery```| Abstract base class derived from ```QueryBase``` that represents queries with two ```Query``` operands. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L194)|
| ```OrQuery```| Class derived from ```BinaryQuery``` that returns the **union** of the line numbers in which its two operands appear. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L246) |
|```AndQuery```| Class derived from ```BinaryQuery``` that returns the **intersection** of the line numbers in which its two operands appear. [source](https://github.com/ericdeansanchez/QueryCpp/blob/dd77e519c986abebc67a100c64d2898a12d35476/Query.cpp#L218) |
