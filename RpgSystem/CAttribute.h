/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Attributes are the basic stats of an entity, but are usually not
 *    used in calculations. Instead they are derived to abilities.
 **/

#ifndef _CATTRIBUTE_H_
#define _CATTRIBUTE_H_

#include "../library/FileSystem/CFileStream.h"

namespace RpgSystem
{

	enum EBaseAttributes
	{
		BA_STRENGTH = 0,
		BA_CONSTITUTION,
		BA_AGILITY,
		BA_INTELLIGENCE,
		BA_FAITH,

		NUM_ATTRIBUTES
	};

	enum EVarAttributes
	{
		HA_HEALTH = 0,
		HA_MANA,
		HA_STAMINA,

		NUM_VARATTRIBUTES
	};

	/* **********************************************************************************
	* CLASS CAttribute
	* Derived:	(base class)
	*
	* Purpose:	To keep information about a single character attribute.
	*
	* Usage:	Keep the information correct by setting the base value and then adding
	*			bonus values ontop of it. When all bonus values have been added, re-
	*			calculate the modifier.
	*
	* ***********************************************************************************
	* (011022): First version.
	* **********************************************************************************/
	class CAttribute
	{
		public:

			CAttribute();
			~CAttribute();

			virtual void setBase(float base);
			virtual void resetBonus();
			virtual void addBonus(float bonus);
			virtual void addPercentBonus(float bonus);

			virtual float getBase();
			virtual float getBonus();
			virtual float getPercentBonus();
			virtual inline float getValue();

			virtual bool read(FileSystem::CFileStream& file, int version);
			virtual bool write(FileSystem::CFileStream& file, int version);

		private:

		protected:

			float	m_base;			// the base value
			float	m_bonus;		// temporary bonuses acquired from items and spells
			float	m_percentbonus;	// temporary percentile bonuses

	};

	/* **********************************************************************************
	* CLASS CVarAttribute
	* Derived:	(base class)
	*
	* Purpose:	To keep information about an attribute that has a max and a current value.
	*
	* Usage:	n/a
	*
	* ***********************************************************************************
	* (011022): First version.
	* **********************************************************************************/
	class CVarAttribute : public CAttribute
	{
		public:

			CVarAttribute();
			~CVarAttribute();

			virtual void setBase(float value);
			virtual void resetBonus();
			virtual void addBonus(float bonus);
			virtual void addPercentBonus(float bonus);
			
			virtual float getCurrent();
			virtual void setCurrent(float value);

			virtual inline float addToCurrent(float amount);
			virtual void restore();

			virtual bool read(FileSystem::CFileStream& file, int version);
			virtual bool write(FileSystem::CFileStream& file, int version);

		private:

		protected:

			float	m_current;	// the current value, can be less than 0 but never more than max
	};

	// static attribute functions

	char* getAttributeName(int attribute);
	char* getVarAttributeName(int attribute);

} // end of RpgSystem namespace


#endif
