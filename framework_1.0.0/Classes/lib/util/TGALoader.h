#ifndef __TGALOADER_H__
#define __TGALOADER_H__



class TGALoader
{
public:
	int			color_type;
	int			image_type;
	int			color_length;
	int			color_size;
	int			image_width ;
	int			image_height;
	int			image_bit;
	void*		bit_data;			// Texture data is stored here
	int			bit_data_length;	// The length of the bit_data array. This value is differs depending on what format the TGA is in.
	bool		loaded;				// True if the texture has been created
			

	TGALoader()
	{
		init();
	}

	~TGALoader()
	{
		release();
	}
	

	void init(void)
	{
		color_type			= 0;
		image_type			= 0;
		color_length		= 0;
		color_size			= 0;
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
			

	// Takes the raw tga data, parses it, and sets it up in the TGALoader properties.
	void read(byte* data)
	{
		// Used to keep track of where we currently are looking in the raw data.
		int offset = 0;

		// Skip the ID length, it's not useful.
		offset++;

		// Get the color map type. This indicates whether a color map is included.
		color_type = data[offset];
		offset++;

		// Store image type. We are expecting either an uncompressed color-mapped image or an uncompressed true-color image.
		// A value of 1 is color-mapped, 2 is true-color.
		image_type = data[offset];
		offset++;

		// Move to the subfield of the color map to get the length of the color map and store it.
		offset += 2;
		color_length = (data[offset + 1] << 8) | data[offset];

		// Go to the color map entry size, which is the number of bits per pixel.
		offset += 2;
		color_size = data[offset];
		// Move over the color size.
		offset++;

		// We are now at the image specification field.
		// Skip the origin information (4 bytes) to get the width and height.
		offset += 4;

		// Store width and height.
		image_width = (data[offset + 1] << 8) | data[offset];
		offset += 2;
		image_height = (data[offset + 1] << 8) | data[offset];
		offset += 2;

		// Store the pixel depth, or bits per pixel.
		image_bit = data[offset];
		offset++;

		// Skip the image descriptor.
		offset++;

		if (color_type == 0)
		{
			byte *img_temp = data + offset;
			// We need to rearrange the colors since TGAs store it in BGR format and we need it in RGB format. OGL 1.0 doesn't support GL_BGRA!
			bit_data_length = image_width * image_height * 4;
			bit_data = new byte[bit_data_length];
			for (int i = 0; i < bit_data_length; i += 4)
			{
				// Swap blue and red components.
				((byte*)bit_data)[i] = img_temp[i + 2];
				// Green is in the same place.
				((byte*)bit_data)[i + 1] = img_temp[i + 1];
				// Swap blue and red components.
				((byte*)bit_data)[i + 2] = img_temp[i];
				// Store alpha.
				((byte*)bit_data)[i + 3] = img_temp[i + 3];
			}
		}
		else if (color_type == 1)
		{
			bit_data_length = image_width  * image_height;
			// Since ints are 4 bytes in size, we can just create an int array and avoid the multiply with 4 above.
			bit_data = new int[bit_data_length];

			// In case this image is a color-mapped image, we need to read the data differently.
			void* color_temp = NULL;
			color_temp = new int[color_length];

			// First, store the color map.
			int r, g, b;
			for (int i = 0; i < color_length; ++i)
			{
				int r = ((data[offset + 2] < 0) ? (256 + data[offset + 2]) : data[offset+  2]);
				int g = ((data[offset + 1] < 0) ? (256 + data[offset + 1]) : data[offset + 1]);
				int b = ((data[offset + 0] < 0) ? (256 + data[offset + 0]) : data[offset + 0]);

				// Store the color.
				((int*)color_temp)[i] = 0;
				((int*)color_temp)[i] = (b << 16)|(g << 8)|r;

				offset += 3;
			}

			// Now set up the raw image data using the color map.
			int col_index = 0;
			for (int i = 0; i < bit_data_length; ++i)
			{
				// Store the current color index.
				col_index = ((data[offset] < 0) ? (256 + data[offset]) : data[offset]);
				offset++;

				// If the color is magenta, make it a completely transparent value.
				if (((int*)color_temp)[col_index] != 0x00FF00FF00)
				{
					// A non-special color, just store it.
					((int*)bit_data)[i] = (0xff << 24) | ((int*)color_temp)[col_index];
				}
				else
				{
					// Special handling for magenta color.
					((int*)bit_data)[i] = (0x00 << 24) | ((int*)color_temp)[col_index];
				}
			}

			// Clean up the color map.
			delete color_temp;
			color_temp = NULL;
		}

		loaded = TRUE;
	}
};


#endif