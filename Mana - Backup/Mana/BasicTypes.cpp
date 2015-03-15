#include "BasicTypes.h"
#include "IOException.h"
#include "Util.h"

int numberGraphicPrimitiveTypes = 8;
KeyCodeType TypeServices::keyList[] = {
	KEY_UNKNOWN,	KEY_UP_ARROW, 	KEY_DOWN_ARROW,	KEY_RIGHT_ARROW,	KEY_LEFT_ARROW,
	KEY_BACKSPACE,	KEY_PAGE_UP,	KEY_PAGE_DOWN,	KEY_SPACE,	KEY_ESC,
	KEY_ENTER,	KEY_LEFT_MOUSE,	KEY_MIDDLE_MOUSE,	KEY_RIGHT_MOUSE,	KEY_0,
	KEY_1,	KEY_2,	KEY_3,	KEY_4,	KEY_5,	KEY_6,	KEY_7,	KEY_8,	KEY_9,
	KEY_A,	KEY_B,	KEY_C,	KEY_D,	KEY_E,	KEY_F,	KEY_G,	KEY_H,	KEY_I,	KEY_J,
	KEY_K,	KEY_L,	KEY_M,	KEY_N,	KEY_O,	KEY_P,	KEY_Q,	KEY_R,	KEY_S,	KEY_T,
	KEY_U,	KEY_V,	KEY_W,	KEY_X,	KEY_Y,	KEY_Z
};

 const PrimitiveDataType PRIM_NONE(PrimitiveDataType::PRIM_TYPE_NONE);
 const PrimitiveDataType PRIM_BYTE(PrimitiveDataType::PRIM_TYPE_BYTE);
 const PrimitiveDataType PRIM_SHORT(PrimitiveDataType::PRIM_TYPE_SHORT);
 const PrimitiveDataType PRIM_INT(PrimitiveDataType::PRIM_TYPE_INT);
 const PrimitiveDataType PRIM_LONG(PrimitiveDataType::PRIM_TYPE_LONG);
 const PrimitiveDataType PRIM_UBYTE(PrimitiveDataType::PRIM_TYPE_UBYTE);
 const PrimitiveDataType PRIM_USHORT(PrimitiveDataType::PRIM_TYPE_USHORT);
 const PrimitiveDataType PRIM_UINT(PrimitiveDataType::PRIM_TYPE_UINT);
 const PrimitiveDataType PRIM_ULONG(PrimitiveDataType::PRIM_TYPE_ULONG);
 const PrimitiveDataType PRIM_FLOAT(PrimitiveDataType::PRIM_TYPE_FLOAT);
 const PrimitiveDataType PRIM_DOUBLE(PrimitiveDataType::PRIM_TYPE_DOUBLE);
 const PrimitiveDataType PRIM_C_STRING(PrimitiveDataType::PRIM_TYPE_C_STRING);
 const PrimitiveDataType PRIM_STRING(PrimitiveDataType::PRIM_TYPE_STRING);
 const PrimitiveDataType PRIM_POINTER(PrimitiveDataType::PRIM_TYPE_POINTER);
 const PrimitiveDataType PRIM_CHAR(PrimitiveDataType::PRIM_TYPE_CHAR);

 GLenum TypeServices::graphicPrimitiveToOpenGLType(GraphicPrimitiveType typ)
{
	GLenum result = 0;

	switch(typ)
	{	
	case GRAPH_PRIM_POINT_LIST:
		result = GL_POINTS;
		break;
	case GRAPH_PRIM_TRI_LIST:
		result = GL_TRIANGLES;
		break;
	case GRAPH_PRIM_TRI_STRIP:
		result = GL_TRIANGLE_STRIP;
		break;
	case GRAPH_PRIM_LINE_LIST:
		result = GL_LINES;
		break;
	case GRAPH_PRIM_LINE_STRIP:
		result = GL_LINE_STRIP;
		break;
	case GRAPH_PRIM_QUAD_LIST:
		result = GL_QUADS;
		break;
	case GRAPH_PRIM_QUAD_STRIP:
		result = GL_QUAD_STRIP;
		break;
	}

	return result;
}

 GLenum TypeServices::textureCombinationTypeToOpenGLType(TextureCombinationType typ)
 {
	 GLenum result = GL_MODULATE;
	 switch(typ)
	 {
	 case TEX_COMB_MODULATE:
		 result = GL_MODULATE;
		 break;
	 case TEX_COMB_REPLACE:
		 result = GL_REPLACE;
		 break;
	 case TEX_COMB_DECAL:
		 result = GL_DECAL;
		 break;
	 }
	 return result;
 }

 KeyCodeType TypeServices::intToKeyCodeType(int key)
 {
	 KeyCodeType result = KEY_UNKNOWN;
	 if(key > 0 && key < 48)
	 {
		 result = keyList[key];
	 }
	 return result;
 }

PrimitiveDataType::PrimitiveDataType(const PrimitiveDataType &other)
{
	value = other.value;
}

bool operator==(const PrimitiveDataType &first, const PrimitiveDataType &second)
{
	return first.value == second.value;
}

bool operator!=(const PrimitiveDataType &first, const PrimitiveDataType &second)
{
	return first.value != second.value;
}

PrimitiveDataType PrimitiveDataType::operator=(PrimitiveDataType &other)
{
	value = other.value;
	return *this;
}

std::string PrimitiveDataType::toString(PrimitiveDataType &typ)
{
	std::string result;
	switch(typ.value)
	{
	case PRIM_TYPE_NONE:
		result = "None";
		break;
	case PRIM_TYPE_BYTE:
		result = "byte";
		break;
	case PRIM_TYPE_SHORT:
		result = "short";
		break;
	case PRIM_TYPE_INT:
		result = "int";
		break;
	case PRIM_TYPE_LONG:
		result = "long";
		break;
	case PRIM_TYPE_UBYTE:
		result = "unsigned byte";
		break;
	case PRIM_TYPE_USHORT:
		result = "unsigned short";
		break;
	case PRIM_TYPE_UINT:
		result = "unsigned int";
		break;
	case PRIM_TYPE_ULONG:
		result = "unsigned long";
		break;
	case PRIM_TYPE_FLOAT:
		result = "float";
		break;
	case PRIM_TYPE_DOUBLE:
		result = "double";
		break;
	case PRIM_TYPE_C_STRING:
		result = "char*";
		break;
	case PRIM_TYPE_STRING:
		result = "std::string";
		break;
	case PRIM_TYPE_CHAR:
		result = "char";
		break;
	case PRIM_TYPE_POINTER:
		result = "void*";
		break;
	}
	return result;
}

GLenum PrimitiveDataType::toOpenGLType()
{
	GLenum result;
	switch(value)
	{
	case PRIM_TYPE_NONE:
		result = GL_INT;
		break;
	case PRIM_TYPE_BYTE:
		result = GL_BYTE;
		break;
	case PRIM_TYPE_SHORT:
		result = GL_SHORT;
		break;
	case PRIM_TYPE_INT:
		result = GL_INT;
		break;
	case PRIM_TYPE_LONG:
		result = GL_INT;
		break;
	case PRIM_TYPE_UBYTE:
		result = GL_UNSIGNED_BYTE;
		break;
	case PRIM_TYPE_USHORT:
		result = GL_UNSIGNED_SHORT;
		break;
	case PRIM_TYPE_UINT:
		result = GL_UNSIGNED_INT;
		break;
	case PRIM_TYPE_ULONG:
		result = GL_UNSIGNED_INT;
		break;
	case PRIM_TYPE_FLOAT:
		result = GL_DOUBLE;
		break;
	case PRIM_TYPE_DOUBLE:
		result = GL_DOUBLE;
		break;
	case PRIM_TYPE_C_STRING:
		result = GL_INT;
		break;
	case PRIM_TYPE_STRING:
		result = GL_INT;
		break;
	case PRIM_TYPE_CHAR:
		result = GL_INT;
		break;
	case PRIM_TYPE_POINTER:
		result = GL_INT;
		break;
	}
	return result;
}

std::ostream& operator<<(std::ostream &os, PrimitiveDataType &typ)
{
	os << PrimitiveDataType::toString(typ);
	return os;
}

Exception::Exception(Exception& e)
{
	message = e.message;
	lineNum = e.lineNum;
	fileName = e.fileName;
	nestedException = e.nestedException;
}

Exception::Exception(std::string& msg, std::string fname, int line, Exception& e)
{
	message = msg;
	fileName = fname;
	lineNum = line;
	nestedException = new Exception(e);
}

Exception::~Exception()
{
	if(nestedException)
	{
		delete nestedException;
		nestedException = NULL;
	}
}

std::string Exception::getExceptionName()
{
	return std::string("Exception");
}

std::string Exception::getMessage()
{
	return message;
}

std::string Exception::getFileName()
{
	return fileName;
}

int Exception::getLine()
{
	return lineNum;
}

bool Exception::hasNestedException()
{
	return nestedException != NULL;
}

Exception* Exception::getNestedException()
{
	return nestedException;
}

std::string Exception::getReasonChain()
{
	std::string result;
	result.append(toString() );
	Exception* e = nestedException;
	while(e != NULL)
	{
		result.append("\n");
		result.append(e->toString() );
		e = e->getNestedException();
	}
	return result;
}

std::string Exception::toString()
{
	std::string result;
	result.append(getExceptionName() );
	result.append(": ");
	result.append(fileName);
	result.append("(");
	result.append(Util::toString(lineNum) );
	result.append("): ");
	result.append(message);

	return result;
}

std::ostream& operator<<( std::ostream& os, Exception& e)
{
	os << e.toString();
	return os;
}

std::string IOException::getExceptionName()
{
	return std::string("IOException");
}

std::string FileNotFoundException::getExceptionName()
{
	return std::string("FileNotFoundException");
}

std::string UnsupportedOperationException::getExceptionName()
{
	return std::string("UnsupportedOperationException");
}

std::string CorruptException::getExceptionName()
{
	return std::string("CorruptException");
}

std::string IndexOutOfBoundsException::getExceptionName()
{
	return std::string("IndexOutOfBoundsException");
}

IndexOutOfBoundsException::IndexOutOfBoundsException(IndexOutOfBoundsException& e)
{
	message = e.message;
	lineNum = e.lineNum;
	fileName = e.fileName;
	nestedException = e.nestedException;
	badIndex = e.badIndex;
}

std::string IndexOutOfBoundsException::toString()
{
	std::string result;
	result.append(getExceptionName() );
	result.append(": ");
	result.append(fileName);
	result.append("(");
	result.append(Util::toString(lineNum) );
	result.append("): index=");
	result.append(Util::toString(badIndex) );
	result.append(" ");
	result.append(message);

	return result;
}

std::string DuplicateNameException::getExceptionName()
{
	return std::string("DuplicateNameException");
}

DuplicateNameException::DuplicateNameException(DuplicateNameException& e)
{
	message = e.message;
	duplicateName = e.duplicateName;
	fileName = e.fileName;
	nestedException = e.nestedException;
}

std::string DuplicateNameException::toString()
{
	std::string result;
	result.append(getExceptionName() );
	result.append(": ");
	result.append(fileName);
	result.append("(");
	result.append(Util::toString(lineNum) );
	result.append("): name=");
	result.append(duplicateName);
	result.append(" ");
	result.append(message);

	return result;
}