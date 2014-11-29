/******************************************************************************
 * @section DESCRIPTION
 *
 * Header file for vic_driver_image routines
 *
 * @section LICENSE
 *
 * The Variable Infiltration Capacity (VIC) macroscale hydrological model
 * Copyright (C) 2014 The Land Surface Hydrology Group, Department of Civil
 * and Environmental Engineering, University of Washington.
 *
 * The VIC model is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

#ifndef VIC_DRIVER_IMAGE_H
#define VIC_DRIVER_IMAGE_H

#include <stdbool.h>
#include <netcdf.h>
#include <stdio.h>
#include <vic_driver_shared.h>

#define MAXDIMS 10

/******************************************************************************
 * @brief    Structure to store location information for individual grid cells.
 * @details  The global and local indices show the position of the grid cell
 *           within the global and local (processor) domains. When the model is
 *           run on a single processor, the glonal and local domains are
 *           identical. The model is run over a list of cells.
 *****************************************************************************/
typedef struct {
    double latitude; // latitude of grid cell center
    double longitude; // longitude of grid cell center
    double area; // area of grid cell
    double frac; // fraction of grid cell that is active
    size_t global_cell_idx; // index of grid cell in global list of grid cells
    size_t global_x_idx; // index of x-dimension in global domain
    size_t global_y_idx; // index of y-dimension in global domain
    size_t local_cell_idx; // index of grid cell in local list of grid cells
    size_t local_x_idx; // index of x-dimension in local domain
    size_t local_y_idx; // index of y-dimension in local domain
} location_struct;


/******************************************************************************
 * @brief    Structure to store local and global domain information. If the
 *           model is run on a single processor, then the two are identical.
 *****************************************************************************/
typedef struct {
    size_t ncells_global; // number of active grid cell on global domain
    size_t n_nx; // size of x-index;
    size_t n_ny; // size of y-index
    size_t ncells_local; // number of active grid cell on local domain
    location_struct *locations; // locations structs for local domain
} domain_struct;

/******************************************************************************
 * @brief    Structure for netcdf file information. Initially to store
 *           information for the output files (state and history)
 *****************************************************************************/
typedef struct {
    char fname[MAXSTRING + 1];
    char c_fillvalue;
    int i_fillvalue;
    double d_fillvalue;
    float f_fillvalue;
    int nc_id;
    int band_dimid;
    int front_dimid;
    int frost_dimid;
    int layer_dimid;
    int ni_dimid;
    int nj_dimid;
    int node_dimid;
    int root_zone_dimid;
    int time_dimid;
    int veg_dimid;
    size_t band_size;
    size_t front_size;
    size_t frost_size;
    size_t layer_size;
    size_t ni_size;
    size_t nj_size;
    size_t node_size;
    size_t root_zone_size;
    size_t time_size;
    size_t veg_size;
    bool open;
} nc_file_struct;

/******************************************************************************
 * @brief    Structure for netcdf variable information
 *****************************************************************************/
typedef struct {
    char nc_var_name[MAXSTRING]; // variable name
    char nc_units[MAXSTRING]; // variable name
    int nc_dimids[MAXDIMS]; // ids of dimensions
    int nc_counts[MAXDIMS]; // size of dimid
    int nc_type; // netcdf type
    int nc_aggtype; // aggregation type as defined in vic_def.h
    int nc_dims; // number of dimensions
    int nc_write; // TRUE: write to file; FALSE: don't
} nc_var_struct;

/******************************************************************************
 * @brief    Structure for mapping the vegetation types for each grid cell as
 *           stored in VIC's veg_con_struct to a regular array.
 *****************************************************************************/
typedef struct {
    size_t nv_types; // total number of vegetation types
                     // size of vidx and Cv arrays
    size_t nv_active; // number of active vegetation types. Because of the
                      // way that VIC defines nveg, this is nveg+1
                      // (for bare soil) or nveg+2 (if the treeline option
                      // is active as well)
    int *vidx;      // array of indices for active vegetation types
    double *Cv;     // array of fractional coverage for nc_types
} veg_con_map_struct;

void alloc_atmos(atmos_data_struct *atmos);
void alloc_veg_hist(veg_hist_struct *veg_hist);
double air_density(double t, double p);
double average(double *ar, size_t n);
out_data_struct *create_output_list(void);
void free_atmos(atmos_data_struct *atmos);
void free_out_data(out_data_struct **out_data);
void free_veg_hist(veg_hist_struct *veg_hist);
size_t get_global_domain(char *fname, domain_struct *global_domain);
size_t get_global_idx(domain_struct *domain, size_t i);
global_param_struct get_global_param(filenames_struct *, FILE *);
size_t get_nc_dimension(char *nc_name, char *dim_name);
int get_nc_field_double(char *nc_name, char *var_name, size_t *start,
                        size_t *count, double *var);
int get_nc_field_float(char *nc_name, char *var_name, size_t *start,
                       size_t *count, float *var);
int get_nc_field_int(char *nc_name, char *var_name, size_t *start,
                     size_t *count, int *var);
void init_output_list(out_data_struct *out_data, int write, char *format,
                      int type, double mult);
void initialize_domain(domain_struct *domain);
void initialize_energy(energy_bal_struct **energy, size_t nveg);
void initialize_history_file(nc_file_struct *nc);
void initialize_location(location_struct *location);
int initialize_model_state(all_vars_struct *all_vars, size_t Nveg,
                           size_t Nnodes, double surf_temp,
                           soil_con_struct *soil_con, veg_con_struct *veg_con);
void initialize_soil_con(soil_con_struct *soil_con);
void initialize_state_file(nc_file_struct *nc);
void initialize_veg_con(veg_con_struct *veg_con);
FILE *open_file(char *string, char *type);
int parse_output_info(FILE *gp, out_data_struct **out_data);
void print_atmos_data(atmos_data_struct *atmos);
void print_cell_data(cell_data_struct *cell, size_t nlayers, size_t nfrost,
                     size_t npet);
void print_dmy(dmy_struct *dmy);
void print_domain(domain_struct *domain, bool print_loc);
void print_energy_bal(energy_bal_struct *eb, size_t nnodes, size_t nfronts);
void print_filenames(filenames_struct *fnames);
void print_filep(filep_struct *fp);
void print_force_type(force_type_struct *force_type);
void print_global_param(global_param_struct *gp);
void print_lake_con(lake_con_struct *lcon, size_t nlnodes);
void print_lake_var(lake_var_struct *lvar, size_t nlnodes, size_t nfronts,
                    size_t nlayers, size_t nnodes, size_t nfrost, size_t npet);
void print_layer_data(layer_data_struct *ldata, size_t nfrost);
void print_location(location_struct *location);
void print_nc_file(nc_file_struct *nc);
void print_nc_var(nc_var_struct *nc_var, size_t ndims);
void print_option(option_struct *option);
void print_out_data(out_data_struct *out, size_t nelem);
void print_out_data_file(out_data_file_struct *outf);
void print_param_set(param_set_struct *param_set);
void print_save_data(save_data_struct *save);
void print_snow_data(snow_data_struct *snow);
void print_soil_con(soil_con_struct *scon, size_t nlayers, size_t nnodes,
                    size_t nfrost, size_t nbands, size_t nzwt);
void print_veg_con(veg_con_struct *vcon, size_t nroots, char blowing, char lake,
                   char carbon, size_t ncanopy);
void print_veg_var(veg_var_struct *vvar, size_t ncanopy);
void print_veg_con_map(veg_con_map_struct *veg_con_map);
void print_veg_lib(veg_lib_struct *vlib, char carbon);
int put_nc_field_double(char *nc_name, bool *open, int *nc_id, double fillval,
                        int *dimids, int ndims, char *var_name, size_t *start,
                        size_t *count, double *var);
int put_nc_field_int(char *nc_name, bool *open, int *nc_id, int fillval,
                     int *dimids, int ndims, char *var_name, size_t *start,
                     size_t *count, int *var);
double q_to_vp(double q, double p);
void sprint_location(char *str, location_struct *loc);
void vic_alloc(void);
void vic_nc_info(nc_file_struct *nc_hist_file, out_data_struct **out_data,
                 nc_var_struct *nc_vars);
void vic_finalize(void);
void vic_force(void);
void vic_image_run(void);
void vic_init(void);
void vic_init_output(void);
void vic_restore(void);
void vic_start(void);
void vic_store(void);
void vic_write(void);
char will_it_snow(double *t, double t_offset, double max_snow_temp,
                  double *prcp, size_t n);

#endif
