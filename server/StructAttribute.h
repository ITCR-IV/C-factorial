#ifndef STRUCT_ATTRIBUTE_H
#define STRUCT_ATTRIBUTE_H

#include <string>

/*!
 * \brief This class goes into the vector of the struct definitions map where the vector holds the information of the differente attributes it holds
 * 
 */
class StructAttribute
{
public:
    //! type is a string representation of the type of the attribute, must be one of TypeConstants.h constants
    std::string type;

    //! name is the name by which the attribute is identified
    std::string name;

    //! This attribute holds the value in string representation if there is a default value assigned to the attribute, if there isn't it'll hold an empty string;
    std::string defaultValue;

    /*!
     * \brief Construct a new StructAttribute object
     * 
     * \param type String representation of type which must be one of the ones defined in TypeConstants (Reference example: reference<int>)
     * \param name the name of the attribute
     * \param value the default value of the attribute represented as a string, if omitted it defaults to an empty string
     */
    StructAttribute(std::string type, std::string name, std::string value = "") : type(type), name(name), defaultValue(value) {}
};

#endif