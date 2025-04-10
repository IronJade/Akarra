
#ifndef _CUP_SKILLS_H_
#define _CUP_SKILLS_H_

namespace Comm
{

	// CUP_S_SKILLINFO ---------------------------------------------------------------------------
	const unsigned char	SKILLINFOFLAG_INITIAL		= 0x1;
	const unsigned char	SKILLINFOFLAG_SPECIALIZED	= 0x2;
	struct SCUPSSkillInfo
	{
		unsigned char	type;

		unsigned char	flags;
		//unsigned char	initial;		// 1 if this skill was sent on connect (ie not a new skill)
		//unsigned char	gems;			// bits: 000ybgrw (0 = not set, 1 = set)
		//unsigned char	specialized;	// 0 = not specialized, 1 = specialized
		unsigned char	activerows;		// number of active rows
		unsigned char	padding[3];

		unsigned short	gemrows2[5];	// each row uses 000000yybbggrrww

		int				skillnumber;	// skill id number
		int				versionnumber;	// so the client can detect if it needs to download new skill info
	};

	// CUP_S_AVAILGEMS ---------------------------------------------------------------------------
	struct SCUPSAvailableGems
	{
		unsigned char	type;
		unsigned char	reserved;

		unsigned char	totalgems;
		unsigned char	freegems[5];
	};

	// CUP_S_NEWSKILLDATA ------------------------------------------------------------------------
	struct SCUPSSkillData
	{
		unsigned char	type;
		unsigned char	padding;

		unsigned char	skilltype;		// talent, skill or spell
		unsigned char	skillicon;		// which gui icon the skill uses

		int				skillnumber;	// which skill that is updated
		int				skillversion;	// so we can detect the next version

		char			name[32];		// skill name
		char			desc[512];		// skill description
	};

}

#endif
