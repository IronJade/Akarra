/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    A character effect is a spell effect which has been placed on an entity
 *    and needs to be updated every second.
 **/

#ifndef _CCHREFFECT_H_
#define _CCHREFFECT_H_

namespace FileSystem
{
	class CFileStream;
}

namespace RpgSystem
{
	class CEffectType;
	class IAttributeObject;

	// A CChrEffect is a "dynamical" effect that is placed on a character or a monster for
	// a limited amount of time. CChrEffects are mainly used for poisons and boosting spells.
	class CChrEffect
	{
		public:

			CChrEffect();
			~CChrEffect();

			bool read(FileSystem::CFileStream& file, int version);
			bool write(FileSystem::CFileStream& file);

			int getID();
			bool isOverruled(int skill);
			bool affectsStats();

			CEffectType* getEffect();
			char* getEffectName();
			float getCastRating();

			void setEffect(CEffectType* effect);
			void startEffect(CEffectType* effect, IAttributeObject* source, int identifier, int skill = -1);

			bool runEffect(IAttributeObject* target, int difftime);

			int getSourceID();
			int getRemainingLifeMillis();

		protected:

			int				m_identifier;		// unique id (so we can send info to the client)
			int				m_skillsource;		// from what skill id this effect came (prevents stacking)

			CEffectType*	mp_effect;			// the effect
			char			ma_effectname[32];	// since we can't save the pointer to a file
			int				m_life;				// remaining effect time
			int				m_seconds;			// seconds counter
			float			m_sourceCR;			// cast rating that was used when the effect started

			int				m_attackergid;		// id number of the source that placed the effect on
												// the character. Since id numbers are flexible (the same
												// monster may not always have the same id) they are not
												// saved when the character is saved, so the source can
												// only be tracked as long as the character doesn't
												// disconnect. Invalid id numbers are set to -1.

	};
}

#endif
