//
//  FileIO.mm
//  framework
//
//  Created by Johnny Chan on 12/21/09.
//  Copyright 2009 LlamaFace. All rights reserved.
//

#include "FileIO.h"
#include <stdio.h>
#include <string.h>
#import <Foundation/NSFileHandle.h>
#import <Foundation/NSBundle.h>
#import <Foundation/NSFileManager.h>
#import <Foundation/NSString.h>

bool FileIO_Write(const char *file_name, void *buf, unsigned long size)
{
	BOOL ok;
	
	// Create an NSString from the char* filename passed in
	NSString *nsFullFileName = [NSString stringWithUTF8String:file_name];
	
	// "You can write to your application's local storage area. You find the path by calling the Foundation function 
	// NSSearchPathForDirectoriesInDomains. For more information, see "The Application Sandbox" in iPhone OS Programming Guide."
	
	NSArray *pathForDirectories = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *documentsDirectory = [pathForDirectories objectAtIndex:0];
	
	
	// Add the filename to the path
	NSString *wholePath = [documentsDirectory stringByAppendingPathComponent:nsFullFileName];
	
	// Get the NSFileManager to See if a file by this name exists
	NSFileManager *fileManager;
	fileManager = [NSFileManager defaultManager];
	
	//- (BOOL)fileExistsAtPath:(NSString *)path
	BOOL exists = [fileManager fileExistsAtPath:wholePath];
	
	if(exists)
	{		
		// If something does exist, remove it
		if([fileManager isDeletableFileAtPath:wholePath])
		{
			[fileManager removeItemAtPath:wholePath error:NULL];
		}
	}
	
	// Create the data for writing
	NSData *dataToWrite = [NSData dataWithBytes:buf length:size];
	
	// Create the file and write the data to it
	//- (BOOL)createFileAtPath:(NSString *)path contents:(NSData *)contents attributes:(NSDictionary *)attributes
	ok = [fileManager createFileAtPath:wholePath contents:dataToWrite attributes:nil];
	
	return ok;
}

static size_t file_size;	// The size of the most recent file read in

FILE* FileIO_StartOpen(const char *file_name)
{
	// Parse out the filename and extension type from the file_name pasfile://localhost/Users/daimatsumoto/projects/trunk/iphone_common/IPLStuff/IPLGeometry.csed in
	NSString *nsFullFileName = [NSString stringWithUTF8String:file_name];
//	NSString *nsFileName = [nsFullFileName stringByDeletingPathExtension];
//	NSString *fileType = [nsFullFileName pathExtension];
	
	NSString* filePath;
	char *FileOpenMode = NULL;

	static char *FileOpenModeString = "rwb";
	NSArray *pathForDirectories = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *documentsDirectory = [pathForDirectories objectAtIndex:0];
	
	// Add the filename to the path
	filePath = [documentsDirectory stringByAppendingPathComponent:nsFullFileName];
	FileOpenMode = FileOpenModeString;

	
	if (filePath != NULL)
	{
		// Get the file size.
		NSFileHandle* temp_handle = [NSFileHandle fileHandleForReadingAtPath:filePath];
		NSData* temp_data = [temp_handle availableData];
		file_size = [temp_data length];
	}
	else
	{
		return NULL;
	}

	// Get the path as a c string so we can open it with fopen()
	file_name = [filePath UTF8String];
	
	// Open the file.
	FILE *fileptr = fopen(file_name, FileOpenMode);
	
	return fileptr;
}

void FileIO_Read(const char *file_name, void *buf, unsigned long size, long offset)
{
	
	// Parse out the filename and extension type from the file_name passed in
	NSString *nsFullFileName = [NSString stringWithUTF8String:file_name];
//	NSString *nsFileName = [nsFullFileName stringByDeletingPathExtension];
//	NSString *fileType = [nsFullFileName pathExtension];
	NSString* filePath;

	NSArray *pathForDirectories = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *documentsDirectory = [pathForDirectories objectAtIndex:0];
	
	// Add the filename to the path
	filePath = [documentsDirectory stringByAppendingPathComponent:nsFullFileName];	

	
	// Get the path as a c string so we can open it with fopen()
	file_name = [filePath UTF8String];
	
	FILE *fileptr;
	fileptr = fopen(file_name, "rb");
	if (fileptr)
	{
		fseek(fileptr, offset, SEEK_SET);
		file_size = fread(buf, 1, size, fileptr); 
		fclose(fileptr);
	}
}

bool FileIO_StartClose(FILE* filePtr)
{
	if (filePtr)
	{
		// Close the file.
		fclose(filePtr);
	}
	return true;
}