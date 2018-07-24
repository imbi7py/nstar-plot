/*
  -------------------------------------------------------------------
  
  Copyright (C) 2018, Andrew W. Steiner
  
  This is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.
  
  O2scl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this file. If not, see <http://www.gnu.org/licenses/>.

  -------------------------------------------------------------------
*/
#include <iostream>
#include <string>

#include <o2scl/hdf_io.h>
#include <o2scl/hdf_eos_io.h>
#include <o2scl/nstar_cold.h>
#include <o2scl/eos_had_skyrme.h>

using namespace std;
using namespace o2scl;
using namespace o2scl_const;
using namespace o2scl_hdf;

int main(void) {

  cout.setf(ios::scientific);

  // Skyrme EOS for core
  eos_had_skyrme sk;
  skyrme_load(sk,"NRAPR");

  // Neutron star structure
  nstar_cold nc;
  nc.set_eos(sk);
  nc.calc_eos();
  nc.calc_nstar();

  // Get full EOS including crust
  table_units<> teos;
  teos.line_of_names("ed pr");
  teos.line_of_units("MeV/fm^3 MeV/fm^3");
  for(double pr=1.0e-1;pr<1.0e3;pr*=1.2) {
    double ed, nb;
    nc.def_eos_tov.get_eden_user(pr/hc_mev_fm,ed,nb);
    double line[2]={pr,ed*hc_mev_fm};
    teos.line_of_data(2,line);
  }

  // Output the table(s) to file(s)
  hdf_file hf;
  hf.open_or_create("eos_mvsr.o2");
  hdf_output(hf,*nc.get_tov_results(),"mvsr");
  hdf_output(hf,teos,"eos");
  hf.close();
    
  return 0;
}
