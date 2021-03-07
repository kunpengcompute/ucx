/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Bcast长稳测试，覆盖连续、不连续数据类型
 * Author: shizhibao
 * Create: 2021-01-09
 */

#include "mpi_test_common.h"

/* for MPI_INT */
#define repeat_bcast_test1(type, mpi_type)                      \
    {                                                           \
        DECL_MALLOC_INOUT_SOL(type);                            \
        SET_INDEX_SUM(sol, 987654321);                          \
        while (repeats--) {                                     \
            if (rank == root) { SET_INDEX_SUM(io, 987654321); } \
            else { SET_INDEX_CONST(io, -1); }                   \
            BCAST_NOT_FREE(mpi_type, root, io, sol);            \
        }                                                       \
        free(io); free(sol);                                    \
    }

/* for MPI_LONG_INT */
#define repeat_bcast_test2(type, mpi_type)                      \
    {                                                           \
        DECL_MALLOC_INOUT_SOL(type);                            \
        SET_INDEX_STRUCT_SUM(sol, 9876543210, a);               \
        SET_INDEX_STRUCT_SUM(sol, 123456789, b);                \
        while (repeats--) {                                     \
            if (rank == root) {                                 \
                SET_INDEX_STRUCT_SUM(io, 9876543210, a);        \
                SET_INDEX_STRUCT_SUM(io, 123456789, b);         \
            } else {                                            \
                SET_INDEX_STRUCT_SUM(io, -1, a);                \
                SET_INDEX_STRUCT_SUM(io, -1, b);                \
            }                                                   \
            STRUCT_BCAST_NOT_FREE(mpi_type, root, io, sol);     \
        }                                                       \
        free(io); free(sol);                                    \
    }

struct long_int {
    long a;
    int b;
};

int main(int argc, char *argv[])
{
    int rank, size, root;
    int repeats, count;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    count = 3;
    root = 0;

    /* loop repeats times on contiguous datatype */
    repeats = 1000000;
    repeat_bcast_test1(int, MPI_INT);

    /* loop repeats times on non-contiguous datatype */
    repeats = 1000000;
    repeat_bcast_test2(struct long_int, MPI_LONG_INT);

    WAIT_ALL_SUCCESS();

    MPI_Finalize();
    return 0;
}

