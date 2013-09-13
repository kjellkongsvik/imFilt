/*
 * work.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: kjell
 */
#include "readWriteFile.h"

ReadWriteFile::ReadWriteFile(char* argv, char* in, char* out)
{
	InitializeMagick(argv);

	Image image;

	  try {
	    // Read a file into image object
	    image.read( in );

	    // Crop the image to specified size (width, height, xOffset, yOffset)
	    //image.resize( Geometry(1000,1000) );
	    image.modifyImage();
	    // Set the image type to TrueColor DirectClass representation.
	    image.type(TrueColorType);
	    // Request pixel region with size 60x40, and top origin at 20x30
	    ssize_t columns = 1;
	    PixelPacket *pixel_cache = image.getPixels(1,1,columns,1);
	    // Set pixel at column 5, and row 10 in the pixel cache to red.
	    ssize_t column = 1;
	    ssize_t row = 1;
	    PixelPacket *pixel = pixel_cache+row*columns+column;
	    *pixel = Color("red");
	    // Save changes to underlying image .
	    image.syncPixels();
	    // Write the image to a file
	    image.write( out );
	  }
	  catch( Exception &error_ )
	    {
	      cout << "Caught exception: " << error_.what() << endl;

	    }

	cout<<"hei"<<endl;
}
