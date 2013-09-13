/*
 * work.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: kjell
 */
#include "work.h"

Work::Work(int argc, char* argv[])
{
	InitializeMagick(*argv);

	Image image;

	  try {
	    // Read a file into image object
	    image.read( "/home/kjell/ola.jpg" );

	    // Crop the image to specified size (width, height, xOffset, yOffset)
	    image.resize( Geometry(1000,1000) );

	    // Write the image to a file
	    image.write( "/home/kjell/ola_.jpg" );
	  }
	  catch( Exception &error_ )
	    {
	      cout << "Caught exception: " << error_.what() << endl;

	    }

	cout<<"hei"<<endl;
}
