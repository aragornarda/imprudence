/** 
 * @file llvoground.cpp
 * @brief LLVOGround class implementation
 *
 * Copyright (c) 2001-2007, Linden Research, Inc.
 * 
 * The source code in this file ("Source Code") is provided by Linden Lab
 * to you under the terms of the GNU General Public License, version 2.0
 * ("GPL"), unless you have obtained a separate licensing agreement
 * ("Other License"), formally executed by you and Linden Lab.  Terms of
 * the GPL can be found in doc/GPL-license.txt in this distribution, or
 * online at http://secondlife.com/developers/opensource/gplv2
 * 
 * There are special exceptions to the terms and conditions of the GPL as
 * it is applied to this Source Code. View the full text of the exception
 * in the file doc/FLOSS-exception.txt in this software distribution, or
 * online at http://secondlife.com/developers/opensource/flossexception
 * 
 * By copying, modifying or distributing this software, you acknowledge
 * that you have read and understood your obligations described above,
 * and agree to abide by those obligations.
 * 
 * ALL LINDEN LAB SOURCE CODE IS PROVIDED "AS IS." LINDEN LAB MAKES NO
 * WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY,
 * COMPLETENESS OR PERFORMANCE.
 */

#include "llviewerprecompiledheaders.h"

#include "llvoground.h"

#include "llviewercontrol.h"

#include "llagent.h"
#include "lldrawable.h"
#include "llface.h"
#include "llsky.h"
#include "llviewercamera.h"
#include "llviewerregion.h"
#include "pipeline.h"

LLVOGround::LLVOGround(const LLUUID &id, const LLPCode pcode, LLViewerRegion *regionp)
:	LLViewerObject(id, pcode, regionp)
{
	mbCanSelect = FALSE;
}


LLVOGround::~LLVOGround()
{
}

BOOL LLVOGround::idleUpdate(LLAgent &agent, LLWorld &world, const F64 &time)
{
 	if (mDead || !(gPipeline.hasRenderType(LLPipeline::RENDER_TYPE_GROUND)))
	{
		return TRUE;
	}
	
	if (mDrawable)
	{
		gPipeline.markRebuild(mDrawable, LLDrawable::REBUILD_VOLUME, TRUE);
	}
	return TRUE;
}


void LLVOGround::updateTextures(LLAgent &agent)
{
}


LLDrawable *LLVOGround::createDrawable(LLPipeline *pipeline)
{
	pipeline->allocDrawable(this);
	mDrawable->setLit(FALSE);

	LLDrawPool *poolp = gPipeline.getPool(LLDrawPool::POOL_GROUND);

	mDrawable->addFace(poolp, NULL);

	return mDrawable;
}

BOOL LLVOGround::updateGeometry(LLDrawable *drawable)
{
	LLStrider<LLVector3> verticesp;
	LLStrider<LLVector3> normalsp;
	LLStrider<LLVector2> texCoordsp;
	U32 *indicesp;
	S32 index_offset;
	LLFace *face;	

	if (drawable->getNumFaces() < 1)
		drawable->addFace(gPipeline.getPool(LLDrawPool::POOL_GROUND), NULL);
	face = drawable->getFace(0); 
	face->setPrimType(LLTriangles);
	face->setSize(6, 12);
	index_offset = face->getGeometry(verticesp,normalsp,texCoordsp, indicesp);
	if (-1 == index_offset)
	{
		return TRUE;
	}

	///////////////////////////////////////
	//
	//
	//
	LLVector3 at_dir = gCamera->getAtAxis();
	at_dir.mV[VZ] = 0.f;
	if (at_dir.normVec() < 0.01)
	{
		// We really don't care, as we're not looking anywhere near the horizon.
	}
	LLVector3 left_dir = gCamera->getLeftAxis();
	left_dir.mV[VZ] = 0.f;
	left_dir.normVec();

	// Our center top point
	LLVector3 center_top, center_bottom;

	LLColor4 ground_color = gSky.getFogColor();
	ground_color.mV[3] = 1.f;
	face->setFaceColor(ground_color);
	
	if (gCamera->getOrigin().mV[VZ] < gAgent.getRegion()->getWaterHeight())
	{
		// Underwater
		//center_top = gCamera->getOrigin() + at_dir*gCamera->getFar();
		center_top = gCamera->getOrigin() - LLVector3(0, 0, 5);
		center_bottom = gCamera->getOrigin() + at_dir*gCamera->getFar();;
		//center_top.mV[VZ] = gAgent.getRegion()->getWaterHeight() + 0.5f;
		center_bottom.mV[VZ] = -100.f;
	}
	else
	{
		// Above water
		center_top = gCamera->getOrigin() - LLVector3(0, 0, 30);
		if ((gPipeline.getVertexShaderLevel(LLPipeline::SHADER_ENVIRONMENT) > 0))
		{
			center_top.mV[VZ] = gAgent.getRegion()->getWaterHeight();
		}
		//center_top = gCamera->getOrigin() + at_dir*9000.f;
		center_bottom = gCamera->getOrigin() - at_dir*gCamera->getFar();
		//center_top.mV[VZ] = 0.f;
		//center_bottom.mV[VZ] = gAgent.getRegion()->getWaterHeight();
	}

	*(verticesp++)  = center_top + LLVector3(6000, 6000, 0);
	*(verticesp++)  = center_top + LLVector3(-6000, 6000, 0);
	*(verticesp++)  = center_top + LLVector3(-6000, -6000, 0);
	
	*(verticesp++)  = center_top + LLVector3(-6000, -6000, 0);
	*(verticesp++)  = center_top + LLVector3(6000, -6000, 0);
	*(verticesp++)  = center_top + LLVector3(6000, 6000, 0);
	
	
	// Triangles for each side
	*indicesp++ = index_offset + 0;
	*indicesp++ = index_offset + 1;
	*indicesp++ = index_offset + 3;

	*indicesp++ = index_offset + 0;
	*indicesp++ = index_offset + 3;
	*indicesp++ = index_offset + 2;

	*indicesp++ = index_offset + 2;
	*indicesp++ = index_offset + 3;
	*indicesp++ = index_offset + 5;

	*indicesp++ = index_offset + 2;
	*indicesp++ = index_offset + 5;
	*indicesp++ = index_offset + 4;

	*(texCoordsp++) = LLVector2(0.f, 0.f);
	*(texCoordsp++) = LLVector2(1.f, 0.f);
	*(texCoordsp++) = LLVector2(0.f, 1.f);
	*(texCoordsp++) = LLVector2(1.f, 1.f);
	*(texCoordsp++) = LLVector2(0.f, 2.f);
	*(texCoordsp++) = LLVector2(1.f, 2.f);

	LLPipeline::sCompiles++;
	return TRUE;
}
