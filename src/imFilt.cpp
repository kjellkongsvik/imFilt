//============================================================================
// Name        : imFilt.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <mpi.h>
#include <Magick++.h>

using namespace std;
using namespace Magick;

int main(int argc, char** argv) {

	int rank, size;

	MPI_Init (&argc, &argv);

	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);

	InitializeMagick(argv[0]);

	Image image;

	try {
		// Read a file into image object
		if(rank==0){
			image.read( argv[1] );

			int nx = image.columns();
			int ny = image.rows();
			int l=0;
			Image im0(image);
			im0.crop(Geometry(nx/size, ny, 0, 0));

			if(size>1){
				Image im1(image);

				im1.crop(Geometry(nx/size, ny, nx/size, 0));

				Blob blob;
				im1.write(&blob);
				l = blob.length();
				MPI_Send(&l, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
				MPI_Send((void*)blob.data(), blob.length(), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
			}

			im0.blur(200,10);
			image.draw(DrawableCompositeImage(0,0,im0));

			if(size>1)
			{
				void* d = malloc(l);
				MPI_Recv(d, l, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				Blob blob_(d, l);
				Image im1_(blob_);
				image.draw(DrawableCompositeImage(nx/size,0,im1_));
			}
		}
		if(rank>0){

			int l;
			MPI_Recv(&l, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			void* d = malloc(l);
			MPI_Recv(d, l, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			Blob blob(d, l);
			Image im1(blob);
			im1.blur(200,10);
			im1.write(&blob);
			MPI_Send((void*)blob.data(), blob.length(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
//			//
		}



		//		Work *w = new Work(image, rank, size);

		if(rank==0){
			image.write( argv[2] );
		}
	}
	catch( Exception &error_ )
	{
		cout << "Caught exception: " << error_.what() << endl;

	}


	MPI_Finalize();

	return 0;
}
