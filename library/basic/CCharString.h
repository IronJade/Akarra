/** 
 *  File description (may not be accurate):
 *    Signed char string class used throughout the project. This class was based on
 *    somebody else's class, but I don't remember who. In other words, the EULA doesn't
 *    count for this file.
 **/

#ifndef  CHARSTRING_H
#define  CHARSTRING_H

#include <vector>

namespace Basic
{

	/* **********************************************************************************
	* CLASS CCharString
	* Derived:	(base class)
	*
	* Purpose:	To manage a 8 bit per character, dynamic length, null-terminated string.
	*
	* Usage:	Create the string using any of the constructors. Numerous operators and
	*			general functions are available for your convenience. Use getString() or
	*			just typecast to (char*) to retrieve the string.
	*
	* ***********************************************************************************
	* (020510): Added CCharString(const float value, const int maxdigits);
	* (011220): contains(char*), isDigit(int), toInt(), toLong() and toDouble()
	*           functions added.
	* (01????): First version
	* **********************************************************************************/
	class CCharString
	{

		public: 

			// Constructors:
			
			CCharString();
			CCharString(const char* text);
			CCharString(const char chr);
			CCharString(const int value);
			CCharString(const int value, const char kilodelimiter);
			CCharString(const unsigned long value, const char kilodelimiter);
			CCharString(const unsigned long value);
			CCharString(const float value);
			CCharString(const float value, const int maxdigits);
			CCharString(const double value);
 			CCharString(const CCharString *other);
			CCharString(const CCharString &other);
			CCharString(const CCharString *other, const int num);
			CCharString(const CCharString &other, const int num);
			CCharString(const char *text, const int num);

			// Destructor

			~CCharString();

			// General purpose functions:

			void		removeAll();
			char*		getString();
			char*		getText();
			int			getStringLength() const;
			bool		isValid() const;

			// Assignment operators:

			CCharString	&operator = (const char rhs);
			CCharString	&operator = (const int rhs);
			CCharString	&operator = (const unsigned long rhs);
			CCharString	&operator = (const char *rhs);
			CCharString	&operator = (const float rhs);
			CCharString	&operator = (const double rhs);
			CCharString	&operator = (const CCharString *rhs);
			CCharString	&operator = (const CCharString &rhs);

			// Comparision operators:

			bool		operator == (const char *rhs);
			bool		operator != (const char *rhs);
			bool		operator <  (const char *rhs);
			bool		operator >  (const char *rhs);

			bool		operator == (const CCharString *rhs);
			bool		operator != (const CCharString *rhs);
			bool		operator < (const CCharString *rhs);
			bool		operator > (const CCharString *rhs);
			bool		operator == (const CCharString &rhs);
			bool		operator != (const CCharString &rhs);
			bool		operator < (const CCharString &rhs);
			bool		operator > (const CCharString &rhs);

			// Concatenate operators:

			CCharString &operator += (char rhs);
			CCharString	&operator += (const int rhs);
			CCharString	&operator += (const unsigned long rhs);
			CCharString	&operator += (const char *rhs);
			CCharString	&operator += (const float rhs);
			CCharString	&operator += (const double rhs);
			CCharString	&operator += (const CCharString *rhs);
			CCharString	&operator += (const CCharString &rhs);

			CCharString operator + (char rhs) const;
			CCharString	operator + (const int rhs) const;
			CCharString	operator + (const unsigned long rhs) const;
			CCharString	operator + (const char *rhs) const;
			CCharString	operator + (const float rhs) const;
			CCharString	operator + (const double rhs) const;
			CCharString	operator + (const CCharString *rhs) const;
			CCharString	operator + (const CCharString &rhs) const;

			// Shifting operators:
			
			CCharString	operator << (const int amount);
			CCharString	operator >> (const int amount);
			CCharString	&operator <<= (const int amount);
			CCharString	&operator >>= (const int amount);

			// Accessing:
			
			char		operator [] (const int pos) const;
			void		set(const char chr, const int pos);

			// Conversion operators:

			operator	char*(); 
			operator	int();
			operator	float();
			operator	double();
			operator	unsigned long ();

			// Conversion functions:

			CCharString	&hexToDec();
			CCharString	&decToHex();

			CCharString& toLowerCase();
			CCharString& toUpperCase();

			void		replace(const char item, const char type);

			CCharString	getLeft(const int num);
			CCharString	getRight(const int num);
			CCharString	getMiddle(const int left, const int right);

			// Filepath handling:

			CCharString	extractFileName();
			CCharString	extractExtension();
			CCharString	extractPath();

			void		eliminateExtension();
			void		eliminateFilename();
			bool		hasExtension();

			bool		contains(char c);
			bool		contains(char* str);
            bool        isDigit(int index);
            
            int         toInt();
            long        toLong();
            double      toDouble();
            
            CCharString getSubString(int begin, int end);

			// Static functions with general use
			// (these functions doesn't use any member variables)

			static char	*pointToLastCharacter(char *text, char search);
			static char	*pointToNextCharacter(char *text, char search);

			static char	*getPositionFromEnd(char *text, int pos);
			static char	*getEnd(char *text);

			static void	convertAllType1ToType2(char *str, char type1, char type2); 

		private:
			void		m_clear();
			bool		m_createMem(int length);

		protected:

			char		*ma_text;
	};

	/* **********************************************************************************
	* CLASS CTokenizedCharString
	* Derived:	(base class)
	*
	* Purpose:	To tokenize a CCharString and place the tokens into a vector.
	*
	* Usage:	Tokenize a string by either creating the class with the non-default
	*			constructor or call tokenizeString(). The tokens are then accessible by
	*			calling getToken().
	*
	* ***********************************************************************************
	* (011220): First version
	* **********************************************************************************/
	class CTokenizedCharString
	{
		public:

			CTokenizedCharString();
			CTokenizedCharString(CCharString& string, char delimiter, bool quotes = false);
			~CTokenizedCharString();

			int tokenizeString(CCharString& string, char delimiter, bool quotes = false);
			int getNumTokens();
			CCharString& getToken(int pos);

			void removeAll();

			// this function puts tokens together again, placing a delimiter between them
			CCharString bundleTokens(int first, int last, char delimiter);

		private:

		protected:

			std::vector<CCharString>	m_tokens;

	};

} // end of Basic namespace

#endif
