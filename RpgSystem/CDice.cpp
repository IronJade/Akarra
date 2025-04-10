/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Dice rolling class. Don't really remember where this one is used. I think I
 *    made it just for fun.
 **/

#include "CDice.h"

namespace RpgSystem
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceRoll::CDiceRoll() (constructor)
//
// Description:	Default constructor. Clears all member data.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CDiceRoll::CDiceRoll()
:
	m_num(0), m_sides(0)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceRoll::CDiceRoll(int, int) (constructor)
//
// Description:	Constructor that sets the number of rolls and sides of the dice.
// Parameters:	int n:	Number of dice.
//				int s:	Number of sides of the dice.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CDiceRoll::CDiceRoll(int n, int s)
:
	m_num(n), m_sides(s)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceRoll::~CDiceRoll() (destructor)
//
// Description:	Default destructor. Removes acquired memory.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CDiceRoll::~CDiceRoll()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceRoll::roll()
//
// Description:	Rolls the dice.
// Parameters:	None.
// Returns:		Returns an integer with values from (m_num) up to (m_num * m_sides).
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CDiceRoll::roll()
{
	if (m_sides <= 1)
	{
		return m_num;
	}

	int count = 0;
	for (int i = 0; i < m_num; i++)
	{
		count += (rand() % m_sides) + 1;
	}
	return count;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceRoll::getMax()
//
// Description:	Calculates the maximum value the dice can create.
// Parameters:	None.
// Returns:		Returns an integer with the highest value.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CDiceRoll::getMax()
{
	if (m_sides <= 1)
	{
		return m_num;
	}

	return m_num * m_sides;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceRoll::getMin()
//
// Description:	Calculates the minimum value the dice can create.
// Parameters:	None.
// Returns:		Returns an integer with the lowest value.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CDiceRoll::getMin()
{
	return m_num;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceRoll::set()
//
// Description:	Sets the number of sides and number of dice.
// Parameters:	int num:	Number of dice.
//				int sides:	Number of sides of the dice.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CDiceRoll::set(int num, int sides)
{
	m_num = num;
	m_sides = sides;
}

// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//  ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ****
// ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceList::CDiceList() (constructor)
//
// Description:	Default constructor. Clears all member data.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CDiceList::CDiceList()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceList::~CDiceList() (destructor)
//
// Description:	Default destructor. Removes acquired memory.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CDiceList::~CDiceList()
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceList::roll()
//
// Description:	Rolls all the dice in the list and returns the results.
// Parameters:	None.
// Returns:		The roll result.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CDiceList::roll()
{
	int total = 0;

	for (int i = 0; i < (int)m_list.size(); i++)
	{
		total += m_list[i].roll();
	}

	return total;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceList::rollMax()
//
// Description:	Calculates the maximum possible value for the dice.
// Parameters:	None.
// Returns:		The maxiumum value.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CDiceList::rollMax()
{
	int total = 0;

	for (int i = 0; i < (int)m_list.size(); i++)
	{
		total += m_list[i].getMax();
	}

	return total;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceList::rollMin()
//
// Description:	Calculates the minimum possible value for the dice.
// Parameters:	None.
// Returns:		The minimum value.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int CDiceList::rollMin()
{
	int total = 0;

	for (int i = 0; i < (int)m_list.size(); i++)
	{
		total += m_list[i].getMin();
	}

	return total;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CDiceList::createList()
//
// Description:	Creates the dice list from a string.
// Parameters:	char* string:	A string that describes a dice setup. Such string can look
//								like "2d6+4" or similar combinatins, where 2 is the number
//								of dice, 6 is the sides of the dice and 4 is a constant
//								value.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void CDiceList::createList(char *string)
{
	// remove old list
	m_list.clear();

	int pos = 0;

	while (string[pos] != 0)
	{
		// get N
		int n = 0;
		while ((string[pos] >= '0') && (string[pos] <= '9'))
		{
			n *= 10;
			n += string[pos] - '0';
			pos++;
		}

		// check if we found a N value at all
		if (!n)
		{
			// nope, this is an errournous string
			return;
		}

		// check if we have found a real dice, or just a constant
		int s = 0;
		if (string[pos] == DICE_IDENTIFIER)
		{
			// this is a real dice identifier, so get S
			pos++; // skip the 'd'			
			while ((string[pos] >= '0') && (string[pos] <= '9'))
			{
				s *= 10;
				s += string[pos] - '0';
				pos++;
			}
		}

		// create dice
		if (!s) s = 1;
		m_list.push_back(CDiceRoll(n, s));
		
		// get action
		// legal characters are '+' (which means continue) or NULL (which means we're done)
		// all other characters are errornous, so they make the function quit
		if (string[pos] == '+')
		{
			pos++; // skip it
		}
		else if (string[pos] != 0)
		{
			return;
		}
	}
}

} // end of RpgSystem namespace
