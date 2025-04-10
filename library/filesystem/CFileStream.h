#include <stdio.h>

#ifndef _CFILESTREAM_H_
#define _CFILESTREAM_H_

namespace FileSystem{

    class CFileStream{
        public:
			CFileStream() {}
            CFileStream(FILE* f, long start, long end, bool unicode);
			
			bool read(void* buffer, int length);
			bool read(void* buffer, int type, int length);	// takes a type and a type count
			int  readInt();
			char readChar();
			void readTextLine(char* buffer, int max_len);
			void readTextLine(wchar_t* buffer, int max_len);

            // If, for example a <space> is found first, the function returns directly
            void readString(char* buffer, int max_len);  
            
            // Returns false if EndOfFile is reached, otherwise true
            bool readStringIgnoreSpace(char* buffer, int max_len);

			bool write(void* buffer, int length);	// takes a byte count
			bool write(void* buffer, int type, int length);	// takes a type and a type count
			void write(int var);					// writes an int
			void writeChar(char c);

            long getSize();
            long getSizeLeft();
            long getPosition(); 
            void seekStart(int offset);
            void seekEnd(int offset);
			void seekCurrent(int offset);
			bool findString(const char* string, bool end);  // bool = place pointer at end of string
            bool isUnicode();
			bool isEof();
			bool isOk();

			FILE* getFilePointer(){ return mp_file; }

        private:
            FILE* mp_file;
            long m_start;
            long m_end;
			bool m_unicode;
    };

}

#endif