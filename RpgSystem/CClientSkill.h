
// client-side skill data

#ifndef _CCLIENTSKILL_H_
#define _CCLIENTSKILL_H_

#include <vector>

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{
	enum EClientSkillTypeFileVersion
	{
		CSTFV_ORIGINAL = 0,

		CSTFV_030211,		// increased the description size to 512 chars

		CSTFV_CURRENT
	};

	class CClientSkillType
	{
		public:

			CClientSkillType();
			~CClientSkillType();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

		public:

			int		u_skillid;
			int		u_skillversion;

			int		u_skillicon;
			int		u_skillvariant;		// 0 = talent, 1 = skill, 2 = spell

			char	ua_name[32];
			char	ua_desc[512];
	};
	typedef std::vector<CClientSkillType*> TDClientSkillTypeVec;

	enum EClientSkillBundleFileVersion
	{
		CSBFV_ORIGINAL = 0,

		CSBFV_CURRENT
	};

	class CClientSkillBundle
	{
		public:

			CClientSkillBundle();
			~CClientSkillBundle();

			void removeAll();

			bool open(char* filename);
			bool save(char* filename);

			CClientSkillType* getSkill(int id);
			void updateSkill(int id, int version, int icon, int variant, char* name, char* desc);

		protected:

			void m_clear();

		protected:

			TDClientSkillTypeVec	m_skills;
	};
}

#endif
