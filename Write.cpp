/*
 *   WRITE FILE
 *   C++ CODE written by Akhil Vaid, NSIT, New Delhi, India for GSOC 2019 Evaluation Test
 */
#include "hdf5.h"
#include<iostream>

#define FILE            "outputs.h5"
#define DATASET         "DS1"
#define DIM0            100
#define ARRAY_DIM       4
#define ARRAY_RANK      1

using namespace std;

typedef struct {
    int     x;
    int     y;
    int     z;
    int     p[ARRAY_DIM];
    
} SimpleStruct;                                 /* Compound type */

int main ()
{
    hid_t       file, memtype, space, dset,array_tid;     /*The Handles */
    herr_t      status;                                             /*status variable*/
    hsize_t     dims[1] = {DIM0};                                   // sizes
    hsize_t     i;
    hsize_t     adims[]={ARRAY_DIM};

    SimpleStruct    arra[DIM0];                 /* Write buffer */
    int         ndims;                          //dimensions

    /*
     * Initialize data.
     */
    for( int i=0; i<dims[0];i++)
    {
        arra[i].x    = i+1 ;
        arra[i].y    = i+101 ;
        arra[i].z    = i+501 ;
        arra[i].p[0] = (i+1)*2 ;
        arra[i].p[1] = (i+1)*3 ;
        arra[i].p[2] = (i+1)*4 ;
        arra[i].p[3] = (i+1)*5 ;
    }
    /*
     * Create a new file using the default properties.
     */
    file = H5Fcreate (FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    /*
     * Create variable-length string datatype.
     */
    array_tid = H5Tarray_create(H5T_NATIVE_INT, ARRAY_RANK, adims);
    /*
     * Create the compound datatype for memory.
     */
    memtype = H5Tcreate (H5T_COMPOUND, sizeof (SimpleStruct)); 
    H5Tinsert (memtype, "x", HOFFSET (SimpleStruct, x),   H5T_NATIVE_INT);
    H5Tinsert (memtype, "y", HOFFSET (SimpleStruct, y),   H5T_NATIVE_INT);
    H5Tinsert (memtype, "z", HOFFSET (SimpleStruct, z),   H5T_NATIVE_INT);
    H5Tinsert (memtype, "p", HOFFSET (SimpleStruct, p),   array_tid);
    //space
    space = H5Screate_simple (1, dims, NULL);
    /*
     * Create the dataset and write the compound data to it.
     */
    dset   = H5Dcreate (file, DATASET, memtype, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Dwrite (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, arra);

    /*
     * Close and release resources.
     */
    H5Dclose (dset);
    H5Sclose (space);
    H5Fclose (file);
    H5Tclose (memtype);

    return 0;
}
