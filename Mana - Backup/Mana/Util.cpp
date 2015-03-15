#include "Util.h"
#include "BasicTypes.h"

std::string Util::toString(int n)
{
	std::ostringstream os;
	os << n;
	return os.str();
}

std::string Util::toString(float n)
{
	std::ostringstream os;
	os << n;
	return os.str();
}

static std::string formatMessage(std::string msg, std::string param1)
{
	std::string r1("%1");
	std::string result(msg);
	size_t pos = result.find(r1);
	while(pos != std::string::npos)
	{
		result.replace(pos, 2, param1);
		pos = result.find(r1, pos + 2);
	}
	return result;
}

std::string formatMessage(std::string msg, std::string param1, std::string param2)
{
	std::string r1("%1");
	std::string r2("%2");
	std::string result(msg);

	size_t pos = result.find(r1);
	while(pos != std::string::npos)
	{
		result.replace(pos, 2, param1);
		pos = result.find(r1, pos + 2);
	}

	pos = result.find(r2);
	while(pos != std::string::npos)
	{
		result.replace(pos, 2, param2);
		pos = result.find(r2, pos + 2);
	}

	return result;
}

int Util::toInt(std::string &str)
{
	int result = 0;
	std::stringstream is(str);
	is >> result;
	if( is.bad() || is.fail() )
	{
		throw CorruptException(std::string("not an int: ") + str, std::string(__FILE__), __LINE__);
	}

	return result;
}

float Util::toFloat(std::string &str)
{
	float result = 0;
	std::stringstream is(str);
	is >> result;
	if( is.bad() || is.fail() )
	{
		throw CorruptException(std::string("not a float: ") + str, std::string(__FILE__), __LINE__);
	}

	return result;
}

void Util::toFloatVector(std::string &str, std::vector<float> &values)
{
	float tmp = 0;
	std::stringstream is(str);
	while(is >> tmp)
	{
		values.push_back(tmp);
	}
}

void Util::toIntVector(std::string &str, std::vector<int> &values)
{
	int tmp = 0;
	std::stringstream is(str);
	while(is >> tmp)
	{
		values.push_back(tmp);
	}
}


std::string Util::getFileExtension(std::string &fname)
{
	size_t dot = fname.find_last_of('.');
	if(dot == std::string::npos) return std::string();
	return fname.substr(dot+1);
}