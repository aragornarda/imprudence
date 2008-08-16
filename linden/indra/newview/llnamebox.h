/** 
 * @file llnamebox.h
 * @brief display and refresh a name from the name cache
 *
 * Copyright (c) 2003-2007, Linden Research, Inc.
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

#ifndef LL_LLNAMEBOX_H
#define LL_LLNAMEBOX_H

#include <set>

#include "llview.h"
#include "llstring.h"
#include "llfontgl.h"
#include "linked_lists.h"
#include "lltextbox.h"

class LLNameBox
:	public LLTextBox
{
public:
	LLNameBox(const std::string& name, const LLRect& rect, const LLUUID& name_id = LLUUID::null, BOOL is_group = FALSE, const LLFontGL* font = NULL, BOOL mouse_opaque = TRUE );
		// By default, follows top and left and is mouse-opaque.
		// If no text, text = name.
		// If no font, uses default system font.
	virtual ~LLNameBox();

	void setNameID(const LLUUID& name_id, BOOL is_group);

	void refresh(const LLUUID& id, const char* first, const char* last,
				 BOOL is_group);

	static void refreshAll(const LLUUID& id, const char* firstname,
						   const char* lastname, BOOL is_group);

private:
	static std::set<LLNameBox*> sInstances;

private:
	LLUUID mNameID;

};

#endif
