
#include "CVar.h"

namespace Basic
{

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVar::CVar() (constructor)
//
// Description:	Default constructor. Sets the var to 0.
// Parameters:	None.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVar::CVar()
:
	m_string(),
	m_value(0.0f)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVar::CVar(const CVar&) (constructor)
//
// Description:	Constructor that copies another var.
// Parameters:	const CVar& rhs:	A CVar that should be copied.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVar::CVar(const CVar& rhs)
:
	m_string(rhs.m_string),
	m_value(rhs.m_value)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVar::CVar(const CVar*) (constructor)
//
// Description:	Constructor that copies another var.
// Parameters:	const CVar* rhs:	A CVar that should be copied.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVar::CVar(const CVar* rhs)
:
	m_string(rhs->m_string),
	m_value(rhs->m_value)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVar::CVar(const int) (constructor)
//
// Description:	Constructor that sets the variable to the value of an integer.
// Parameters:	const int rhs:	The variable's new value.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVar::CVar(const int rhs)
:
	m_string((int)rhs),
	m_value((float)rhs)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVar::CVar(const float) (constructor)
//
// Description:	Constructor that sets the variable to the value of a float.
// Parameters:	const float rhs:	The variable's new value.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVar::CVar(const float rhs)
:
	m_string((float)rhs),
	m_value((float)rhs)
{
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVar::CVar(const char*) (constructor)
//
// Description:	Constructor that sets the variable's string and converts it to a float.
// Parameters:	const char* rhs:	The variable's new value.
// Returns:		Nothing.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVar::CVar(const char* rhs)
:
	m_string(rhs),
	m_value(0.0f)
{
	m_value = (float)m_string;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVar::operator = (const CVar& rhs)
//
// Description:	Operator that copies another var.
// Parameters:	const CVar& rhs:	The variable that should be copied.
// Returns:		A reference to itself.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVar& CVar::operator = (const CVar& rhs)
{
	m_string = rhs.m_string;
	m_value = rhs.m_value;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVar::operator = (const CVar* rhs)
//
// Description:	Operator that copies another var.
// Parameters:	const CVar* rhs:	The variable that should be copied.
// Returns:		A reference to itself.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVar& CVar::operator = (const CVar* rhs)
{
	m_string = rhs->m_string;
	m_value = rhs->m_value;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVar::operator = (const int rhs)
//
// Description:	Operator that sets the string and float according to an integer.
// Parameters:	const int rhs:	The new value.
// Returns:		A reference to itself.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVar& CVar::operator = (const int rhs)
{
	m_string = rhs;
	m_value = (float)m_string;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVar::operator = (const float rhs)
//
// Description:	Operator that sets the string and float according to another float.
// Parameters:	const float rhs:	The new value.
// Returns:		A reference to itself.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVar& CVar::operator = (const float rhs)
{
	m_string = rhs;
	m_value = (float)m_string;

	return *this;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// CVar::operator = (const char* rhs)
//
// Description:	Operator that sets the string and converts it to a float.
// Parameters:	const char* rhs:	The new string.
// Returns:		A reference to itself.
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
CVar& CVar::operator = (const char* rhs)
{
	m_string = rhs;
	m_value = (float)m_string;

	return *this;
}

} // end of Basic namespace

