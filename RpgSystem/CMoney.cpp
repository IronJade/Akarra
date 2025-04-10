/** 
 *  AKARRA SOURCE CODE (c) Jens Bergensten
 *  See EULA.txt for information about this file.
 *
 *  File description (may not be accurate):
 *    Old money rules.
 **/

#include "CMoney.h"

namespace RpgSystem
{

int CMoney::getMaxCarry(int level, int moneytype)
{
	int value = 0;

	if (moneytype == 0)
	{
		value = level * level * 5;
	}
	else
	{
		value = level * level * 2;
	}

	return ((value > 50000) ? 50000 : value);
}

char* CMoney::getMoneyName(int moneytype)
{
	const char* const MONEY_NAMES[NUM_MONEYTYPES] =
	{
		"Coins",
		"Drengel",
		"Quats"
	};

	return (char*)MONEY_NAMES[moneytype];

}

Basic::CCharString CMoney::getMoneyString(int amount, int type)
{
	if (type == MT_MENIT)
	{
		Basic::CCharString string(amount);
		string += " Drengel";

		//if (amount != 1)
		//{
		//	string += "s";
		//}

		return Basic::CCharString(string);
	}
	else if (type == MT_IWID)
	{
		Basic::CCharString string(amount);
		string += " Quat";

		if (amount != 1)
		{
			string += "s";
		}

		return Basic::CCharString(string);
	}

	// "normal" type is much more complicated

	if (amount == 0)
	{
		return Basic::CCharString("0 Coins");
	}

	Basic::CCharString string;

	// get gold amount
	int gold = amount / 1000;
	int silver = (amount % 1000) / 100;
	int copper = (amount % 1000) % 100;

	if (gold > 0)
	{
		string += gold;
		string += " Gold";

		if (silver < 1 && copper < 1)
		{
			if (gold == 1)
			{
				string += " Coin";
			}
			else
			{
				string += " Coins";
			}
			return Basic::CCharString(string);
		}
	}

	if (silver > 0)
	{
		if (gold > 0 && copper > 0)
		{
			string += ", ";
		}
		else if (gold > 0)
		{
			string += " & ";
		}
		string += silver;
		string += " Silver";

		if (copper < 1)
		{
			if (silver == 1)
			{
				string += " Coin";
			}
			else
			{
				string += " Coins";
			}
			return Basic::CCharString(string);
		}
	}

	if (copper > 0)
	{
		if (gold > 0 || silver > 0)
		{
			string += " & ";
		}
		string += copper;
		string += " Copper Coin";

		if (copper != 1)
		{
			string += "s";
		}
	}

	return Basic::CCharString(string);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// This is a helper function that returns a "value" string. For iwid and menit coin types
// this is just a string with the the exact amount, such as "42", but for the normal coin
// type this is translated into an abbreviated multi-coin string, such as "1gc, 4sc, 23cc"
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
Basic::CCharString CMoney::getMoneyValue(int amount, int type)
{
	if (type == MT_MENIT)
	{
		Basic::CCharString string(amount);
		string += "dc";
		return Basic::CCharString(string);
	}
	if (type == MT_IWID)
	{
		Basic::CCharString string(amount);
		string += "qc";
		return Basic::CCharString(string);
	}

	// this is the normal coin type, convert it into gc, sc and cc
	int gold = amount / 1000;
	int silver = (amount % 1000) / 100;
	int copper = (amount % 1000) % 100;

	Basic::CCharString string;

	if (gold > 0)
	{
		string = gold;
		string += "gc";
	}
	if (silver > 0)
	{
		if (gold > 0)
		{
			string += ", ";
		}
		string += silver;
		string += "sc";
	}
	if (copper > 0)
	{
		if (gold > 0 || silver > 0)
		{
			string += ", ";
		}
		string += copper;
		string += "cc";
	}

	return Basic::CCharString(string);
}

}

