# OperativeArray
Create operative arrays with bonus features to perform comparisons and such.

## Constructors
```c++
OperativeArray<T>(T[] array, std::vector<int> ignoreIndecies, uint64_t itemCount)
OperativeArray<T>(T[] array, uint64_t itemCount)
OperativeArray<T>(T val_index_0, std::vector<int> ignoreIndecies, uint64_t itemCount[optional])
OperativeArray<T>(T val_index_0, uint64_t itemCount[optional])
OperativeArray<T>(std::string& itemString)
```


- T: primitive datatype
- int size: size of the array
- T[] array: array for initialization (casting pointer to reference possible)
- T value: value for the first index
- std::vector<int> ignoreIndecies: vector of indices to ignore (Wildcards)
- The constructor that takes a string reference as a parameter can be used to set an entire OperativeArray expressed the following way: 
  - separate items with commas (,)
  - support for integers of base 10 (decimal), base 16 (hexadecimal), and base 8 (octal). Decimal numbers (float/double) are also supported.
  - Wildcards (indices to be ignored) can be noted with a hashtag (#)
  - Example: 
    - "0x1337, 069, 420, #, -42, -0x2012, #, 0"
    - The values at index 3 and 6 will be ignored on any comparison operations

## Operators
### []
Access OperativeArray's element by index value.

### ==
Checks arrays for equality.

### !=
Checks arrays for inequality.

### <
Checks if the left-handed array is smaller.

### <=
Checks if the left-handed array is smaller or equal.

### >
Checks if the left-handed array is greater.

### >=
Checks if the left-handed array is greater or equal.

### &
AND indices with each other.

### |
ORs indices with each other.

### ^
XORs indices with each other.

The right-handed array can be of type T, OperativeArray, or casted pointer to reference.

## Methods
### void SetIgnoreIndecies(const std::vector<int>& ignoreIndecies)
Set ignoreIndecies manually

### void Resize(uint64_t itemCount)
Set new array size

### uint64_t ItemCount()
Returns the item count

### bool BitAND(const T& other)
Checks if all true bits of the passed array can be found within the source array.

### bool BitOR(const T& other)
 Checks if at least one true bit of each element of the passed array can be found within the source array.

 ## ToDo
 ### Operators
 =, ~, +, -, *, /, %, +=, -=, *=, /=, &=, |= ^=
