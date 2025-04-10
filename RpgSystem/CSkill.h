/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Skills, as edited in the skills editor.
 **/

#ifndef _CSKILL_H_
#define _CSKILL_H_

#include "../library/basic/CCharString.h"
#include "../library/filesystem/CFileStream.h"
#include <vector>

namespace RpgSystem
{
	class CCharacterAttributes;
	class CEffectType;
	struct SRequirement;

	enum ESkillSlot
	{
		SLOT_WHITE = 0,
		SLOT_RED,
		SLOT_GREEN,
		SLOT_BLUE,
		SLOT_YELLOW,

		NUM_GEM_SLOTS
	};
	char* getGemColorName(int gem);

	// 030204: I changed the skillsystem to merge several skills into bigger ones (Fighting I,
	// Fighting II ... Fighting V, becomes a single Fighting skill, for example). The new skill
	// format supports up to 25 slots.	
	const int NUM_SKILL_ROWS	= 5;
	const int NUM_GEMS_ROW		= NUM_GEM_SLOTS * 2;
	const int NUM_SKILL_SLOTS	= NUM_GEMS_ROW * NUM_SKILL_ROWS;

	// -----

	class CSkillType
	{
		public:

			static char* getSkillName(int skillnumber);
			//static int getSkillNumber(char* skillname);

			//static bool canLearnSkill(int skill, CCharacterAttributes& attributes, SRequirement** failedreq);
			//static bool canSpecializeSkill(int skill, CCharacterAttributes& attributes, SRequirement** failedreq);

			//static char* getSkillEffectName(int skill, int numgems);

			//static bool isSpell(int skill);

			//static char* getSkillDescription(int skill);

	};

	// NEW SKILL TYPE -----------------------------------------------------------------

	enum ESkillTypeFileVersions
	{
		STFV_ORIGINAL = 0,

		STFV_030204,		// Increased the number of slots to 25 for the new skillsystem, and increased
							// the size of the comments to 512 bytes
		STFV_030326,		// added m_flags
		STFV_030508,		// skills now have 10 gems per row
		STFV_030509,		// added requirements to gems and removed effect names (they are dynamically created now)

		STFV_CURRENT
	};

	enum ESkillVariant
	{
		SV_TALENT = 0,		// passive skill
		SV_SKILL,			// active skill
		SV_SPELL,			// active skill

		NUM_SKILL_VARIANTS
	};

	class CSkillType2
	{
		public:

			CSkillType2();
			~CSkillType2();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);

			int getVersion();
			int getId();
			void setId(int id);
			
			char* getName();
			void setName(char* name);
			char* getDesc();
			void setDesc(char* desc);
			int getIcon();
			void setIcon(int icon);

			bool getFlag(int flag);
			void setFlag(int flag, bool setting);

			int getVariantType();
			void setVariantType(int type);

			Basic::CCharString getEffectName(int slot);
			//void setEffectName(int slot, char* effect);
			char* getEffectBaseName();
			void setEffectName(char* name);
			void setRunTimeEffect(int slot, CEffectType* effect);
			CEffectType* getRunTimeEffect(int slot);

			int getRequirementAttribute();
			void setRequirementAttribute(int attr);
			int getRequirement(int slots);
			void setRequirement(int slots, int requirement);

		protected:

			void m_clear();

		protected:

			bool	m_modified;
			int		m_version;			// skill version number
			int		m_skillid;			// skill id number
			int		m_flags;

			char	ma_skillname[32];	// skillname
			char	ma_skilldesc[512];	// skill description
			int		m_icon;				// sprite displayed in the GUI

			int		m_varianttype;		// if it's a talent, skill or spell

			int		m_reqattribute;		// which attribute (0-4) this skill requires
			int		ma_requirements[NUM_SKILL_SLOTS];	// how much is required to add a gem of the next level

			// skill data:

			//char	ma_effectnames[NUM_SKILL_SLOTS][32];	// name of the effects associated with different skill powers
			char	ma_effectnamebase[32];					// effect name, 1x0, 1x1, 1x2... are added automatically
			CEffectType* mpa_runtime[NUM_SKILL_SLOTS];	// these effects are loaded by the server at startup, so we don't have to double-check everything

	};
	const int	STFLAG_NOLOCK		= 0x1;	// This skill can always be modified even if you have used it
	const int	STFLAG_STACKABLE	= 0x2;	// This skill can be placed several times on the same target
	const int	STFLAG_APPEARANCE	= 0x4;	// This skill affects the character's appearance (so it needs to be updated)

	typedef std::vector<CSkillType2*> TDSkillTypeVec;

	// SKILL TYPE BUNDLE ---------------------------------------------------------------------------

	enum ESkillTypeBundleFileVersions
	{
		STBFV_ORIGINAL = 0,

		STBFV_CURRENT
	};

	class CSkillTypeBundle
	{
		public:

			CSkillTypeBundle();
			~CSkillTypeBundle();

			void removeAll();

			bool read(FileSystem::CFileStream& file);
			bool write(FileSystem::CFileStream& file);
			bool open(char* filename);
			bool save(char* filename);

			TDSkillTypeVec& getSkills();
			int getNumSkills();

			CSkillType2* getSkill(char* skillname);
			CSkillType2* getSkill(int id);

			CSkillType2* createNewSkill();
			bool addSkill(CSkillType2* skill);

		protected:

			void m_clear();

		protected:

			TDSkillTypeVec	m_skills;

	};

	/* **********************************************************************************
	* STRUCTURE SSkillData
	*
	* Purpose:	To store information about a single skill/spell for a character in such
	*			small memory space as possible (for fast internet sends). This structure
	*			only cares about variable data (not about constant skill information,
	*			such as requirements and effects).
	*
	* ***********************************************************************************
	* (011027): First version.
	* **********************************************************************************/
	struct SSkillData
	{
		unsigned char	locked		: 1;	// if the skill is locked, this bit is 1
		unsigned char	slot_white	: 1;	// if a skillstone is placed in either slot, the bit is true
		unsigned char	slot_red	: 1;
		unsigned char	slot_green	: 1;
		unsigned char	slot_blue	: 1;
		unsigned char	slot_yellow	: 1;
		unsigned char	enabled		: 1;	// 1 if the character is able to use this skill, 0 otherwise
		unsigned char	reserved	: 1;	// for future expansion
	};

	/* **********************************************************************************
	* CLASS CSkill
	* Derived:	(base class)
	*
	* Purpose:	To keep information about a single character skill.
	*
	* Usage:	n/a
	*
	* ***********************************************************************************
	* (030204): Changed system to support 5 "rows" of gems. NOTE: You can only activate
	*			each row once, but the positions of the gems have no correlation to the
	*			position of the rows (for example, learning "Fighting IV" enables row
	*			number 4 and adds 5 slots, but if you don't know "Fighting III" those
	*			slots will be placed on position 10-14). Due to this it is impossible
	*			to remove rows since you don't know which slots that you should disable.
	* (011027): First version.
	* **********************************************************************************/
	class CSkill
	{
		public:

			CSkill();
			~CSkill();

			void clear();

			bool read(FileSystem::CFileStream& file, int version);
			bool write(FileSystem::CFileStream& file);

			void updateTimer();
			int getDelay();
			void setDelay(int seconds);

			CSkillType2* getSkillType();
			int getSkillId();
			void setNewSkill(CSkillType2* skill, int startrow);	// used for new skills
			void setSkill(CSkillType2* skill);		// used for skills that have been loaded from disk
			//void setSkillType(int skill);
			char* getSkillName();

			//char* getEffectName();
			CEffectType* getEffectType();
			//void setEffectType(CEffectType* effect);

			void setSpecialized(bool set);
			bool isSpecialized();

			bool addStone(int slot);
			bool removeStone(int slot);
			bool hasStone(int slot);
			int getNumStones();

			bool addRow(int row);
			bool hasRow(int row);
			int getNumRows();

		private:

		protected:

			//SSkillData	m_data;	// the current skill state

			CSkillType2*	mp_skill;			// skill data, loaded at runtime
			int				m_skill;			// skill id number

			unsigned char	ma_gems[NUM_SKILL_SLOTS];	// set to non-zero when a gem has been added
			unsigned char	ma_rows[NUM_SKILL_ROWS];	// which rows that have been enabled
			unsigned int	m_flags;

			int				m_changedelay;		// time (seconds) until the user is allowed to change gems

			int				m_numrows;			// recalced when necessary

			//CEffectType*	mp_effecttype;		// current effect type (fetched from the skilltype)
	};

	const unsigned int	SKILLFLAG_SPECIALIZED		= 0x1;	// was previously called "locked"


	// great, o-holy, skill list
	enum ESkillList
	{
		SKILL_FIGHTING_1 = 0,		// improves attack ability for all weapons
		SKILL_WEAPON_MASTERY_1,
		SKILL_DODGE_1,
		SKILL_AXE_1,
		SKILL_SWORD_1,
		SKILL_SHIELD_USE_1,
		SKILL_VENOM_RESISTANCE,

		NUM_SKILLS
	};

	// to conserver memory and packet size, we only keep a single bit for each skill
	// that the character has... so we group them in bytes by 8 pairs at a time
	//const unsigned int NUM_SKILL_BYTES = (NUM_SKILLS / 8) + 1;

	// static functions for the skills

	//void setSkillBit(unsigned char* skillbytes, int skill, bool set);
	//bool skillBitIsSet(unsigned char* skillbytes, int skill);
	//bool canUseSkill(int skill, CCharacterAttributes& attributes);
	//bool canLockSkill(int skill, CCharacterAttributes& attributes);

	//char* getSkillName(int skill);
	//char* getSkillDescription(int skill);
	//Basic::CCharString getFullSkillUseInfo(int skill);
	//Basic::CCharString getFullSkillLockInfo(int skill);

} // end of RpgSystem namespace

#endif
