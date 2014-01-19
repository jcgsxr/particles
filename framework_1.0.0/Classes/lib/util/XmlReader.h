#ifndef __XMLREADER_H__
#define __XMLREADER_H__

#include "StringD.h"

static const int XmlNodeType_None = 				0;
static const int XmlNodeType_Element = 				1;		//< An element (for example, <item> ).
static const int XmlNodeType_Attribute = 			2;		//< An attribute (for example, id='123' ).
static const int XmlNodeType_Text = 				3;		//< The text content of a node.
static const int XmlNodeType_CDATA = 				4;		//< A CDATA section (for example, <![CDATA[my escaped text]]> ).
static const int XmlNodeType_EntityReference = 		5;		//< A reference to an entity (for example, &num; ).
static const int XmlNodeType_Entity = 				6;		//< An entity declaration (for example, <!ENTITY...> ).
static const int XmlNodeType_ProcessingInstruction = 7;		//< A processing instruction (for example, <?pi test?> ).
static const int XmlNodeType_Comment = 				8;		//< A comment (for example, <!-- my comment --> ).
static const int XmlNodeType_Document = 			9;		//< A document object that, as the root of the document tree, provides access to the entire XML document.
static const int XmlNodeType_DocumentType = 		10;		//< The document type declaration, indicated by the following tag (for example, <!DOCTYPE...> ).
static const int XmlNodeType_DocumentFragment = 	11;		//< A document fragment.
static const int XmlNodeType_Notation = 			12;		//< A notation in the document type declaration (for example, <!NOTATION...> ).
static const int XmlNodeType_Whitespace = 			13;		//< White space between markup.
static const int XmlNodeType_SignificantWhitespace = 14;	//< White space between markup in a mixed content model or white space within the xml:space="preserve" scope.
static const int XmlNodeType_EndElement = 			15;		//< An end element tag (for example, </item> ).
static const int XmlNodeType_EndEntity = 			16;		//< Returned when XmlReader gets to the end of the entity replacement as a result of a call to System.Xml.XmlReader.ResolveEntity().
static const int XmlNodeType_XmlDeclaration = 		17;		//< The XML declaration (for example, <?xml version='1.0'?> ).



// Class for holding an XML attribute.
class Attribute
{
	Attribute *m_next;

public:
	String		Name;
	String		Value;

public:
	Attribute();
	~Attribute();
	
	// Sets the next Attribute in the list to the value specified.
	void	setNext(Attribute *attribute);
	
	Attribute *getNext(){ return m_next; }
};




// Custom XML Reader for use in network APIs.  The reader is created with a
// String which acts as the XML stream.  Each call to Read() parses the next element
// from the stream.  The element's name and value are stored as parameters of the 
// reader, and a linked list of attributes (if included) are created.  Each call to
// Read() replaces the element and attributes stored.  The end of stream is reached 
// when a call to Read() returns false.
//
// Note that this is not a complete, valid XML reader and is customized (and simplified)
// based on the needs of the APIs using it.
class XmlReader
{
private:
	String m_text;				//< XML stream to use for reading tokens.
	Attribute *m_Attribute;		//< Linked list containing all attributes attached to the current element.
	int m_nAttributes;			//< Number of attributes stored for the current XML element.

public:
	int NodeType;				//< Type of the most recently parsed XML element.
	String Name;					//< Name of the most recently parsed XML element.
	String Value;				//< Value of the most recently parsed XML element.
	bool IsEmptyElement;
	int m_pos;					//< Current position in the XML stream. (only for debugging purposes)
	int m_end;					//< Position of the end of the XML stream. (only for debugging purposes)
	
	// Default constructor.
	XmlReader(String xml, int size);
	~XmlReader();
	
	// Default destructor.  Removes all attributes from the linked list.
	void destroy();

	// Creates a new attribute with the name and value indicated by the locations
	// specified in the String.  The attribute will be added to the list for the reader.
	void AddAttribute(int startName, int endName, int startValue, int endValue);
	
	// @brief parses the next element from the XML text for the reader.  The next element 
	// consists of a name/value pair, along with any relevant attributes.
	//
	// @return True if the next element was succesfully parsed; false otherwise or if the
	// end of the XML stream has been reached.
	bool Read();

	// @brief Returns the value of the attribute indicated by the index passed.  The index
	// refers to the order of attributes within the XML element (with the first being zero).
	// 
	// @return The value of the attribute, or an empty String if the attribute index is
	// out of range.
	String GetAttribute(int index);
	
	// @brief Returns the value of the attribute indicated by the name passed.
	//
	// @return The value of the attribute, or an empty String if the attribute was not found.
	String GetAttribute(String name);
};

#endif