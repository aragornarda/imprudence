/** 
 * @file llfloatereditui.cpp
 * @author James Cook
 * @date May 2005
 * @brief In-world user interface editor
 *
 * Copyright (c) 2005-2007, Linden Research, Inc.
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

#include "llfloatereditui.h"

#include "lluiconstants.h"
#include "llbutton.h"
#include "lllineeditor.h"
#include "llspinctrl.h"
#include "lltextbox.h"

LLFloaterEditUI* LLFloaterEditUI::sInstance = NULL;

void	LLFloaterEditUI::navigateHierarchyButtonPressed(void*	data)
{
	LLView* view = LLView::sEditingUIView;
	if( !view ) return;
	LLView*	parent = view->getParent();
	const LLView::child_list_t*	viewChildren = view->getChildList();
	const LLView::child_list_t*	parentChildren = parent->getChildList();
	//LLView::child_list_t::iterator
	std::list<LLView*>::const_iterator	itor;
	std::list<LLView*>::size_type	idx;
	std::list<LLView*>::size_type	sidx;
	for(idx = 0,itor = parentChildren->begin();itor!=parentChildren->end();itor++,idx++){
		if((*itor)==view)break;
	}
	switch((int)data)
	{
		case	0	://up
			view = view->getParent();
		break;
		case	1	://down
			view = viewChildren->begin()!=viewChildren->end() ? (*viewChildren->begin()) : NULL;
		break;
		case	2	://left
		{			
			if(idx==0)
				idx = parentChildren->size()-1;
			else
				idx--;
			if( idx < 0 || idx >= parentChildren->size())break;
			for(sidx = 0,itor = parentChildren->begin();itor!=parentChildren->end();itor++,sidx++){
				if(sidx == idx)
				{
					view = (*itor);
					break;
				}
			}
		}
		break;
		case	3	://right
		{
			if(idx==parentChildren->size()-1)
				idx = 0;
			else
				idx++;
			if( idx < 0 || idx >= parentChildren->size())break;
			for(sidx = 0,itor = parentChildren->begin();itor!=parentChildren->end();itor++,sidx++){
				if(sidx == idx)
				{
					view = (*itor);
					break;
				}
			}
		}
		break;
	}
	if (view)
	{
		sEditingUIView = view;
		sInstance->refresh();
	}
}
	
LLFloaterEditUI::LLFloaterEditUI()
:	LLFloater("floater_ui_editor", LLRect(0, 200, 200, 0), "Edit User Interface"),
	mLastView(NULL),
	mLabelLine(NULL),
	mWidthSpin(NULL),
	mHeightSpin(NULL)
{
	LLView::sEditingUI = TRUE;

	S32 x = HPAD;
	S32 y = getRect().getHeight() - LINE - LINE - VPAD;
	const S32 R1 = HPAD + 40;

	LLLineEditor* line = NULL;
	LLSpinCtrl* spin = NULL;
	LLTextBox* text = NULL;

	LLButton*	button = NULL;


	text = new LLTextBox("Selected UI Widget:", LLRect(x, y+16, x+100, y));
	addChild(text);
	y -= VPAD + 16;

	text = new LLTextBox("Label:", LLRect(x, y+16, x+40, y));
	addChild(text);
	x = R1;

	line = new LLLineEditor("label_line", LLRect(x, y+20, x+100, y),
		"",
		NULL,
		254,
		onCommitLabel,
		NULL,
		NULL,
		this);
	addChild(line);
	mLabelLine = line;

	x = HPAD;
	y -= VPAD + 20;

	spin = new LLSpinCtrl("height_spin", LLRect(x, y+20, x+100, y),
		"Height:", LLFontGL::sSansSerifSmall,
		onCommitHeight,
		this,
		0.f,
		2.f,
		1000.f,
		1.f);
	spin->setPrecision(0);
	addChild(spin);
	mHeightSpin = spin;

	y -= VPAD + 20;

	spin = new LLSpinCtrl("width_spin", LLRect(x, y+20, x+100, y),
		"Width:", LLFontGL::sSansSerifSmall,
		onCommitWidth,
		this,
		0.f,
		2.f,
		1000.f,
		1.f);
	spin->setPrecision(0);
	addChild(spin);
	mWidthSpin = spin;

	y -= VPAD + 20;

	text = new LLTextBox("XML Name:", LLRect(x, y+16, x+60, y));
	addChild(text);
	x+=60;
	text = new LLTextBox("xml_name", LLRect(x, y+16, x+100, y));
	addChild(text);
	x-=50;

	y -= VPAD + 20;

	x += 40;
	button = new LLButton("up",LLRect(x, y+16, x+32, y));
	addChild(button);
	x -= 40;
	y -= VPAD + 20;
	button = new LLButton("<<",LLRect(x, y+16, x+32, y));
	addChild(button);
	x += 40;
	button = new LLButton("rfrsh",LLRect(x, y+16, x+32, y));
	addChild(button);
	x += 40;
	button = new LLButton(">>",LLRect(x, y+16, x+32, y));
	addChild(button);
	x -= 40;
	y -= VPAD + 20;
	button = new LLButton("dn",LLRect(x, y+16, x+32, y));
	addChild(button);

	childSetAction("up",navigateHierarchyButtonPressed,(void*)0);
	childSetAction("dn",navigateHierarchyButtonPressed,(void*)1);
	childSetAction("<<",navigateHierarchyButtonPressed,(void*)2);
	childSetAction(">>",navigateHierarchyButtonPressed,(void*)3);
	childSetAction("rfrsh",navigateHierarchyButtonPressed,(void*)4);
	sInstance = this;
}

// virtual
LLFloaterEditUI::~LLFloaterEditUI()
{
	LLView::sEditingUI = FALSE;
	LLView::sEditingUIView = NULL;
	sInstance = NULL;
}

// virtual
void LLFloaterEditUI::draw()
{
	refresh();
	LLFloater::draw();
}

void LLFloaterEditUI::refresh()
{
	LLView* view = LLView::sEditingUIView;

	// same selection
	if (view == mLastView) return;

	// user deselected
	if (!view) 
	{
		mLastView = NULL;
		mLabelLine->setText("");
		mLabelLine->setEnabled(FALSE);
		mWidthSpin->set(0.f);
		mWidthSpin->setEnabled(FALSE);
		mHeightSpin->set(0.f);
		mHeightSpin->setEnabled(FALSE);
		return;
	}

	// HACK - don't allow widgets in this window to be selected
	LLView* parent = view->getParent();
	while (parent)
	{
		if (parent == this)
		{
			// user selected one of our children, slam them back
			LLView::sEditingUIView = mLastView;
			return;
		}
		parent = parent->getParent();
	}

	refreshCore();

	mLastView = view;
}

void LLFloaterEditUI::refreshCore()
{
	LLView* view = LLView::sEditingUIView;

	LLRect r = view->getRect();
	F32 width = (F32)r.getWidth();
	F32 height = (F32)r.getHeight();
	mWidthSpin->set(width);
	mHeightSpin->set(height);

	EWidgetType widget_type = view->getWidgetType();
	switch (widget_type)
	{
	default:
	case WIDGET_TYPE_VIEW:
		refreshView(view);
		break;
	case WIDGET_TYPE_BUTTON:
		refreshButton(view);
		break;
	case WIDGET_TYPE_CHECKBOX:
	case WIDGET_TYPE_COLOR_SWATCH:
	case WIDGET_TYPE_COMBO_BOX:
	case WIDGET_TYPE_LINE_EDITOR:
	case WIDGET_TYPE_SCROLL_LIST:
	case WIDGET_TYPE_NAME_LIST:
	case WIDGET_TYPE_SLIDER:
	case WIDGET_TYPE_VOLUME_SLIDER:
	case WIDGET_TYPE_SPINNER:
	case WIDGET_TYPE_TEXT_EDITOR:
	case WIDGET_TYPE_TEXTURE_PICKER:
	case WIDGET_TYPE_TEXT_BOX:
	case WIDGET_TYPE_RADIO_GROUP:
	case WIDGET_TYPE_ICON:
		refreshView(view);
		break;
	}
}

void LLFloaterEditUI::refreshView(LLView* view)
{
	mLabelLine->setEnabled(FALSE);
	mLabelLine->setText("");
	childSetText("xml_name",view->getName());
}

void LLFloaterEditUI::refreshButton(LLView* view)
{
	LLButton* btn = (LLButton*)view;
	LLString label = btn->getLabelUnselected();
	mLabelLine->setEnabled(TRUE);
	mLabelLine->setText(label);
	childSetText("xml_name",view->getName());
}

// static
void LLFloaterEditUI::show(void*)
{
	LLFloaterEditUI* self = new LLFloaterEditUI();
	self->center();
	self->open();
}

// static
BOOL LLFloaterEditUI::handleKey(KEY key, MASK mask)
{
	if (!LLView::sEditingUIView) return FALSE;

	S32 step = 2;
	BOOL handled = FALSE;
	LLRect r = LLView::sEditingUIView->getRect();

	if (mask == MASK_NONE)
	{
		if (key == KEY_RIGHT)
		{
			r.translate(step,0);
			handled = TRUE;
		}
		else if (key == KEY_LEFT)
		{
			r.translate(-step,0);
			handled = TRUE;
		}
		else if (key == KEY_UP)
		{
			r.translate(0,step);
			handled = TRUE;
		}
		else if (key == KEY_DOWN)
		{
			r.translate(0,-step);
			handled = TRUE;
		}
	}
	else if (mask == MASK_SHIFT)
	{
		if (key == KEY_RIGHT)
		{
			r.mRight += step;
			handled = TRUE;
		}
		else if (key == KEY_LEFT)
		{
			r.mRight -= step;
			handled = TRUE;
		}
		else if (key == KEY_UP)
		{
			r.mTop += step;
			handled = TRUE;
		}
		else if (key == KEY_DOWN)
		{
			r.mTop -= step;
			handled = TRUE;
		}
	}

	if (handled)
	{
		LLView::sEditingUIView->reshape(r.getWidth(), r.getHeight());
		LLView::sEditingUIView->setRect(r);
		if (sInstance) sInstance->refreshCore();
	}

	return handled;
}

// static
void LLFloaterEditUI::onCommitLabel(LLUICtrl* ctrl, void* data)
{
	LLView* view = LLView::sEditingUIView;
	if (!view) return;

	LLLineEditor* line = (LLLineEditor*)ctrl;
	const LLString& text = line->getText();
	if (view->getWidgetType() == WIDGET_TYPE_BUTTON)
	{
		LLButton* btn = (LLButton*)view;
		btn->setLabelUnselected(text);
		btn->setLabelSelected(text);
	}
}

// static
void LLFloaterEditUI::onCommitHeight(LLUICtrl* ctrl, void* data)
{
	LLView* view = LLView::sEditingUIView;
	if (!view) return;

	LLSpinCtrl* spin = (LLSpinCtrl*)ctrl;
	F32 height = spin->get();
	LLRect r = view->getRect();
	r.mTop = r.mBottom + (S32)height;
	view->reshape(r.getWidth(), r.getHeight());
	view->setRect(r);
}

// static
void LLFloaterEditUI::onCommitWidth(LLUICtrl* ctrl, void* data)
{
	LLView* view = LLView::sEditingUIView;
	if (!view) return;

	LLSpinCtrl* spin = (LLSpinCtrl*)ctrl;
	F32 width = spin->get();
	LLRect r = view->getRect();
	r.mRight = r.mLeft + (S32)width;
	view->reshape(r.getWidth(), r.getHeight());
	view->setRect(r);
}
