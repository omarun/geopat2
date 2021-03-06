/****************************************************************************
 *
 * PROGRAM:	gpat_segquality - part of GeoPAT 2
 * AUTHOR(S):	Pawel Netzel
 * PURPOSE:	program for a grid of motifels segmentation;
 *		segmentation code by Jaroslaw Jasiewicz, Jacek Niesterowicz, 
 *		Tomasz Stepinski from GRASS GeoPAT 
 * COPYRIGHT:	(C) Pawel Netzel, Space Informatics Lab,
 *		University of Cincinnati
 *              http://sil.uc.edu
 *
 *		This program is free software under 
 *		the GNU General Public License (>=v3). 
 *		https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 *****************************************************************************/
#include "../../lib/ezGDAL/ezgdal.h"
#include "../../lib/SML/sml.h"
#include <gdal.h>
#include <gdal_alg.h>
#include <ogr_srs_api.h>

void write_raster(SML_DATA_HEADER* dh, void *map, char *raster_fname, int size) {

    int r, c, i;
    int nrows = dh->file_win->rows;
    int ncols = dh->file_win->cols;
    int nodata = (int)0x80000000;
    int *row = map;
    EZGDAL_LAYER *l;
    SML_WINDOW *win;

    if(raster_fname==NULL)
        return;

    if(map==NULL) {
        ezgdal_show_message(stdout, "No results map to write");
        return;
    }

    win = sml_create_window_copy(dh->file_win);

    win->at[1] /= (double)size;
    win->at[5] /= (double)size;

    l = ezgdal_create_layer(raster_fname,
                          win->proj,
                          "Int32",
                          win->at,
                          nrows*size,
                          ncols*size,
                          &nodata);


    fprintf(stdout, "Writing output ... ");
    ezgdal_show_progress(stdout,0,nrows);
    for(r=0; r<nrows; r++) {
        for(c=0; c<ncols; c++)
            for(i=0; i<size; i++)
                l->buffer[c*size+i]=row[c];
        for(i=0; i<size; i++)
            ezgdal_write_buffer(l, r*size+i);
        row += ncols;
        ezgdal_show_progress(stdout,r,nrows);
    }
    ezgdal_show_progress(stdout,100,100);

    ezgdal_close_layer(l);

    sml_free_window(win);
}

void write_raster_dbl(SML_DATA_HEADER* dh, void *map, char *raster_fname, int size) {

    int r, c, i;
    int nrows = dh->file_win->rows;
    int ncols = dh->file_win->cols;
    int nodata = (int)0x80000000;
    double *row = map;
    EZGDAL_LAYER *l;
    SML_WINDOW *win;

    if(raster_fname==NULL)
        return;

    if(map==NULL) {
        ezgdal_show_message(stdout, "No results map to write");
        return;
    }

    win = sml_create_window_copy(dh->file_win);

    win->at[1] /= (double)size;
    win->at[5] /= (double)size;

    l = ezgdal_create_layer(raster_fname,
                          win->proj,
                          "Float64",
                          win->at,
                          nrows*size,
                          ncols*size,
                          &nodata);


    fprintf(stdout, "Writing output...");
    ezgdal_show_progress(stdout,0,nrows);
    for(r=0; r<nrows; r++) {
        for(c=0; c<ncols; c++)
            for(i=0; i<size; i++)
                l->buffer[c*size+i]=row[c];
        for(i=0; i<size; i++)
            ezgdal_write_buffer(l, r*size+i);
        row += ncols;
        ezgdal_show_progress(stdout,r,nrows);
    }
    ezgdal_show_progress(stdout,100,100);

    ezgdal_close_layer(l);

    sml_free_window(win);
}

