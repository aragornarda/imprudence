/** 
 * @file lltoolmgr.h
 * @brief LLToolMgr class header file
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

#ifndef LL_TOOLMGR_H
#define LL_TOOLMGR_H

#include "doublelinkedlist.h"
#include "llkeyboard.h"

class LLTool;
class LLToolset;

// Key bindings for common operations
const MASK MASK_VERTICAL		= MASK_CONTROL;
const MASK MASK_SPIN			= MASK_CONTROL | MASK_SHIFT;
const MASK MASK_ZOOM			= MASK_NONE;
const MASK MASK_ORBIT			= MASK_CONTROL;
const MASK MASK_PAN				= MASK_CONTROL | MASK_SHIFT;
const MASK MASK_COPY			= MASK_SHIFT;

class LLToolMgr
{
public:
	LLToolMgr();
	~LLToolMgr();

	// Must be called after gSavedSettings set up.
	void			initTools();

	LLTool*			getCurrentTool(MASK override_mask);

	BOOL			inEdit();
	void			useSelectedTool( LLToolset* vp );

	void			setTransientTool(LLTool* tool);
	void			clearTransientTool();
	BOOL			usingTransientTool();

	void			onAppFocusGained();
	void			onAppFocusLost();

protected:
	friend class LLToolset;  // to allow access to setCurrentTool();
	void			setCurrentTool(LLTool* tool);

protected:
	LLTool*			mCurrentTool;
	LLTool*			mSavedTool;		// The current tool at the time application focus was lost.
	LLTool*			mTransientTool;
	LLTool*			mOverrideTool; // Tool triggered by keyboard override
};

// Sets of tools for various modes
class LLToolset
{
public:
	LLToolset() : mSelectedTool(NULL) {}

	LLTool*			getSelectedTool()				{ return mSelectedTool; }

	void			addTool(LLTool* tool);

	void			selectTool( LLTool* tool );
	void			selectToolByIndex( S32 index );
	void			selectFirstTool();
	void			selectNextTool();
	void			selectPrevTool();

	void			handleScrollWheel(S32 clicks);

	BOOL			isToolSelected( S32 index );

protected:
	LLTool*			mSelectedTool;
	LLDoubleLinkedList<LLTool> mToolList;
};

// Handy callbacks for switching tools
void select_tool(void *tool);


// Globals (created and destroyed by LLViewerWindow)
extern LLToolMgr*   gToolMgr;

extern LLToolset* gCurrentToolset;
extern LLToolset* gBasicToolset;
extern LLToolset *gCameraToolset;
//extern LLToolset *gLandToolset;
extern LLToolset* gMouselookToolset;
extern LLToolset* gFaceEditToolset;

extern LLTool*		gToolNull;

#endif
