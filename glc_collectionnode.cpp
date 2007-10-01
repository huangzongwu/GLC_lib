/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2006 Laurent Ribon (laumaya@users.sourceforge.net)
 Version 0.9.7, packaged on September, 2007.

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

//! \file glc_collectionnode.cpp implementation of the GLC_CollectionNode class.

#include "glc_collectionnode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Default constructor
GLC_CollectionNode::GLC_CollectionNode()
: m_pGeom(NULL)
, m_ListID(0)
, m_pBoundingBox(NULL)
, m_pNumberOfInstance(new int(1))
{
	
}

// Contruct node with a geometry
GLC_CollectionNode::GLC_CollectionNode(GLC_Geometry* pGeom)
: m_pGeom(pGeom)
, m_ListID(0)
, m_pBoundingBox(NULL)
, m_pNumberOfInstance(new int(1))
{
	
}

// Copy constructor
GLC_CollectionNode::GLC_CollectionNode(const GLC_CollectionNode& inputNode)
: m_pGeom(inputNode.m_pGeom)
, m_ListID(0)
, m_pBoundingBox(NULL)
, m_pNumberOfInstance(inputNode.m_pNumberOfInstance)
{
	// Increment the number of instance
	++(*m_pNumberOfInstance);
}

// Destructor
GLC_CollectionNode::~GLC_CollectionNode()
{
	if ((--(*m_pNumberOfInstance)) == 0)
	{
		// this is the last instance, delete the geometry
		if (m_pGeom != NULL)
		{
			delete m_pGeom;
			m_pGeom= NULL;
		}
		// delete instance counter
		delete m_pNumberOfInstance;
	}
	
	if (m_pBoundingBox != NULL)
	{
		delete m_pBoundingBox;
		m_pBoundingBox= NULL;
	}
	
	if (m_ListID != 0)
	{
		glDeleteLists(m_ListID, 1);
		m_ListID= 0;
	}		
}

//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

// Get the geometry of the node
GLC_Geometry* GLC_CollectionNode::getGeometry(void)
{
	return m_pGeom;
}

// Get the validity of the OpenGL list
bool GLC_CollectionNode::getListValidity(void) const
{
	if ((m_pGeom != NULL) && (m_ListID != 0))
	{
		return m_pGeom->getValidity();
	}
	else return false;
}

// Get the bounding box
GLC_BoundingBox GLC_CollectionNode::getBoundingBox(void)
{
	if (getBoundingBoxValidity())
	{
		return *m_pBoundingBox;
	}
	else if (m_pGeom != NULL)
	{
		computeBoundingBox();
		if (m_pBoundingBox != NULL)
		{
			return *m_pBoundingBox;
		}
		
	}
	
	GLC_BoundingBox nullBoundingBox;
	return nullBoundingBox;
}

// Get the validity of the Bounding Box
bool GLC_CollectionNode::getBoundingBoxValidity(void) const
{
	if ((m_pGeom != NULL) && (m_pBoundingBox != NULL))
	{
		return (m_pGeom->getListIsValid()) && (m_pGeom->getValidity());
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

// Set the node Geometry
bool GLC_CollectionNode::setGeometry(GLC_Geometry* pGeom)
{
		if (NULL == m_pGeom) return false;
		else
		{
			m_pGeom= pGeom;
			return true;
		}
}

//////////////////////////////////////////////////////////////////////
// OpenGL Functions
//////////////////////////////////////////////////////////////////////

// Display the Node
void GLC_CollectionNode::glExecute(GLenum Mode)
{
	bool computeBox= false;
	
	// Geometry OpenGl list invalid
	if (!m_pGeom->getListIsValid())
	{
		m_pGeom->glLoadTexture();
		m_pGeom->createList(Mode);
		computeBox= true;
	}
	
	// Geometry invalid or collection node list ID == 0
	if ((!m_pGeom->getValidity()) || (m_ListID == 0))
	{
		qDebug() << "GLC_CollectionNode::GlExecute: geometry validity : " << m_pGeom->getValidity();
		qDebug() << "GLC_CollectionNode::GlExecute: list ID : " << m_ListID;
		
		if (m_ListID == 0)
		{
			qDebug() << "GLC_CollectionNode::GlExecute: List not found";
			m_ListID= glGenLists(1);
		}		
		glNewList(m_ListID, Mode);
			m_pGeom->glExecute(Mode);
		glEndList();
		qDebug() << "GLC_CollectionNode::GlExecute : Display list " << m_ListID << " created";
		computeBox= true;	
	}
	else
	{
		glCallList(m_ListID);
	}
	
	if (computeBox)
	{
		computeBoundingBox();
	}
	
}

//////////////////////////////////////////////////////////////////////
// private services functions
//////////////////////////////////////////////////////////////////////


// compute the node bounding box
// m_pGeom should be not null
void GLC_CollectionNode::computeBoundingBox(void)
{
	if (m_pBoundingBox != NULL)
	{
		delete m_pBoundingBox;
		m_pBoundingBox= NULL;
	}
	
	m_pBoundingBox= m_pGeom->getBoundingBox();
	
}



