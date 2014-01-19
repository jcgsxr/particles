/*
 *  Font.h
 *  framework
 *
 *  Created by Johnny Chan on 11/22/09.
 *  Copyright 2009 LlamaFace. All rights reserved.
 *
 */

#ifndef __FONT_H__
#define __FONT_H__

#include "Sprite.h"
#include "ArrayList.h"

//class CSprite;

/*! \struct fontType
 *	\brief The font type.
 *
 * A font type holds font dimensions, ascii data, and a font sprite table.
 */
typedef struct fontType
{
	int _font_char_width;		/*!< The pixel width of the font character. */
	int _font_char_height;		/*!< The pixel height of the font character. */
	int _font_chars_per_row;	/*!< The number of characters per row in the font sprite table. */
	int _font_chars_per_col;	/*!< The number of characters per column in the font sprite table. */
	int _font_start_char;		/*!< The ascii value of the first character in the font sprite table. */
	int _font_table_offset_x;	/*!< The x offset to the top left corner of the font table in the font sprite table image. */
	int _font_table_offset_y;	/*!< The y offset to the top left corner of the font table in the font sprite table image. */
	CSprite _font_sprite_table;	/*!< The font sprite table CImage. */
} fontType;

/*! \struct fontLoadInfo
 *	\brief The font loading info.
 *
 * This is used for passing in font loading data upon initialization.
 */
typedef struct fontLoadInfo
{
	const char *image_file_name;		/*!< The image file name of the font sprite table. */
	int font_start_char;		/*!< The ascii value of the first character in the font sprite table. */
	int font_char_width;		/*!< The pixel width of the font character. */
	int font_char_height;		/*!< The pixel height of the font character. */
	int font_chars_per_row;		/*!< The number of characters per row in the font sprite table. */
	int font_chars_per_col;		/*!< The number of characters per column in the font sprite table. */
	int font_table_offset_x;	/*!< The x offset to the top left corner of the font table in the font sprite table image. */
	int font_table_offset_y;	/*!< The y offset to the top left corner of the font table in the font sprite table image. */
} fontLoadInfo;
	

/*! \class CFont
 * \brief The Font class.
 *
 * The font class handles custom fonts loaded from a font sprite table.
 * Font sprite tables are organized in ascii order so that font characters can be calculated using a standard formula, given number of characters per row and column in the sprite table.
 */
class CFont
{
public:	
	/*! \fn CFont()
	 *  \brief The CFont class constructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	CFont();
	
	/*! \fn ~CFont()
	 *  \brief The CFont class destructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	~CFont();
	
	/*! \fn init(const fontLoadInfo** fontInfo, int numFonts)
	 *  \brief The CFont class initializer function.
	 *  
	 *	\param fontInfo Pointer to the font info data.
	 *	\param numFonts The number of fonts to load.
	 *  \return n/a
	 */
	void init(const fontLoadInfo** fontInfo, int numFonts);
	
	/*! \fn destroy(void)
	 *  \brief The CFont class destroy function.
	 *  
	 * All memory allocated by the _font_table array is deallocated here.
	 *	\param n/a
	 *  \return n/a
	 */
	void destroy(void);
	
	/*! \fn getFontCharWidth(int font)
	 *  \brief Returns the font character width of the given font type.
	 *  
	 *	\param font The font type.
	 *  \return The font character width.
	 */
	int getFontCharWidth(int font);
	
	/*! \fn getFontCharHeight(int font)
	 *  \brief Returns the font character height of the given font type.
	 *  
	 *	\param font The font type.
	 *  \return The font character height.
	 */
	int getFontCharHeight(int font);
	
	/*! \fn getStringWidth(int font, const char* str)
	 *  \brief Returns the width of the string in pixels of the given string constructed by the given font type.
	 *  
	 *	\param font The font type.
	 *	\param str The string.
	 *  \return The width of the string constructed by the given font type.
	 */
	int getStringWidth(int font, const char* str);
	
	/*! \fn setAlpha(int font, GLfloat alpha)
	 *  \brief Sets the alpha value of the given font type.
	 *  
	 * This can be used to initiate alpha blending for fonts. Useful for fading fonts in or out.
	 *	\param font The font type.
	 *	\param alpha The alpha value. 0.0 indicates completely transparent, 1.0 indicates fully opaque.
	 *  \return n/a
	 */
	void setAlpha(int font, GLfloat alpha);
	
	/*! \fn drawString(int font, const char* str, int x, int y, float alpha = 1.0)
	 *  \brief Renders a string with the given font type on the screen.
	 *  
	 *	\param font The font type.
	 *	\param str The string.
	 *	\param x The x location on the screen to render the string.
 	 *	\param y The y location on the screen to render the string.
 	 *	\param alpha The alpha value of the string.
	 *  \return n/a
	 */
	void drawString(int font, const char* str, int x, int y, float alpha = 1.0);
	
	/*! \fn draw3DString(int font, const char* str, int x, int y, int z, float alpha = 1.0)
	 *  \brief Renders a string with the given font type on the screen.
	 *  
	 * The string is rendered in the 3D space.
	 *	\param font The font type.
	 *	\param str The string.
	 *	\param x The x location on the screen to render the string.
 	 *	\param y The y location on the screen to render the string.
 	 *	\param z The z location on the screen to render the string.
 	 *	\param alpha The alpha value of the string.
	 *  \return n/a
	 */
	void draw3DString(int font, const char* str, int x, int y, int z, float alpha = 1.0);
	
	/*! \fn drawFontChar(int font, int fontChar, int x, int y)
	 *  \brief Renders one font character onto the screen.
	 *
	 * Used mainly as a helper function for drawString(). This is rendered in the 2D space.  
	 *	\param font The font type.
	 *	\param fontChar The character to render.
	 *	\param x The x location on the screen to render the character.
 	 *	\param y The y location on the screen to render the character.
	 *  \return n/a
	 */
	void drawFontChar(int font, int fontChar, int x, int y);
	
	/*! \fn drawFontChar(int font, int fontChar, int x, int y, int offsetX, int offsetY, int clipX, int clipY)
	 *  \brief Renders one font character onto the screen.
	 *
	 * Used mainly as a helper function for drawString(). This is rendered in the 2D space and is used when a custom clip rectangle is required. 
	 *	\param font The font type.
	 *	\param fontChar The character to render.
	 *	\param x The x location on the screen to render the character.
 	 *	\param y The y location on the screen to render the character.
	 *	\param offsetX The x offset into the character.
	 *	\param offsetY The y offset into the character.
	 *	\param clipX The clip width for the character.
	 *	\param clipY The clip height for the character.
	 *  \return n/a
	 */
	void drawFontChar(int font, int fontChar, int x, int y, int offsetX, int offsetY, int clipX, int clipY);
	
	
	/*! \fn draw3DFontChar(int font, int fontChar, int x, int y, int z)
	 *  \brief Renders one font character onto the screen.
	 *  
	 * Used mainly as a helper function for drawString(). This is rendered in the 3D space.
	 *	\param font The font type.
	 *	\param fontChar The character to render.
	 *	\param x The x location on the screen to render the character.
 	 *	\param y The y location on the screen to render the character.
	 *  \return n/a
	 */
	void draw3DFontChar(int font, int fontChar, int x, int y, int z);
	
	/*! \fn setScale(int font, float x, float y)
	 *  \brief Sets the rendering scale for subsequent string draws.
	 *  
	 *	\param font The font type to scale.
	 *	\param x The x rendering scale.
 	 *	\param y The y rendering scale.
	 *  \return n/a
	 */
	inline void setScale(int font, float x, float y) { _font_table[font]._font_sprite_table._scale.x = x; _font_table[font]._font_sprite_table._scale.y = y; }
	
	/*! \fn setScale(int font, float scale)
	 *  \brief Sets the rendering scale for subsequent string draws.
	 *
	  *	\param font The font type to scale.
	 *	\param scale The rendering scale.
	 *  \return n/a
	 */
	inline void setScale(int font, float scale) { _font_table[font]._font_sprite_table._scale = Vector3(scale, scale, scale); }
	
	/*! \fn setGapOffset(float offset)
	 *  \brief Sets the gap offset between letters.
	 *
	 *	\param offset The gap offset.
	 *  \return n/a
	 */
	inline void setGapOffset(float offset) { _gap_offset = offset; }
	
private:

	ArrayList<fontType> _font_table;	/*!< The font table. */
	
	int _num_fonts;			/*!< The total number of font types loaded. */
	
	float _gap_offset;	/*!< Any extra spacing between letters. */
};


#endif