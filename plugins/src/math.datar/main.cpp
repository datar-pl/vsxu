#include <vsxfst.h> // file system and string functions
#include <vsx_param.h> // parameter definitions
#include <vsx_module.h> // module base class + definition
#include <vsx_math_3d.h> // vsx_vector and matrix maths

class vsx_float_holder : public vsx_module {
  // in
  vsx_module_param_float* in;
  vsx_module_param_float* if_change;
  // out
  vsx_module_param_float* out;
  // internal
  //float hold;

public:

  void module_info(vsx_module_info* info)
  {
    info->identifier = "maths;accumulators;float_holder";
    info->description = 
      "If if_to_change is outside of [-0.5;0.5] range\n"
      "float_out = float_in. \n"
      "Else float_out is last set float_in.\n"
    ;
    /*
     * Parameter specifications
     *   Used to tell artiste what parameters we want in the GUI and how 
     *   those should behave.
     * 
     * Syntax:
     * 
     * [name]:[type][?[flag][=value]]
     * 
     * - Name and type are required.
     * - Flag can be many things, controllers or options on how the GUI should
     *   treat it.
     * 
     * To prevent connections to a parameter, add the non-connection flag:
     * nc=1
     * 
     * This will make it impossible to connect the parameter to any other 
     * module. That is, only the human operating VSXu can set it. 
     * Use this when you do operations that involve large chunks of memory
     * or heavy one-time precalculations.
     * 
     * Setting "Default Controller" (which controller is displayed when double-
     *  clicking the param):
     *   default_controller=[value]
     * 
     * Different values (controllers) for different parameters are available:
     *  
     *   controller_knob        float
     *   controller_slider      float, float3, float4
     *   controller_edit        string/resource  
     *   controller_resource    resource (to use it with strings, make it 
     *                          default)
     * 
     * Example:
     *   To make a knob default (when double-clicking) go like this:
     *   angle:float?default_controller=controller_knob
     * 
     */
    info->in_param_spec = 
      "float_in:float,"
      "if_to_change:float"
    ;
  
    info->out_param_spec = "float_out:float";
    info->component_class = "parameters";
  }

  // this method uses the engine's parameter holder to create data holders for 
  // each of the parameters also binding them to their name (string)
  void declare_params(
    vsx_module_param_list& in_parameters, 
    vsx_module_param_list& out_parameters
  )
  {
    loading_done = true;
    in = (vsx_module_param_float*)in_parameters.create(
      VSX_MODULE_PARAM_ID_FLOAT, 
      "float_in"
    );
    in->set(0.0f);
    
    if_change = (vsx_module_param_float*)in_parameters.create(
      VSX_MODULE_PARAM_ID_FLOAT, 
      "if_to_change"
    );
    if_change->set(0.0f);
    
    out = (vsx_module_param_float*)out_parameters.create(
      VSX_MODULE_PARAM_ID_FLOAT,
      "float_out"
    );
    out->set(0.0f);
  }
  
  // this is run once per frame/iteration of the whole engine
  void run()
  {
	float change = if_change->get();
	change = fabs(change);
    if (change > 0.5f)
	{
		out->set(in->get());
	}
  }
};
  

  class vsx_float3_holder : public vsx_module {
  // in
  vsx_module_param_float3* in;
  vsx_module_param_float* if_change;
  // out
  vsx_module_param_float3* out;
  // internal
  //float hold;

public:

  void module_info(vsx_module_info* info)
  {
    info->identifier = "maths;accumulators;float3_holder";
    info->description = 
      "If if_to_change is outside of [-0.5;0.5] range\n"
      "float3_out = float3_in. \n"
      "Else float3_out is last set float3_in.\n"
    ;
    /*
     * Parameter specifications
     *   Used to tell artiste what parameters we want in the GUI and how 
     *   those should behave.
     * 
     * Syntax:
     * 
     * [name]:[type][?[flag][=value]]
     * 
     * - Name and type are required.
     * - Flag can be many things, controllers or options on how the GUI should
     *   treat it.
     * 
     * To prevent connections to a parameter, add the non-connection flag:
     * nc=1
     * 
     * This will make it impossible to connect the parameter to any other 
     * module. That is, only the human operating VSXu can set it. 
     * Use this when you do operations that involve large chunks of memory
     * or heavy one-time precalculations.
     * 
     * Setting "Default Controller" (which controller is displayed when double-
     *  clicking the param):
     *   default_controller=[value]
     * 
     * Different values (controllers) for different parameters are available:
     *  
     *   controller_knob        float
     *   controller_slider      float, float3, float4
     *   controller_edit        string/resource  
     *   controller_resource    resource (to use it with strings, make it 
     *                          default)
     * 
     * Example:
     *   To make a knob default (when double-clicking) go like this:
     *   angle:float?default_controller=controller_knob
     * 
     */
    info->in_param_spec = 
      "float3_in:float3,"
      "if_to_change:float"
    ;
  
    info->out_param_spec = "float3_out:float3";
    info->component_class = "parameters";
  }

  // this method uses the engine's parameter holder to create data holders for 
  // each of the parameters also binding them to their name (string)
  void declare_params(
    vsx_module_param_list& in_parameters, 
    vsx_module_param_list& out_parameters
  )
  {
    loading_done = true;
    in = (vsx_module_param_float3*)in_parameters.create(
      VSX_MODULE_PARAM_ID_FLOAT3, 
      "float3_in"
    );
    in->set(0.0f, 0);
    in->set(0.0f, 1);
    in->set(0.0f, 2);
    
    if_change = (vsx_module_param_float*)in_parameters.create(
      VSX_MODULE_PARAM_ID_FLOAT, 
      "if_to_change"
    );
    if_change->set(0.0f);
    
    out = (vsx_module_param_float3*)out_parameters.create(
      VSX_MODULE_PARAM_ID_FLOAT3,
      "float3_out"
    );
    out->set(0.0f, 0);
    out->set(0.0f, 1);
    out->set(0.0f, 2);
  }
  
  // this is run once per frame/iteration of the whole engine
  void run()
  {
	float change = if_change->get();
	change = fabs(change);
    if (change > 0.5f)
	{
		out->set(in->get(0), 0);
		out->set(in->get(1), 1);
		out->set(in->get(2), 2);
	}
  }
};


  class vsx_float4_holder : public vsx_module {
  // in
  vsx_module_param_float4* in;
  vsx_module_param_float* if_change;
  // out
  vsx_module_param_float4* out;
  // internal
  //float hold;

public:

  void module_info(vsx_module_info* info)
  {
    info->identifier = "maths;accumulators;float4_holder";
    info->description = 
      "If if_to_change is outside of [-0.5;0.5] range\n"
      "float4_out = float4_in. \n"
      "Else float4_out is last set float4_in.\n"
    ;
    /*
     * Parameter specifications
     *   Used to tell artiste what parameters we want in the GUI and how 
     *   those should behave.
     * 
     * Syntax:
     * 
     * [name]:[type][?[flag][=value]]
     * 
     * - Name and type are required.
     * - Flag can be many things, controllers or options on how the GUI should
     *   treat it.
     * 
     * To prevent connections to a parameter, add the non-connection flag:
     * nc=1
     * 
     * This will make it impossible to connect the parameter to any other 
     * module. That is, only the human operating VSXu can set it. 
     * Use this when you do operations that involve large chunks of memory
     * or heavy one-time precalculations.
     * 
     * Setting "Default Controller" (which controller is displayed when double-
     *  clicking the param):
     *   default_controller=[value]
     * 
     * Different values (controllers) for different parameters are available:
     *  
     *   controller_knob        float
     *   controller_slider      float, float3, float4
     *   controller_edit        string/resource  
     *   controller_resource    resource (to use it with strings, make it 
     *                          default)
     * 
     * Example:
     *   To make a knob default (when double-clicking) go like this:
     *   angle:float?default_controller=controller_knob
     * 
     */
    info->in_param_spec = 
      "float4_in:float4,"
      "if_to_change:float"
    ;
  
    info->out_param_spec = "float4_out:float4";
    info->component_class = "parameters";
  }

  // this method uses the engine's parameter holder to create data holders for 
  // each of the parameters also binding them to their name (string)
  void declare_params(
    vsx_module_param_list& in_parameters, 
    vsx_module_param_list& out_parameters
  )
  {
    loading_done = true;
    in = (vsx_module_param_float4*)in_parameters.create(
      VSX_MODULE_PARAM_ID_FLOAT4, 
      "float4_in"
    );
    in->set(0.0f, 0);
    in->set(0.0f, 1);
    in->set(0.0f, 2);
    in->set(0.0f, 3);
    
    if_change = (vsx_module_param_float*)in_parameters.create(
      VSX_MODULE_PARAM_ID_FLOAT, 
      "if_to_change"
    );
    if_change->set(0.0f);
    
    out = (vsx_module_param_float4*)out_parameters.create(
      VSX_MODULE_PARAM_ID_FLOAT4,
      "float4_out"
    );
    out->set(0.0f, 0);
    out->set(0.0f, 1);
    out->set(0.0f, 2);
    out->set(0.0f, 3);
  }
  
  // this is run once per frame/iteration of the whole engine
  void run()
  {
	float change = if_change->get();
	change = fabs(change);
    if (change > 0.5f)
	{
		out->set(in->get(0), 0);
		out->set(in->get(1), 1);
		out->set(in->get(2), 2);
		out->set(in->get(3), 3);
	}
  }
};


//******************************************************************************
//*** F A C T O R Y ************************************************************
//******************************************************************************

#ifndef _WIN32
#define __declspec(a)
#endif

extern "C" {
__declspec(dllexport) vsx_module* create_new_module(unsigned long module);
__declspec(dllexport) void destroy_module(vsx_module* m,unsigned long module);
__declspec(dllexport) unsigned long get_num_modules();
}


vsx_module* MOD_CM(unsigned long module) {
  switch (module) {
    case 0: return (vsx_module*)new vsx_float_holder;
    case 1: return (vsx_module*)new vsx_float3_holder;
    case 2: return (vsx_module*)new vsx_float4_holder;
  } // switch
  return 0;
}


void MOD_DM(vsx_module* m,unsigned long module) {
  switch(module) {
    case 0: delete (vsx_float_holder*)m; break;
    case 1: delete (vsx_float3_holder*)m; break;
    case 2: delete (vsx_float4_holder*)m; break;
  }
}


unsigned long MOD_NM() {
  return 3;
}  
