#pragma once
#include<string>
#include <iostream>
#include <fstream>

constexpr char constantFile[] = "Constants.txt";

template<class type>
class Constants
{
public:
	Constants(std::string name, type defaultValue);
	type getValue();
private:
	std::string _name;
	type _default;
    bool _read = false;
    type _value;
};

template<class type>
inline Constants<type>::Constants(std::string name, type defaultValue)
{
	_name = name;
	_default = defaultValue;
}

template<class type>
inline type Constants<type>::getValue()
{
    if (_read)
    {
        return _value;
    }
    _read = true;
    _value = _default;
    std::string line;

    std::ifstream input_file(constantFile);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '" << constantFile << "'" << std::endl;
        return _value;
    }

    while (getline(input_file, line)) {
        if (line.rfind(_name, 0) == 0)
        {
            auto strValue = std::stringstream(line.substr(_name.size()+1));
            type value;
            strValue >> value;
            _value = value;
            return value;
        }
    }

	return _value;
}

namespace ConstantsValues
{
    extern Constants<float> DominationMultiplier;
    extern Constants<int> DominationMinimum;
    extern Constants<size_t> ImageSize;
    constexpr size_t boardSize = 8;
    extern Constants<size_t> CameraInput;
    extern Constants<size_t> port;
    extern Constants<size_t> Verifies;
    extern Constants<size_t> TileDif;
}