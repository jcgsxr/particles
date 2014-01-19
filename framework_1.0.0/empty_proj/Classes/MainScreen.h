/*
 *  MainScreen.h
 *  framework
 *
 *  Created by Johnny Chan on 10/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __MAINSCREEN_H__
#define __MAINSCREEN_H__

class CImage;
class CSprite;

class CMainScreen : public CBasicInterface
{	
public:	
	CMainScreen();
	~CMainScreen();
	
	void init(void);
	void destroy(void);
	void draw(void);
	void update(void);
	void handleTouch(float x, float y, eTouchPhase phase);
	void handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2);

private:
	// Visible bg color.
	color _bg_color;	
};


#endif