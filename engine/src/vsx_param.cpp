/**
* Project: VSXu: Realtime visual programming language, music/audio visualizer, animation tool and much much more.
*
* @author Jonatan Wallmander, Robert Wenzel, Vovoid Media Technologies Copyright (C) 2003-2011
* @see The GNU Public License (GPL)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "vsx_param.h"

#ifdef VSXU_EXE
  #define VSX_P_INT
  #define VSX_P_RENDER
  #define VSX_P_FLOAT3
  #define VSX_P_DOUBLE
  #define VSX_P_STRING
  #define VSX_P_TEXTURE
  #define VSX_P_FLOAT
  #define VSX_P_FLOAT4
  #define VSX_P_MATRIX
  #define VSX_P_MESH
  #define VSX_P_BITMAP
  #define VSX_P_PARTICLESYSTEM
  #define VSX_P_FLOAT_ARRAY
  #define VSX_P_FLOAT3_ARRAY
  #define VSX_P_SEQUENCE
  #define VSX_P_SEGMENT_MESH
  #define VSX_P_ABSTRACT
  #define VSX_P_QUATERNION
  #define VSX_P_QUATERNION_ARRAY
#endif

#ifdef VSX_P_STRING
  #define VSX_P_RESOURCE
#endif

vsx_module_param_abs* vsx_module_param_list::create(int type, const char* name, bool crit, bool all_required) {
  vsx_module_param_abs* ptemp = 0;
  switch (type) {
#ifdef VSX_P_INT
    case VSX_MODULE_PARAM_ID_INT: ptemp = new vsx_module_param_int(name); break;
#endif
#ifdef VSX_P_RENDER
    case VSX_MODULE_PARAM_ID_RENDER: ptemp = new vsx_module_param_render(name); break;
#endif
#ifdef VSX_P_FLOAT3
    case VSX_MODULE_PARAM_ID_FLOAT3: ptemp = new vsx_module_param_float3(name); break;
#endif
#ifdef VSX_P_DOUBLE
    case VSX_MODULE_PARAM_ID_DOUBLE: ptemp = new vsx_module_param_double(name); break;
#endif
#ifdef VSX_P_STRING
    case VSX_MODULE_PARAM_ID_STRING: ptemp = new vsx_module_param_string(name); break;
#endif
#ifdef VSX_P_TEXTURE
    case VSX_MODULE_PARAM_ID_TEXTURE: ptemp = new vsx_module_param_texture(name); break;
#endif
#ifdef VSX_P_FLOAT
    case VSX_MODULE_PARAM_ID_FLOAT: ptemp = new vsx_module_param_float(name); break;
#endif
#ifdef VSX_P_FLOAT4
    case VSX_MODULE_PARAM_ID_FLOAT4: ptemp = new vsx_module_param_float4(name); break;
#endif
#ifdef VSX_P_MATRIX
    case VSX_MODULE_PARAM_ID_MATRIX: ptemp = new vsx_module_param_matrix(name); break;
#endif
#ifdef VSX_P_MESH
    case VSX_MODULE_PARAM_ID_MESH: ptemp = new vsx_module_param_mesh(name); break;
#endif
#ifdef VSX_P_BITMAP
    case VSX_MODULE_PARAM_ID_BITMAP: ptemp = new vsx_module_param_bitmap(name); break;
#endif
#ifdef VSX_P_PARTICLESYSTEM
    case VSX_MODULE_PARAM_ID_PARTICLESYSTEM: ptemp = new vsx_module_param_particlesystem(name); break;
#endif
#ifdef VSX_P_FLOAT_ARRAY
    case VSX_MODULE_PARAM_ID_FLOAT_ARRAY: ptemp = new vsx_module_param_float_array(name); break;
#endif
#ifdef VSX_P_FLOAT3_ARRAY
    case VSX_MODULE_PARAM_ID_FLOAT3_ARRAY: ptemp = new vsx_module_param_float3_array(name); break;
#endif
#ifdef VSX_P_QUATERNION_ARRAY
    case VSX_MODULE_PARAM_ID_QUATERNION_ARRAY: ptemp = new vsx_module_param_quaternion_array(name); break;
#endif
#ifdef VSX_P_SEQUENCE
    case VSX_MODULE_PARAM_ID_SEQUENCE: ptemp = new vsx_module_param_sequence(name); break;
#endif
#ifdef VSX_P_SEGMENT_MESH
    case VSX_MODULE_PARAM_ID_SEGMENT_MESH: ptemp = new vsx_module_param_segment_mesh(name); break;
#endif
#ifdef VSX_P_ABSTRACT
    case VSX_MODULE_PARAM_ID_ABSTRACT: ptemp = new vsx_module_param_abstract(name); break;
#endif
#ifdef VSX_P_QUATERNION
    case VSX_MODULE_PARAM_ID_QUATERNION: ptemp = new vsx_module_param_quaternion(name); break;
#endif
#ifdef VSX_P_RESOURCE
    case VSX_MODULE_PARAM_ID_RESOURCE: ptemp = new vsx_module_param_resource(name); break;
#endif
  }; // case

  ptemp->type = type;
  ptemp->critical = crit;
  ptemp->all_required = all_required;
  id_vec.push_back(ptemp);
  return ptemp;

}

vsx_module_param_list::~vsx_module_param_list() {
#ifdef VSXU_EXE
  #ifdef VSXU_DEVELOPER
  //printf("Removing internal parameters created by the module\nNumber of params: %d\n",id_vec.size());
  #endif
  for (unsigned long i = 0; i < id_vec.size(); ++i) {
    #ifdef VSXU_DEVELOPER
    //printf("removing module param %d :: %s\n",i,id_vec[i]->name.c_str());
    #endif
    switch (id_vec[i]->type) {
      case VSX_MODULE_PARAM_ID_INT: delete ((vsx_module_param_int*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_RENDER: delete ((vsx_module_param_render*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_FLOAT3: delete ((vsx_module_param_float3*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_DOUBLE: delete ((vsx_module_param_double*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_STRING: delete ((vsx_module_param_string*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_TEXTURE: delete ((vsx_module_param_texture*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_FLOAT: delete ((vsx_module_param_float*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_FLOAT4: delete ((vsx_module_param_float4*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_MATRIX: delete ((vsx_module_param_matrix*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_MESH: delete ((vsx_module_param_mesh*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_BITMAP: delete ((vsx_module_param_bitmap*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_PARTICLESYSTEM: delete ((vsx_module_param_particlesystem*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_FLOAT_ARRAY: delete ((vsx_module_param_float_array*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_FLOAT3_ARRAY: delete ((vsx_module_param_float3_array*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_QUATERNION_ARRAY: delete ((vsx_module_param_quaternion_array*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_SEQUENCE: delete ((vsx_module_param_sequence*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_SEGMENT_MESH: delete ((vsx_module_param_segment_mesh*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_ABSTRACT: delete ((vsx_module_param_abstract*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_QUATERNION: delete ((vsx_module_param_quaternion*)id_vec[i]); break;
      case VSX_MODULE_PARAM_ID_RESOURCE: delete ((vsx_module_param_resource*)id_vec[i]); break;
    }; // case
  }  
  #ifdef VSXU_DEBUG
  printf("end of module parameter deletion\n");
  #endif
#endif
}


