/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    This class is used to save a character's quest status to file.
 **/

#ifndef _CCHARACTERQUESTS_H_
#define _CCHARACTERQUESTS_H_

#include "CQuest.h"
#include <vector>

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{
	class CQuestInstance;

	// this class doesn't hold that much information, it's mostly used
	// to store the character's quest information in a decent location
	class CCharacterQuests
	{
		public:

			CCharacterQuests();
			~CCharacterQuests();

			void removeAll();

			bool read(FileSystem::CFileStream& file, int version);
			bool write(FileSystem::CFileStream& file);

			// 020913: I moved these functions to the server character instead since
			//         they required so much of the other system resources
			//CQuestInstance* getQuestInstance(char* questname);
			//int getQuestState(char* questname);
			//int setQuestState(char* questname, int state);

		public:

			std::vector<CQuestInstance*>	u_questlist;
	};
}

#endif
