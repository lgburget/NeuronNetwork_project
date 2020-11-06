#include <tclap/CmdLine.h>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>


/*!
  A base class for errors thrown in this program.
  Each error type has a specific exit code.
  Error messages will be passed by the exception caller.
*/
class SimulError : public std::runtime_error {
public:
    SimulError(const char *c, int v=0) : std::runtime_error(c), code(v) {}
    SimulError(const std::string &s, int v=0) : std::runtime_error(s), code(v) {}
    int value() const {return code;}
protected:
    const int code;
};

#define _SIMULERR_(_N, _id) class _N : public SimulError { \
    public: _N(const char *c) : SimulError(c,_id) {} \
            _N(const std::string &s) : SimulError(s,_id) {} };


/// *Specific error codes*
_SIMULERR_(TCLAP_ERROR, 10)
_SIMULERR_(CFILE_ERROR, 20)
_SIMULERR_(OUTPUT_ERROR, 30)

#undef _SIMULERR_

/// * default parameter values *
#define _Numbers_ 7.
#define _Proportion_Excitatory_ 0.4
#define _Simulation_Time_ 30.
#define _Connectivity_ 30.
#define _Intensity_ 25.
#define _Delta_T_ 0.5
#define _Discharge_Threshold_ 30
