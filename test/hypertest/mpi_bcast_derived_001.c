/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Bcast连续自定义数据类型测试，单个数据大小覆盖0、
 *              am_short&bcopy_one临界值左右、超大几种场景
 * Author: shizhibao
 * Create: 2021-01-09
 */

#include "mpi_test_common.h"

static int g_count[] = {0, 1, 100};

#define bcast_arr_int_test(type, mpi_type, n)                   \
    {                                                           \
        DECL_MALLOC_INOUT_SOL(type);                            \
        if (rank == root) { SET_INDEX_ARRAY_SUM(io, 123, a, n); } \
        else { SET_INDEX_ARRAY_CONST(io, -1, a, n); }           \
        SET_INDEX_ARRAY_SUM(sol, 123, a, n);                    \
        ARRAY_BCAST_AND_FREE(mpi_type, root, io, sol, n);       \
    }

#define declare_arr_int_struct(n)                                       \
struct arr_int##n {                                                     \
    int a[n];                                                           \
};

declare_arr_int_struct(0);
declare_arr_int_struct(10);
declare_arr_int_struct(45);
declare_arr_int_struct(300);
declare_arr_int_struct(2000);
declare_arr_int_struct(3000);
declare_arr_int_struct(100000);

#define mpi_dt_create(n)                            \
    MPI_Datatype dt_int##n;                         \
    MPI_Type_contiguous(n, MPI_INT, &dt_int##n);    \
    MPI_Type_commit(&dt_int##n)

#define mpi_dt_free(n)                              \
    MPI_Type_free(&dt_int##n)

int main(int argc, char *argv[])
{
    int rank, size;
    int i, count;
    int root = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    mpi_dt_create(0);
    mpi_dt_create(10);
    mpi_dt_create(45);
    mpi_dt_create(300);
    mpi_dt_create(2000);
    mpi_dt_create(3000);
    mpi_dt_create(100000);

    /* test small, medium, large and super large length */
    for (i = 0; i < ARRAY_SIZE(g_count); i++) {
        count = g_count[i];
        bcast_arr_int_test(struct arr_int0, dt_int0, 0);
        bcast_arr_int_test(struct arr_int10, dt_int10, 10);
    }

    mpi_dt_free(0);
    mpi_dt_free(10);
    mpi_dt_free(45);
    mpi_dt_free(300);
    mpi_dt_free(2000);
    mpi_dt_free(3000);
    mpi_dt_free(100000);

    WAIT_ALL_SUCCESS();

    MPI_Finalize();
    return 0;
}

