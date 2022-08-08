#include <systemc>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <stdint.h>
#include "SysCFileHandler.h"
#include "ap_int.h"
#include "ap_fixed.h"
#include <complex>
#include <stdbool.h>
#include "autopilot_cbe.h"
#include "hls_stream.h"
#include "hls_half.h"
#include "hls_signal_handler.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

// wrapc file define:
#define AUTOTB_TVIN_gmem "../tv/cdatafile/c.fast_accel.autotvin_gmem.dat"
#define AUTOTB_TVOUT_gmem "../tv/cdatafile/c.fast_accel.autotvout_gmem.dat"
// wrapc file define:
#define AUTOTB_TVIN_img_in "../tv/cdatafile/c.fast_accel.autotvin_img_in.dat"
#define AUTOTB_TVOUT_img_in "../tv/cdatafile/c.fast_accel.autotvout_img_in.dat"
// wrapc file define:
#define AUTOTB_TVIN_threshold "../tv/cdatafile/c.fast_accel.autotvin_threshold.dat"
#define AUTOTB_TVOUT_threshold "../tv/cdatafile/c.fast_accel.autotvout_threshold.dat"
// wrapc file define:
#define AUTOTB_TVIN_img_out "../tv/cdatafile/c.fast_accel.autotvin_img_out.dat"
#define AUTOTB_TVOUT_img_out "../tv/cdatafile/c.fast_accel.autotvout_img_out.dat"
// wrapc file define:
#define AUTOTB_TVIN_rows "../tv/cdatafile/c.fast_accel.autotvin_rows.dat"
#define AUTOTB_TVOUT_rows "../tv/cdatafile/c.fast_accel.autotvout_rows.dat"
// wrapc file define:
#define AUTOTB_TVIN_cols "../tv/cdatafile/c.fast_accel.autotvin_cols.dat"
#define AUTOTB_TVOUT_cols "../tv/cdatafile/c.fast_accel.autotvout_cols.dat"

#define INTER_TCL "../tv/cdatafile/ref.tcl"

// tvout file define:
#define AUTOTB_TVOUT_PC_gmem "../tv/rtldatafile/rtl.fast_accel.autotvout_gmem.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_img_in "../tv/rtldatafile/rtl.fast_accel.autotvout_img_in.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_threshold "../tv/rtldatafile/rtl.fast_accel.autotvout_threshold.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_img_out "../tv/rtldatafile/rtl.fast_accel.autotvout_img_out.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_rows "../tv/rtldatafile/rtl.fast_accel.autotvout_rows.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_cols "../tv/rtldatafile/rtl.fast_accel.autotvout_cols.dat"

class INTER_TCL_FILE {
  public:
INTER_TCL_FILE(const char* name) {
  mName = name; 
  gmem_depth = 0;
  img_in_depth = 0;
  threshold_depth = 0;
  img_out_depth = 0;
  rows_depth = 0;
  cols_depth = 0;
  trans_num =0;
}
~INTER_TCL_FILE() {
  mFile.open(mName);
  if (!mFile.good()) {
    cout << "Failed to open file ref.tcl" << endl;
    exit (1); 
  }
  string total_list = get_depth_list();
  mFile << "set depth_list {\n";
  mFile << total_list;
  mFile << "}\n";
  mFile << "set trans_num "<<trans_num<<endl;
  mFile.close();
}
string get_depth_list () {
  stringstream total_list;
  total_list << "{gmem " << gmem_depth << "}\n";
  total_list << "{img_in " << img_in_depth << "}\n";
  total_list << "{threshold " << threshold_depth << "}\n";
  total_list << "{img_out " << img_out_depth << "}\n";
  total_list << "{rows " << rows_depth << "}\n";
  total_list << "{cols " << cols_depth << "}\n";
  return total_list.str();
}
void set_num (int num , int* class_num) {
  (*class_num) = (*class_num) > num ? (*class_num) : num;
}
void set_string(std::string list, std::string* class_list) {
  (*class_list) = list;
}
  public:
    int gmem_depth;
    int img_in_depth;
    int threshold_depth;
    int img_out_depth;
    int rows_depth;
    int cols_depth;
    int trans_num;
  private:
    ofstream mFile;
    const char* mName;
};

static void RTLOutputCheckAndReplacement(std::string &AESL_token, std::string PortName) {
  bool no_x = false;
  bool err = false;

  no_x = false;
  // search and replace 'X' with '0' from the 3rd char of token
  while (!no_x) {
    size_t x_found = AESL_token.find('X', 0);
    if (x_found != string::npos) {
      if (!err) { 
        cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port" 
             << PortName << ", possible cause: There are uninitialized variables in the C design."
             << endl; 
        err = true;
      }
      AESL_token.replace(x_found, 1, "0");
    } else
      no_x = true;
  }
  no_x = false;
  // search and replace 'x' with '0' from the 3rd char of token
  while (!no_x) {
    size_t x_found = AESL_token.find('x', 2);
    if (x_found != string::npos) {
      if (!err) { 
        cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'x' on port" 
             << PortName << ", possible cause: There are uninitialized variables in the C design."
             << endl; 
        err = true;
      }
      AESL_token.replace(x_found, 1, "0");
    } else
      no_x = true;
  }
}
extern "C" void fast_accel_hw_stub_wrapper(volatile void *, int, volatile void *, int, int);

extern "C" void apatb_fast_accel_hw(volatile void * __xlx_apatb_param_img_in, int __xlx_apatb_param_threshold, volatile void * __xlx_apatb_param_img_out, int __xlx_apatb_param_rows, int __xlx_apatb_param_cols) {
  refine_signal_handler();
  fstream wrapc_switch_file_token;
  wrapc_switch_file_token.open(".hls_cosim_wrapc_switch.log");
  int AESL_i;
  if (wrapc_switch_file_token.good())
  {

    CodeState = ENTER_WRAPC_PC;
    static unsigned AESL_transaction_pc = 0;
    string AESL_token;
    string AESL_num;{
      static ifstream rtl_tv_out_file;
      if (!rtl_tv_out_file.is_open()) {
        rtl_tv_out_file.open(AUTOTB_TVOUT_PC_gmem);
        if (rtl_tv_out_file.good()) {
          rtl_tv_out_file >> AESL_token;
          if (AESL_token != "[[[runtime]]]")
            exit(1);
        }
      }
  
      if (rtl_tv_out_file.good()) {
        rtl_tv_out_file >> AESL_token; 
        rtl_tv_out_file >> AESL_num;  // transaction number
        if (AESL_token != "[[transaction]]") {
          cerr << "Unexpected token: " << AESL_token << endl;
          exit(1);
        }
        if (atoi(AESL_num.c_str()) == AESL_transaction_pc) {
          std::vector<sc_bv<8> > gmem_pc_buffer(32768);
          int i = 0;

          rtl_tv_out_file >> AESL_token; //data
          while (AESL_token != "[[/transaction]]"){

            RTLOutputCheckAndReplacement(AESL_token, "gmem");
  
            // push token into output port buffer
            if (AESL_token != "") {
              gmem_pc_buffer[i] = AESL_token.c_str();;
              i++;
            }
  
            rtl_tv_out_file >> AESL_token; //data or [[/transaction]]
            if (AESL_token == "[[[/runtime]]]" || rtl_tv_out_file.eof())
              exit(1);
          }
          if (i > 0) {{
            int i = 0;
            for (int j = 0, e = 16384; j < e; j += 1, ++i) {
            ((char*)__xlx_apatb_param_img_in)[j] = gmem_pc_buffer[i].to_int64();
          }
            for (int j = 0, e = 16384; j < e; j += 1, ++i) {
            ((char*)__xlx_apatb_param_img_out)[j] = gmem_pc_buffer[i].to_int64();
          }}}
        } // end transaction
      } // end file is good
    } // end post check logic bolck
  
    AESL_transaction_pc++;
    return ;
  }
static unsigned AESL_transaction;
static AESL_FILE_HANDLER aesl_fh;
static INTER_TCL_FILE tcl_file(INTER_TCL);
std::vector<char> __xlx_sprintf_buffer(1024);
CodeState = ENTER_WRAPC;
//gmem
aesl_fh.touch(AUTOTB_TVIN_gmem);
aesl_fh.touch(AUTOTB_TVOUT_gmem);
//img_in
aesl_fh.touch(AUTOTB_TVIN_img_in);
aesl_fh.touch(AUTOTB_TVOUT_img_in);
//threshold
aesl_fh.touch(AUTOTB_TVIN_threshold);
aesl_fh.touch(AUTOTB_TVOUT_threshold);
//img_out
aesl_fh.touch(AUTOTB_TVIN_img_out);
aesl_fh.touch(AUTOTB_TVOUT_img_out);
//rows
aesl_fh.touch(AUTOTB_TVIN_rows);
aesl_fh.touch(AUTOTB_TVOUT_rows);
//cols
aesl_fh.touch(AUTOTB_TVIN_cols);
aesl_fh.touch(AUTOTB_TVOUT_cols);
CodeState = DUMP_INPUTS;
unsigned __xlx_offset_byte_param_img_in = 0;
unsigned __xlx_offset_byte_param_img_out = 0;
// print gmem Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_gmem, __xlx_sprintf_buffer.data());
  {  __xlx_offset_byte_param_img_in = 0*1;
  if (__xlx_apatb_param_img_in) {
    for (int j = 0  - 0, e = 16384 - 0; j != e; ++j) {
sc_bv<8> __xlx_tmp_lv = ((char*)__xlx_apatb_param_img_in)[j];

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_gmem, __xlx_sprintf_buffer.data()); 
      }
  }
  __xlx_offset_byte_param_img_out = 16384*1;
  if (__xlx_apatb_param_img_out) {
    for (int j = 0  - 0, e = 16384 - 0; j != e; ++j) {
sc_bv<8> __xlx_tmp_lv = ((char*)__xlx_apatb_param_img_out)[j];

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_gmem, __xlx_sprintf_buffer.data()); 
      }
  }
}
  tcl_file.set_num(32768, &tcl_file.gmem_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_gmem, __xlx_sprintf_buffer.data());
}
// print img_in Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_img_in, __xlx_sprintf_buffer.data());
  {
    sc_bv<64> __xlx_tmp_lv = __xlx_offset_byte_param_img_in;

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_img_in, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.img_in_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_img_in, __xlx_sprintf_buffer.data());
}
// print threshold Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_threshold, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_threshold);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_threshold, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.threshold_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_threshold, __xlx_sprintf_buffer.data());
}
// print img_out Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_img_out, __xlx_sprintf_buffer.data());
  {
    sc_bv<64> __xlx_tmp_lv = __xlx_offset_byte_param_img_out;

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_img_out, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.img_out_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_img_out, __xlx_sprintf_buffer.data());
}
// print rows Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_rows, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_rows);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_rows, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.rows_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_rows, __xlx_sprintf_buffer.data());
}
// print cols Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_cols, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_cols);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_cols, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.cols_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_cols, __xlx_sprintf_buffer.data());
}
CodeState = CALL_C_DUT;
fast_accel_hw_stub_wrapper(__xlx_apatb_param_img_in, __xlx_apatb_param_threshold, __xlx_apatb_param_img_out, __xlx_apatb_param_rows, __xlx_apatb_param_cols);
CodeState = DUMP_OUTPUTS;
// print gmem Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVOUT_gmem, __xlx_sprintf_buffer.data());
  {  __xlx_offset_byte_param_img_in = 0*1;
  if (__xlx_apatb_param_img_in) {
    for (int j = 0  - 0, e = 16384 - 0; j != e; ++j) {
sc_bv<8> __xlx_tmp_lv = ((char*)__xlx_apatb_param_img_in)[j];

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVOUT_gmem, __xlx_sprintf_buffer.data()); 
      }
  }
  __xlx_offset_byte_param_img_out = 16384*1;
  if (__xlx_apatb_param_img_out) {
    for (int j = 0  - 0, e = 16384 - 0; j != e; ++j) {
sc_bv<8> __xlx_tmp_lv = ((char*)__xlx_apatb_param_img_out)[j];

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVOUT_gmem, __xlx_sprintf_buffer.data()); 
      }
  }
}
  tcl_file.set_num(32768, &tcl_file.gmem_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVOUT_gmem, __xlx_sprintf_buffer.data());
}
CodeState = DELETE_CHAR_BUFFERS;
AESL_transaction++;
tcl_file.set_num(AESL_transaction , &tcl_file.trans_num);
}