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

	int radius = atoi(argv[2]);
	float weight = atof(argv[3]);
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
			Image im0(image);
			im0.crop(Geometry(nx/size, ny, 0, 0));

			for(int ip=1;ip<size;ip++){
				Image im(image);

				im.crop(Geometry(nx/size, ny, (nx/size)*ip, 0));

				Blob blob;
				im.write(&blob);
				int l = blob.length();
				MPI_Send(&l, 1, MPI_INT, ip, 0, MPI_COMM_WORLD);
				MPI_Send((void*)blob.data(), blob.length(), MPI_CHAR, ip, 0, MPI_COMM_WORLD);
			}

			im0.blur(radius, weight);
			image.draw(DrawableCompositeImage(0,0,im0));

			for(int ip=1;ip<size;ip++){
				int l;
				MPI_Recv(&l, 1, MPI_INT, ip, ip, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				void* d = malloc(l);
				MPI_Recv(d, l, MPI_CHAR, ip, ip, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				Blob blob_(d, l);
				Image im(blob_);
				image.draw(DrawableCompositeImage((nx/size)*ip,0,im));
				free(d);
			}
		}
		if(rank>0){

			int l;
			MPI_Recv(&l, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			void* d = malloc(l);
			MPI_Recv(d, l, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			Blob blob(d, l);
			Image im(blob);
			im.blur(radius, weight);
			im.write(&blob);
			MPI_Send(&l, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
			MPI_Send((void*)blob.data(), blob.length(), MPI_CHAR, 0, rank, MPI_COMM_WORLD);
		}


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
