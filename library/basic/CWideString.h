/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    wchar_t implementation of CCharString. Templates, anyone? =)
 **/


#ifndef  WIDESTRING_H
#define  WIDESTRING_H

#include <STDLIB.H>

namespace Basic
{
	class CCharString;

	class CWideString
	{

		public: 

			// Constructors:
			
			CWideString();
			CWideString(const wchar_t *text);
			CWideString(const wchar_t chr);
			CWideString(const int value);
			CWideString(const unsigned long value);
			CWideString(const float value);
			CWideString(const double value);
 			CWideString(const CWideString *other);
			CWideString(const CWideString &other);
			CWideString(const CWideString *other, const int num);
			CWideString(const CWideString &other, const int num);
			CWideString(const wchar_t *text, const int num);

			// Destructor

			~CWideString();

			// General purpose functions:

			void		removeAll();
			wchar_t		*getString();
			wchar_t		*getText();
			int			getStringLength() const;
			bool		isValid() const;

			// Assignment operators:

			CWideString	&operator = (const wchar_t rhs);
			CWideString	&operator = (const int rhs);
			CWideString	&operator = (const unsigned long rhs);
			CWideString	&operator = (const wchar_t *rhs);
			CWideString	&operator = (const float rhs);
			CWideString	&operator = (const double rhs);
			CWideString	&operator = (const CWideString *rhs);
			CWideString	&operator = (const CWideString &rhs);

			// Comparision operators:

			bool		operator == (const wchar_t *rhs);
			bool		operator != (const wchar_t *rhs);
			bool		operator <  (const wchar_t *rhs);
			bool		operator >  (const wchar_t *rhs);

			bool		operator == (const CWideString *rhs);
			bool		operator != (const CWideString *rhs);
			bool		operator < (const CWideString *rhs);
			bool		operator > (const CWideString *rhs);
			bool		operator == (const CWideString &rhs);
			bool		operator != (const CWideString &rhs);
			bool		operator < (const CWideString &rhs);
			bool		operator > (const CWideString &rhs);

			// Concatenate operators:

			CWideString &operator += (wchar_t rhs);
			CWideString	&operator += (const int rhs);
			CWideString	&operator += (const unsigned long rhs);
			CWideString	&operator += (const wchar_t *rhs);
			CWideString	&operator += (const float rhs);
			CWideString	&operator += (const double rhs);
			CWideString	&operator += (const CWideString *rhs);
			CWideString	&operator += (const CWideString &rhs);

			CWideString operator + (wchar_t rhs) const;
			CWideString	operator + (const int rhs) const;
			CWideString	operator + (const unsigned long rhs) const;
			CWideString	operator + (const wchar_t *rhs) const;
			CWideString	operator + (const float rhs) const;
			CWideString	operator + (const double rhs) const;
			CWideString	operator + (const CWideString *rhs) const;
			CWideString	operator + (const CWideString &rhs) const;

			// Shifting operators:
			
			CWideString	operator << (const int amount);
			CWideString	operator >> (const int amount);
			CWideString	&operator <<= (const int amount);
			CWideString	&operator >>= (const int amount);

			// Accessing:
			
			wchar_t		operator [] (const int pos) const;
			void		set(const wchar_t chr, const int pos);

			// Conversion operators:

			operator	wchar_t*(); 
			operator	int();
			operator	float();
			operator	double();
			operator	unsigned long ();
			operator	CCharString ();

			// Conversion functions:

			CWideString	&hexToDec();
			CWideString	&decToHex();

			CWideString& toLowerCase();
			CWideString& toUpperCase();

			void		replace(const wchar_t item, const wchar_t type);

			CWideString	getLeft(const int num);
			CWideString	getRight(const int num);
			CWideString	getMiddle(const int left, const int right);

			// Filepath handling:

			CWideString	extractFileName();
			CWideString	extractExtension();
			CWideString	extractPath();

			void		eliminateExtension();
			void		eliminateFilename();
			bool		hasExtension();

			bool		contains(wchar_t c);

			void		convertMBCS(char *string, int size);

			// Static functions with general use
			// (these functions doesn't use any member variables)

			static wchar_t	*pointToLastCharacter(wchar_t *text, wchar_t search);
			static wchar_t	*pointToNextCharacter(wchar_t *text, wchar_t search);

			static wchar_t	*getPositionFromEnd(wchar_t *text, int pos);
			static wchar_t	*getEnd(wchar_t *text);

			static void	convertAllType1ToType2(wchar_t *str, wchar_t type1, wchar_t type2); 

		private:
			void		m_clear();
			bool		m_createMem(int length);

		protected:

			wchar_t		*ma_text;
	};

} // end of Basic namespace

#endif
