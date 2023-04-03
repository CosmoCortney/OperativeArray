# OperativeArray
Create operative arrays to perform comparisions and such.

## Constructors
```c++
OperativeArray<T>(T[] array, std::vector<int> ignoreIndecies, uint64_t itemCount)
OperativeArray<T>(T[] array, uint64_t itemCount)
OperativeArray<T>(T val_index_0, std::vector<int> ignoreIndecies, uint64_t itemCount[optional])
OperativeArray<T>(T val_index_0, uint64_t itemCount[optional])
```

- T: primitive datatype
- int size: size of the array
- T[] array: array for initialization (casting pointer to reference possible)
- T value: value for the first index
- std::vector<int> ignoreIndecies: vector of indecies to ignore (Wildcards)

## Operators
### []
Access OperativeArray's element by index value.

### ==
Checks arrays for equality.

### !=
Checks arrays for inequality.

### <
Checks if left-handed array is smaller.

### <=
Checks if left-handed array is smaller or equal.

### >
Checks if left-handed array is greater.

### >=
Checks if left-handed array is greater or equal.

### &
AND indecies with each other.

### |
ORs indecies with each other.

### ^
XORs indecies with each other.

Right-handed array can be of type T, OperativeArray or casted pointer to reference.

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