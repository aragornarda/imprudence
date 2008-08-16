/** 
 * @file llvfsthread.h
 * @brief LLVFSThread definition
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

#ifndef LL_LLVFSTHREAD_H
#define LL_LLVFSTHREAD_H

#include <queue>
#include <string>
#include <map>
#include <set>

#include "llapr.h"

#include "llqueuedthread.h"

#include "llvfs.h"

//============================================================================

class LLVFSThread : public LLQueuedThread
{
	//------------------------------------------------------------------------
public:
	enum operation_t {
		FILE_READ,
		FILE_WRITE,
		FILE_RENAME
	};

	//------------------------------------------------------------------------
public:

	class Request : public QueuedRequest
	{
	protected:
		~Request() {}; // use deleteRequest()
		
	public:
		Request(handle_t handle, U32 priority, U32 flags,
				operation_t op, LLVFS* vfs,
				const LLUUID &file_id, const LLAssetType::EType file_type,
				U8* buffer, S32 offset, S32 numbytes);

		S32 getBytesRead()
		{
			return mBytesRead;
		}
		S32 getOperation()
		{
			return mOperation;
		}
		U8* getBuffer()
		{
			return mBuffer;
		}
		LLVFS* getVFS()
		{
			return mVFS;
		}
		std::string getFilename()
		{
			char tbuf[40];
			mFileID.toString(tbuf);
			return std::string(tbuf);
		}
		
		/*virtual*/ void finishRequest();
		/*virtual*/ void deleteRequest();

		bool processIO();
		
	private:
		operation_t mOperation;
		
		LLVFS* mVFS;
		LLUUID mFileID;
		LLAssetType::EType mFileType;
		
		U8* mBuffer;	// dest for reads, source for writes, new UUID for rename
		S32 mOffset;	// offset into file, -1 = append (WRITE only)
		S32 mBytes;		// bytes to read from file, -1 = all (new mFileType for rename)
		S32	mBytesRead;	// bytes read from file
	};

	//------------------------------------------------------------------------
public:
	static std::string sDataPath;
	static void setDataPath(const std::string& path) { sDataPath = path; }
	
public:
	LLVFSThread(bool threaded = TRUE, bool runalways = TRUE);
	~LLVFSThread();	

	// Return a Request handle
	handle_t read(LLVFS* vfs, const LLUUID &file_id, const LLAssetType::EType file_type,
				  U8* buffer, S32 offset, S32 numbytes, U32 pri=PRIORITY_NORMAL, U32 flags = 0);
	handle_t write(LLVFS* vfs, const LLUUID &file_id, const LLAssetType::EType file_type,
				   U8* buffer, S32 offset, S32 numbytes, U32 flags);
	handle_t rename(LLVFS* vfs, const LLUUID &file_id, const LLAssetType::EType file_type,
					const LLUUID &new_id, const LLAssetType::EType new_type, U32 flags);
	// Return number of bytes read
	S32 readImmediate(LLVFS* vfs, const LLUUID &file_id, const LLAssetType::EType file_type,
					  U8* buffer, S32 offset, S32 numbytes);
	S32 writeImmediate(LLVFS* vfs, const LLUUID &file_id, const LLAssetType::EType file_type,
					   U8* buffer, S32 offset, S32 numbytes);

	/*virtual*/ bool processRequest(QueuedRequest* req);

	static void initClass(bool local_is_threaded = TRUE, bool run_always = TRUE); // Setup sLocal
	static S32 updateClass(U32 ms_elapsed);
	static void cleanupClass();		// Delete sLocal

public:
	static LLVFSThread* sLocal;		// Default worker thread
};

//============================================================================


#endif // LL_LLVFSTHREAD_H
