#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cmath>
#include <vector>
#include "Golp.h"
#include <string>
#include <iomanip>
#include <chrono>

using namespace std;

int id, p;

float comm_time = 0;                        // count communication time
float gol_time = 0;                         // count game of life time

//int recv_prow, recv_pcol;
int recv_ratio[2];

void assign_grids(int* prow, int* pcol, const int m, const int n)
{
    double mat_ratio = (double)m / (double)n;
    vector<int> divs;
    for (int i = 1; i <= p / 2; i++)
    {
        if (p % i == 0)
        {
            divs.push_back(i);
        }
    }
    divs.push_back(p); // push back p itself

    // get the pair of divisors that have the most similar ratio as the preset field
    double best_ratio = (double)1 / (double)p;              // set initial ratio
    int best_vert = 1, best_hori = p;                       // record num of procs in row/col direction
    double smallest_diff = abs(best_ratio - mat_ratio);     // record ratio difference
    for (int i = 0; i < divs.size(); i++)
    {
        double cur_ratio = (double)divs[i] / (p / (double)divs[i]);
        double cur_diff = abs(cur_ratio - mat_ratio);
        if (cur_diff < smallest_diff)
        {
            smallest_diff = cur_diff;
            best_ratio = cur_ratio;
            best_vert = divs[i];
            best_hori = p / divs[i];
        }
    }

    int norm_prow = (double)m / (double)best_vert + 0.5;    // get rounded num of rows for each proc
    int norm_pcol = (double)n / (double)best_hori + 0.5;    // get rounded num of cols for each proc
    int last_prow = m - (best_vert - 1) * norm_prow;        // get rows for procs at last row
    int last_pcol = n - (best_hori - 1) * norm_pcol;        // get cols for procs at last col

    // assign num of rows and cols to their corresponding index as procs
    for (int i = 0; i < best_vert; i++)
    {
        for (int j = 0; j < best_hori; j++)
        {
            if (i != best_vert - 1) prow[i * best_hori + j] = norm_prow;
            else prow[i * best_hori + j] = last_prow;

            if (j != best_hori - 1) pcol[i * best_hori + j] = norm_pcol;
            else pcol[i * best_hori + j] = last_pcol;
        }
    }
    //cout << "Mat ratio: " << mat_ratio << endl;
    //cout << "Best pair: " << best_vert << " " << best_hori << endl;
    //cout << "Best ratio: " << best_ratio << endl;
    //cout << "Processor rows: " << endl;
    //for (int i = 0; i < p; i++) cout << prow[i] << " ";
    //cout << endl;
    //cout << "Processor cols: " << endl;
    //for (int i = 0; i < p; i++) cout << pcol[i] << " ";
    //cout << endl;
    //int sum = 0;
    //for (int i = 0; i < p; i++)
    //    sum += prow[i] * pcol[i];
    //cout << "Total count now is: " << sum << " comparing to m * n:" << m * n << endl;
    recv_ratio[0] = best_vert;
    recv_ratio[1] = best_hori;
    if (id == 0) cout << "Split up strategy: " << best_vert << " in rows, " << best_hori << " in cols." << endl;
}

// get neighbours: periodic (const 8 neighbours)
vector<int> nb_list(8);                 // to save neighbours
void get_adj_periodic(int id)
{
    int i_id = id / recv_ratio[1];      // proc row index
    int j_id = id % recv_ratio[1];      // proc col index
    int cnt = 0;                        // get count of neighbours
    for (int ii = -1; ii <= 1; ii++)
    {
        for (int jj = -1; jj <= 1; jj++)
        {
            int i = ii + i_id;
            int j = jj + j_id;
            if (i != i_id || j != j_id)
            {
                int nb_i = (i + recv_ratio[0]) % recv_ratio[0];     // find neighbour's row index
                int nb_j = (j + recv_ratio[1]) % recv_ratio[1];     // find neighbour's row index
                int nb_id = nb_i * recv_ratio[1] + nb_j;
                nb_list[cnt] = nb_id;   // save neighbour id
                cnt++;
            }
        }
    }
    //cout << "Process " << id << " has neighbours: " << endl;
    //for (int i = 0; i < 8; i++)
    //{
    //    cout << nb_list[i] << " ";
    //}
    //cout << endl;
    //cout.flush();
}

void game_of_life_periodic(const int loops, Golp &old_mesh, int* send_tags, int* recv_tags, MPI_Request* requests)
{
    get_adj_periodic(id);
    for (int l = 0; l < loops; l++)
    {
        auto comm_start = chrono::high_resolution_clock::now();
        // send communications
        old_mesh.left_top_send(nb_list[0], send_tags[0], &requests[0]);
        old_mesh.top_row_send(nb_list[1], send_tags[1], &requests[1]);
        old_mesh.right_top_send(nb_list[2], send_tags[2], &requests[2]);
        old_mesh.left_col_send(nb_list[3], send_tags[3], &requests[3]);
        old_mesh.right_col_send(nb_list[4], send_tags[4], &requests[4]);
        old_mesh.left_bot_send(nb_list[5], send_tags[5], &requests[5]);
        old_mesh.bot_row_send(nb_list[6], send_tags[6], &requests[6]);
        old_mesh.right_bot_send(nb_list[7], send_tags[7], &requests[7]);
        // recv communications
        old_mesh.left_top_recv(nb_list[0], recv_tags[0], &requests[8]);
        old_mesh.top_row_recv(nb_list[1], recv_tags[1], &requests[9]);
        old_mesh.right_top_recv(nb_list[2], recv_tags[2], &requests[10]);
        old_mesh.left_col_recv(nb_list[3], recv_tags[3], &requests[11]);
        old_mesh.right_col_recv(nb_list[4], recv_tags[4], &requests[12]);
        old_mesh.left_bot_recv(nb_list[5], recv_tags[5], &requests[13]);
        old_mesh.bot_row_recv(nb_list[6], recv_tags[6], &requests[14]);
        old_mesh.right_bot_recv(nb_list[7], recv_tags[7], &requests[15]);

        // wait for all communications to finish
        MPI_Waitall(16, requests, MPI_STATUSES_IGNORE);
        //if (id == 0) old_mesh.show_proc(id);
        auto comm_end = chrono::high_resolution_clock::now();
        comm_time += (comm_end - comm_start).count();
        // store matrix
        //old_mesh.store_proc(id, l, true);

        auto gol_start = chrono::high_resolution_clock::now();
        old_mesh.game_of_life();
        auto gol_end = chrono::high_resolution_clock::now();
        gol_time += (gol_end - gol_start).count();
    }
    // store last result
    //old_mesh.store_proc(id, loops, true);
    //if (id == 0) old_mesh.show_proc(id);
}

// for non_periodic case
vector<int> act_nbs;                    // to store available neighbours
vector<MPI_Datatype> send_type_list;    // to store send type for corresponding neighbour
vector<MPI_Datatype> recv_type_list;    // to store recv type for corresponding neighbour
vector<int> index_list;                 // to store indices to call corresponding types and neighbours
void game_of_life_non_periodic(const int loops, Golp& old_mesh, int* send_tags, int* recv_tags, MPI_Request* requests)
{
    // find neighbours: 
    int i_id = id / recv_ratio[1];
    int j_id = id % recv_ratio[1];
    int cnt = 0;
    for (int ii = -1; ii <= 1; ii++)
    {
        for (int jj = -1; jj <= 1; jj++)
        {
            int i = ii + i_id;
            int j = jj + j_id;
            if ((i >= 0 && i < recv_ratio[0]) && (j >= 0 && j < recv_ratio[1]) && (i != i_id || j != j_id))
            {
                int nb_id = i * recv_ratio[1] + j;
                act_nbs.push_back(nb_id);                               // get existing neighbours inside the field
                send_type_list.push_back(old_mesh.send_types[cnt]);     // record send types 
                recv_type_list.push_back(old_mesh.recv_types[cnt]);     // record recv types
                index_list.push_back(cnt);                              // record indices for types and neighbours
                cnt++;
            }
            if (i < 0 || i >= recv_ratio[0] || j < 0 || j >= recv_ratio[1])
            {
                cnt++;                                                  // get count even not satisfied
            }
        }
    }

    // communicate with neighbours
    for (int l = 0; l < loops; l++)
    {
        cnt = 0;
        int nb_num = act_nbs.size();                                    // get actual number of neighbours: same as recv & send types
        auto comm_start = chrono::high_resolution_clock::now();
        for (int i = 0; i < nb_num; i++)
        {
            MPI_Isend(MPI_BOTTOM, 1, send_type_list[i], act_nbs[i], send_tags[index_list[i]], MPI_COMM_WORLD, &requests[cnt]);
            cnt++;
        }

        for (int i = 0; i < nb_num; i++)
        {
            MPI_Irecv(MPI_BOTTOM, 1, recv_type_list[i], act_nbs[i], recv_tags[index_list[i]], MPI_COMM_WORLD, &requests[cnt]);
            cnt++;
        }

        MPI_Waitall(cnt, requests, MPI_STATUSES_IGNORE);    // wait for all communications to finish
        auto comm_end = chrono::high_resolution_clock::now();
        comm_time += (comm_end - comm_start).count();

        // store matrix for current proc
        //old_mesh.store_proc(id, l, false);
        auto gol_start = chrono::high_resolution_clock::now();
        old_mesh.game_of_life();
        auto gol_end = chrono::high_resolution_clock::now();
        gol_time += (gol_end - gol_start).count();
    }
    // store last result
    //old_mesh.store_proc(id, loops, false);
}


int main(int argc, char* argv[])
{
    const int m = atoi(argv[1]);
    const int n = atoi(argv[2]);
    const int loops = atoi(argv[3]);
    const bool periodic_boundary = atoi(argv[4]);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    srand(time(NULL) + id * 10);

    if (id == 0)
    {
        cout << "Task matrix size: " << m << " x " << n << endl;
        cout << "Number of processors: " << p << endl;
        cout << "Periodic boundary: " << periodic_boundary << endl;
        cout << "Loop times: " << loops << endl;
    }
    // initialising tag_nums: send & recv using reverse order of tags
    int* send_tags = new int[8];
    for (int i = 0; i < 8; i++) send_tags[i] = i + 1;
    int* recv_tags = new int[8];
    for (int i = 0; i < 8; i++) recv_tags[i] = 8 - i;
    // initialising requests: 8 for periodic and random from 0 - 8 for non-periodic
    MPI_Request* requests = new MPI_Request[16];

    // rows and cols responsible for all proc
    int* prow = new int[p];
    int* pcol = new int[p];

    // get prow and pcol
    assign_grids(prow, pcol, m, n);
    // get local rows and cols
    int recv_prow = prow[id];
    int recv_pcol = pcol[id];

    // initialise mesh and an empty storer
    Golp* old_mesh = new Golp(recv_prow, recv_pcol);
    old_mesh->rand_init();

    if (id == 0) {
        cout << "\nTimings: " << endl;
        cout << setw(12) << "proc num" << setw(12) << "id" << setw(12) 
             << "total" << setw(12) << "comm" << setw(12) << "game" << endl;
    }
    // timing start
    auto start = chrono::high_resolution_clock::now();

    float total_time = 0;
    if (periodic_boundary)
    {
        old_mesh->gen_col_types();          // generate col mpi types
        auto total_start = chrono::high_resolution_clock::now();
        game_of_life_periodic(loops, *old_mesh, send_tags, recv_tags, requests);
        auto total_end = chrono::high_resolution_clock::now();
        total_time += (total_end - total_start).count();
        old_mesh->clean_col_types();        // free col types
    }
    else
    {
        old_mesh->gen_all_types();          // generate all mpi types
        old_mesh->update_send_types();      // update send types array
        old_mesh->update_recv_types();      // update recv types array
        auto total_start = chrono::high_resolution_clock::now();
        game_of_life_non_periodic(loops, *old_mesh, send_tags, recv_tags, requests);
        auto total_end = chrono::high_resolution_clock::now();
        total_time += (total_end - total_start).count();
        old_mesh->clean_all_types();        // free all types
    }

    // Timing: for const matrix size and loop time, different process number
    if (id == 0)
    {
        cout << setw(12) << p
            << setw(12) << id
            << setw(12) << total_time / 1e9
            << setw(12) << comm_time / 1e9
            << setw(12) << gol_time / 1e9 << endl;
    }

    MPI_Finalize();
    delete old_mesh;
    delete[] prow;
    delete[] pcol;
    delete[] requests;
    delete[] send_tags;
    delete[] recv_tags;
}