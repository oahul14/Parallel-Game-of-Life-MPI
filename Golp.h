#pragma once
#include <mpi.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Golp
{
public:
	Golp(int rows, int cols);										// constructor: initialise the size of the matrix
	~Golp();														// delete mdata

	void gen_col_type(int j, MPI_Datatype *mpi_type);				// generate col mpi type method
	void gen_row_type(int j, MPI_Datatype* mpi_type);				// generate row type method
	void gen_corner_type(int i, int j, MPI_Datatype* mpi_type);		// generate corners type method
	void gen_all_types();											// generate all mpi types, including col types
	void clean_all_types();											// free all mpi types (for non-periodic)

	void update_send_types();										// update send mpi types (non-periodic)
	void update_recv_types();										// update recv mpi types (non-periodic)

	void rand_init();												// initialise local process matrix (ghost cell size)
	void show_proc(int id);											// print out local process matrix
	void store_proc(int id, int iter, bool peri);					// store procss matrix into a txt file
	void game_of_life(Golp *another);								// for local process matrix do game of life
	
	bool** mdata;

	// to store mpi types
	MPI_Datatype send_types[8];
	MPI_Datatype recv_types[8];

	// mpi types about to be generated
	MPI_Datatype mpi_left_col_send;
	MPI_Datatype mpi_left_col_recv;
	MPI_Datatype mpi_right_col_send;
	MPI_Datatype mpi_right_col_recv;

	MPI_Datatype mpi_top_row_send;
	MPI_Datatype mpi_top_row_recv;
	MPI_Datatype mpi_bot_row_send;
	MPI_Datatype mpi_bot_row_recv;

	MPI_Datatype mpi_left_top_send;
	MPI_Datatype mpi_left_top_recv;
	MPI_Datatype mpi_right_top_send;
	MPI_Datatype mpi_right_top_recv;
	MPI_Datatype mpi_left_bot_send;
	MPI_Datatype mpi_left_bot_recv;
	MPI_Datatype mpi_right_bot_send;
	MPI_Datatype mpi_right_bot_recv;

private:
	int prows, pcols;		// actual matrix size i.e. without ghost cells
	int grows, gcols;		// matrix size with ghost cells
};

