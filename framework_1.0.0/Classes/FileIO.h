//
//  FileIO.h
//  framework
//
//  Created by Johnny Chan on 12/21/09.
//  Copyright 2009 LlamaFace. All rights reserved.
//


// Write to hard drive.  These writes do not go to the hard drive immediately,
// but are cached in the system.  Reading from written files should not be
// attempted until subsequent launch of app.  Storage is limited to 3 files
// with a combined size of 100kB.  Note that writing of files that already
// exist is performed in overwrite mode (files can grow, but not shrink).
//
// file_name    : relative file name from volume to write to
// buf          : buffer to write
// size         : number of bytes to write
//
// return       : true return indicates successful write
//                false return indicates unsuccessful write
extern bool FileIO_Write(const char *file_name, void *buf, unsigned long size);


// Start asynchronous open file from hard drive.
//
// file_name    : relative file name from volume to open

//
// return       : != 0 return indicates successful start
//                == 0 return indicates lack of resources to start or error
extern FILE* FileIO_StartOpen(const char *file_name);


// Start asynchronous close file
//
// filePtr		: Pointer to the file to close.
//
// return       : true return indicates successful start
//                false return indicates lack of resources to start or error
extern bool FileIO_StartClose(FILE* filePtr);


// Read from the file and fill the given buffer with the data using the specified offset and size.
extern void FileIO_Read(const char *file_name, void *buf, unsigned long size, long offset);