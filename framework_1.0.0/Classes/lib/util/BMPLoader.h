#ifndef __BMPLOADER_H__
#define __BMPLOADER_H__



class BMPLoader
{
public:
	int			file_size;
	int			image_width;
	int			image_height;
	int			image_bit;			// The bits per pixel or color depth.
	void*		bit_data;			// Texture data is stored here
	int			bit_data_length;	// The length of the bit_data array in ints.
	bool		loaded;				// True if the texture has been created
			

	BMPLoader()
	{
		init();
	}

	~BMPLoader()
	{
		release();
	}
	

	void init(void)
	{
		file_size			= 0;
		image_width 		= 0;
		image_height		= 0;
		image_bit			= 0;
		bit_data			= NULL;	
		bit_data_length		= 0;
		loaded				= FALSE;
	}

	
	// This deletes the texture buffer (not the OpenGL texture)
	void release()
	{
		if (bit_data)
		{
			delete bit_data;
			bit_data = NULL;
		}
	}
	
	int readInt(byte* buff, int pos)
	{
		return ((buff[pos] & 0xFF) | ((buff[pos + 1] & 0xFF) << 8) | ((buff[pos + 2] & 0xFF) << 16) | ((buff[pos + 3] & 0xFF) << 24));
	}

	// Takes the raw bmp data, parses it, and sets it up in the BMPLoader properties.
	void read(byte* data)
	{
		// Used to keep track of where we currently are looking in the raw data.
		int offset = 0;

		// Skip past the first 2 bytes. We will assume that the magic number will always be "BM".
		offset += 2;

		// Get the file size.
		file_size = readInt(data, offset);

		// Move to the starting address of the actual image data.
		offset += 8;

		// Save image data location.
		int data_offset = readInt(data, offset);
		byte *img_temp = data + data_offset;

		// Move to the beginning of the DIB header data.
		offset += 4;

		// Get DIB header size. This will indicate which DIB format this BMP is in.
		// We will assume however, that the file will be saved in Windows V3 format.
		int DIB_header_size = readInt(data, offset);

		if (DIB_header_size != 40)
		{
			//DBGPRINTF("TextureEntry::loadBMP() failed - Invalid DIB format");
			return;
		}

		// Move to the image width, and save it.
		offset += 4;
		image_width = readInt(data, offset);

		// Move to the image height, and save it.
		offset += 4;
		image_height = readInt(data, offset);

		// Move past the image height.
		offset += 4;

		// Skip the color planes info to go to color depth.
		offset += 2;

		// Store the bits per pixel/color depth.
		image_bit = ((data[offset + 1] << 8) & 0xFF) | data[offset] & 0xFF;

		// We need to rearrange the colors since BMPs store it in BGR format and we need it in RGB format. OGL 1.0 doesn't support GL_BGRA!
		if (bit_data)
		{
			delete bit_data;
			bit_data = NULL;
		}
		bit_data_length = image_width * image_height * 4;
		bit_data = new byte[bit_data_length];
		// We will have to set the data stride depending on color depth.
		int stride = 4;
		if (image_bit == 24)
		{
			stride = 3;
		}
		int img_data_index = 0;
		for (int i = 0; i < bit_data_length; i += 4)
		{
			// Swap blue and red components.
			((byte*)bit_data)[i] = img_temp[img_data_index + 2];
			// Green is in the same place.
			((byte*)bit_data)[i + 1] = img_temp[img_data_index + 1];
			// Swap blue and red components.
			((byte*)bit_data)[i + 2] = img_temp[img_data_index];

			// Store alpha only if there exists an alpha channel.
			if (image_bit == 32)
			{
				// Store alpha.
				((byte*)bit_data)[i + 3] = img_temp[img_data_index + 3];
			}
			else
			{
				// Otherwise just make alpha 1.
				((byte*)bit_data)[i + 3] = (byte)255;
			}
			img_data_index += stride;
		}


		loaded = TRUE;

		// JC: TODO: There are some other checks that should be made, such as whether the image is compressed or not.
	}
};


#endif