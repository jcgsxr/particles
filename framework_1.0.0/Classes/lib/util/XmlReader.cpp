#include "xmlreader.h"



Attribute::Attribute()
: m_next(NULL)
{
	
}


Attribute::~Attribute()
{

}


void Attribute::setNext(Attribute *attr)
{
	m_next = attr;
}
	
	


XmlReader::XmlReader(String xml, int size)
{
	m_text = xml;
	
	m_pos = 0;
	m_end = size;
	
	m_Attribute = NULL;
	m_nAttributes = 0;
}

XmlReader::~XmlReader()
{	
	Attribute* attr = m_Attribute;
	Attribute* attrToDelete = NULL;
	
	while(attr != NULL)
	{
		attrToDelete = attr;
		attr = attr->getNext();
		
		SAFE_DEL(attrToDelete);
	}
}


void XmlReader::destroy()
{	
	//m_text = NULL;
	m_Attribute = NULL;
	m_nAttributes = 0;
}


void XmlReader::AddAttribute(int startName, int endName, int startValue, int endValue)
{
	if (m_text.equals(NULL))
		return;
	//if (m_text == NULL)
	//	return;
		
	Attribute *newattribute = new Attribute();
	newattribute->Name = m_text.substring(startName, endName);
	newattribute->Value = m_text.substring(startValue, endValue);
	
	newattribute->setNext(m_Attribute);
	m_Attribute = newattribute;
	m_nAttributes++;
}


bool XmlReader::Read()
{
	NodeType = XmlNodeType_None;
	Name = "";
	Value = "";
	
	m_Attribute = NULL;
	m_nAttributes = 0;
	
	if (!m_text.toCharArray() || (m_pos >= m_end))
		return false;
	
	switch (m_text.charAt(m_pos))
	{
		//------------------------------------------------------------------------------
		// BEGIN TAG
		//------------------------------------------------------------------------------
		case '<':
			m_pos++;
			
			if (m_pos < m_end)
			{
				switch (m_text.charAt(m_pos))
				{
					//------------------------------------------------------------------
					// XML DECLARATION
					//------------------------------------------------------------------
					case '?':
						if (m_end - m_pos > 5)
						{
							// Look for the "xml" characters at the start of the declaration.
							if ((m_text.charAt(m_pos+1) == 'x') && (m_text.charAt(m_pos+2) == 'm') && (m_text.charAt(m_pos+3) == 'l'))
							{
								// Loop over any characters until we get to the first whitespace character.
								while ((m_text.charAt(m_pos) != '\r') && (m_text.charAt(m_pos) != '\n') && (m_text.charAt(m_pos) != ' ') && (m_text.charAt(m_pos) !='\t'))
								{
									m_pos++;
									
									if (m_pos == m_end)
										return false;
								}
								
								int startValue = m_pos;
								
								// Keep looping until we are done or hit end of stream.
								while (m_pos < m_end - 1)
								{
									// Loop over characters until we get to the end of the declaraction.
									if ((m_text.charAt(m_pos+0) == '?') && (m_text.charAt(m_pos+1) == '>'))
									{
										Value = m_text.substring(startValue, m_pos);
										m_pos += 2;
										NodeType = XmlNodeType_XmlDeclaration;
										return true;
									}
									
									m_pos++;
								}
							}
						}
						break;
						
					//------------------------------------------------------------------
					// END ELEMENT
					//------------------------------------------------------------------
					case '/':
						if (m_end - m_pos > 2)
						{
							m_pos++;
							
							int startName = m_pos;
							int endName = m_pos;
							
							// Loop over characters until we hit the close tag or whitespace.
							while ((m_text.charAt(m_pos) != '\r') && (m_text.charAt(m_pos) != '\n') && (m_text.charAt(m_pos) != ' ') && (m_text.charAt(m_pos) !='\t'))
							{
								if (m_text.charAt(m_pos) == '>')
								{
									endName = m_pos;
									Name = m_text.substring(startName, endName);
									NodeType = XmlNodeType_EndElement;
									m_pos++;
									return true;
								}
								
								m_pos++;
								
								if (m_pos == m_end)
									return false;
							}
							
							endName = m_pos - 1;
							
							// If there is whitespace, skip to the end of it before returning the element name.
							while ((m_text.charAt(m_pos) == '\r') || (m_text.charAt(m_pos) == '\n') || (m_text.charAt(m_pos) == ' ') || (m_text.charAt(m_pos) =='\t'))
							{
								if (m_text.charAt(m_pos) == '>')
								{
									Name = m_text.substring(startName, endName);
									NodeType = XmlNodeType_EndElement;
									m_pos++;
									return true;
								}
								
								m_pos++;
								
								if (m_pos == m_end)
									return false;
							}
							
							return false;
						}
						break;
						
					//------------------------------------------------------------------
					// ELEMENT
					//------------------------------------------------------------------
					default:
						if (m_end - m_pos > 2)
						{
							int startName = m_pos;
							int endName = m_pos;
							
							// Loop over characters until we hit whitespace characters, the end tag or similar.
							while ((m_text.charAt(m_pos) != '\r') && (m_text.charAt(m_pos) != '\n') && (m_text.charAt(m_pos) != ' ') && (m_text.charAt(m_pos) !='\t'))
							{
								if (m_text.charAt(m_pos) == '/')
								{
									if (m_end - m_pos > 1)
									{
										endName = m_pos;
										m_pos++;
										
										if (m_text.charAt(m_pos) == '>')
										{
											IsEmptyElement = true;
											Name = m_text.substring(startName, endName);
											NodeType = XmlNodeType_Element;
											m_pos++;
											return true;
										}
									}
									
									return false;
								}
								else
								if (m_text.charAt(m_pos) == '>')
								{
									IsEmptyElement = false;
									endName = m_pos;
									Name = m_text.substring(startName, endName);
									NodeType = XmlNodeType_Element;
									m_pos++;
									return true;
								}
								
								m_pos++;
								
								if (m_pos == m_end)
									return false;
							}
							
							endName = m_pos;
							Name = m_text.substring(startName, endName);
							NodeType = XmlNodeType_Element;
							
							// Parse out any attributes for the element and add them to the list.
							while ((m_end - m_pos > 0))
							{
								// If there is whitespace, skip to the end of it first.
								while ((m_text.charAt(m_pos) == '\r') || (m_text.charAt(m_pos) == '\n') || (m_text.charAt(m_pos) == ' ') || (m_text.charAt(m_pos) =='\t'))
								{
										m_pos++;
										
										if (m_pos == m_end)
											return false;
								}
								
								// If this is a closing tag, then parse appropriately.
								if (m_text.charAt(m_pos) == '/')
								{
									if (m_end - m_pos > 1)
									{
										endName = m_pos;
										m_pos++;
										
										if (m_text.charAt(m_pos) == '>')
										{
											IsEmptyElement = true;
											m_pos++;
											return true;
										}
									}
									
									return false;
								} 
								else
								if (m_text.charAt(m_pos) == '>')
								{
									IsEmptyElement = false;
									m_pos++;
									return true;
								}
								
								// If we get here then there are attributes remaining to be parsed.
								int startAttributeName = m_pos;
								
								// Skip to the end of the name of the attribute.
								while (m_text.charAt(m_pos) != '=')
								{
									if (m_text.charAt(m_pos) == '>')
										return false;
									
									m_pos++;
									
									if (m_pos == m_end)
										return false;
								}
								
								int endAttributeName = m_pos;
								m_pos++;
								
								// Skip until we get to the start of the attribute value (enclosed in single or double quotes).
								while ((m_text.charAt(m_pos) != '\"') && (m_text.charAt(m_pos) != '\''))
								{
									// If we found a non-whitespace character before the quote character, then the xml is malformed so exit.
									if ((m_text.charAt(m_pos) != '\r') && (m_text.charAt(m_pos) != '\n') && (m_text.charAt(m_pos) != ' ') && (m_text.charAt(m_pos) !='\t'))
										return false;
										
									m_pos++;
									
									if (m_pos == m_end)
										return false;
								}
								
								// Save a copy of the quote type used so that we can look for the closing end.
								char quote_type = m_text.charAt(m_pos);
								
								int startAttributeValue = m_pos;
								m_pos++;
								
								if (m_pos == m_end)
									return false;
								
								// Loop until we find the closing quote character.
								while (m_text.charAt(m_pos) != quote_type)
								{
									m_pos++;
									
									if (m_pos == m_end)
										return false;
								}
								
								m_pos++;
								int endAttributeValue = m_pos;
								
								// Add this attribute to our list for the element.
								AddAttribute(startAttributeName, endAttributeName, startAttributeValue+1, endAttributeValue-1);
							}
						}
						break;
				}
			}
			break;
		//------------------------------------------------------------------------------
		// WHITESPACE
		//------------------------------------------------------------------------------
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			{
				int startWhitespace = m_pos;
				
				// Loop over any whitespace until we get to the first non-whitespace character.
				while ((m_text.charAt(m_pos) == '\r') || (m_text.charAt(m_pos) == '\n') || (m_text.charAt(m_pos) == ' ') || (m_text.charAt(m_pos) =='\t'))
				{
					m_pos++;
					
					// Set the value to simply be whitespace characters.
					if (m_pos >= m_end)
					{
						Value = m_text.substring(startWhitespace, m_pos);
						NodeType = XmlNodeType_Whitespace;
						return true;
					}
				}
				
				// Set the value to simply be whitespace characters.
				int endWhiteSpace = m_pos;
				Value = m_text.substring(startWhitespace, endWhiteSpace);
				NodeType = XmlNodeType_Whitespace;
				return true;
			}
			//break;
			
		//------------------------------------------------------------------------------
		// TEXT
		//------------------------------------------------------------------------------
		default:
			if (m_pos < m_end)
			{
				int startText = m_pos;
				
				while (m_text.charAt(m_pos) != '<')
				{
					m_pos++;
					
					if (m_pos == m_end)
						return false;
				}
				
				Value = m_text.substring(startText, m_pos);
				NodeType = XmlNodeType_Text;
				return true;
			}
			break;
	}
	
	return false;
}


String XmlReader::GetAttribute(int index)
{
	int n = m_nAttributes - index;
	
	// If the attribute index is valid, find it now.
	if (n >= 0)
	{
		Attribute *p = m_Attribute;
		
		// Loop until we get to the desired attribute in the list.
		for (int i=0; i < n; i++)
		{
			p = p->getNext();
		}
		
		// Return the attirbute's value.
		return p->Value;
	}
	
	// Return an empty String if the attribute index was out of range.
	return "";
}


String XmlReader::GetAttribute(String name)
{
	Attribute *p = m_Attribute;
	
	// Loop as long as we have remaining attributes to check.
	while (p != NULL)
	{
		// Does the attribute name match?  If so, return the value.
		if ((!p->Name.equals(NULL)) && (p->Name.equals(name.toCharArray())))
			return p->Value;
		
		// Increment to the next attribute.
		p = p->getNext();
	}
	
	// Return an empty String if the attribute name was not found.
	return "";
}