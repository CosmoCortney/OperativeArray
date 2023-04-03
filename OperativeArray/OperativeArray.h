#pragma once
#include<iostream>    
#include<array> 
#include<vector>
#include<type_traits>
#include <numeric>
#include <array>

template <typename T> class OperativeArray
{
private:
    T* _arr; //using pointers instead so sizes determination during run time is easily possible. couldn't get rid of "illegal zero-sized array error" otherwise
    bool* _ignoreIndices;
    uint64_t _itemCount = 0;

    void AssignArray(const T* arr)
    {
        std::memcpy(_arr, arr, sizeof(T) * _itemCount);
    }

    template <typename U> T DoAnd(const U& a, const U& b)
    {
        return a & b;
    }

    template <typename U> std::enable_if_t<std::is_floating_point<U>::value, OperativeArray<U>> BitwiseAnd(const U& a, const U& b)
    {
        return *this;
    }

    template <typename U> std::enable_if_t<!std::is_floating_point<U>::value, OperativeArray<U>> BitwiseAnd(const U& a, const U& b)
    {
        OperativeArray<U> result;
        for (int i = 0; i < _itemCount; ++i)
            if (!_ignoreIndices[i])
                result._arr[i] = DoAnd(a[i], b[i]);
        return result;
    }

    template <typename U> T DoOr(const U& a, const U& b)
    {
        return a | b;
    }

    template <typename U> std::enable_if_t<std::is_floating_point<U>::value, OperativeArray<U>> BitwiseOr(const U& a, const U& b)
    {
        return *this;
    }

    template <typename U> std::enable_if_t<!std::is_floating_point<U>::value, OperativeArray<U>> BitwiseOr(const U& a, const U& b)
    {
        OperativeArray<U> result;
        for (int i = 0; i < _itemCount; ++i)
            if (!_ignoreIndices[i])
                result._arr[i] = DoOr(a[i], b[i]);
        return result;
    }

    template <typename U> T DoXor(const U& a, const U& b)
    {
        return a ^ b;
    }

    template <typename U> std::enable_if_t<std::is_floating_point<U>::value, OperativeArray<U>> BitwiseXor(const U& a, const U& b)
    {
        return *this;
    }

    template <typename U> std::enable_if_t<!std::is_floating_point<U>::value, OperativeArray<U>> BitwiseXor(const U& a, const U& b)
    {
        OperativeArray<U> result;
        for (int i = 0; i < _itemCount; ++i)
            if (!_ignoreIndices[i])
                result._arr[i] = DoXor(a[i], b[i]);
        return result;
    }

    template <typename U> bool DoCmpAnd(const U& a, const U& b)
    {
        return a & b != b;
    }

    template <typename U> std::enable_if_t<std::is_floating_point<U>::value, bool> AndContains(const U& other)
    {
        return false;
    }

    template <typename U> std::enable_if_t<!std::is_floating_point<U>::value, bool> AndContains(const U& other)
    {
        OperativeArray<U> result;
        for (int i = 0; i < _itemCount; ++i)
            if (!_ignoreIndices[i])
                if(DoCmpAnd(_arr[i], other[i]))
                    return false;
        return true;
    }

    template <typename U> bool DoCmpOr(const U& a, const U& b)
    {
        return !(a | b == 0);
    }

    template <typename U> std::enable_if_t<std::is_floating_point<U>::value, bool> OrContains(const U& other)
    {
        return false;
    }

    template <typename U> std::enable_if_t<!std::is_floating_point<U>::value, bool> OrContains(const U& other)
    {
        OperativeArray<U> result;
        for (int i = 0; i < _itemCount; ++i)
            if (!_ignoreIndices[i])
                if (DoCmpOr(_arr[i], other[i]))
                    return false;
        return true;
    }

    template <typename U> bool IsEqual(const U& other)
    {
        for (int i = 0; i < _itemCount; ++i)
        {
            if (!_ignoreIndices[i])
                if (_arr[i] != other[i])
                    return false;
        }
        return true;
    }

    template <typename U> bool IsNotEqual(const U& other)
    {
        for (int i = 0; i < _itemCount; ++i)
        {
            if (!_ignoreIndices[i])
                if (_arr[i] == other[i])
                    return false;
        }
        return true;
    }

    template <typename U> bool IsGreater(const U& other)
    {
        for (int i = _itemCount - 1; i >= 0; --i)
        {
            if (!_ignoreIndices[i])
                if (_arr[i] <= other[i])
                    return false;
        }
        return true;
    }

    template <typename U> bool IsLower(const U& other)
    {
        for (int i = _itemCount - 1; i >= 0; --i)
        {
            if (!_ignoreIndices[i])
                if (_arr[i] >= other[i])
                    return false;
        }
        return true;
    }

    template <typename U> bool IsGreaterOrEqual(const U& other)
    {
        for (int i = _itemCount - 1; i >= 0; --i)
        {
            if (!_ignoreIndices[i])
                if (_arr[i] < other[i])
                    return false;
        }
        return true;
    }

    template <typename U> bool IsLowerOrEqual(const U& other)
    {
        for (int i = _itemCount - 1; i >= 0; --i)
        {
            if (!_ignoreIndices[i])
                if (_arr[i] > other[i])
                    return false;
        }
        return true;
    }

    void Unassign()
    {
        free(_arr);
        _arr = nullptr;
        free(_ignoreIndices);
        _ignoreIndices = nullptr;
    }

    void Allocate()
    {
        _arr = reinterpret_cast<T*>(calloc(1, sizeof(T) * _itemCount));
        _ignoreIndices = reinterpret_cast<bool*>(calloc(1, sizeof(bool) * _itemCount));
    }

public:
    OperativeArray() {}
    OperativeArray(const T val, const std::vector<int>& ignoreIndices, uint64_t itemCount = 1)
    {
        _itemCount = itemCount;
        Allocate();
        Resize(itemCount);
        *_arr = val;
        SetignoreIndices(ignoreIndices);
    }

    OperativeArray(const T val, uint64_t itemCount = 1)
    {
        _itemCount = itemCount;
        Allocate();
        Resize(itemCount);
        *_arr = val;
    }

    OperativeArray(const T* vals, const std::vector<int>& ignoreIndices, uint64_t itemCount)
    {
        _itemCount = itemCount;
        Allocate();
        AssignArray(vals);
        SetignoreIndices(ignoreIndices);
    }

    OperativeArray(const T* vals, uint64_t itemCount)
    {
        _itemCount = itemCount;
        Allocate();
        AssignArray(vals);
    }

    ~OperativeArray()
    {
        Unassign();
    }

    void SetignoreIndices(const std::vector<int>& ignoreIndices)
    {
        for (int i = 0; i < ignoreIndices.size(); ++i)
        {
            _ignoreIndices[ignoreIndices[i]] = true;
        }
    }

    void Resize(uint64_t newSize)
    {
        _arr = reinterpret_cast<T*>(realloc(_arr, sizeof(T) * newSize));
        _ignoreIndices = reinterpret_cast<bool*>(realloc(_ignoreIndices, sizeof(bool) * newSize));
    }

    uint64_t ItemCount()
    {
        return _itemCount;
    }

    /*void Print()
    {
        for (int i = 0; i < size; ++i)
        {
            std::cout << "ignore: " << _ignoreIndices[i] << std::endl;
            std::cout << "arr: " << _arr[i] << std::endl;
        }
    }*/

    const T& operator[](int index) const
    {
        return _arr[index];
    }

    T& operator[](const int index)
    {
        return _arr[index];
    }

    bool operator==(const T& other)
    {
        return IsEqual(other);
    }

    /// <summary>
    /// Checks arrays for equality.
    /// </summary>
    /// <remarks>
    /// Iterates through the source array and passed array and checks all considered elements for equality. If at least one considered element does not match the other one at the same index the condition is considered false.
    /// </remarks>
    /// <param name="other"> = array of type T, OperativeArray or cast pointer to reference.</param>
    /// <returns>Returns true if all considered elements of each index are equal, otherwise false.</returns>
    bool operator==(const OperativeArray& other)
    {
        return IsEqual(other);
    }

    bool operator!=(const T& other)
    {
        return IsNotEqual(other);
    }

    /// <summary>
    /// Checks arrays for inequality.
    /// </summary>
    /// <remarks>
    /// Iterates through the source array and passed array and checks all considered elements for inequality. If at least one considered element does matches the other one at the same index the condition is considered false.
    /// </remarks>
    /// <param name="other"> = array of type T, OperativeArray or cast pointer to reference.</param>
    /// <returns>Returns true if all considered elements of each index are not equal, otherwise false.</returns>
    bool operator!=(const OperativeArray& other)
    {
        return IsNotEqual(other);
    }

    bool operator<(const T& other)
    {
        return IsLower(other);
    }

    /// <summary>
    /// Checks if left-handed array is smaller.
    /// </summary>
    /// <remarks>
    /// Iterates through the source array and passed array and checks if the considered left-handed element is smaller. If a right-handed and considered element at the given index of significance is greater or equal to the left-handed one the condition is considered false.
    /// </remarks>
    /// <param name="other"> = array of type T, OperativeArray or cast pointer to reference.</param>
    /// <returns>Returns true if all considered elements of each index are lower, otherwise false.</returns>
    bool operator<(const OperativeArray& other)
    {
        return IsLower(other);
    }

    bool operator<=(const T& other)
    {
        return IsLowerOrEqual(other);
    }

    /// <summary>
    /// Checks if left-handed array is smaller or equal.
    /// </summary>
    /// <remarks>
    /// Iterates through the source array and passed array and checks if the considered left-handed element is smaller or equal. If a right-handed and considered element at the given index of significance is greater than the left-handed one the condition is considered false.
    /// </remarks>
    /// <param name="other"> = array of type T, OperativeArray or cast pointer to reference.</param>
    /// <returns>Returns true if all considered elements of each index are lower or equal, otherwise false.</returns>
    bool operator<=(const OperativeArray& other)
    {
        return IsLowerOrEqual(other);
    }

    bool operator>(const T& other)
    {
        return IsGreater(other);
    }

    /// <summary>
    /// Checks if left-handed array is greater.
    /// </summary>
    /// <remarks>
    /// Iterates through the source array and passed array and checks if the considered left-handed element is greater. If a right-handed and considered element at the given index of significance is smaller or equal to the left-handed one the condition is considered false.
    /// </remarks>
    /// <param name="other"> = array of type T, OperativeArray or cast pointer to reference.</param>
    /// <returns>Returns true if all considered elements of each index are greater, otherwise false.</returns>
    bool operator>(const OperativeArray& other)
    {
        return IsGreater(other);
    }

    bool operator>=(const T& other)
    {
        return IsGreaterOrEqual(other);
    }
    
    /// <summary>
    /// Checks if left-handed array is greater or equal.
    /// </summary>
    /// <remarks>
    /// Iterates through the source array and passed array and checks if the considered left-handed element is greater or equal. If a right-handed and considered element at the given index of significance is smaller to the left-handed one the condition is considered false.
    /// </remarks>
    /// <param name="other"> = array of type T, OperativeArray or cast pointer to reference.</param>
    /// <returns>Returns true if all considered elements of each index are greater or equal, otherwise false.</returns>
    bool operator>=(const OperativeArray& other)
    {
        return IsGreaterOrEqual(other);
    }

    /// <summary>
    /// Checks if all true bits of the passed array can be found within the source array.
    /// </summary>
    /// <remarks>
    /// Iterates through the source array and passed array and checks if the considered element of the source array contains all true bits of the one of the passed array. If at least one bit of the passed array at the current considered index is not true the condition is considered false.
    /// </remarks>
    /// <param name="other"> = array of type T, OperativeArray or cast pointer to reference.</param>
    /// <returns>Returns true if all considered elements of the source array contain all true bits passed one, otherwise false.</returns>
    bool BitAND(const T& other)
    {
        return AndContains(other);
    }

    /// <summary>
    /// Checks if at least one true bit of each element of the passed array can be found within the source array.
    /// </summary>
    /// <remarks>
    /// Iterates through the source array and passed array and checks if the considered element of the source array contains at least one true bit of the one of the passed array. If none of the bits of the considered index of the passed array can be found in the source array the condition is considered false.
    /// </remarks>
    /// <param name="other"> = array of type T, OperativeArray or cast pointer to reference.</param>
    /// <returns>Returns true if all considered elements of the source array contain all true bits passed one, otherwise false.</returns>
    bool BitOR(const T& other)
    {
        return OrContains(other);
    }

    OperativeArray<T> operator&(const T& other)
    {
        return BitwiseAnd(other, _arr);
    }

    /// <summary>
    /// ANDs source array with passed arrray.
    /// </summary>
    /// <remarks>
    /// Iterates through the source array and passed array and ANDs each considered element of both arrays.
    /// </remarks>
    /// <param name="other"> = array of type T, OperativeArray or cast pointer to reference.</param>
    /// <returns>Returns a new OperativeArray with all considered elements ANDed.</returns>
    OperativeArray<T> operator&(const OperativeArray& other)
    {
        return BitwiseAnd(other, _arr);
    }

    OperativeArray<T> operator|(const T& other)
    {
        return BitwiseOr(other, _arr);
    }

    /// <summary>
    /// ORs source array with passed arrray.
    /// </summary>
    /// <remarks>
    /// Iterates through the source array and passed array and ORs each considered element of both arrays.
    /// </remarks>
    /// <param name="other"> = array of type T, OperativeArray or cast pointer to reference.</param>
    /// <returns>Returns a new OperativeArray with all considered elements ORed.</returns>
    OperativeArray<T> operator|(const OperativeArray& other)
    {
        return BitwiseOr(other, _arr);
    }

    OperativeArray<T> operator^(const T& other)
    {
        return BitwiseXor(other, _arr);
    }

    /// <summary>
    /// XORs source array with passed arrray.
    /// </summary>
    /// <remarks>
    /// Iterates through the source array and passed array and XORs each considered element of both arrays.
    /// </remarks>
    /// <param name="other"> = array of type T, OperativeArray or cast pointer to reference.</param>
    /// <returns>Returns a new OperativeArray with all considered elements XORed.</returns>
    OperativeArray<T> operator^(const OperativeArray& other)
    {
        return BitwiseXor(other, _arr);
    }
};