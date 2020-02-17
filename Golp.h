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

	// col mpi types needed for both periodic and non-periodic
	void gen_col_type(int j, MPI_Datatype *mpi_type);				// generate col mpi type method
	void gen_col_types();											// generate all column types
	void left_col_send(int dest, int tag_num, MPI_Request* req);	// send left col using generated col type
	void left_col_recv(int src, int tag_num, MPI_Request* req);		// recv left col using generated col type
	void right_col_send(int dest, int tag_num, MPI_Request* req);	// send right col using generated col type
	void right_col_recv(int src, int tag_num, MPI_Request* req);	// recv right col using generated col type
	
	void top_row_send(int dest, int tag_num, MPI_Request* req);		// send top row: using MPI_C_BOOL
	void top_row_recv(int src, int tag_num, MPI_Request* req);		// recv top row: using MPI_C_BOOL
	void bot_row_send(int dest, int tag_num, MPI_Request* req);		// send bot row: using MPI_C_BOOL
	void bot_row_recv(int src, int tag_num, MPI_Request* req);		// recv bot row: using MPI_C_BOOL

	void left_top_send(int dest, int tag_num, MPI_Request* req);	// send left top corner: using MPI_C_BOOL
	void left_top_recv(int src, int tag_num, MPI_Request* req);		// recv left top corner: using MPI_C_BOOL
	void right_top_send(int dest, int tag_num, MPI_Request* req);	// send right top corner: using MPI_C_BOOL
	void right_top_recv(int src, int tag_num, MPI_Request* req);	// recv right top corner: using MPI_C_BOOL
	void left_bot_send(int dest, int tag_num, MPI_Request* req);	// send left bot corner: using MPI_C_BOOL
	void left_bot_recv(int src, int tag_num, MPI_Request* req);		// recv left bot corner: using MPI_C_BOOL
	void right_bot_send(int dest, int tag_num, MPI_Request* req);	// send right bot corner: using MPI_C_BOOL
	void right_bot_recv(int src, int tag_num, MPI_Request* req);	// recv right bot corner: using MPI_C_BOOL

	// for non-periodic:
	void gen_row_type(int j, MPI_Datatype* mpi_type);				// generate row type method
	void gen_corner_type(int i, int j, MPI_Datatype* mpi_type);		// generate corners type method
	void gen_all_types();											// generate all mpi types, including col types
	void clean_all_types();											// free all mpi types (for non-periodic)
	void clean_col_types();											// free col mpi types only (for periodic)

	void update_send_types();										// update send mpi types (non-periodic)
	void update_recv_types();										// update recv mpi types (non-periodic)

	void rand_init();												// initialise local process matrix (ghost cell size)
	void show_proc(int id);											// print out local process matrix
	void store_proc(int id, int iter, bool peri);					// store procss matrix into a txt file
	void game_of_life();								// for local process matrix do game of life
	
	bool* mdata;

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
	int* sum_mat;			// sum matrix to store neighbour sums
};

