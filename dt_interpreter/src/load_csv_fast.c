//START_INCLUDES
#include "dt_incs.h"
#include "txt_to_I1.h"
#include "txt_to_I2.h"
#include "txt_to_I4.h"
#include "txt_to_I8.h"
#include "txt_to_F4.h"
#include "txt_to_F8.h"
#include "get_cell.h"
#include "mmap.h"
#include "rand_file_name.h"
#include "file_exists.h"
#include "trim.h"
//STOP_INCLUDES
#include "load_csv_fast.h"

/*Given a csv file, this method will convert the file into nC binary files
 * where nC is the number of columns in the csv file and each file will
 * contain the contents of a column from the csv file*/

/*Steps:
 *1) call get cell
 *2) convert cell from string to C type using _txt_to_* methods
 *3) write binary returned from _txt_to_* method to file
 *4) return files
 */
#define LEN_BASE_FILE_NAME 64
#define MAX_LEN_DIR_NAME 255

typedef enum _qtype_type { undef, I1, I2, I4, I8, F4, F8, B1 } qtype_type;
//START_FUNC_DECL
int
load_csv_fast(
    const char * const q_data_dir,
    const char * const infile,
    uint32_t nC,
    uint64_t *ptr_nR,
    char ** fldtypes, /* [nC] */
    bool is_hdr, /* [nC] */
    bool * is_load, /* [nC] */
    bool * has_nulls, /* [nC] */
    uint64_t * num_nulls, /* [nC] */
    char ***ptr_out_files,
    char ***ptr_nil_files,
    /* Note we set nil_files and out_files only if below == NULL */
    char *str_for_lua,
    size_t sz_str_for_lua,
    int *ptr_n_str_for_lua 
    )
//STOP_FUNC_DECL
{
  int status = 0, bak_status = 0;
  char *mmap_file = NULL; //X
  size_t file_size = 0; //nX
  FILE **ofps = NULL; /* Output File PointerS */
  FILE **nn_ofps = NULL; /* NN Output File PointerS */
  qtype_type *qtypes = NULL;
  bool *is_trim = NULL; // whether to trim or not */
  uint64_t *nn_buf = NULL;
  char **out_files = NULL;
  char **nil_files = NULL;
  char *opdir = NULL; 
  uint64_t *word_B1 = NULL; // used for 64 bit integer buffer if col is B1
  /*
  fprintf(stderr, "C: q_data_dir = %s, \n", q_data_dir);
  fprintf(stderr, "C: infile     = %s, \n", infile);
  fprintf(stderr, "C: sz_str_for_lua     = %d, \n", (int)sz_str_for_lua);
  fprintf(stderr, "C: n_str_for_lua     = %d, \n", *ptr_n_str_for_lua);
  fprintf(stderr, "C: is_hdr     = %d, \n", is_hdr);
  fprintf(stderr, "C: str_for_lua = %s, \n", str_for_lua);
*/
  //---------------------------------
  *ptr_n_str_for_lua = 0;
  if ( ( infile == NULL ) || ( *infile == '\0' ) ) { go_BYE(-1); }
  if ( nC == 0 ) { go_BYE(-1); }
  if ( ptr_nR == NULL ) { go_BYE(-1); }
  if ( ( q_data_dir == NULL ) || ( *q_data_dir == '\0' ) ) {

  if ( out_files == NULL ) { go_BYE(-1); }
#define MAX_LEN_DIR_NAME 255
    char cwd[MAX_LEN_DIR_NAME+1];
    memset(cwd, '\0', MAX_LEN_DIR_NAME+1);
    if ( getcwd(cwd, MAX_LEN_DIR_NAME) == NULL ) { go_BYE(-1); }
    opdir = strdup(cwd);
  }
  else {
    opdir = strdup(q_data_dir);
    // FOR TESTING opdir = strdup("/home/subramon/local/Q/data");
  }
  //---------------------------------
  // allocate space and initialize other resources

  if ( ( str_for_lua != NULL ) && ( sz_str_for_lua > 0 ) ) {
    memset(str_for_lua, '\0', sz_str_for_lua);
  }
  word_B1 = malloc(nC * sizeof(uint64_t));
  for ( uint32_t i = 0; i < nC; i++ ) {
    word_B1[i] = 0;
  }

  for ( uint32_t i = 0; i < nC; i++ ) {
    num_nulls[i] = 0;
  }

  is_trim = malloc(nC * sizeof(bool));
  return_if_malloc_failed(is_trim);
  for ( uint32_t i = 0; i < nC; i++ ) { is_trim[i] = false; }

  nn_buf = malloc(nC * sizeof(uint64_t));
  return_if_malloc_failed(nn_buf);
  out_files = malloc(nC * sizeof(char *));
  return_if_malloc_failed(out_files);
  nil_files = malloc(nC * sizeof(char *));
  return_if_malloc_failed(nil_files);

  for ( uint32_t i = 0; i < nC; i++ ) {
    nn_buf[i] = 0;
    out_files[i] = NULL;
    nil_files[i] = NULL;
  }

  if ( opdir[strlen(opdir)-1] == '/' ) { 
    opdir[strlen(opdir)-1] = '\0';
  }
  int ddir_len = strlen(opdir) + 8 + LEN_BASE_FILE_NAME;
  for ( uint32_t i = 0; i < nC; i++ ) {
    if ( !is_load[i] ) { continue; }
    char buf[LEN_BASE_FILE_NAME+1];
    memset(buf, '\0', LEN_BASE_FILE_NAME+1);
    status = rand_file_name(buf, LEN_BASE_FILE_NAME);
    out_files[i] = malloc(ddir_len * sizeof(char));
    sprintf(out_files[i], "%s/%s", opdir, buf);

    if ( has_nulls[i] ) {
      nil_files[i] = malloc(ddir_len * sizeof(char));
      sprintf(nil_files[i], "%s/_nn%s", opdir, buf);
    }
    // fprintf(stderr, "%s, %s \n", out_files[i], nil_files[i]);
  }

  *ptr_nR = 0;
  // set up qtypes  -- convert from strings to enum
  qtypes = malloc(nC * sizeof(qtype_type));
  return_if_malloc_failed(qtypes);
  bool  some_load = false;
  for ( uint32_t i = 0; i < nC; i++ ) {
    if ( !is_load[i] ) {
      qtypes[i] = undef;
      continue;
    }
    some_load = true;
    if ( strcasecmp(fldtypes[i], "I1") == 0 ) {
      qtypes[i] = I1; is_trim[i] = true;
    }
    else if ( strcasecmp(fldtypes[i], "I2") == 0 ) {
      qtypes[i] = I2; is_trim[i] = true;
    }
    else if ( strcasecmp(fldtypes[i], "I4") == 0 ) {
      qtypes[i] = I4; is_trim[i] = true;
    }
    else if ( strcasecmp(fldtypes[i], "I8") == 0 ) {
      qtypes[i] = I8; is_trim[i] = true;
    }
    else if ( strcasecmp(fldtypes[i], "F4") == 0 ) {
      qtypes[i] = F4; is_trim[i] = true;
    }
    else if ( strcasecmp(fldtypes[i], "F8") == 0 ) {
      qtypes[i] = F8; is_trim[i] = true;
    }
    else if ( strcasecmp(fldtypes[i], "B1") == 0 ) {
      qtypes[i] = B1; is_trim[i] = true;
    }
    else { 
      fprintf(stderr, "Unknown fldtype [%s] \n", fldtypes[i]);
      go_BYE(-1); 
    }
  }
  if ( !some_load ) { go_BYE(-1); }
  // malloc output file pointers and nil output file pointers
  ofps = malloc(nC * sizeof(FILE *));
  return_if_malloc_failed(ofps);
  nn_ofps = malloc(nC * sizeof(FILE *));
  return_if_malloc_failed(nn_ofps);
  for ( uint32_t i = 0; i < nC; i++ ) {
    ofps[i] = NULL;
    nn_ofps[i] = NULL;
  }
  // fopen output file pointers and nil output file pointers
  for ( uint32_t i = 0; i < nC; i++ ) {
    if ( !is_load[i] ) {
      continue;
    }
    if ( ( out_files[i] == NULL ) || ( out_files[i][0] == '\0' ) ) { 
      go_BYE(-1);
    }
    ofps[i] = fopen(out_files[i], "wb");
    return_if_fopen_failed(ofps[i], out_files[i], "wb");

    if ( has_nulls[i] ) { 
      if ( ( nil_files[i] == NULL ) || ( nil_files[i][0] == '\0' ) ) { 
        go_BYE(-1);
      }
      nn_ofps[i] = fopen(nil_files[i], "wb");
      return_if_fopen_failed(nn_ofps[i], nil_files[i], "wb");
    } 
  }
  //---------------------------------

  //mmap the file
  status = rs_mmap(infile, &mmap_file, &file_size, false); //false b/c not writing to file
  cBYE(status);
  if ( ( mmap_file == NULL ) || ( file_size == 0 ) )  { go_BYE(-1); }

  int8_t tempI1; int16_t tempI2; int32_t tempI4; int64_t tempI8; float tempF4; double tempF8;

  size_t xidx = 0;
  uint64_t row_ctr = 0;
  uint32_t col_ctr = 0;
  bool is_last_col;
  char null_val[8];
  memset(null_val, '\0', 8); // we write 0 when value is null
#define BUFSZ 2047 
  // TODO BUFSZ should come from max of qconsts.qtypes[*].max_txt_width
  char lbuf[BUFSZ+1];
  char buf[BUFSZ+1];
  bool is_val_null;
  //read from the input file and write to the output file

  while ( true ) {
    memset(buf, '\0', BUFSZ+1);

    // Decide whether this is the last column on the row. Needed by get_cell
    if ( col_ctr == nC-1 ) { 
      is_last_col = true;
    }
    else {
      is_last_col = false;
    }

    xidx = get_cell(mmap_file, file_size, xidx, is_last_col, lbuf, BUFSZ);
    if ( xidx == 0 ) { 
      go_BYE(-1); } //means the file is empty or some error
    if ( xidx > file_size ) { break; } // check == or >= 
    if ( is_trim[col_ctr] ) { 
      status = trim(lbuf, buf, BUFSZ); cBYE(status);
    }
/*
    fprintf(stderr, "%llu, %u, %llu, %s \n", 
       (unsigned long long)row_ctr, col_ctr, 
       (unsigned long long)xidx, buf);
*/
    // Deal with header line 
    //row_ctr == 0 means we are reading the first line which is the header
    if ( is_hdr ) { 
      if ( row_ctr != 0 ) { go_BYE(-1); }
      col_ctr++;
      if ( is_last_col ) {
        col_ctr = 0;
        is_hdr = false;
      }
      if ( xidx == file_size ) { break; } // check == or >= 
      continue; 
    }
    // If this column is not to be loaded then continue 
    if ( !is_load[col_ctr] ) {
      col_ctr++;
      if ( col_ctr == nC ) { 
        col_ctr = 0;
        row_ctr++;
      }
      if ( xidx == file_size ) { break; } // check == or >= 
      continue;
    }
    
    if ( buf[0] == '\0' ) { // got back null value
      is_val_null = true;
      if ( !has_nulls[col_ctr] ) { 
        fprintf(stderr, " got null value when user said no null values row_ctr = %" PRIu64 ", col_ctr = %d \n", row_ctr, col_ctr);
        go_BYE(-1);
      }
    }
    else {
      is_val_null = false;
    }

    //element is not nil, write to not nil buffer
    if ( !is_val_null ) { 
      int8_t bit_idx = row_ctr % 64;
      nn_buf[col_ctr] |= ((uint64_t)1 << bit_idx);
    }
    else {
      // bit already 0 during initialization so no need to set it to 0
      num_nulls[col_ctr] += 1;
    }

    //nil buffer is full
    if ( ( row_ctr % 64 ) == 63 && has_nulls[col_ctr]) { // ( row_ctr & 0xFF ) == 0xFF 
      fwrite(&(nn_buf[col_ctr]), 1, sizeof(uint64_t), nn_ofps[col_ctr]);
      nn_buf[col_ctr] = 0; //reset
    }

    //write element to file
    switch ( qtypes[col_ctr] ) {
      case B1:
        {
          int8_t bit_idx = row_ctr % 64;
          uint8_t bit_val = 0;

          //if ( bit_idx == 64 ) { word_B1[col_ctr] = 0; }
          if ( is_val_null ) { 
            bit_val = 0;
          }
          else {
            status = txt_to_I1(buf, &tempI1); 
            if ( ( tempI1 < 0 ) || ( tempI1 > 1 ) )  { go_BYE(-1); }
            bit_val = (uint8_t)tempI1;
          }
          word_B1[col_ctr] |= ((uint64_t)bit_val << bit_idx);
          
          if ( bit_idx == 63 ) {
            fwrite(word_B1+col_ctr, 1, sizeof(uint64_t), ofps[col_ctr]);
            word_B1[col_ctr] = 0;
          }
        }
        break;
      case I1:
        if ( is_val_null ) { 
          fwrite(&null_val, 1, sizeof(int8_t), ofps[col_ctr]);
        }
        else {
          status = txt_to_I1(buf, &tempI1); 
          fwrite(&tempI1, 1, sizeof(int8_t), ofps[col_ctr]);
        }
        break;
      case I2:
        if ( is_val_null ) { 
          fwrite(&null_val, 1, sizeof(int16_t), ofps[col_ctr]);
        }
        else {
          status = txt_to_I2(buf, &tempI2); 
          fwrite(&tempI2, 1, sizeof(int16_t), ofps[col_ctr]);
        }
        break;
      case I4:
        if ( is_val_null ) { 
          fwrite(&null_val, 1, sizeof(int32_t), ofps[col_ctr]);
        }
        else {
          status = txt_to_I4(buf, &tempI4); 
          fwrite(&tempI4, 1, sizeof(int32_t), ofps[col_ctr]);
        }
        break;
      case I8:
        if ( is_val_null ) { 
          fwrite(&null_val, 1, sizeof(int64_t), ofps[col_ctr]);
        }
        else {
          status = txt_to_I8(buf, &tempI8); 
          fwrite(&tempI8, 1, sizeof(int64_t), ofps[col_ctr]);
        }
        break;
      case F4:
        if ( is_val_null ) { 
          fwrite(&null_val, 1, sizeof(float), ofps[col_ctr]);
        }
        else {
          status = txt_to_F4(buf, &tempF4); 
          fwrite(&tempF4, 1, sizeof(float), ofps[col_ctr]);
        }
        break;
      case F8:
        if ( is_val_null ) { 
          fwrite(&null_val, 1, sizeof(double), ofps[col_ctr]);
        }
        else {
          status = txt_to_F8(buf, &tempF8); 
          fwrite(&tempF8, 1, sizeof(double), ofps[col_ctr]);
        }
        break;
      default:
        //should not come here
        go_BYE(-1);
        break;
    }
    if ( status < 0 ) { 
      fprintf(stderr, "Error for row %lu, col %d, cell [%s]\n",
          row_ctr, col_ctr, buf);
    }
    cBYE(status);
    col_ctr++;
    if ( col_ctr == nC ) { 
      col_ctr = 0;
      row_ctr++;
    }
    /*this check needs to be done after the file has been written to because it
     * is possible that on the last get_cell, xidx is incremented to file_size
     * or greater, but the value from that last get_cell still needs to be
     * written to file*/
    if ( xidx == file_size ) { break; } // check == or >= 
  }

  //header row
  *ptr_nR = row_ctr;

  // Handle any buffers that need to be flushed
  for ( uint32_t i = 0; i < nC; i++ ) {
    //write any remaining nil element info to file
    if ( ( nn_ofps[i] != NULL ) && ( row_ctr % 64 != 0 ) ) {
      fwrite(nn_buf + i, 1, sizeof(uint64_t), nn_ofps[i]);
    }
    //write any remaining B1 element info to file
    if ( ( ofps[i] != NULL ) && ( row_ctr % 64 != 0 ) && 
        ( qtypes[i] == B1 ) ) {
      fwrite(word_B1+i, 1, sizeof(uint64_t), ofps[i]);
    }
  }
  if ( ( str_for_lua != NULL ) && ( sz_str_for_lua > 0 ) ) {
    strcpy(str_for_lua, "local lVector = require 'Q/RUNTIME/lua/lVector'\nlocal T = {};\n");
    char xbuf[2*ddir_len + 128];
    int xcol_ctr = 1; // Lua indexes from 1 
    for ( uint32_t i = 0; i < nC; i++ ) {
      if ( !is_load[i] ) { continue; }
      if ( num_nulls[i] == 0 ) {  
        sprintf(xbuf, "T[%d] = lVector({ qtype = \"%s\", file_name = \"%s\", num_elements = %" PRIu64 "});\n", xcol_ctr, fldtypes[i], out_files[i],row_ctr);
      }
      else {
        sprintf(xbuf, "T[%d] = lVector({ qtype = \"%s\", file_name = \"%s\", nn_file_name = \"%s\", num_elements = %" PRIu64 "});\n", xcol_ctr, fldtypes[i], out_files[i], nil_files[i],row_ctr);
      }
      // TODO: Check for buffer overflow 
      strcat(str_for_lua, xbuf);
      xcol_ctr++;
    }
    sprintf(str_for_lua + strlen(str_for_lua),"return T\n");
  }
  else {
    *ptr_out_files = out_files;
    *ptr_nil_files = nil_files;
  }

BYE:
  bak_status = status;
  // Close open files 
  for ( uint32_t i = 0; i < nC; i++ ) {
    if ( ofps != NULL ) { 
      fclose_if_non_null(ofps[i]);
    }
    if ( nn_ofps != NULL ) { 
      fclose_if_non_null(nn_ofps[i]);
    }
  }

  // delete nil_files with no nil elements
  for ( uint32_t i = 0; i < nC; i++ ) {
    if ( !is_load[i] ) { continue; }
    if ( ( has_nulls[i] ) && ( num_nulls[i] == 0 ) ) { 
      if ( nil_files[i][0] == '\0' ) { WHEREAMI; return -1; }
      if ( !file_exists(nil_files[i]) ) { 
        WHEREAMI; return -1; }
      status = remove(nil_files[i]); 
      if ( status < 0 ) { WHEREAMI; return -1; }
      free_if_non_null(nil_files[i]); 
      // printf("%s: removing file for Column %d\n", infile, i);
    }
  }
  if ( ( str_for_lua != NULL ) && ( sz_str_for_lua > 0 ) ) {
    // delete nil files and out_files
    if ( nil_files != NULL ) { 
      for ( uint32_t i = 0; i < nC; i++ ) {
        free_if_non_null(nil_files[i]);
      }
      free_if_non_null(nil_files);
    }
    if ( out_files != NULL ) { 
      for ( uint32_t i = 0; i < nC; i++ ) {
        free_if_non_null(out_files[i]);
      }
      free_if_non_null(out_files);
    }
  }

  free_if_non_null(ofps);
  free_if_non_null(qtypes);
  free_if_non_null(nn_ofps);
  mcr_rs_munmap(mmap_file, file_size);
  free_if_non_null(opdir);
  free_if_non_null(nn_buf);
  free_if_non_null(word_B1);
  free_if_non_null(is_trim);
  if ( ( str_for_lua != NULL ) && ( sz_str_for_lua > 0 ) && 
       ( ptr_n_str_for_lua != NULL ) ) {
    *ptr_n_str_for_lua = strlen(str_for_lua);
  }
  // fprintf(stderr, "bak_status = %d \n", bak_status); WHEREAMI;
  return bak_status;
}
