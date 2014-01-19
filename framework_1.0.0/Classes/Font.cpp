/*
 *  Font.cpp
 *  framework
 *
 *  Created by Johnny Chan on 11/22/09.
 *  Copyright 2009 LlamaFace. All rights reserved.
 *
 */

#include <OpenGLES/ES1/gl.h>
#include "Font.h"
#include "Sprite.h"
#include "Graphics.h"
#include <string.h>
#include "SystemDefines.h"



CFont::CFont()
{

}


CFont::~CFont()
{

}


void CFont::init(const fontLoadInfo** fontInfo, int numFonts)
{
	_gap_offset = 0.0;
	_num_fonts = numFonts;
	
	//_font_table = new fontType*[numFonts];
	//memset(_font_table, 0, sizeof(fontType*) * numFonts);
	_font_table = ArrayList<fontType>::alloc(numFonts);
	
	// For convenience.
	fontLoadInfo theFontInfo;
	
	// Loop over all font infos and load them into memory now.
	for (int i = 0; i < numFonts; ++i)
	{
		//_font_table[i] = new fontType;
		//memset(_font_table[i], 0, sizeof(fontType));
		
		theFontInfo = ((fontLoadInfo*)fontInfo)[i];
		
		// Load the image of the font character table.
		//_font_table[i]._font_sprite_table = new CSprite();
		_font_table[i]._font_sprite_table.init();
		_font_table[i]._font_sprite_table.loadSpriteImage((char*)theFontInfo.image_file_name);
		
		_font_table[i]._font_char_width = theFontInfo.font_char_width;
		_font_table[i]._font_char_height = theFontInfo.font_char_height;
		_font_table[i]._font_chars_per_row = theFontInfo.font_chars_per_row;
		_font_table[i]._font_chars_per_col = theFontInfo.font_chars_per_col;
		_font_table[i]._font_start_char = theFontInfo.font_start_char;
		_font_table[i]._font_table_offset_x = theFontInfo.font_table_offset_x;
		_font_table[i]._font_table_offset_y = theFontInfo.font_table_offset_y;
	}
	
}


void CFont::destroy()
{
	for (int i = 0; i < _num_fonts; ++i)
	{
		_font_table[i]._font_sprite_table.destroy();
	}
	_font_table = NULL;
}


void CFont::setAlpha(int font, GLfloat alpha)
{
	_font_table[font]._font_sprite_table._color.a = alpha;
}


int CFont::getFontCharWidth(int font)
{
	return _font_table[font]._font_char_width;
}


int CFont::getFontCharHeight(int font)
{
	return _font_table[font]._font_char_height;
}


int CFont::getStringWidth(int font, const char* str)
{
	return strlen(str) * (_font_table[font]._font_char_width + _gap_offset);
}


void CFont::drawString(int font, const char* str, int x, int y, float alpha)
{
	int char_x = x;
	
	// Set the alpha value first before drawing the string.
	_font_table[font]._font_sprite_table._color.a = alpha;
	
	for (int i = 0; i < strlen(str); ++i)
	{
		drawFontChar(font, str[i], char_x, y);
		char_x += _font_table[font]._font_char_width + _gap_offset;
	}
}


void CFont::draw3DString(int font, const char* str, int x, int y, int z, float alpha)
{
	int char_x = x;
	
	// Set the alpha value first before drawing the string.
	_font_table[font]._font_sprite_table._color.a = alpha;
	
	for (int i = 0; i < strlen(str); ++i)
	{
		draw3DFontChar(font, str[i], char_x, y, z);
		char_x += _font_table[font]._font_char_width + _gap_offset;
	}
}


void CFont::drawFontChar(int font, int fontChar, int x, int y)
{
	int font_offset_x = ((fontChar - _font_table[font]._font_start_char) % _font_table[font]._font_chars_per_row) * _font_table[font]._font_char_width;
	int font_offset_y = ((fontChar - _font_table[font]._font_start_char) / _font_table[font]._font_chars_per_row) * _font_table[font]._font_char_height;
	
	CGraphics::drawSprite(
			  _font_table[font]._font_sprite_table, 
			  x, 
			  y, 
			  font_offset_x + _font_table[font]._font_table_offset_x,
			  font_offset_y + _font_table[font]._font_table_offset_y,
			  _font_table[font]._font_char_width,
			  _font_table[font]._font_char_height);
}


void CFont::drawFontChar(int font, int fontChar, int x, int y, int offsetX, int offsetY, int clipX, int clipY)
{
	int font_offset_x = ((fontChar - _font_table[font]._font_start_char) % _font_table[font]._font_chars_per_row) * _font_table[font]._font_char_width;
	int font_offset_y = ((fontChar - _font_table[font]._font_start_char) / _font_table[font]._font_chars_per_row) * _font_table[font]._font_char_height;
	
	CGraphics::drawSprite(
						  _font_table[font]._font_sprite_table, 
						  x, 
						  y, 
						  font_offset_x + _font_table[font]._font_table_offset_x + offsetX,
						  font_offset_y + _font_table[font]._font_table_offset_y + offsetY,
						  clipX,
						  clipY);
}


void CFont::draw3DFontChar(int font, int fontChar, int x, int y, int z)
{
	int font_offset_x = ((fontChar - _font_table[font]._font_start_char) % _font_table[font]._font_chars_per_row) * _font_table[font]._font_char_width;
	int font_offset_y = ((fontChar - _font_table[font]._font_start_char) / _font_table[font]._font_chars_per_row) * _font_table[font]._font_char_height;
	
	CGraphics::draw3DSprite(
							_font_table[font]._font_sprite_table, 
							(float)x, 
							(float)y,
							(float)z,
							(float)font_offset_x + _font_table[font]._font_table_offset_x,
							(float)font_offset_y + _font_table[font]._font_table_offset_y,
							(float)_font_table[font]._font_char_width,
							(float)_font_table[font]._font_char_height);
	
}