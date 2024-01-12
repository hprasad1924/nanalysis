#include "input_data.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <dirent.h>
#include <math.h>
#include <TGeoManager.h>
#include <TList.h>
#include <TGeoMaterial.h>

#include "dirs.h"
#include "jednostki.h"


////////////////////////////////////////
// Public methods
////////////////////////////////////////

input_data::input_data()
{
}

////////////////////////////////////////

input_data::~input_data()
{
}

////////////////////////////////////////

void input_data::initialize( params _par )
{
  par = _par;
  initialize_input_path();
  initialize_nucl_list();
  initialize_data_containers();
}

////////////////////////////////////////

void input_data::load_data()
{
  for( int i = 0; i < containers.size(); i++ )
    containers[i].read_data_file();

  for( int i = 0; i < nucl_containers.size(); i++ )
  {
    for( int j = 0; j < nucl_containers[i].size(); j++ )
      nucl_containers[i][j].read_data_file();
  }
}

////////////////////////////////////////

data_container* input_data::get_data_container( int i )
{
  return &containers[i];
}

////////////////////////////////////////

data_container* input_data::get_nucl_data_container( int i, int protons, int neutrons )
{
  if ( par.target_type )
  {
    int nucl = protons * 1000 + neutrons;
    int j    = 0;
    while ( j < nucl_list.size() )
    {
      if ( nucl_list[j] == nucl )
        return &nucl_containers[i][j];
      j++;
    }
    if ( par.target_type == 2 )
    {
      throw "input_data error: An unexpected isotope in detector simulation.";
    }
    else
    {
      throw "input_data error: Cannot find a requested nucleus.";
    }
  }
  else
  {
    return &nucl_containers[i][0];
  }
}


////////////////////////////////////////
// Private methods
////////////////////////////////////////

void input_data::initialize_input_path()
{
  // generate the input_path
  stringstream name_sstream;
  name_sstream << get_data_dir() << "input/";   // data_dir + relative folder
  input_path = name_sstream.str();

  // check if the directory exists
  DIR* dir = opendir( input_path.c_str() );
  if ( dir )                                    // the directory exists
  {
    closedir( dir );
  }
  else
  {
    throw "input_data error: Could not find the input folder.";
  }
}

////////////////////////////////////////

void input_data::initialize_nucl_list()
{
  if ( par.target_type == 0 )      // single nucleus target
  {
    nucl_list.push_back( par.nucleus_p * 1000 + par.nucleus_n );
  }
  else if ( par.target_type == 1 ) // multiple nuclei target
  {
    stringstream par_stream( par.target_content );
    string par_line;
    while( getline( par_stream, par_line ) )
    {
      stringstream par_values( par_line );
      int protons, neutrons;
      par_values >> protons >> neutrons;
      nucl_list.push_back( protons * 1000 + neutrons );
    }
  }
  else if ( par.target_type == 2 ) // complex detector target
  {
    TList* materials = gGeoManager->GetListOfMaterials();
    TGeoMaterial* mat;
    TGeoMixture* mix;
    for ( int i = 0; i < materials->GetSize(); i++ )
    {
      mat = (TGeoMaterial*) materials->At(i);
      if ( mat->IsMixture() )
      {
        mix = (TGeoMixture*) mat;
        for ( int j = 0; j < mix->GetNelements(); j++ )
        {
          nucl_list.push_back( mix->GetZmixt()[j] * 1000
                      + (int)( mix->GetAmixt()[j] - mix->GetZmixt()[j] + 0.5) );
        }
      }
      else
      {
        nucl_list.push_back( mat->GetZ() * 1000 + (int)( mat->GetA() - mat->GetZ() + 0.5) );
      }
    }
  }
  else
  {
    throw "input_data error: Cannot create the list of nuclei.";
  }

  // check for duoble-couting in nucl_list
  sort( nucl_list.begin(), nucl_list.end() );
  nucl_list.erase( unique( nucl_list.begin(), nucl_list.end() ), nucl_list.end() );
}

////////////////////////////////////////

void input_data::initialize_data_containers()
{
  // Provide the parameter that governs the data,
  // then the number of different fields in the file, their names and the method of interpolation:
  //  0 is taking floor,
  //  1 is linear interpolation,
  // -1 means it is the input axis.

  // Totally independent

  // # 0
  // kaskada_NN_xsec
    int    cascade_NN_xsec_number_of_fields     = 3;
    string cascade_NN_xsec_data_fields[]        = {"energy", "xsec_ii", "xsec_ij"};
    int    cascade_NN_xsec_interpolate_fields[] = {-1, 1, 1};
    double cascade_NN_xsec_unit_fields[]        = { 1, millibarn, millibarn};
    containers.push_back( data_container( input_path, "kaskada_NN_xsec", par.kaskada_NN_xsec,
                                          cascade_NN_xsec_number_of_fields, cascade_NN_xsec_data_fields,
                                          cascade_NN_xsec_interpolate_fields, cascade_NN_xsec_unit_fields ));

  // # 1
  // kaskada_NN_inel
    int    cascade_NN_inel_number_of_fields     = 5;
    string cascade_NN_inel_data_fields[]        = {"energy", "inel_ii", "inel_ij", "inel_1pii", "inel_1pij"};
    int    cascade_NN_inel_interpolate_fields[] = {-1, 1, 1, 1, 1};
    if( par.kaskada_NN_inel <= 1 )  // for dataset 0 and 1 use no interpolation
    {cascade_NN_inel_interpolate_fields[1] = 0;cascade_NN_inel_interpolate_fields[2]=0;
     cascade_NN_inel_interpolate_fields[3] = 0;cascade_NN_inel_interpolate_fields[4]=0;}
    double cascade_NN_inel_unit_fields[]        = { 1, 1, 1, 1, 1};
    containers.push_back( data_container( input_path, "kaskada_NN_inel", par.kaskada_NN_inel,
                                          cascade_NN_inel_number_of_fields, cascade_NN_inel_data_fields,
                                          cascade_NN_inel_interpolate_fields, cascade_NN_inel_unit_fields ));

  // # 2
  // kaskada_NN_angle
    int    cascade_NN_angle_number_of_fields    = 7;
    string cascade_NN_angle_data_fields[]       = {"energy", "angle_A_ii", "angle_A_ij", "angle_B_ii",
                                                   "angle_B_ij", "angle_C_ii", "angle_C_ij"};
    int    cascade_NN_angle_interpolate_fields[]= {-1, 1, 1, 1, 1, 1, 1};
    double cascade_NN_angle_unit_fields[]       = { 1, 1, 1, 1, 1, 1, 1};
    containers.push_back( data_container( input_path, "kaskada_NN_angle", par.kaskada_NN_angle,
                                          cascade_NN_angle_number_of_fields, cascade_NN_angle_data_fields,
                                          cascade_NN_angle_interpolate_fields, cascade_NN_angle_unit_fields ));

  // Nucleus dependent

  // # 0
  // kaskada_NN_corr
    int    cascade_NN_corr_number_of_fields     = 5;
    string cascade_NN_corr_data_fields[]        = {"distance", "corr_ii", "corr_ij", "norm_ii", "norm_ij"};
    int    cascade_NN_corr_interpolate_fields[] = {-1, 1, 1, 1, 1};
    double cascade_NN_corr_unit_fields[]        = {fermi, 1, 1, 1, 1};
    vector<data_container> constructor;
    for( int i=0; i<nucl_list.size(); i++ )
      constructor.push_back( data_container( input_path, "kaskada_NN_corr", par.kaskada_NN_corr,
                                             cascade_NN_corr_number_of_fields, cascade_NN_corr_data_fields,
                                             cascade_NN_corr_interpolate_fields, cascade_NN_corr_unit_fields,
                                             nucl_list[i]/1000, nucl_list[i]%1000 ));
    nucl_containers.push_back( constructor );
}
