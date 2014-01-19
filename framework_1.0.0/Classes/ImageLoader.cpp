#include "GameData.h"
#include "ImageLoader.h"
#include "Image.h"
#include "Sprite.h"
#include <string.h>
#include "AppDefines.h"

#if defined (ENABLE_IMAGELOADER_SYSTEM)

CImageLoader::CImageLoader()
{
	memset(&_images, 0, sizeof(CImage*) * MAX_LOADED_IMAGES);
}

CImageLoader::~CImageLoader()
{
	unloadImagePack();
}

void CImageLoader::loadImagePack(const uint32 imagePack[], int numImages)
{
	// Unload any currently loaded image pack.
	unloadImagePack();
	
	for (int i = 0; i < numImages; ++i)
	{
		//_images[imagePack[i]] = new CImage();
		
		// Grab the filename here.
		_images[imagePack[i]].load(imageFileNames[imagePack[i]]);
	}
}

void CImageLoader::unloadImagePack()
{
	for (int i = 0; i < MAX_LOADED_IMAGES; ++i)
	{
		_images[i].unload();
	}
}

CImage* CImageLoader::getImage(uint32 imageID)
{
	return &_images[imageID];
}

#else

CImageLoader::CImageLoader()
{

}

CImageLoader::~CImageLoader()
{

}

void CImageLoader::loadImagePack(int *imagePack, int numImages)
{

}

void CImageLoader::unloadImagePack()
{

}

CImage* CImageLoader::getImage(uint32 imageID)
{

}

#endif