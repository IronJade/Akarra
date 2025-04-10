#include "CFileStream.h"
#include <string.h>

namespace FileSystem{

CFileStream::CFileStream(FILE* f, long istart, long iend, bool iunicode) 
	: mp_file(f), m_start(istart), m_end(iend), m_unicode(iunicode) 
{}

bool CFileStream::read(void* buffer, int length)
{
	return (fread(buffer, 1, length, mp_file) == (unsigned int)length) ? true : false;
}

bool CFileStream::read(void* buffer, int type, int length)
{
	length *= type;
	return (fread(buffer, 1, length, mp_file) == (unsigned int)length) ? true : false;
}

int CFileStream::readInt()
{
	int value(0);

	fread(&value, sizeof(int), 1, mp_file);

	return value;
}

char CFileStream::readChar()
{
	return (char)getc(mp_file);
}

void CFileStream::readTextLine(char* buffer, int max_len)
{
	/*char c;
	int count(0);

	while((c = getc(file) != EOF)
	{
		if(c == '\n' || c == '\r' || count >= max_len)
		{
			buffer[count] = 0;
			return 
		}
		buffer[count++] = c;
	}*/
	int a = 0;
	while(true)
	{
		if (read(&buffer[a], 1) == false || buffer[a] == '\n' || buffer[a] == '\r' || buffer[a] == 0)
		{
			buffer[a] = 0;
			return;
		}
		a++;
	} 
}

void CFileStream::readTextLine(wchar_t* buffer, int max_len)
{
	int a = 0;
	bool ret;

	while(true)
	{
		// a new line in a file is a <CR><LF>) combination (0x0D and 0x0A). 
		//	'\r' = 13 0x0D carriage return
		//	'\n' = 10 0x0A newline
		ret = read(&buffer[a], sizeof(wchar_t), 1);
		if (ret == false || buffer[a] == L'\0')
		{
			buffer[a] = L'\0';
			return;
		}
		if (buffer[a] == L'\r')
		{
			//	Read newline as well
			wchar_t tmp;
			read(&tmp, sizeof(wchar_t), 1);

			buffer[a] = L'\0';
			return;
		}
		a++;
	}

}

void CFileStream::readString(char* buffer, int max_len)
{
    int count(0);    

    while(count < max_len)
    {
        char c = (char)getc(mp_file);

        switch(c)
        {
            case EOF:
            case ' ':
            case '\n':
            case '\t':
            case '\r':
                
                buffer[count] = 0;

                return;

            default:
                buffer[count++] = c;
        }
    }
}

bool CFileStream::readStringIgnoreSpace(char* buffer, int max_len)
{
    int count(0);    

    char c;

    do
    {
      c = (char)getc(mp_file);

      if(c == EOF)
      {
          return false;
      }

    } while (c == ' ' || c == '\n' || c == '\t' || c == '\r' );
    
    buffer[count++] = c;

    while(count < max_len)
    {
        c = (char)getc(mp_file);

        switch(c)
        {
            case EOF:

                buffer[count] = 0;
                return false;

            case ' ':
            case '\n':
            case '\t':
            case '\r':
                
                buffer[count] = 0;
                return true;

            default:
                buffer[count++] = c;
        }
    }
    return false;   // Should never be reached
}

bool CFileStream::write(void* buffer, int length)
{
	return (fwrite(buffer, 1, length, mp_file) == (unsigned int)length) ? true : false;
}

bool CFileStream::write(void *buffer, int type, int length)
{
	length *= type;
	return write(buffer, length);
}


void CFileStream::write(int var)
{
	fwrite(&var, sizeof(int), 1, mp_file);
}

void CFileStream::writeChar(char c)
{
	//fputc(c, mp_file);
    fwrite(&c, 1, sizeof(char), mp_file);
}

long CFileStream::getSize()
{
	return m_end - m_start;
}

long CFileStream::getSizeLeft()
{
	return m_end - ftell(mp_file);
}

long CFileStream::getPosition()
{
	return m_start + ftell(mp_file);
}

void CFileStream::seekStart(int offset)
{
	fseek(mp_file, m_start + offset, SEEK_SET);
}

void CFileStream::seekEnd(int offset)
{
	fseek(mp_file, m_end - offset, SEEK_SET);
}

void CFileStream::seekCurrent(int offset)
{
	fseek(mp_file, offset, SEEK_CUR);
}

bool CFileStream::isUnicode()
{
	return m_unicode;
}

bool CFileStream::isEof()
{
	return ftell(mp_file) >= m_end;
}

bool CFileStream::isOk()
{
	return (mp_file) ? true : false;
}

bool CFileStream::findString(const char* string, bool place_end)
{
	int counter(0);
	int length = strlen(string);
	int pointer_count = m_start;

	fseek(mp_file, m_start, SEEK_SET);
	
	char c;
	while(++pointer_count < m_end)  // get character
	{
		c = (char)getc(mp_file);
		if(c == string[counter] || (c - 32) == string[counter] || (c + 32) == string[counter])  // compare it with passed string's chars (Ignore upper-lowercase)
		{
			counter++;
			if(counter == length)  // true if the strings match
			{
				if(!place_end)           // Sets the filepointer to beginning of string
				{
					fseek(mp_file, -counter, SEEK_CUR);
				}
				return true;
			}
		}
		else
		{
			counter = 0;  // Reset counter if chars didn't match
		}
	}
	return false;
}

} // End namespace

