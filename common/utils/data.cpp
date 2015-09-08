#include "data.h"

namespace utils
{
    Data::DataMap Data::s_empty_map;
    Data::DataArray Data::s_empty_array;
    Data Data::s_nil(Data::Nil);

    Data *Data::NilValue()
    {
        return &s_nil;
    }

    Data::Data(Data::Type type) :
        _type(type)
    {
        if (_type == String)
        {
            _value.string = nullptr;
        }
        else if (type == Array)
        {
            _value.array = new DataArray();
        }
        else if (type == Map)
        {
            _value.map = new DataMap();
        }
    }
    Data::Data(double value) :
        _type(Number)
    {
        _value.number = value;
    }
    Data::Data(int32_t value) :
        _type(Int32)
    {
        _value.int32 = value;
    }
    Data::Data(uint32_t value) :
        _type(Uint32)
    {
        _value.uint32 = value;
    }
    Data::Data(bool value) :
        _type(Boolean)
    {
        _value.boolean = value;
    }
    Data::Data(const std::string &value) :
        _type(String)
    {
        _value.string = new std::string(value);
    }
    Data::Data(const char *value) :
        _type(String)
    {
        _value.string = new std::string(value);
    }

    Data::~Data()
    {
        if (_type == String && _value.string != nullptr)
        {
            delete _value.string;
        }
        else if (_type == Array)
        {
            delete _value.array;
        }
        else if (_type == Map)
        {
            delete _value.map;
        }
    }

    Data::Type Data::type() const
    {
        return _type;
    }
    const std::string Data::type_name() const
    {
        switch (_type)
        {
            case Number: return "Number";
            case Int32: return "Int32";
            case Uint32: return "UInt32";
            case String: return "String";
            case Boolean: return "Boolean";
            case Array: return "Array";
            case Map: return "Map";
            case Nil: return "Nil";
            default:  return "Unknown";
        }
    }

    void Data::number(double value)
    {
        if (_type == Number)
        {
            _value.number = value;
        }
        else if (_type == Uint32 || _type == Int32)
        {
            _type = Number;
            _value.number = value;
        }
    }
    double Data::number() const
    {
        if (_type == Number)
        {
            return _value.number;
        }
        else if (_type == Int32)
        {
            return static_cast<double>(_value.int32);
        }
        else if (_type == Uint32)
        {
            return static_cast<double>(_value.uint32);
        }
        return 0.0;
    }

    void Data::int32(int32_t value)
    {
        if (_type == Int32)
        {
            _value.int32 = value;
        }
        else if (_type == Uint32 || _type == Number)
        {
            _type = Int32;
            _value.int32 = value;
        }
    }
    int32_t Data::int32() const
    {
        if (_type == Int32)
        {
            return _value.int32;
        }
        else if (_type == Number)
        {
            return static_cast<int32_t>(_value.number);
        }
        else if (_type == Uint32)
        {
            return static_cast<int32_t>(_value.uint32);
        }
        return 0;
    }

    void Data::uint32(uint32_t value)
    {
        if (_type == Uint32)
        {
            _value.uint32 = value;
        }
        else if (_type == Number || _type == Int32)
        {
            _type = Uint32;
            _value.uint32 = value;
        }
    }
    uint32_t Data::uint32() const
    {
        if (_type == Uint32)
        {
            return _value.uint32;
        }
        else if (_type == Number)
        {
            return static_cast<uint32_t>(_value.number);
        }
        else if (_type == Int32)
        {
            return static_cast<uint32_t>(_value.int32);
        }
        return 0u;
    }

    void Data::boolean(bool value)
    {
        if (_type == Boolean)
        {
            _value.boolean = value;
        }
    }
    bool Data::boolean() const
    {
        if (_type == Boolean)
        {
            return _value.boolean;
        }
        return false;
    }

    bool Data::is_nil() const
    {
        return _type == Nil;
    }

    void Data::string(const std::string &value)
    {
        if (_type == String)
        {
            _value.string = new std::string(value);
        }
    }
    void Data::string(const char *value)
    {
        if (_type == String)
        {
            _value.string = new std::string(value);
        }
    }
    std::string Data::string() const
    {
        if (_type == String)
        {
            return std::string(*_value.string);
        }
        return std::string();
    }

    // Map {{{
    Data::DataMap::const_iterator Data::begin_map() const
    {
        return _type == Map ? _value.map->cbegin() : s_empty_map.cend();
    }
    Data::DataMap::iterator Data::begin_map()
    {
        return _type == Map ? _value.map->begin() : s_empty_map.end();
    }

    Data::DataMap::const_iterator Data::end_map() const
    {
        return _type == Map ? _value.map->cend() : s_empty_map.cend();
    }
    Data::DataMap::iterator Data::end_map()
    {
        return _type == Map ? _value.map->end() : s_empty_map.end();
    }

    void Data::at(const std::string &key, double value)
    {
        if (_type == Map)
        {
            (*_value.map)[key] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(const std::string &key, int32_t value)
    {
        if (_type == Map)
        {
            (*_value.map)[key] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(const std::string &key, uint32_t value)
    {
        if (_type == Map)
        {
            (*_value.map)[key] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(const std::string &key, bool value)
    {
        if (_type == Map)
        {
            (*_value.map)[key] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(const std::string &key, const std::string &value)
    {
        if (_type == Map)
        {
            (*_value.map)[key] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(const std::string &key, const char *value)
    {
        if (_type == Map)
        {
            (*_value.map)[key] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(const std::string &key, Data *value)
    {
        if (_type == Map)
        {
            (*_value.map)[key] = std::unique_ptr<Data>(value);
        }
    }

    Data *Data::at(const std::string &key) const
    {
        if (_type == Map)
        {
            auto find = _value.map->find(key);
            if (find != _value.map->end())
            {
                return find->second.get();
            }
        }
        return &s_nil;
    }
    bool Data::has(const std::string &key) const
    {
        if (_type == Map)
        {
            return _value.map->find(key) != _value.map->cend();
        }
        return false;;
    }

    std::size_t Data::size_map() const
    {
        return _type == Map ? _value.map->size() : 0u;
    }
    // }}}

    // Array {{{
    Data::DataArray::const_iterator Data::begin_array() const
    {
        return _type == Array ? _value.array->cbegin() : s_empty_array.cend();
    }
    Data::DataArray::iterator Data::begin_array()
    {
        return _type == Array ? _value.array->begin() : s_empty_array.end();
    }

    Data::DataArray::const_iterator Data::end_array() const
    {
        return _type == Array ? _value.array->cend() : s_empty_array.cend();
    }
    Data::DataArray::iterator Data::end_array()
    {
        return _type == Array ? _value.array->end() : s_empty_array.end();
    }

    void Data::push(double value)
    {
        if (_type == Array)
        {
            (*_value.array).push_back(std::unique_ptr<Data>(new Data(value)));
        }
    }
    void Data::push(int32_t value)
    {
        if (_type == Array)
        {
            (*_value.array).push_back(std::unique_ptr<Data>(new Data(value)));
        }
    }
    void Data::push(uint32_t value)
    {
        if (_type == Array)
        {
            (*_value.array).push_back(std::unique_ptr<Data>(new Data(value)));
        }
    }
    void Data::push(bool value)
    {
        if (_type == Array)
        {
            (*_value.array).push_back(std::unique_ptr<Data>(new Data(value)));
        }
    }
    void Data::push(const std::string &value)
    {
        if (_type == Array)
        {
            (*_value.array).push_back(std::unique_ptr<Data>(new Data(value)));
        }
    }
    void Data::push(const char *value)
    {
        if (_type == Array)
        {
            (*_value.array).push_back(std::unique_ptr<Data>(new Data(value)));
        }
    }
    void Data::push(Data *value)
    {
        if (_type == Array)
        {
            (*_value.array).push_back(std::unique_ptr<Data>(value));
        }
    }

    void Data::at(std::size_t index, double value)
    {
        if (_type == Array)
        {
            (*_value.array)[index] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(std::size_t index, int32_t value)
    {
        if (_type == Array)
        {
            (*_value.array)[index] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(std::size_t index, uint32_t value)
    {
        if (_type == Array)
        {
            (*_value.array)[index] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(std::size_t index, bool value)
    {
        if (_type == Array)
        {
            (*_value.array)[index] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(std::size_t index, const std::string &value)
    {
        if (_type == Array)
        {
            (*_value.array)[index] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(std::size_t index, const char *value)
    {
        if (_type == Array)
        {
            (*_value.array)[index] = std::unique_ptr<Data>(new Data(value));
        }
    }
    void Data::at(std::size_t index, Data *value)
    {
        if (_type == Array)
        {
            (*_value.array)[index] = std::unique_ptr<Data>(value);
        }
    }

    Data *Data::at(std::size_t index) const
    {
        if (_type == Array && index < _value.array->size())
        {
            return (*_value.array)[index].get();
        }
        return &s_nil;
    }

    std::size_t Data::size_array() const
    {
        return _type == Array ? _value.array->size() : 0u;
    }
    // }}}

}
