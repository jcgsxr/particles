import java.io.*;

import javax.microedition.lcdui.*;

public class FontEx implements Constants
{
	private char[] symbol;
	private byte[] sInfo;
	private Image[] sImg;
	public int overlay;
	public final int HEIGHT;
	private final int offset;

	private static final int S_W = 4;
	private final int defaultIdx;

	public static FontEx font_menu;
	public static FontEx font_pos1;
	public static FontEx font_pos2;
	public static FontEx font_pos2_red;
	public static FontEx font_time;
	public static FontEx font_time_red;
	public static FontEx font_speed;
	public int numberWidth;

	public static void init()
	{
		try
		{
			DataInputStream dis = new DataInputStream(Class.forName(
				"java.lang.Object").getResourceAsStream("/font_menu.rpf"));
			font_menu = FontEx.create(FONT_MENU_SIZE, dis, FONT_MENU_HOR_OFFSET, true, FONT_MENU_VERT_OFFSET, 0xFFFFFF);
			dis.close();
		}
		catch (Exception e)
		{
			System.out.println("Fail to load: /font_menu.rpf");
		}
		try
		{
			DataInputStream dis = new DataInputStream(Class.forName(
				"java.lang.Object").getResourceAsStream("/font_pos1.rpf"));
			font_pos1 = FontEx.create(FONT_POS1_SIZE, dis, FONT_POS1_HOR_OFFSET, true, FONT_POS1_VERT_OFFSET, 0xFFFFFF);
			dis.close();
		}
		catch (Exception e)
		{
			e.printStackTrace();
			System.out.println("Fail to load: /font_pos1.rpf");
		}
		try
		{
			DataInputStream dis = new DataInputStream(Class.forName(
				"java.lang.Object").getResourceAsStream("/font_pos2.rpf"));
			font_pos2 = FontEx.create(FONT_POS2_SIZE, dis, FONT_POS2_HOR_OFFSET, true, FONT_POS2_VERT_OFFSET, 0xFFFFFF);
			dis.close();
		}
		catch (Exception e)
		{
			System.out.println("Fail to load: /font_pos2.rpf");
		}
		try
		{
			DataInputStream dis = new DataInputStream(Class.forName(
				"java.lang.Object").getResourceAsStream("/font_pos2.rpf"));
			font_pos2_red = FontEx.create(FONT_POS2_SIZE, dis, FONT_POS2_HOR_OFFSET, true, FONT_POS2_VERT_OFFSET, 0xFF0040);
			dis.close();
		}
		catch (Exception e)
		{
			System.out.println("Fail to load: /font_pos2.rpf");
		}

		try
		{
			DataInputStream dis = new DataInputStream(Class.forName(
				"java.lang.Object").getResourceAsStream("/font_time.rpf"));
			font_time = FontEx.create(FONT_TIME_SIZE, dis, FONT_TIME_HOR_OFFSET, true, FONT_TIME_VERT_OFFSET, 0xFFFFFF);
			dis.close();
		}
		catch (Exception e)
		{
			System.out.println("Fail to load: /font_time.rpf");
		}
		try
		{
			DataInputStream dis = new DataInputStream(Class.forName(
				"java.lang.Object").getResourceAsStream("/font_time.rpf"));
			font_time_red = FontEx.create(FONT_TIME_SIZE, dis, FONT_TIME_HOR_OFFSET, true, FONT_TIME_VERT_OFFSET, 0xFF0000);
			dis.close();
		}
		catch (Exception e)
		{
			System.out.println("Fail to load: /font_time.rpf");
		}
		
		try
		{
			DataInputStream dis = new DataInputStream(Class.forName(
				"java.lang.Object").getResourceAsStream("/font_speed.rpf"));
			font_speed = FontEx.create(FONT_TIME_SIZE, dis, FONT_TIME_HOR_OFFSET, true, FONT_TIME_VERT_OFFSET, 0xFFFF00);
			dis.close();
		}
		catch (Exception e)
		{
			System.out.println("Fail to load: /font_speed.rpf");
		}
	}

	private FontEx(int height, char[] sym, byte[] sInfo, int[] sOff,
			byte[] sData, int over, boolean sh, int offset, int color)
	{
		symbol = sym;
		this.sInfo = sInfo;
		overlay = over;
		this.HEIGHT = height;
		this.offset = offset;

		makeNumberMonospace();

		sInfo[S_W] = (byte) (sInfo[5 + S_W] - sInfo[5 + S_W] / 3);

		sImg = new Image[symbol.length];
		for (int i = symbol.length; --i >= 0;)
		{
			int off = i * 5;
			int w = sInfo[off++];
			int h = sInfo[off];
			if (w != 0 && h != 0)
			{
				int s_off = sOff[i];
				int[] charBuffer = new int[(w + 1) * (h + 1)];
				int src_off = 0;
				int dst_off = 0;
				for (int y = h; y-- > 0;)
				{
					for (int x = w; x-- > 0;)
					{
						int a = (sData[s_off + src_off++] << 24);
						if (sh)
						{
							charBuffer[dst_off] = mergeColor(charBuffer[dst_off], a | color);
							charBuffer[dst_off + w + 2] = a;
						}
						else charBuffer[dst_off] = a | 0xFFFFFF;
						dst_off++;
					}
					dst_off++;
				}
				byte[] data = createPngDataStructure(charBuffer, w + 1, true);
				sImg[i] = Image.createImage(data, 0, data.length);
			}
		}

		// find default symbol index
		int c = '?';
		int i1 = 0;
		int i2 = symbol.length - 1;
		int idx = (i1 + i2) / 2;
		if ((int) symbol[i2] == c) idx = i2;
		else if ((int) symbol[i1] == c) idx = i1;
		else
		{
			while (i2 - i1 > 1)
			{
				if ((int) symbol[idx] > c) i2 = idx;
				else if ((int) symbol[idx] < c) i1 = idx;
				else break;
				idx = (i2 + i1) / 2;
			}
			if (symbol[idx] != c) idx = 1;
		}
		defaultIdx = idx;

		sData = null;
		
		numberWidth = mGetStringWidth("0");
	}
	
	private static int mergeColor(int c1, int c2)
	{
		int a1 = (c1 >> 24) & 0xFF;
		int r1 = (c1 >> 16) & 0xFF;
		int g1 = (c1 >> 8) & 0xFF;
		int b1 = c1 & 0xFF;

		int a2 = (c2 >> 24) & 0xFF;
		int r2 = (c2 >> 16) & 0xFF;
		int g2 = (c2 >> 8) & 0xFF;
		int b2 = c2 & 0xFF;

		int a = (a1 + a2 - (a1 * a2 / 255));

		int r = a == 0 ? 0 : (r1 * a1 + r2 * a2) / a;
		if (r > 0xFF) r = 0xFF;
		int g = a == 0 ? 0 : (g1 * a1 + g2 * a2) / a;
		if (g > 0xFF) g = 0xFF;
		int b = a == 0 ? 0 : (b1 * a1 + b2 * a2) / a;
		if (b > 0xFF) b = 0xFF;

		return (a << 24) | (r << 16) | (g << 8) | b;
	}

	private void makeNumberMonospace()
	{
		int first = symbol.length;
		while (first-- > 0)
		{
			if (symbol[first] == '0') break;
		}
		
		int maxW = 0;
		if (first >= 0)
		{
			for (int i = first + 10; --i >= first;)
			{
				if (maxW < sInfo[(i * 5) + S_W]) maxW = sInfo[(i * 5) + S_W];
			}
	
			for (int i = first + 10; --i >= first;)
			{
				sInfo[(i * 5) + (S_W - 1)] = (byte) ((maxW - sInfo[(i * 5)]) / 2);
				sInfo[(i * 5) + S_W] = (byte) maxW;
			}
		}

		first = symbol.length;
		while ( first-- > 0)
		{
			if (symbol[first] == '-')
			{
				sInfo[(first * 5) + (S_W - 1)] = (byte) ((maxW - sInfo[(first * 5)]) / 2);
				sInfo[(first * 5) + S_W] = (byte) maxW;
				break;
			}
		}
	}

	public int getStringHeight()
	{
		return HEIGHT + (HEIGHT >> 1);
	}

	private static FontEx create(int height, DataInputStream di, int overlay,
			boolean shadow, int offset, int color) throws IOException
	{
		FontEx font = null;
			int symbolNum = di.readShort();
			char[] symbol = new char[symbolNum];
			for (int i = 0; i < symbolNum; i++)
			{
				symbol[i] = di.readChar();
			}

			byte[] sInfo = new byte[5 * symbolNum];
			di.readFully(sInfo);

			int bDataSize = 0;
			int[] sOff = new int[symbolNum];
			int w, dx;
			for (int i = 0; i < symbolNum; i++)
			{
				int off = i * 5;
				sOff[i] = bDataSize;
				bDataSize += (w = sInfo[off++]) * sInfo[off++];
				dx = sInfo[++off];
				sInfo[++off] += w + dx;
			}
			byte[] sData = new byte[bDataSize];
			di.read(sData);

		font = new FontEx(height, symbol, sInfo, sOff, sData, overlay, shadow,
			offset, color);

		return font;
	}

	public void mDraw(String str, int x, int y, Graphics g)
	{
		mDraw(str, x, y, g, Graphics.LEFT | Graphics.TOP);
	}

	public void mDraw(String str, int x, int y, Graphics g, int anchor)
	{
		if (str != null && str.length() > 0)
		{
			int[] idxStr = getIndexedString(str);
			if ((anchor & Graphics.HCENTER) > 0) drawIndexedString(g, idxStr, x - idxStr[0] / 2, y + offset);
			else drawIndexedString(g, idxStr, x, y + offset);
		}
	}

	public void drawIndexedString(Graphics g, int[] idx1, int x0, int y0)
	{
		int size = idx1.length - 1;
		int dx = x0;
		for (int i = 0; i < size; i++)
		{
			int idx = idx1[i + 1];
			int offset = (idx << 2) + idx;
			if (sImg[idx] != null)
			{
				g.drawImage(sImg[idx], dx + sInfo[offset + 3], y0 + sInfo[offset + 2], Graphics.TOP | Graphics.LEFT);
			}
			dx += sInfo[offset + S_W] - overlay;
		}
	}

	public int[] getIndexedString(String txt)
	{
		int size = txt.length();
		if (size == 0) return null;
		int[] idx1 = new int[size + 1];
		int W = 0;
		for (int i = 0; i < size; i++)
		{
			int c = (int) txt.charAt(i);
			int i1 = 0;
			int i2 = symbol.length - 1;
			int idx = (i1 + i2) / 2;
			if ((int) symbol[i2] == c) idx = i2;
			else if ((int) symbol[i1] == c) idx = i1;
			else
			{
				while (i2 - i1 > 1)
				{
					if ((int) symbol[idx] > c) i2 = idx;
					else if ((int) symbol[idx] < c) i1 = idx;
					else break;
					idx = (i2 + i1) / 2;
				}
				if (symbol[idx] != c) idx = defaultIdx;
			}
			W += sInfo[((idx1[i + 1] = idx) * 5) + S_W] - overlay;
		}
		W += overlay < 0 ? 0 : overlay;
		idx1[0] = W;
		return idx1;
	}

	public int mGetStringWidth(String str)
	{
		return substringWidth(str, 0, str.length());
	}

	public int substringWidth(String str, int start, int end)
	{
		int W = 0;
		for (int i = start; i < end; i++)
		{
			char c = str.charAt(i);
			int i1 = 0;
			int i2 = symbol.length - 1;
			int idx = (i1 + i2) / 2;
			if (symbol[i2] == c) idx = i2;
			else if (symbol[i1] == c) idx = i1;
			else
			{
				while (i2 - i1 > 1)
				{
					if (symbol[idx] > c) i2 = idx;
					else if (symbol[idx] < c) i1 = idx;
					else break;
					idx = (i2 + i1) / 2;
				}
				if (symbol[idx] != c) idx = defaultIdx;
			}
			W += sInfo[(idx * 5) + S_W] - overlay;
		}
		W += overlay < 0 ? 0 : overlay;
		return W;
	}

	public int mGetCenteredYCoord(int y, int h)
	{
		return y + (h - HEIGHT) / 2;
	}
	
    private static final byte[] PNG_HEADER =
    {
        // PNG file signature
        (byte) 0x89,  0x50,  0x4e,  0x47,  0x0d,  0x0a,  0x1a,  0x0a,
        // IHDR Image Header 
        0x00, 0x00, 0x00, 0x0d, 'I', 'H', 'D', 'R',
        // IEND chunk
        0x00, 0x00, 0x00, 0x00, 'I', 'E', 'N', 'D', 
        (byte) 0xae, 0x42, 0x60, (byte) 0x82
    };

    public static byte[] createPngDataStructure(int[] rgb, int scanlength, boolean alpha)
    {
        int srcPnt = 0;
        int h = rgb.length / scanlength; 
        int bpp = alpha ? 4 : 3;
        int scanLinesPerPackedBlock = 0xffff / (scanlength * bpp + 1); 
        int packedBlocksNum = (h - 1) / scanLinesPerPackedBlock + 1;        
        int size = h * (scanlength * bpp + 1) + packedBlocksNum * 5 + (2 + 4);
        byte[] pngData = new byte[57 + size];
        int pnt;
        int mark = 12;
        // png signature and initial IHDR chunk header
        System.arraycopy(PNG_HEADER, 0, pngData, 0, pnt = 16);
        // width
        pngData[pnt++] = (byte) (scanlength >> 24);
        pngData[pnt++] = (byte) (scanlength >> 16);        
        pngData[pnt++] = (byte) (scanlength >> 8);
        pngData[pnt++] = (byte) scanlength;
        // height
        pngData[pnt++] = (byte) (h >> 24);
        pngData[pnt++] = (byte) (h >> 16);        
        pngData[pnt++] = (byte) (h >> 8);
        pngData[pnt++] = (byte) h;
        // Bit depth
        pngData[pnt++] = 8;
        // Color type 
        pngData[pnt++] = (byte) (alpha ? 6 : 2);
        // Compression type
        pngData[pnt++] = 0;        
        // Filter type
        pngData[pnt++] = 0;        
        // Interlace type
        pngData[pnt++] = 0;
        // IHDR crc
        pnt = writeCrc(pngData, mark, pnt);
        // IDAT chunk size       
        pngData[pnt++] = (byte) (size >> 24);
        pngData[pnt++] = (byte) (size >> 16);
        pngData[pnt++] = (byte) (size >> 8);
        pngData[pnt++] = (byte) size;        
        // IDAT label
        pngData[mark = pnt++] = 'I';
        pngData[pnt++] = 'D';
        pngData[pnt++] = 'A';
        pngData[pnt++] = 'T';
        // Compression method/flags code
        pngData[pnt++] = 0x78;
        // Additional flags/check bits
        pngData[pnt++] = 0x01;
        // initial value for adler32 checksum
        int adler32_1 = 1;
        int adler32_2 = 0;        
        // zlib packed blocks
        while (--packedBlocksNum >= 0)
        {
            // no compression, final block mark if any
            pngData[pnt++] = (byte) (packedBlocksNum == 0 ? 1 : 0);
            // block size
            int scanLines;
            size = (scanLines = (h > scanLinesPerPackedBlock ? scanLinesPerPackedBlock : h)) * (scanlength * bpp + 1);
            h -= scanLines;
            // block size
            pngData[pnt++] = (byte) size;
            pngData[pnt++] = (byte) (size >> 8);
            // one's complement of block size
            pngData[pnt++] = (byte) (~size);
            pngData[pnt++] = (byte) ((~size) >> 8);
            while (--scanLines >= 0)
            {
                // filter type = 0 (just skip this byte) 
                pnt++;
                // store actual RGB data followed by alpha channel if required
                for (int i = scanlength; --i >= 0;)
                {
                    int color = rgb[srcPnt++];                    
                    pngData[pnt++] = (byte) (color >> 16);
                    pngData[pnt++] = (byte) (color >> 8);
                    pngData[pnt++] = (byte) color;
                    if (alpha)
                    {
                        pngData[pnt++] = (byte) (color >> 24);                        
                    }
                }
            }
            
            /* calculate adler-32 checksum */
            while (size > 0)
            {
                /* largest prime smaller than 65536 */
                final int BASE = 65521;              
                adler32_1 = (adler32_1 + (pngData[pnt - size--] & 0xff)) % BASE;
                adler32_2 = (adler32_2 + adler32_1) % BASE;
            }
            
        }
        pngData[pnt++] = (byte) (adler32_2 >> 8);            
        pngData[pnt++] = (byte) adler32_2;
        pngData[pnt++] = (byte) (adler32_1 >> 8);
        pngData[pnt++] = (byte) adler32_1;
        pnt = writeCrc(pngData, mark, pnt);
        System.arraycopy(PNG_HEADER, 16, pngData, pnt, 12);
        return pngData;
    }
    
    public static int[] crcTable = new int[0x100];

    private static int crc(byte[] buf, int offset, int len)
    {
      int c = 0xffffffff;
      while (len-- > 0)
      {
        c = (crcTable[(c ^ buf[offset++]) & 0xff]) ^ (c >>> 8);
      }
      return ~c;
    }
    
    private static int writeCrc(byte[] buf, int start, int end)
    {
        int crc = crc(buf, start, end - start);
        buf[end++] = (byte) (crc >> 24);
        buf[end++] = (byte) (crc >> 16);
        buf[end++] = (byte) (crc >> 8);
        buf[end++] = (byte) crc;        
        return end;
    }
    
    public static int crc(byte[] data, int initialValue)
    {
        initialValue = ~initialValue;
        for (int i = -1, len = data.length; ++i < len;)
        {
            initialValue = (crcTable[(initialValue ^ data[i]) & 0xff]) ^ (initialValue >>> 8);
        }        
        return ~initialValue;
    }
    
    static 
    {
        /* crc table initialization */
        int c;
        for (int i = 0x100; (c = --i) >= 0;)
        {
          for (int j = 8; --j >= 0;)
          {
            c = ( (c & 1) != 0 ? 0xedb88320 : 0) ^ (c >>> 1);
          }
          crcTable[i] = c;
        }
    }

}
