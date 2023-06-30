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
    T* _arr{}; //using pointers instead so size determinations are easily possible during run time. can't get rid of "illegal zero-sized array error" otherwise
    bool* _ignoreIndices{};
    uint64_t _itemCount = 0;

    void AssignArray(const T* arr)
    {
        std::memcpy(_arr, arr, sizeof(T) * _itemCount);
    }

    void CopyIgnoreIndices(const bool* ignoreIndices) const
    {
        std::memcpy(_ignoreIndices, ignoreIndices, _itemCount);
    }

    template <typename U> T DoAnd(const U& a, const U& b)
    {
        return a & b;
    }

    template <typename U> std::enable_if_t<std::is_floating_point_v<U>, OperativeArray<U>> BitwiseAnd(const U& a, const U& b)
    {
        return *this;
    }

    template <typename U> std::enable_if_t<!std::is_floating_point_v<U>, OperativeArray<U>> BitwiseAnd(const U& a, const U& b)
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

    template <typename U> std::enable_if_t<std::is_floating_point_v<U>, OperativeArray<U>> BitwiseOr(const U& a, const U& b)
    {
        return *this;
    }

    template <typename U> std::enable_if_t<!std::is_floating_point_v<U>, OperativeArray<U>> BitwiseOr(const U& a, const U& b)
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

    template <typename U> std::enable_if_t<std::is_floating_point_v<U>, OperativeArray<U>> BitwiseXor(const U& a, const U& b)
    {
        return *this;
    }

    template <typename U> std::enable_if_t<!std::is_floating_point_v<U>, OperativeArray<U>> BitwiseXor(const U& a, const U& b)
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

    template <typename U> std::enable_if_t<std::is_floating_point_v<U>, bool> AndContains(const U& other)
    {
        return false;
    }

    template <typename U> std::enable_if_t<!std::is_floating_point_v<U>, bool> AndContains(const U& other)
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

    template <typename U> std::enable_if_t<std::is_floating_point_v<U>, bool> OrContains(const U& other)
    {
        return false;
    }

    template <typename U> std::enable_if_t<!std::is_floating_point_v<U>, bool> OrContains(const U& other)
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
        if (_itemCount > 0)
        {
            free(_arr);
            _arr = nullptr;
            free(_ignoreIndices);
            _ignoreIndices = nullptr;
            _itemCount = 0;
        }
    }

    void Allocate()
    {
        _arr = static_cast<T*>(calloc(1, sizeof(T) * _itemCount));
        _ignoreIndices = static_cast<bool*>(calloc(1, sizeof(bool) * _itemCount));
    }

    void Copy(const OperativeArray& other)
    {
        _itemCount = other._itemCount;
        Allocate();
        AssignArray(other._arr);
        CopyIgnoreIndices(other._ignoreIndices);
    }

public:
    OperativeArray() { }

    OperativeArray(const OperativeArray& other)
    {
        Copy(other);
    }

    OperativeArray(const T val, const std::vector<int>& ignoreIndices, const uint64_t itemCount = 1)
    {
        _itemCount = itemCount;
        Allocate();
        Resize(itemCount);
        *_arr = val;
        SetIgnoreIndices(ignoreIndices);
    }

    OperativeArray(const T val, uint64_t itemCount = 1)
    {
        _itemCount = itemCount;
        Allocate();
        Resize(itemCount);
        *_arr = val;
        const std::vector<int> dummy = {};
        SetIgnoreIndices(dummy);
    }

    OperativeArray(const T* vals, const std::vector<int>& ignoreIndices, const uint64_t itemCount)
    {
        _itemCount = itemCount;
        Allocate();
        AssignArray(vals);
        SetIgnoreIndices(ignoreIndices);
    }

    OperativeArray(const T* vals, const uint64_t itemCount)
    {
        _itemCount = itemCount;
        Allocate();
        AssignArray(vals);
        const std::vector<int> dummy = {};
        SetIgnoreIndices(dummy);
    }

    OperativeArray(const std::string& itemString)
    {
        std::stringstream stream(itemString);
        std::string item;
        std::vector<std::string> itemList;
        std::vector<int> ignoreList;

        while (std::getline(stream, item, ','))
        {
            itemList.push_back(item);
        }

        T* arr = new T[itemList.size()];

        for (int i = 0; i < itemList.size(); ++i)
        {
            if (itemList[i].compare("#") == 0)
            {
                ignoreList.push_back(i);
                arr[i] = (T)0;
            }
            else
            {
                if constexpr (std::is_same_v <T, float>)
                {
                    arr[i] = std::stof(itemList[i]);
                }
                if constexpr (std::is_same_v <T, double>)
                {
                    arr[i] = std::stod(itemList[i]);
                }
                else
                {
                    arr[i] = std::stoll(itemList[i], 0, 0);
                }
            }
        }

        _itemCount = itemList.size();
        Allocate();
        AssignArray(arr);
        SetIgnoreIndices(ignoreList);
        delete[] arr;
    }

    bool IsIgnoredIndex(const uint64_t index)
    {
        return _ignoreIndices[index];
    }

    void operator=(const OperativeArray& other)
    {
        Copy(other);
    }

    ~OperativeArray()
    {
        Unassign();
    }

    void SetIgnoreIndices(const std::vector<int>& ignoreIndices) const
    {
        for (int i = 0; i < ignoreIndices.size(); ++i)
        {
            _ignoreIndices[ignoreIndices[i]] = true;
        }
    }

    void Resize(const uint64_t newSize)
    {
        _arr = static_cast<T*>(realloc(_arr, sizeof(T) * newSize));
        _ignoreIndices = static_cast<bool*>(realloc(_ignoreIndices, sizeof(bool) * newSize));
    }

    uint64_t ItemCount() const
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

    const T& operator[](const int index) const
    {
        return _arr[index];
    }

    T& operator[](const int index)
    {
        return _arr[index];
    }

    bool operator==(const T& other)
    {
        if constexpr (std::is_integral_v<T>)
        {
            for (uint64_t index = 0; index < _itemCount; ++index)
                if (_arr[index] != other)
                    return false;

            return true;
        }

        else
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
        if constexpr (std::is_integral_v<T>)
        {
            for (uint64_t index = 0; index < _itemCount; ++index)
                if (_arr[index] != other)
                    return true;

            return false;
        }
        else
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

    OperativeArray<T> operator-(const T& other)
    {
        OperativeArray result(0, _ignoreIndices, _itemCount);
        T underflow = 0;

        for (int i = result.ItemCount()-1; i >= 0; --i)
        {
            result[i] = _arr[i] - (other[i] + underflow);

            if (result[i] > _arr[i])
                underflow = other[i] - _arr[i];
            else
                underflow = 0;
        }

        return result;
    }

    OperativeArray<T> operator-(const OperativeArray& other)
    {
        OperativeArray result = *this;
        T underflow = 0;

        for (int i = result.ItemCount() - 1; i >= 0; --i)
        {
            result[i] = _arr[i] - (other[i] + underflow);

            if (result[i] > _arr[i])
                underflow = other[i] - _arr[i];
            else
                underflow = 0;
        }

        return result;

        //return *this - other._arr;
    }

    OperativeArray<T> operator+(const T& other)
    {
        OperativeArray result(0, _ignoreIndices, _itemCount);
        T overflow = 0;

        for (int i = result.ItemCount() - 1; i >= 0; --i)
        {
            result[i] = _arr[i] + (other[i] + overflow);

            if (result[i] < _arr[i])
                overflow = abs(other[i] - _arr[i]);
            else
                overflow = 0;
        }

        return result;
    }

    OperativeArray<T> operator+(const OperativeArray& other)
    {
        return *this + other._arr;
    }

    OperativeArray<T> operator*(const T* other)
    {
        OperativeArray<T> result(0, _ignoreIndices, _itemCount);
        T overflow = 0;

        for (int i = result.ItemCount() - 1; i >= 0; --i)
        {
            result[i] = _arr[i] * (other[i] + overflow);

            if (result[i] < _arr[i])
                overflow = std::abs(other[i] - _arr[i]);
            else
                overflow = 0;
        }

        return result;
    }

    /*OperativeArray<T> operator*(const T other)
    {
        OperativeArray<T> result(0, _ignoreIndices, _itemCount);
        T overflow = 0;

        for (int i = result.ItemCount() - 1; i >= 0; --i)
        {
            result[i] = _arr[i] * (other + overflow);

            if (result[i] < _arr[i])
                overflow = std::abs(other - _arr[i]);
            else
                overflow = 0;
        }

        return result;
    }*/

    /*OperativeArray<T> operator*(const OperativeArray& other)
    {
        return *this * other._arr;
    }*/

    const std::type_info* UnderlyingTypeID() const
    {
        return &typeid(T);
    }
};