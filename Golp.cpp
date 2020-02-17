#include "Golp.h"

// Constructor
Golp::Golp(int rows, int cols)
{
	prows = rows;
	pcols = cols;
	grows = prows + 2;
	gcols = pcols + 2;
	this->mdata = new bool[grows * gcols];
	this->sum_mat = new int[grows * gcols];
	//for (int i = 0; i < grows * gcols; i++) this->sum_mat[i] = 0;
}

// Destructor
Golp::~Golp()
{
	delete[] this->sum_mat;
	delete[] this->mdata;
}

// for general type setup for cols: including send & recv (indicated by j)
void Golp::gen_col_type(int j, MPI_Datatype *mpi_type)
{
	vector<int> block_length;
	vector<MPI_Aint> addresses;
	vector<MPI_Datatype> typelist;
	for (int i = 1; i <= prows; i++)
	{
		block_length.push_back(1);
		MPI_Aint temp;
		MPI_Get_address(&this->mdata[i * gcols + j], &temp);
		addresses.push_back(temp);
		typelist.push_back(MPI_C_BOOL);
	}
	MPI_Type_create_struct(block_length.size(), &block_length[0], &addresses[0], &typelist[0], mpi_type);
	MPI_Type_commit(mpi_type);
}

// generate all col types
void Golp::gen_col_types()
{
	gen_col_type(1, &mpi_left_col_send);			// 1st col in the mesh
	gen_col_type(0, &mpi_left_col_recv);			// 0th col in the mesh
	gen_col_type(pcols, &mpi_right_col_send);		// second last col in the mesh
	gen_col_type(pcols + 1, &mpi_right_col_recv);	// last col in the mesh
}

// packed sending & recving methods: cols
void Golp::left_col_send(int dest, int tag_num, MPI_Request* req)
{
	MPI_Isend(MPI_BOTTOM, 1, mpi_left_col_send, dest, tag_num, MPI_COMM_WORLD, req);
}
void Golp::left_col_recv(int src, int tag_num, MPI_Request* req)
{
	MPI_Irecv(MPI_BOTTOM, 1, mpi_left_col_recv, src, tag_num, MPI_COMM_WORLD, req);
}
void Golp::right_col_send(int dest, int tag_num, MPI_Request* req)
{
	MPI_Isend(MPI_BOTTOM, 1, mpi_right_col_send, dest, tag_num, MPI_COMM_WORLD, req);
}
void Golp::right_col_recv(int src, int tag_num, MPI_Request* req)
{
	MPI_Irecv(MPI_BOTTOM, 1, mpi_right_col_recv, src, tag_num, MPI_COMM_WORLD, req);
}

// packed sending & recving methods: rows
void Golp::top_row_send(int dest, int tag_num, MPI_Request* req)
{
	MPI_Isend(&this->mdata[1 * gcols + 1], pcols, MPI_C_BOOL, dest, tag_num, MPI_COMM_WORLD, req);
}
void Golp::top_row_recv(int src, int tag_num, MPI_Request* req)
{
	MPI_Irecv(&this->mdata[0 * gcols + 1], pcols, MPI_C_BOOL, src, tag_num, MPI_COMM_WORLD, req);
}
void Golp::bot_row_send(int dest, int tag_num, MPI_Request* req)
{
	MPI_Isend(&this->mdata[prows * gcols + 1], pcols, MPI_C_BOOL, dest, tag_num, MPI_COMM_WORLD, req);
}
void Golp::bot_row_recv(int src, int tag_num, MPI_Request* req)
{
	MPI_Irecv(&this->mdata[(prows + 1) * gcols + 1], pcols, MPI_C_BOOL, src, tag_num, MPI_COMM_WORLD, req);
}

// packed sending & recving methods: corners
void Golp::left_top_send(int dest, int tag_num, MPI_Request* req)
{
	MPI_Isend(&this->mdata[1 * gcols + 1], 1, MPI_C_BOOL, dest, tag_num, MPI_COMM_WORLD, req);
}
void Golp::left_top_recv(int src, int tag_num, MPI_Request* req)
{ 
	MPI_Irecv(&this->mdata[0 * gcols + 0], 1, MPI_C_BOOL, src, tag_num, MPI_COMM_WORLD, req);
}
void Golp::right_top_send(int dest, int tag_num, MPI_Request* req)
{ 
	MPI_Isend(&this->mdata[1 * gcols + pcols], 1, MPI_C_BOOL, dest, tag_num, MPI_COMM_WORLD, req);
}
void Golp::right_top_recv(int src, int tag_num, MPI_Request* req)
{
	MPI_Irecv(&this->mdata[0 * gcols + (pcols + 1)], 1, MPI_C_BOOL, src, tag_num, MPI_COMM_WORLD, req);
}
void Golp::left_bot_send(int dest, int tag_num, MPI_Request* req)
{
	MPI_Isend(&this->mdata[prows * gcols + 1], 1, MPI_C_BOOL, dest, tag_num, MPI_COMM_WORLD, req);
}
void Golp::left_bot_recv(int src, int tag_num, MPI_Request* req)
{ 
	MPI_Irecv(&this->mdata[(prows + 1) * gcols + 0], 1, MPI_C_BOOL, src, tag_num, MPI_COMM_WORLD, req);
}
void Golp::right_bot_send(int dest, int tag_num, MPI_Request* req)
{ 
	MPI_Isend(&this->mdata[prows * gcols + pcols], 1, MPI_C_BOOL, dest, tag_num, MPI_COMM_WORLD, req);
}
void Golp::right_bot_recv(int src, int tag_num, MPI_Request* req) 
{
	MPI_Irecv(&this->mdata[(prows + 1) * gcols + (pcols + 1)], 1, MPI_C_BOOL, src, tag_num, MPI_COMM_WORLD, req);
}

// for general type setup for rows: including send & recv (indicated by i)
void Golp::gen_row_type(int i, MPI_Datatype* mpi_type)
{
	int block_length = pcols;
	MPI_Datatype type = MPI_C_BOOL;

	MPI_Aint address;
	MPI_Aint temp;
	MPI_Get_address(&this->mdata[i * gcols + 1], &temp);
	address = temp;

	MPI_Type_create_struct(1, &block_length, &address, &type, mpi_type);
	MPI_Type_commit(mpi_type);
}

// for general type setup for corners: including send & recv (indicated by i and j)
void Golp::gen_corner_type(int i, int j, MPI_Datatype* mpi_type)
{
	int block_length = 1;
	MPI_Datatype type = MPI_C_BOOL;

	MPI_Aint address;
	MPI_Aint temp;
	MPI_Get_address(&this->mdata[i * gcols + j], &temp);
	address = temp;

	MPI_Type_create_struct(1, &block_length, &address, &type, mpi_type);
	MPI_Type_commit(mpi_type);
}

void Golp::gen_all_types()
{
	gen_row_type(1, &mpi_top_row_send);								// 1st row to send
	gen_row_type(0, &mpi_top_row_recv);								// 0th row to recv
	gen_row_type(prows, &mpi_bot_row_send);							// second last row to send
	gen_row_type(prows + 1, &mpi_bot_row_recv);						// last row to recv

	gen_col_types();												// all cols

	gen_corner_type(1, 1, &mpi_left_top_send);						// inner top left corner to send
	gen_corner_type(0, 0, &mpi_left_top_recv);						// outer top left corner to recv
	gen_corner_type(1, pcols, &mpi_right_top_send);					// inner top right corner to send
	gen_corner_type(0, pcols + 1, &mpi_right_top_recv);				// outer top right corner to recv

	gen_corner_type(prows, 1, &mpi_left_bot_send);					// inner bot left corner to send
	gen_corner_type(prows + 1, 0, &mpi_left_bot_recv);				// outer bot left corner to recv
	gen_corner_type(prows, pcols, &mpi_right_bot_send);				// inner bot right corner to send
	gen_corner_type(prows + 1, pcols + 1, &mpi_right_bot_recv);		// outer bot right corner to recv
}

// free all mpi types
void Golp::clean_all_types()
{
	MPI_Type_free(&mpi_left_col_send);
	MPI_Type_free(&mpi_left_col_recv);
	MPI_Type_free(&mpi_right_col_send);
	MPI_Type_free(&mpi_right_col_recv);

	MPI_Type_free(&mpi_top_row_send);
	MPI_Type_free(&mpi_top_row_recv);
	MPI_Type_free(&mpi_bot_row_send);
	MPI_Type_free(&mpi_bot_row_recv);

	MPI_Type_free(&mpi_left_top_send);
	MPI_Type_free(&mpi_left_top_recv);
	MPI_Type_free(&mpi_right_top_send);
	MPI_Type_free(&mpi_right_top_recv);
	MPI_Type_free(&mpi_left_bot_send);
	MPI_Type_free(&mpi_left_bot_recv);
	MPI_Type_free(&mpi_right_bot_send);
	MPI_Type_free(&mpi_right_bot_recv);
}

// free col types only
void Golp::clean_col_types()
{
	MPI_Type_free(&mpi_left_col_send);
	MPI_Type_free(&mpi_left_col_recv);
	MPI_Type_free(&mpi_right_col_send);
	MPI_Type_free(&mpi_right_col_recv);
}

// assign generated mpi types: send
void Golp::update_send_types()
{
	this->send_types[0] = mpi_left_top_send;
	this->send_types[1] = mpi_top_row_send;
	this->send_types[2] = mpi_right_top_send;
	this->send_types[3] = mpi_left_col_send;
	this->send_types[4] = mpi_right_col_send;
	this->send_types[5] = mpi_left_bot_send;
	this->send_types[6] = mpi_bot_row_send;
	this->send_types[7] = mpi_right_bot_send;
}

// assign generated mpi types: recv
void Golp::update_recv_types()
{
	this->recv_types[0] = mpi_left_top_recv;
	this->recv_types[1] = mpi_top_row_recv;
	this->recv_types[2] = mpi_right_top_recv;
	this->recv_types[3] = mpi_left_col_recv;
	this->recv_types[4] = mpi_right_col_recv;
	this->recv_types[5] = mpi_left_bot_recv;
	this->recv_types[6] = mpi_bot_row_recv;
	this->recv_types[7] = mpi_right_bot_recv;
}

// generate random matrix 
void Golp::rand_init()
{
	for (int i = 0; i < grows; i++)
	{
		for (int j = 0; j < gcols; j++)
		{
			if (i == 0 || j == 0 || i == grows - 1 || j == gcols - 1) 
				this->mdata[i * gcols + j] = 0;
			else 
				this->mdata[i * gcols + j] = rand() % 2;
		}
	}
}

// print out matrix
void Golp::show_proc(int id)
{
	for (int i = 0; i < grows; i++)
	{
		cout << "id " << id << ": ";
		for (int j = 0; j < gcols; j++)
		{
			cout << this->mdata[i * gcols + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

// do game of life and store results in a external storer
void Golp::game_of_life()
{
	// store a matrix containing the sum of the neighbours
	for (int i = 1; i < grows - 1; i++)
	{
		for (int j = 1; j < gcols - 1; j++)
		{
			int sum = 0;
			sum += mdata[(i - 1) * gcols + (j - 1)];
			sum += mdata[(i - 1) * gcols + j];
			sum += mdata[(i - 1) * gcols + (j + 1)];
			sum += mdata[i * gcols + (j - 1)];
			sum += mdata[i * gcols + (j + 1)];
			sum += mdata[(i + 1) * gcols + (j - 1)];
			sum += mdata[(i + 1) * gcols + j];
			sum += mdata[(i + 1) * gcols + (j + 1)];
			sum_mat[i * gcols + j] = sum;
		}
	}

	for (int i = 1; i < grows - 1; i++)
	{
		for (int j = 1; j < gcols - 1; j++)
		{
			this->mdata[i * gcols + j] = this->mdata[i * gcols + j]
				* (sum_mat[i * gcols + j] == 2) + (sum_mat[i * gcols + j] == 3);
		}
	}
}

// store current matrix into a txt file: path at the current directory with 
// different names for periodic and non-periodic
void Golp::store_proc(int id, int iter, bool peri)
{
	string input, fname;
	if (peri)
	{
		fname = "./Periodic/Peri_Iter" + to_string(iter) + "_Proc_" + to_string(id) + ".txt";
	}
	else
	{
		fname = "./NonPeriodic/NonPeri_Iter" + to_string(iter) + "_Proc_" + to_string(id) + ".txt";
	}
	ofstream outfile(fname);
	outfile << "#id: " << id;
	for (int i = 0; i < grows; i++)
	{
		outfile << "\n";
		for (int j = 0; j < gcols; j++)
		{
			outfile << this->mdata[i * gcols + j] << " ";
		}
	}
	outfile.close();
}
