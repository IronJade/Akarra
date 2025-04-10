
#ifndef _CTRIGGER_H_
#define _CTRIGGER_H_

#include "../library/Basic/CCharString.h"
#include "../library/FileSystem/CFileStream.h"
#include "../PlayerSystem/CServerCharacter.h"
#include "CArea.h"

namespace Player
{
	class CServerCharacter;
}

namespace WorldSystem
{

	const int TRIGGER_FILEVERSION = 1;

	enum ETriggerTypes
	{
		TT_TRIGGER = 0,

		TT_SIGN,		// trigger with Look action only
		TT_ATTENTION,	// trigger with Walk action that displays a message
		TT_ENTRANCE,	// trigger that leads to another location and has a description
		TT_TELEPORTER,	// trigger with Walk action that leads to another location
	};

	class ATrigger
	{
		public:

			ATrigger() : u_id(0) { }

			virtual int getType() { return TT_TRIGGER; }

			virtual bool save(FileSystem::CFileStream& file) = 0;
			virtual bool open(FileSystem::CFileStream& file, int version) = 0;
			virtual bool saveId(FileSystem::CFileStream& file);
			virtual bool openId(FileSystem::CFileStream& file, int version);

			virtual void action_Created() { }

			virtual void action_Walk(Player::CServerCharacter* target) { }
			virtual void action_Look(Player::CServerCharacter* target) { }
			virtual void action_Use(Player::CServerCharacter* target) { }
			virtual void action_Attack(Player::CServerCharacter* target) { }
			virtual void action_Enter(Player::CServerCharacter* target) { }

			unsigned char	u_id;	// the trigger's id number in the area where it exists
	};

	// base trigger infos

	class ATriggerDescription
	{
		public:

			virtual bool saveDescription(FileSystem::CFileStream& file);
			virtual bool openDescription(FileSystem::CFileStream& file, int version);

			Basic::CCharString	u_description;
	};

	class ATriggerLocation
	{
		public:

			ATriggerLocation() : u_locx(0), u_locy(0) { u_locarea[0] = 0; }

			virtual bool saveLocation(FileSystem::CFileStream& file);
			virtual bool openLocation(FileSystem::CFileStream& file, int version);

			char	u_locarea[AREA_NAMELENGTH];	// area name
			int		u_locx;						// coordinate within area (0..63)
			int		u_locy;
	};

	class ATriggerFlags
	{
		public:

			ATriggerFlags() : u_flags(0) { }

			virtual bool saveFlags(FileSystem::CFileStream& file);
			virtual bool openFlags(FileSystem::CFileStream& file, int version);

			unsigned int	u_flags;
	};

	// triggers...

	class CSignTrigger : public ATrigger, public ATriggerDescription
	{
		public:

			virtual int getType() { return TT_SIGN; }

			virtual bool save(FileSystem::CFileStream& file);
			virtual bool open(FileSystem::CFileStream& file, int version);

			virtual void action_Look(Player::CServerCharacter* target);
	};

	class CAttentionTrigger : public ATrigger, public ATriggerDescription, public ATriggerFlags
	{
		public:

			virtual int getType() { return TT_ATTENTION; }

			virtual bool save(FileSystem::CFileStream& file);
			virtual bool open(FileSystem::CFileStream& file, int version);

			virtual void action_Walk(Player::CServerCharacter* target);

			static unsigned int	TRIGGERFLAGS_PLAYSOUND;	// play a sound for the player
	};

	class CEntranceTrigger : public ATrigger, public ATriggerDescription, public ATriggerLocation
	{
		public:

			virtual int getType() { return TT_ENTRANCE; }

			virtual bool save(FileSystem::CFileStream& file);
			virtual bool open(FileSystem::CFileStream& file, int version);

			virtual void action_Look(Player::CServerCharacter* target);
			virtual void action_Enter(Player::CServerCharacter* target);
	};

	class CTeleportTrigger : public ATrigger, public ATriggerDescription, public ATriggerLocation,
		public ATriggerFlags
	{
		public:

			virtual int getType() { return TT_TELEPORTER; }

			virtual bool save(FileSystem::CFileStream& file);
			virtual bool open(FileSystem::CFileStream& file, int version);

			virtual void action_Walk(Player::CServerCharacter* target);

			static unsigned int	TRIGGERFLAGS_PLAYSOUND;	// play a sound for the player
	};

} // end of WorldSystem namespace


#endif
