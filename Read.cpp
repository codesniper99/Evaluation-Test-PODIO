/*
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
    hid_t       file, memtype, space, dset,array_tid,datatype1;     /*The Handles */
    herr_t      status;                                             /*status variable*/
    hsize_t     dims[1] = {DIM0};                                   // sizes
    hsize_t     i;
    hsize_t     adims[]={ARRAY_DIM};
    SimpleStruct *rdata;                         /* Read buffer */
    int         ndims;                          //dimensions

    array_tid = H5Tarray_create(H5T_NATIVE_INT, ARRAY_RANK, adims);
    memtype = H5Tcreate (H5T_COMPOUND, sizeof (SimpleStruct));
    H5Tinsert (memtype, "x", HOFFSET (SimpleStruct, x),   H5T_NATIVE_INT);
    H5Tinsert (memtype, "y", HOFFSET (SimpleStruct, y),   H5T_NATIVE_INT);
    H5Tinsert (memtype, "z", HOFFSET (SimpleStruct, z),   H5T_NATIVE_INT);
    H5Tinsert (memtype, "p", HOFFSET (SimpleStruct, p),   array_tid);

    file = H5Fopen (FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
    dset = H5Dopen (file, DATASET, H5P_DEFAULT);
    
    /*
     * Get dataspace and allocate memory for read buffer.
     */
    space = H5Dget_space (dset);
    ndims = H5Sget_simple_extent_dims (space, dims, NULL);
    //Pointer Allocation
    rdata = new SimpleStruct[100];
    /*
     * Read the data.
     */
    H5Dread (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdata);
    /*
     * Output the data to the screen.
     */
    for (i=0; i<dims[0]; i++) {
       cout<<"DATASPACE["<<i+1<<"]\n";
       cout<<rdata[i].x<<"\n"<<rdata[i].y<<"\n"<<rdata[i].z<<"\n"<<rdata[i].p[0]<<"\n"<<rdata[i].p[1]<<"\n"<<rdata[i].p[2]<<"\n"<<rdata[i].p[3]<<"\n\n";
    }
    /*
     * Close and release resources.  
     */
    H5Dvlen_reclaim (memtype, space, H5P_DEFAULT, rdata);
    free (rdata);
    H5Dclose (dset);
    H5Sclose (space);
    H5Tclose (memtype);
    H5Fclose (file);
    return 0;
}
