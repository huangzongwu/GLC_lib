/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2011 J�r�me Forrissier
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 Version 2.0.0, packaged on July 2010.

 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*****************************************************************************/

//! \file glc_filetoworld.cpp implementation of the GLC_FileToWorld class.


#include "glc_fileloader.h"

#include "glc_objtoworld.h"
#include "glc_stltoworld.h"
#include "glc_offtoworld.h"
#include "glc_3dstoworld.h"
#include "glc_3dxmltoworld.h"
#include "glc_colladatoworld.h"
#include "glc_bsreptoworld.h"

#include "../sceneGraph/glc_world.h"
#include "../glc_fileformatexception.h"

//////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////
GLC_FileLoader::GLC_FileLoader(const QGLContext *pContext)
: m_pQGLContext(pContext)
{
}

GLC_FileLoader::~GLC_FileLoader()
{
}

/////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

// Create an GLC_World from an input File
GLC_World GLC_FileLoader::createWorldFromFile(QFile &file, QStringList* pAttachedFileName)
{
#if defined(Q_OS_WIN)
	// We need to force the connection type on Windows, not sure why:
	// Qt::DirectConnection should be selected automatically since
	// source and destination are in the same thread...
#define connect(snd, sig, rcv, memb) \
        connect(snd, sig, rcv, memb, Qt::DirectConnection)
#endif

	GLC_World* pWorld= NULL;
	if (QFileInfo(file).suffix().toLower() == "obj")
	{
		GLC_ObjToWorld objToWorld(m_pQGLContext);
		connect(&objToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= objToWorld.CreateWorldFromObj(file);
		if (NULL != pAttachedFileName)
		{
			(*pAttachedFileName)= objToWorld.listOfAttachedFileName();
		}
	}
	else if (QFileInfo(file).suffix().toLower() == "stl")
	{
		GLC_StlToWorld stlToWorld;
		connect(&stlToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= stlToWorld.CreateWorldFromStl(file);
	}
	else if (QFileInfo(file).suffix().toLower() == "off")
	{
		GLC_OffToWorld offToWorld;
		connect(&offToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= offToWorld.CreateWorldFromOff(file);
	}
	else if (QFileInfo(file).suffix().toLower() == "3ds")
	{
		GLC_3dsToWorld studioToWorld(m_pQGLContext);
		connect(&studioToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= studioToWorld.CreateWorldFrom3ds(file);
		if (NULL != pAttachedFileName)
		{
			(*pAttachedFileName)= studioToWorld.listOfAttachedFileName();
		}
	}
	else if (QFileInfo(file).suffix().toLower() == "3dxml")
	{
		GLC_3dxmlToWorld d3dxmlToWorld(m_pQGLContext);
		connect(&d3dxmlToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= d3dxmlToWorld.createWorldFrom3dxml(file, false);
		if (NULL != pAttachedFileName)
		{
			(*pAttachedFileName)= d3dxmlToWorld.listOfAttachedFileName();
		}
	}
	else if (QFileInfo(file).suffix().toLower() == "dae")
	{
		GLC_ColladaToWorld colladaToWorld(m_pQGLContext);
		connect(&colladaToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= colladaToWorld.CreateWorldFromCollada(file);
		if (NULL != pAttachedFileName)
		{
			(*pAttachedFileName)= colladaToWorld.listOfAttachedFileName();
		}
	}
	else if (QFileInfo(file).suffix().toLower() == "bsrep")
	{
		GLC_BSRepToWorld bsRepToWorld;
		pWorld= bsRepToWorld.CreateWorldFromBSRep(file);
		emit currentQuantum(100);
	}

	if (NULL == pWorld)
	{
		// File extension not recognize or file not loaded
		QString message(QString("GLC_Factory::createWorldFromFile File ") + file.fileName() + QString(" not loaded"));
		GLC_FileFormatException fileFormatException(message, file.fileName(), GLC_FileFormatException::FileNotSupported);
		throw(fileFormatException);
	}
	GLC_World resulWorld(*pWorld);
	delete pWorld;

	return resulWorld;
}