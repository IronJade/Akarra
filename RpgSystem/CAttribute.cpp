/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Attributes are the basic stats of an entity, but are usually not
 *    used in calculations. Instead they are derived to abilities.
 **/

#include "CAttribute.h"

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CAttribute::CAttribute() (constructor)
//
// Description:	Default constructor. Clears all member data.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CAttribute::CAttribute()
:
	m_base(0),
	m_bonus(0),
	m_percentbonus(0)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CAttribute::~CAttribute() (destructor)
//
// Description:	Default destructor. Removes acquired memory.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CAttribute::~CAttribute()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CAttribute::setBase()
//
// Description:	Sets the base attribute value.
// Parameters:	float base:	The requested value.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAttribute::setBase(float base)
{
	m_base = base;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CAttribute::resetBonus()
//
// Description:	Resets the bonus to 0, so bonus-adding can be started.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAttribute::resetBonus()
{
	m_bonus = 0;
	m_percentbonus = 0;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CAttribute::addBonus()
//
// Description:	Adds a value to the bonus.
// Parameters:	float bonus:	The the amount to increase bonus with.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAttribute::addBonus(float bonus)
{
	m_bonus += bonus;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CAttribute::addPercentBonus()
//
// Description:	Adds a value to the percent bonus.
// Parameters:	float bonus:	The the amount to increase bonus with. Note that this is
//								a percentile bonus, so a value of 0.2f would increase
//								the bonus by 20%.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CAttribute::addPercentBonus(float bonus)
{
	m_percentbonus += bonus;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CAttribute::getBase()
//
// Description:	Used to get the base value.
// Parameters:	None.
// Returns:		The base value (float).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CAttribute::getBase()
{
	return m_base;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CAttribute::getBonus()
//
// Description:	Used to get the bonus value.
// Parameters:	None.
// Returns:		The bonus value (float).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CAttribute::getBonus()
{
	return m_bonus;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CAttribute::getPercentBonus()
//
// Description:	Used to get the percentile bonus value.
// Parameters:	None.
// Returns:		The bonus value (float).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CAttribute::getPercentBonus()
{
	return m_percentbonus;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CAttribute::getValue()
//
// Description:	Used to get the total attribute value.
// Parameters:	None.
// Returns:		The total value (float).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CAttribute::getValue()
{
	float value = (m_base + m_bonus) * (m_percentbonus + 1.0f);
	return (value < 0) ? 0.0f : value;
}

bool CAttribute::read(FileSystem::CFileStream& file, int version)
{
	if (!file.isOk())
	{
		return false;
	}

	file.read(&m_base, sizeof(float), 1);
	file.read(&m_bonus, sizeof(float), 1);
	file.read(&m_percentbonus, sizeof(float), 1);

	return true;
}

bool CAttribute::write(FileSystem::CFileStream& file, int version)
{
	if (!file.isOk())
	{
		return false;
	}

	file.write(&m_base, sizeof(float), 1);
	file.write(&m_bonus, sizeof(float), 1);
	file.write(&m_percentbonus, sizeof(float), 1);

	return true;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVarAttribute::CVarAttribute() (constructor)
//
// Description:	Default constructor. Clears all member data.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVarAttribute::CVarAttribute()
:
	CAttribute(),
	m_current(0)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVarAttribute::~CVarAttribute() (destructor)
//
// Description:	Default destructor. Removes acquired memory.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVarAttribute::~CVarAttribute()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVarAttribute::setBase()
//
// Description:	Sets the base max value. This only affects the current value if the new
//				max is less than the current value.
// Parameters:	float value:	The new max value.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CVarAttribute::setBase(float value)
{
	m_base = value;

	if (m_current > getValue())
	{
		m_current = getValue();
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVarAttribute::resetMaxBonus()
//
// Description:	Resets the bonus to 0. Reduces the current value because we do not want
//				exploits.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CVarAttribute::resetBonus()
{
	// get delta value
	float old = getValue();

	m_bonus = 0;
	m_percentbonus = 0;

	m_current -= getValue() - old;

	if (m_current > getValue())
	{
		m_current = getValue();
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVarAttribute::addBonus()
//
// Description:	Increases the bonus of the maximum score. Also increases the current
//				value (and makes sure it isn't more than the new maximum).
// Parameters:	float amount:	The amount to be added.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CVarAttribute::addBonus(float amount)
{
	// get delta value
	float old = getValue();

	m_bonus += amount;
	m_current += getValue() - old;

	if (m_current > getValue())
	{
		m_current = getValue();
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVarAttribute::addPercentBonus()
//
// Description:	Increases the percentile bonus of the maximum score. Also increases the
//				current value (and makes sure it isn't more than the new maximum).
// Parameters:	float amount:	The amount to be added.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CVarAttribute::addPercentBonus(float amount)
{
	// get delta value
	float old = getValue();

	m_percentbonus += amount;
	m_current += getValue() - old;

	if (m_current > getValue())
	{
		m_current = getValue();
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVarAttribute::getCurrent()
//
// Description:	Returns the current value.
// Parameters:	None.
// Returns:		The current value.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CVarAttribute::getCurrent()
{
	return m_current;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Description:	
// Parameters:	
// Returns:		
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CVarAttribute::setCurrent(float value)
{
	m_current = (value <= getValue()) ? value : getValue();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVarAttribute::addToCurrent()
//
// Description:	Adds a value to the current.
// Parameters:	float amount:	The value to be added (use negative values for, say,
//								damage).
// Returns:		The new current value.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float CVarAttribute::addToCurrent(float amount)
{
	m_current += amount;

	if (m_current > getValue())
	{
		m_current = getValue();
	}

	return m_current;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVarAttribute::restore()
//
// Description:	Restores the current value back to max.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CVarAttribute::restore()
{
	m_current = getValue();
}

bool CVarAttribute::read(FileSystem::CFileStream& file, int version)
{
	if (!file.isOk())
	{
		return false;
	}

	file.read(&m_current, sizeof(float), 1);

	return CAttribute::read(file, version);
}

bool CVarAttribute::write(FileSystem::CFileStream& file, int version)
{
	if (!file.isOk())
	{
		return false;
	}

	file.write(&m_current, sizeof(float), 1);

	return CAttribute::write(file, version);
}


// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

const char* ATTRIBUTE_NAME[NUM_ATTRIBUTES] = 
{
	"Strength",
	"Constitution",
	"Agility",
	"Intelligence",
	"Faith"
};

const char* VARATTRIBUTE_NAME[NUM_VARATTRIBUTES] = 
{
	"Health",
	"Mana",
	"Stamina"
};

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

char* getAttributeName(int attribute)
{
	return (char*)ATTRIBUTE_NAME[attribute];
}

char* getVarAttributeName(int attribute)
{
	return (char*)VARATTRIBUTE_NAME[attribute];
}

} // end of RpgSystem namespace
