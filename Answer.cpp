/*
 *   C++ CODE written by Akhil Vaid, NSIT, New Delhi, India for GSOC 2019 Evaluation Test
 */
#include "hdf5.h"
#include <stdio.h>
#include <stdlib.h>

#define FILE            "outputs.h5"
#define DATASET         "DS1"
#define DIM0            100
#define ARRAY_DIM       4
#define ARRAY_RANK      1

typedef struct {
    int     x;
    int     y;
    int     z;
    int     p[ARRAY_DIM];
    
} SimpleStruct;                                 /* Compound type */

int main ()
{
    hid_t       file, filetype, memtype, space, dset,array_tid;     /*The Handles */

    herr_t      status;                                             /*status variable*/

    hsize_t     dims[1] = {DIM0};                                   // sizes
    hsize_t     i;
    hsize_t     adims[]={ARRAY_DIM};

    SimpleStruct    arra[DIM0],                 /* Write buffer */
                *rdata;                         /* Read buffer */

    int         ndims;                          //dimensions

    /*
     * Initialize data.
     */
    for( int i=0; i<dims[0];i++)
    {
        arra[i].x    = i+1 ;
        arra[i].y    = i+101 ;
        arra[i].z    = i+501 ;
        arra[i].p[0] = i*2 ;
        arra[i].p[1] = i*3 ;
        arra[i].p[2] = i*4 ;
        arra[i].p[3] = i*5 ;
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
    
    status = H5Tinsert (memtype, "x", HOFFSET (SimpleStruct, x),   H5T_NATIVE_INT);

    status = H5Tinsert (memtype, "y", HOFFSET (SimpleStruct, y),   H5T_NATIVE_INT);

    status = H5Tinsert (memtype, "z", HOFFSET (SimpleStruct, z),   H5T_NATIVE_INT);

    status = H5Tinsert (memtype, "p", HOFFSET (SimpleStruct, p),   array_tid);
    
    /*
     * Create the compound datatype for the file.  Because the standard
     * types we are using for the file may have different sizes than
     * the corresponding native types, we manually calculate the
     * offset of each member.
     */

    filetype = H5Tcreate (H5T_COMPOUND, 8 + 8 + 8 + 32 );
    status   = H5Tinsert (filetype, "x", 0,         H5T_STD_I64BE);
    status   = H5Tinsert (filetype, "y", 8,         H5T_STD_I64BE);
    status   = H5Tinsert (filetype, "z", 8 + 8,     H5T_STD_I64BE);
    status   = H5Tinsert (filetype, "p", 8 + 8 + 8, array_tid);
    /*
     * Create dataspace.  Setting maximum size to NULL sets the maximum
     * size to be the current size.
     */
    space = H5Screate_simple (1, dims, NULL);

    /*
     * Create the dataset and write the compound data to it.
     */
    dset   = H5Dcreate (file, DATASET, filetype, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Dwrite (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, arra);

    /*
     * Close and release resources.
     */
    status = H5Dclose (dset);
    status = H5Sclose (space);
    status = H5Tclose (filetype);
    status = H5Fclose (file);


    /*
     * Now we begin the read section.  Here assume
     * the dataset has the same name and rank, but can have any size.
     * Therefore we must allocate a new array to read in data using
     * malloc().
     */
    printf("Opening the file\n");
    /*
     * Open file and dataset.
     */
    file = H5Fopen (FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
    dset = H5Dopen (file, DATASET, H5P_DEFAULT);

    /*
     * Get dataspace and allocate memory for read buffer.
     */
    space = H5Dget_space (dset);
    ndims = H5Sget_simple_extent_dims (space, dims, NULL);
    rdata = (SimpleStruct *) malloc (dims[0] * (sizeof (SimpleStruct)+8));

    /*
     * Read the data.
     */
    status = H5Dread (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdata);

    /*
     * Output the data to the screen.
     */
    for (i=0; i<dims[0]; i++) {
        printf ("%s[%d]:\n", DATASET, i);
        printf ("x  : %d\n", rdata[i].x);
        printf ("y  : %d\n", rdata[i].y);
        printf ("z  : %d\n", rdata[i].z);
        printf ("p0 : %d\n", rdata[i].p[0]);
        printf ("p1 : %d\n", rdata[i].p[1]);
        printf ("p2 : %d\n", rdata[i].p[2]);
        printf ("p3 : %d\n\n", rdata[i].p[3]);
    }

    /*
     * Close and release resources.  
     */
    status = H5Dvlen_reclaim (memtype, space, H5P_DEFAULT, rdata);
    free (rdata);
    status = H5Dclose (dset);
    status = H5Sclose (space);
    status = H5Tclose (memtype);
    status = H5Fclose (file);
    printf("\n\nSuccessfully closed all resources");
    return 0;
}
