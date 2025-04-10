/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Dice rolling class. Don't really remember where this one is used. I think I
 *    made it just for fun.
 **/

#ifndef _DICESTRING_H_
#define _DICESTRING_H_

#include <vector>

namespace RpgSystem
{

	const char DICE_IDENTIFIER = 'd';

	/* **********************************************************************************
	* CLASS CDiceRoll
	* Derived:	(base class)
	*
	* Purpose:	To handle a single set of dice. Each set uses a dice with S sides and
	*			rolls it N times.
	*
	* Usage:	Create the class using any of the constructors. Dice results can be get
	*			from the roll(), getMin() or getMax() functions.
	*
	* ***********************************************************************************
	* (011021): First version (rewritten from a previous project).
	* **********************************************************************************/
	class CDiceRoll
	{
		public:
			
			CDiceRoll();
			CDiceRoll(int n, int s);
			~CDiceRoll();

			int roll();
			int getMax();
			int getMin();

			void set(int num, int sides);

		protected:

			int		m_num;		// number of times the dice should be rolled
			int		m_sides;	// number of sides the dice has
	};


	/* **********************************************************************************
	* CLASS CDiceList
	* Derived:	(base class)
	*
	* Purpose:	To handle a whole set of dice. Each set uses a number of CDiceRoll
	*			classes to roll a combination of dice.
	*
	* Usage:	Create the the list of dice using the createList() function. Dice roll
	*			descriptions use a format of "NdS+M" where
	*			N is the number of times the dice should be rolled
	*			S is the number of sides the dice should have
	*			M is a constant (a dice roll with only 1 side)
	*			example string: "1d6+1d4+2"
	*
	* ***********************************************************************************
	* (011021): First version (rewritten from a previous project).
	* **********************************************************************************/
	class CDiceList
	{
		public:

			CDiceList();
			~CDiceList();

			int roll();
			int rollMax();
			int rollMin();

			void createList(char *string);

		protected:

			std::vector<CDiceRoll>	m_list;		// list of dice rolls
	};

} // end of RpgSystem namespace

#endif
