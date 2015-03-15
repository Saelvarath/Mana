#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <sstream>
#include <vector>

/**
* A collection of useful utility methods.
* @author R.N. Robson
*/
class Util
{
public:
	/**
	* Convert an integer to a string.
	* @param n Integer to convert.
	* @return A string representation of the parameter.
	*/
	static std::string toString(int n);

	/**
	* Convert an float to a string.
	* @param n Float to convert.
	* @return A string representation of the parameter.
	*/
	static std::string toString(float n);

	/**
	* Convert a string to an int.  Throw a CorruptException if the conversion fails.
	* @param str The string to convert.
	* @return The integer equivalent to the string.
	*/
	static int toInt(std::string &str);

	/**
	* Convert a string to a float.  Throw a CorruptException if the conversion fails.
	* @param str The string to convert.
	* @return The float equivalent to the string.
	*/
	static float toFloat(std::string &str);

	/**
	* This takes a string consisting of a series of float values separated by
	* white space.  It converts each of these values to a float and returns them
	* all as a vector of floats.  You must pass the vector to fill with the
	* results and this will append the results onto whatever might already
	* be in the vector.
	* @param str The string containing float values
	* @param values The vector onto which to append the results
	*/
	static void toFloatVector(std::string &str, std::vector<float> &values);

	/**
	* This takes a string consisting of a series of int values separated by
	* white space.  It converts each of these values to an int and returns them
	* all as a vector of ints.  You must pass the vector to fill with the
	* results and this will append the results onto whatever might already
	* be in the vector.
	* @param str The string containing int values
	* @param values The vector onto which to append the results
	*/
	static void toIntVector(std::string &str, std::vector<int> &values);

	/**
	* Format a message by replacing all occurences of "%1" in the message string
	* by the value param1.  The original message string is left unaltered.
	*/
	static std::string formatMessage(std::string msg, std::string param1);

	/**
	* Format a message by replacing all occurences of "%1" in the message string
	* by the value param1 and "%2" by param2.  The original message string is left unaltered.
	*/
	static std::string formatMessage(std::string msg, std::string param1, std::string param2);

	/**
	* Find the file extension from a file name and return the extension without
	* the dot which separates the extension from the rest of the filename.
	* @param fname The file name from which to extract the extension
	* @return the file extension of an empty string if there is no extension
	*/
	static std::string getFileExtension(std::string &fname);

};

#endif