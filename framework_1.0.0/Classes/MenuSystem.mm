#include "MenuSystem.h"
#include "Sprite.h"
#include "Image.h"
#include "Graphics.h"
#include "Utils.h"
#include "AppDefines.h"
#include "GameData.h"

#import "SoundEngine.h"

#if defined ENABLE_MENU_SYSTEM

CMenuSystem::CMenuSystem()
{
	
}

CMenuSystem::~CMenuSystem()
{
	
}

void CMenuSystem::init()
{
	_curr_menu_stack_size = -1;
	
	//[[SoundSystem sharedSoundSystem] loadSoundWithKey:@"sound" file:@"testsound" ext:@"wav" loop:false];
	
	// TODO: Make this pass in a valid id.
	//pushMenu(0);
}


void CMenuSystem::destroy()
{
	for (int iMenu = 0; iMenu < _curr_menu_stack_size; ++iMenu)
	{
		for (int iMenuItem = 0; iMenuItem < _menu_stack[iMenu]->_num_menu_items; ++iMenuItem)
		{
			if (_menu_stack[iMenu]->_menu_items[iMenuItem]->_icon_sprite)
			{
				_menu_stack[iMenu]->_menu_items[iMenuItem]->_icon_sprite->destroy();
				delete _menu_stack[iMenu]->_menu_items[iMenuItem]->_icon_sprite;
				_menu_stack[iMenu]->_menu_items[iMenuItem]->_icon_sprite = NULL;
			}
			
			if (_menu_stack[iMenu]->_menu_items[iMenuItem]->_background_sprite)
			{
				_menu_stack[iMenu]->_menu_items[iMenuItem]->_background_sprite->destroy();
				delete _menu_stack[iMenu]->_menu_items[iMenuItem]->_background_sprite;
				_menu_stack[iMenu]->_menu_items[iMenuItem]->_background_sprite = NULL;
			}
		}
	}
}

menu* CMenuSystem::createMenu(int menuID)
{
	const menuData* menu_data = &appMenuData[menuID];
	menu* new_menu = new menu();
	
	// Grab the menu data and create a new menu with it.
	new_menu->_menu_rect = menu_data->_menu_bg_rect;
	new_menu->_num_menu_items = menu_data->_num_menu_items;
	new_menu->_menu_icon_format = MENU_FORMAT_NONE;
	
	for (int i = 0; i < menu_data->_num_menu_items; ++i)
	{
		new_menu->_menu_items[i] = new menuItem();
		new_menu->_menu_items[i]->_menu_item_rect = menu_data->_menu_item_rects[i];
		new_menu->_menu_items[i]->_action = menu_data->_menu_actions[i];
		
		if (menu_data->_icon_image_ids[i] == FILE_ID_IMAGE_NULL)
		{
			new_menu->_menu_items[i]->_icon_sprite = NULL;
		}
		new_menu->_menu_items[i]->_icon_sprite = new CSprite();
		// Account for icon offsets here.
		new_menu->_menu_items[i]->_icon_sprite->init(menu_data->_menu_item_rects[i].x + menu_data->_icon_offsets[i].x, 
													 menu_data->_menu_item_rects[i].y + menu_data->_icon_offsets[i].y);

		// Load the image, which will also take care of binding the texture.
		new_menu->_menu_items[i]->_icon_sprite->loadSpriteImage(menu_data->_icon_image_ids[i]);
		
		// Set the dimension of the icon sprite to match the dimension of the actual image since there
		// should be any physics associated with it.
		new_menu->_menu_items[i]->_icon_sprite->_width = new_menu->_menu_items[i]->_icon_sprite->getImage()->getWidth();
		new_menu->_menu_items[i]->_icon_sprite->_height = new_menu->_menu_items[i]->_icon_sprite->getImage()->getHeight();
		
		// Now deal with the background sprite.
		if (menu_data->_background_image_ids[i] == FILE_ID_IMAGE_NULL)
		{
			new_menu->_menu_items[i]->_background_sprite = NULL;
		}
		new_menu->_menu_items[i]->_background_sprite = new CSprite();
		new_menu->_menu_items[i]->_background_sprite->init();
		new_menu->_menu_items[i]->_background_sprite->loadSpriteImage(menu_data->_background_image_ids[i]);
	}
	
	return new_menu;
}

void CMenuSystem::pushMenu(int menuID)
{
	if (_curr_menu_stack_size >= MENUS_MAX - 1)
	{
		DPRINT_MENUSYS("CMenuSystem::pushMenu failed, reached max stack size");
		return;
	}
	
	_curr_menu_stack_size++;
	_menu_stack[_curr_menu_stack_size] = createMenu(menuID);
	
}

menu* CMenuSystem::getActiveMenu()
{
	if (_curr_menu_stack_size < 0)
	{
		return NULL;
	}
	
	return _menu_stack[_curr_menu_stack_size];
}


void CMenuSystem::handleTouch(float x, float y, eTouchPhase phase)
{
	switch (phase)
	{
		case TouchPhaseBegan:
		{
			if (getActiveMenu() == NULL)
			{
				return;
			}
			
			for (int i = 0; i < getActiveMenu()->_num_menu_items; ++i)
			{
				if (isPointInRect(x, y, getActiveMenu()->_menu_items[i]->_menu_item_rect))
				{
					[[SoundSystem sharedSoundSystem] playSound:@"sound"];
					if (getActiveMenu()->_menu_items[i]->_icon_sprite->_delta_angle.z == 0.0)
					{
						getActiveMenu()->_menu_items[i]->_icon_sprite->_delta_angle.z = 2.0;
					}
					else
					{
						getActiveMenu()->_menu_items[i]->_icon_sprite->_delta_angle.z = 0.0;
					}						
				}
			}
			break;
		}
		case TouchPhaseEnded:
		{
		
			break;
		}
		default:
			break;
	}
}

void CMenuSystem::executeAction(int action)
{
	switch (action)
	{
		case eMenuActionYes:
		{
			
			
			break;
		}
		case eMenuActionNo:
		{
			
			
			break;
		}
		case eMenuActionCloseMenu:
		{
			
			break;
		}
		case eMenuActionGotoTitleScreen:
		{
			
			break;
		}
		case eMenuActionGotoExitScreen:
		{
			
			break;
		}
		case eMenuActionGotoSplashScreen:
		{
			
			break;
		}
		case eMenuActionGotoMainScreen:
		{
			
			break;
		}
		case eMenuActionGotoHelpScreen:
		{
			
			break;
		}
		case eMenuActionOpenOptionsMenu:
		{
			
			break;
		}
		case eMenuActionOpenQuitMenu:
		{
			
			break;
		}
		case eMenuActionOpenConfirmationMenu:
		{
			
			break;
		}
		case eMenuActionSaveData:
		{
			
			break;
		}
		default:
		{
			
			break;
		}
	}
}

void CMenuSystem::processActions(menuItem* item)
{

}

void CMenuSystem::draw(void)
{
	if (_curr_menu_stack_size <= -1)
	{
		return;
	}
	
	// Draw the background menu rectangle first.
	CGraphics::drawRoundedRect(_menu_stack[_curr_menu_stack_size]->_menu_rect);
	
	for (int i = 0; i < _menu_stack[_curr_menu_stack_size]->_num_menu_items; ++i)
	{
		drawMenuItem(_menu_stack[_curr_menu_stack_size]->_menu_items[i]);
	}
}

void CMenuSystem::drawMenuItem(menuItem* pMenuItem)
{
	// Draw item rect, then the icon.
	CGraphics::drawRoundedRect(pMenuItem->_menu_item_rect);
	
	pMenuItem->_icon_sprite->_angle.z += pMenuItem->_icon_sprite->_delta_angle.z;
	
	CGraphics::drawSprite(pMenuItem->_icon_sprite);
}

void CMenuSystem::formatMenuItem(menuItem* pMenuItem, int format)
{
	// Format the menu icons.
	if (format & MENU_VCENTER)
	{
		pMenuItem->_icon_sprite->_y = pMenuItem->_menu_item_rect.y + 
		((pMenuItem->_menu_item_rect.h - pMenuItem->_icon_sprite->_height) >> 1);
	}
	
	if (format & MENU_HCENTER)
	{
		pMenuItem->_icon_sprite->_x = pMenuItem->_menu_item_rect.x + 
		((pMenuItem->_menu_item_rect.w - pMenuItem->_icon_sprite->_width) >> 1);
	}
	
	if (format & MENU_LEFT)
	{
		pMenuItem->_icon_sprite->_x = pMenuItem->_menu_item_rect.x;
	}
	
	if (format & MENU_RIGHT)
	{
		pMenuItem->_icon_sprite->_x = pMenuItem->_menu_item_rect.x +
		pMenuItem->_menu_item_rect.w - pMenuItem->_icon_sprite->_width;
	}
	
	if (format & MENU_TOP)
	{
		pMenuItem->_icon_sprite->_y = pMenuItem->_menu_item_rect.y;
	}
	
	if (format & MENU_BOTTOM)
	{
		pMenuItem->_icon_sprite->_y = pMenuItem->_menu_item_rect.y +
		pMenuItem->_menu_item_rect.h - pMenuItem->_icon_sprite->_height;
	}

}

void CMenuSystem::formatMenu(menu* pMenu, int menuItemModule, int format)
{
	switch (menuItemModule)
	{
		case eMenuModuleIcon:
		{
			for (int i = 0; i < pMenu->_num_menu_items; ++i)
			{
				formatMenuItem(pMenu->_menu_items[i], format);		
			}
			
			pMenu->_menu_icon_format = format;
			
			break;
		}
		case eMenuModuleRect:
		{
			int itemCounter = 0;
			
			// Even spaced formatting requires that the item counter start at 1.
			if (format & (MENU_ORG_EVEN_SPACED_VERT | MENU_ORG_EVEN_SPACED_HORZ))
			{
				itemCounter = 1;
			}
			
			for (int i = 0; i < pMenu->_num_menu_items; ++i)
			{
				// This formats the menu rects in relation to the menu background.
				if (format & MENU_VCENTER)
				{
					pMenu->_menu_items[i]->_menu_item_rect.y = pMenu->_menu_rect.y + 
					((pMenu->_menu_rect.h - pMenu->_menu_items[i]->_menu_item_rect.h) >> 1);
				}
				
				if (format & MENU_HCENTER)
				{
					pMenu->_menu_items[i]->_menu_item_rect.x = pMenu->_menu_rect.x + 
					((pMenu->_menu_rect.w - pMenu->_menu_items[i]->_menu_item_rect.w) >> 1);
				}
				
				if (format & MENU_LEFT)
				{
					pMenu->_menu_items[i]->_menu_item_rect.x = pMenu->_menu_rect.x;
				}
				
				if (format & MENU_RIGHT)
				{
					pMenu->_menu_items[i]->_menu_item_rect.x = pMenu->_menu_rect.x +
					pMenu->_menu_rect.w - pMenu->_menu_items[i]->_menu_item_rect.w;
				}
				
				if (format & MENU_TOP)
				{
					pMenu->_menu_items[i]->_menu_item_rect.y = pMenu->_menu_rect.y;
				}
				
				if (format & MENU_BOTTOM)
				{
					pMenu->_menu_items[i]->_menu_item_rect.x = pMenu->_menu_rect.y +
					pMenu->_menu_rect.h - pMenu->_menu_items[i]->_menu_item_rect.h;
				}
				
				// The following cannot be simultaneously active.
				if (format & MENU_ORG_EVEN_SPACED_VERT)
				{
					int singleItemHeight = pMenu->_menu_rect.h / pMenu->_num_menu_items;
					// Here, we take the height of the menu bg and divide it by the number of items, 
					// then we center the item in each of those spaces.
					pMenu->_menu_items[i]->_menu_item_rect.y = 
					(pMenu->_menu_rect.y + (itemCounter * singleItemHeight) - (singleItemHeight / 2)) -
					(pMenu->_menu_items[i]->_menu_item_rect.h / 2);
					itemCounter++;
				}
				else if (format & MENU_ORG_EVEN_SPACED_HORZ)
				{
					int singleItemWidth = pMenu->_menu_rect.w / pMenu->_num_menu_items;
					// Here, we take the width of the menu bg and divide it by the number of items, 
					// then we center the item in each of those spaces.
					pMenu->_menu_items[i]->_menu_item_rect.x = 
					(pMenu->_menu_rect.x + (itemCounter * singleItemWidth) - (singleItemWidth / 2)) -
					(pMenu->_menu_items[i]->_menu_item_rect.w / 2);
					itemCounter++;
				}
				else if (format & MENU_ORG_TIGHT_PACKED_VERT)
				{
					pMenu->_menu_items[i]->_menu_item_rect.y = pMenu->_menu_rect.y + 
					(pMenu->_menu_items[i]->_menu_item_rect.h * itemCounter) + itemCounter;
					itemCounter++;
				}
				else if (format & MENU_ORG_TIGHT_PACKED_HORZ)
				{
					pMenu->_menu_items[i]->_menu_item_rect.x = pMenu->_menu_rect.x + 
					(pMenu->_menu_items[i]->_menu_item_rect.w * itemCounter) + itemCounter;
					itemCounter++;
				}
				
				// Also update icon sprite coords!
				formatMenuItem(pMenu->_menu_items[i], pMenu->_menu_icon_format);
			}

			break;
		}
		default:
		{
			
			break;
		}
	}
}

#else

CMenuSystem::CMenuSystem()
{
	
}

CMenuSystem::~CMenuSystem()
{
	
}

void CMenuSystem::init()
{

}

void CMenuSystem::destroy()
{

}

menu* CMenuSystem::createMenu(int menuID)
{
	return NULL;
}

void CMenuSystem::pushMenu(int menuID)
{

}

menu* CMenuSystem::getActiveMenu()
{
	return NULL;
}

void CMenuSystem::handleTouch(float x, float y, eTouchPhase phase)
{

}

void CMenuSystem::executeAction(int action)
{

}

void CMenuSystem::processActions(menuItem* item)
{
	
}

void CMenuSystem::draw()
{

}

void CMenuSystem::drawMenuItem(menuItem* pMenuItem)
{

}

void CMenuSystem::formatMenuItem(menuItem* pMenuItem, int format)
{

	
}

void CMenuSystem::formatMenu(menu* pMenu, int menuItemModule, int format)
{

}

#endif