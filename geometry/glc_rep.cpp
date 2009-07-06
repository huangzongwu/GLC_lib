/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 Version 1.1.0, packaged on March, 2009.

 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 *****************************************************************************/

#include "glc_rep.h"

// Default constructor
GLC_Rep::GLC_Rep()
: m_pNumberOfRepresentation(new int(1))
, m_pFileName(new QString())
, m_pName(new QString())
{

}

// Copy Constructor
GLC_Rep::GLC_Rep(const GLC_Rep& rep)
: m_pNumberOfRepresentation(rep.m_pNumberOfRepresentation)
, m_pFileName(rep.m_pFileName)
, m_pName(rep.m_pName)
{
	++(*m_pNumberOfRepresentation);
}

// Assignement operator
GLC_Rep& GLC_Rep::operator=(const GLC_Rep& rep)
{
	if (this != &rep)
	{
		// Clear this representation
		clear();
		m_pNumberOfRepresentation= rep.m_pNumberOfRepresentation;
		++(*m_pNumberOfRepresentation);
		m_pFileName= rep.m_pFileName;
		m_pName= rep.m_pName;
	}

	return *this;
}

// Destructor
GLC_Rep::~GLC_Rep()
{
	// Clear this representation
	clear();
}


//////////////////////////////////////////////////////////////////////
// private services functions
//////////////////////////////////////////////////////////////////////
// Clear current representation
void GLC_Rep::clear()
{
	Q_ASSERT(NULL != m_pNumberOfRepresentation);
	if ((--(*m_pNumberOfRepresentation)) == 0)
	{
		delete m_pNumberOfRepresentation;
		m_pNumberOfRepresentation= NULL;
		delete m_pFileName;
		m_pFileName= NULL;
		delete m_pName;
		m_pName= NULL;
	}
}