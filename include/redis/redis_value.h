#pragma once

#include <boost/variant.hpp>
#include <string>
#include <vector>


class RedisValue {
public:
    struct ErrorTag {};

     RedisValue();
     RedisValue(int i);
     RedisValue(const char *s);
     RedisValue(const std::string &s);
     RedisValue(const std::vector<char> &buf);
     RedisValue(const std::vector<char> &buf, struct ErrorTag &);
     RedisValue(const std::vector<RedisValue> &array);

    // Return the value as a std::string if 
    // type is a byte string; otherwise returns an empty std::string.
     std::string toString() const;

    // Return the value as a std::vector<char> if 
    // type is a byte string; otherwise returns an empty std::vector<char>.
     std::vector<char> toByteArray() const;
    
    // Return the value as a std::vector<RedisValue> if 
    // type is an int; otherwise returns 0.
     int toInt() const;
    
    // Return the value as an array if type is an array;
    // otherwise returns an empty array.
     std::vector<RedisValue> toArray() const;

    // Return the string representation of the value. Use
    // for dump content of the value.
     std::string inspect() const;

    // Return true if value not a error
     bool isOk() const;
    // Return true if value is a error
     bool isError() const;


    // Return true if this is a null.
     bool isNull() const;
    // Return true if type is an int
     bool isInt() const;
    // Return true if type is an array
     bool isArray() const;
    // Return true if type is a string/byte array. Alias for isString();
     bool isByteArray() const;
    // Return true if type is a string/byte array. Alias for isByteArray().
     bool isString() const;

     bool operator == (const RedisValue &rhs) const;
     bool operator != (const RedisValue &rhs) const;

protected:
    template<typename T>
     T castTo() const;

    template<typename T>
     bool typeEq() const;

private:
    struct NullTag {
        inline bool operator == (const NullTag &) const {
            return true;
        }
    };


    boost::variant<NullTag, int, std::vector<char>, std::vector<RedisValue> > value;
    bool error;
};


template<typename T>
T RedisValue::castTo() const
{
    if( value.type() == typeid(T) )
        return boost::get<T>(value);
    else
        return T();
}

template<typename T>
bool RedisValue::typeEq() const
{
    if( value.type() == typeid(T) )
        return true;
    else
        return false;
}