#ifndef __IMAGELOADER_H__
#define __IMAGELOADER_H__

#include "types.h"
#include "Image.h"

const uint32 MAX_LOADED_IMAGES = 256;	/*!< The maximum size of any image pack. */


class CSprite;

/*! \class CImageLoader
 * \brief The Image Loader class.
 *
 * The purpose of the Image Loader class is to organize image loading and unloading into packs, or groups of images. This enables more efficient use of memory and also better organization.
 */
class CImageLoader
{
public:
	CImage _images[MAX_LOADED_IMAGES];		/*!< This holds all images that have been loaded. */

	/*! \fn CImageLoader()
	 *  \brief The CImageLoader class constructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	CImageLoader();
	
	/*! \fn ~CImageLoader()
	 *  \brief The CImageLoader class destructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	~CImageLoader();
	
	/*! \fn loadImagePack(const uint32 imagePack[], int numImages)
	 *  \brief Loads a group of images into memory.
	 *  
	 * This function takes an array of image IDs and loads all images from disk into the image array #_images.
	 *	\param imagePack The array of image IDs.
	 *	\param numImages The number of images in the image ID array.
	 *  \return n/a
	 */
	void loadImagePack(const uint32 imagePack[], int numImages);
	
	/*! \fn unloadImagePack()
	 *  \brief Unloads all images that have been loaded into #_images.
	 *  
	 * This can safely be called at any time, even if there are no images currently loaded.
	 *	\param n/a
	 *  \return n/a
	 */
	void unloadImagePack();

	/*! \fn getImage(uint32 imageID)
	 *  \brief Retrieves an image from the group of loaded images.
	 *  
	 *	\param imageID The image ID of the image to be retrieved.
	 *  \return Pointer to the image CImage.
	 */
	CImage* getImage(uint32 imageID);
};

#endif