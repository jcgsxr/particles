#ifndef __MENUSYSTEM_H__
#define __MENUSYSTEM_H__

#include "types.h"


#define MENU_ITEMS_MAX	16
#define MENUS_MAX	8

 
/*! \enum eMenuFormat
 *	\brief All the possible formats that can be applied to a #menu.
 *
 * These values are passed in to #CMenuSystem::formatMenuItems() along with a menu module.
 */
typedef enum eMenuFormat
{
	MENU_FORMAT_NONE = 0x0,
	MENU_VCENTER	= 0x000001,				/*!< Centers the menu item module vertically. */
	MENU_HCENTER	= 0x000002,				/*!< Centers the menu item module horizontally. */
	MENU_LEFT		= 0x000004,				/*!< Left justifies the menu item module. */
	MENU_RIGHT		= 0x000008,				/*!< Right justifies the menu item module. */
	MENU_TOP		= 0x000010,				/*!< Top justifies the menu item module. */
	MENU_BOTTOM	= 0x000020,					/*!< Bottom justifies the menu item module. */
	MENU_ORG_EVEN_SPACED_VERT	= 0x000040,	/*!< Organize all menu items by spacing them evenly veritcally within the menu bg. */
	MENU_ORG_EVEN_SPACED_HORZ	= 0x000080,	/*!< Organize all menu items by spacing them evenly horizontall within the menu bg. */
	MENU_ORG_TIGHT_PACKED_VERT	= 0x000100,	/*!< Organize all menu items by tightly packing them vertically. */
	MENU_ORG_TIGHT_PACKED_HORZ = 0x000200,	/*!< Organize all menu items by tightly packing them horizontally. */
	MENU_ORG_ALPHA	= 0x000400,				/*!< Alphabetically organizes the menu items according to the menu item string. */
} eMenuFormat;

/*! \enum eMenuActions
 *	\brief All possible menu actions.
 */
typedef enum eMenuAction
{
	eMenuActionYes = 0,				/*!< The general yes or TRUE action. */
	eMenuActionNo,					/*!< The general no or FALSE action. */
	eMenuActionCloseMenu,			/*!< Menu close. */
	eMenuActionGotoTitleScreen,		/*!< Go to title screen. */
	eMenuActionGotoExitScreen,		/*!< Go to exit screen. */
	eMenuActionGotoSplashScreen,	/*!< Go to splash screen. */
	eMenuActionGotoMainScreen,		/*!< Go to main screen. */
	eMenuActionGotoHelpScreen,		/*!< Go to help screen. */
	eMenuActionOpenOptionsMenu,		/*!< Go to options menu. */
	eMenuActionOpenQuitMenu,		/*!< Go to quit menu. */
	eMenuActionOpenConfirmationMenu,	/*!< Go to a yes/no confirmation menu. */
	eMenuActionSaveData,			/*!< Save data. */
} eMenuAction;

/*! \enum eMenuModule
 *	\brief Menu modules represent parts of a menu and is used to identify which part of a menu to format when calling #CMenuSystem::formatMenu().
 */
typedef enum eMenuModule
{
	eMenuModuleIcon = 0,	/*!< The menu's menu item icon images. */
	eMenuModuleBG,			/*!< The menu's menu item background images. */
	eMenuModuleRect			/*!< The menu's menu item rectangles. */
} eMenuModule;

class CImage;
class CSprite;

/*! \struct menuData
 *	\brief The Menu Data type.
 *
 * Holds the data for one menu. Menu's can be loaded using this data structure.
 */
typedef struct menuData
{
	int _num_menu_items;						/*!< The number of menu items in the #Menu. */
	colorRect _menu_bg_rect;					/*!< The menu backgound colored rectangle. */
	colorRect _menu_item_rects[MENU_ITEMS_MAX];	/*!< The individual menu item colored rectangles. */
	eMenuAction _menu_actions[MENU_ITEMS_MAX];	/*!< The individual menu item actions. */
	int _icon_image_ids[MENU_ITEMS_MAX];		/*!< The indivicual menu item icon image IDs. */
	point _icon_offsets[MENU_ITEMS_MAX];		/*!< The optional icon image offsets for each menu item. */
	int _background_image_ids[MENU_ITEMS_MAX];	/*!< The optional individual background images for each menu item. */
} menuData;

/*! \struct subMenuItem
 *	\brief The Sub-menu Item type.
 *
 * Menu items may hold a number of sub-menu items.
 */
typedef struct subMenuItem
{
	rect _menu_rect;			/*!< The rectangle that represents the area of this sub menu item. */
	CImage* _background_image;	/*!< The optional background image of this sub menu item. */ 
	eMenuAction _action;		/*!< The action associated with this sub menu item. */
} subMenuItem;

/*! \struct menuItem
 *	\brief The Menu Item type.
 *
 * Menu items are what constitute a #Menu.
 */
typedef struct menuItem
{
	colorRect _menu_item_rect;			/*!< All rendering and actions will occur within this defined rect. This must be defined for the item to exist and work properly. */
	eMenuAction _action;				/*!< The action that occurs when the menu item is executed via anything. */
	eMenuAction _touch_press_action;	/*!< The action that occurs when the menu item is touched. */
	eMenuAction _touch_release_action;	/*!< The action that occurs when the menu item is released from touch. */
	CSprite* _icon_sprite;				/*!< An icon associated with the menu item.  Not required. */
	CSprite* _background_sprite;		/*!< The background image of this menu item.  Not required. */
	int _index;							/*!< The item id value in the menu itself.*/
	int _num_sub_menu_items;			/*!< The number of sub-menu items that this menu item contains. */
	subMenuItem* _subMenuItems[MENU_ITEMS_MAX];	/*!< Menu items can have an arbitrary amount of sub items. */
} menuItem;
		
/*! \struct menu
 *	\brief The Menu type.
 *
 * Menus are constructed from menu items #menuItem and is the main component of the menu system #CMenuSystem.
 */
typedef struct menu
{
	colorRect _menu_rect;					/*!< The colored menu rectangle of this menu. */
	int _num_menu_items;					/*!< The number of menu items that this menu contains. */
	int _menu_icon_format;					/*!< Stores the current icon positioning format. */
	menuItem* _menu_items[MENU_ITEMS_MAX];	/*!< The menu items associated with this particular menu. */
} menu;
		

/*! \class CMenuSystem
 * \brief The Menu System class.
 *
 * The Menu System class is responsible for translating user input into actions that can range from changing a setting, to transitioning to another screen.
 */
class CMenuSystem
{	
public:
	
	/*! \fn CMenuSystem()
	 *  \brief The CMenuSystem class constructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	CMenuSystem();
	
	/*! \fn ~CMenuSystem()
	 *  \brief The CMenuSystem class destructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	~CMenuSystem();

	/*! \fn init()
	 *  \brief The CMenuSystem initializer function.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void init(void);
	
	/*! \fn drawMenuItem(menuItem* pMenuItem)
	 *  \brief Renders the menu item within the current active menu.
	 *  
	 *	\param pMenuItem The menu item to render.
	 *  \return n/a
	 */
	void drawMenuItem(menuItem* pMenuItem);
	
	/*! \fn formatMenuItem(menuItem* pMenuItem, int format)
	 *  \brief Formats the menu item with an #eMenuFormat.
	 *  
	 *	\param pMenuItem The menu item to format.
	 *	\param format The type of format to apply to the menu item.
	 *  \return n/a
	 */
	void formatMenuItem(menuItem* pMenuItem, int format);
	
	/*! \fn formatMenu(menu* pMenu, int menuItemModule, int format)
	 *  \brief Formats the #menu.
	 *  
	 *	\param pMenu the #menu to format.
	 *	\param menuItemModule The part of the menu to format. Enumerated by #eMenuModule.
	 *	\param format The type of format to apply. Enumerated by #eMenuFormat.
	 *  \return n/a
	 */
	void formatMenu(menu* pMenu, int menuItemModule, int format);
	
	/*! \fn handleTouch(float x, float y, eTouchPhase phase)
	 *  \brief The menu touch handler.
	 *  
	 *	\param x The x location where the touch occured.
	 *	\param y The y location where the touch occured.
	 *	\param phase The type of touch that occured.
	 *  \return n/a
	 */
	void handleTouch(float x, float y, eTouchPhase phase);
	
	/*! \fn destroy()
	 *  \brief The CMenuSystem destroy function.
	 *  
	 * All deacllocaiton of menu properties occur here. 
	 *	\param n/a
	 *  \return n/a
	 */
	void destroy(void);
	
	/*! \fn update()
	 *  \brief The CMenuSystem update function.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void update(void);
	
	/*! \fn draw()
	 *  \brief The CMenuSystem render function.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void draw(void);
	
	/*! \fn processActions(menuItem* item)
	 *  \brief Processes any action that occured during the last frame on this menu item.
	 *  
	 *	\param item The menu item to run the action on.
	 *  \return n/a
	 */
	void processActions(menuItem* item);
	
	/*! \fn executeAction(int action)
	 *  \brief Executes an action on the active #menu.
	 *  
	 *	\param action The action to execute on the #menu.
	 *  \return n/a
	 */
	void executeAction(int action);
	
	/*! \fn createMenu(int menuID)
	 *  \brief Creates a new menu using #appMenuData as the data to load from.
	 *  
	 *	\param menuID The ID used to reference the data from #appMenuData.
	 *  \return A pointer to the newly created menu.
	 */
	menu* createMenu(int menuID);
	
	/*! \fn pushMenu(int menuID)
	 *  \brief Creates a new menu using the menuID and pushes it on to the menu stack #_menu_stack.
	 *  
	 *	\param menuID The ID used to reference the data from #appMenuData.
	 *  \return n/a
	 */
	void pushMenu(int menuID);
	
	/*! \fn popMenu()
	 *  \brief Pops the current active menu from the menu stack #_menu_stack.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void popMenu(void);
	
	/*! \fn getActiveMenu()
	 *  \brief Returns the current active menu in the menu stack #_menu_stack.
	 *  
	 *	\param n/a
	 *  \return Pointer to the current active menu.
	 */
	menu* getActiveMenu(void);
	
	menu* _menu_stack[MENUS_MAX];	/*!< When the menu stack is empty, we just run the current active screen. */
	int _curr_menu_stack_size;		/*!< Keeps track of the current active menu and is updated within the pushMenu() and popMenu() functions. */
};


#endif

