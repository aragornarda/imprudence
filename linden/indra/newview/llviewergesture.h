/** 
 * @file llviewergesture.h
 * @brief LLViewerGesture class header file
 *
 * Copyright (c) 2002-2007, Linden Research, Inc.
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

#ifndef LL_LLVIEWERGESTURE_H
#define LL_LLVIEWERGESTURE_H

#include "llanimationstates.h"
#include "lluuid.h"
#include "llstring.h"
#include "lldarray.h"
#include "llgesture.h"

class LLMessageSystem;

class LLViewerGesture : public LLGesture
{
public:
	LLViewerGesture();
	LLViewerGesture(KEY key, MASK mask, const std::string &trigger, 
		const LLUUID &sound_item_id, const std::string &animation, 
		const std::string &output_string);

	LLViewerGesture(U8 **buffer, S32 max_size); // deserializes, advances buffer
	LLViewerGesture(const LLViewerGesture &gesture);

	// Triggers if a key/mask matches it
	virtual BOOL trigger(KEY key, MASK mask);

	// Triggers if case-insensitive substring matches (assumes string is lowercase)
	virtual BOOL trigger(const std::string &string);

	void doTrigger( BOOL send_chat );

protected:
	static const F32	SOUND_VOLUME;
};

class LLViewerGestureList : public LLGestureList
{
public:
	LLViewerGestureList();

	void saveToServer();
	//void requestFromServer();
	BOOL getIsLoaded() { return mIsLoaded; }

	//void requestResetFromServer( BOOL is_male );

	// See if the prefix matches any gesture.  If so, return TRUE
	// and place the full text of the gesture trigger into
	// output_str
	BOOL matchPrefix(const std::string& in_str, std::string* out_str);

	static void xferCallback(void *data, S32 size, void** /*user_data*/, S32 status);
	static void processGestureUpdate(LLMessageSystem *msg, void** /*user_data*/);

protected:
	LLGesture *create_gesture(U8 **buffer, S32 max_size);

protected:
	BOOL mIsLoaded;
};

extern LLViewerGestureList gGestureList;

#endif
