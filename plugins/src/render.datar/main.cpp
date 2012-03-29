/**
* Project: VSXu: Realtime visual programming language, music/audio visualizer, animation tool and much much more.
*
* @author Jonatan Wallmander, Vovoid Media Technologies Copyright (C) 2003-2011
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

#include "_configuration.h"
#include "vsx_param.h"
#include "vsx_module.h"
#include "vsx_math_3d.h"
#include "vsx_font/vsx_font.h"
#include "vsx_glsl.h"

#include "vsx_sequence.h"

#include <vector>

class vsx_module_path_render_line : public vsx_module {
  // in
  // position sequences
	vsx_module_param_sequence* seqs[7];
/*	vsx_module_param_sequence* y_seq;
	vsx_module_param_sequence* z_seq;
	
  //color sequences
	vsx_module_param_sequence* r_seq;
	vsx_module_param_sequence* g_seq;
	vsx_module_param_sequence* b_seq;
	vsx_module_param_sequence* a_seq;
*/
	vsx_module_param_float* points;
	vsx_module_param_float* line_width;

	// out
	vsx_module_param_render* render_out;
	
	// internal
	vsx_sequence seq_int;
	std::vector<float> vals[7];
	//unsigned long updates[7];
	unsigned long pts;
	

public:

	void module_info(vsx_module_info* info) {
		info->identifier = "renderers;examples;path_line_render";
		info->description = "";
		info->in_param_spec ="spatial:complex{"
			"x_sequence:sequence,"
			"y_sequence:sequence,"
			"z_sequence:sequence"
		"},"
		"color:complex{"
			"r_sequence:sequence,"
			"g_sequence:sequence,"
			"b_sequence:sequence,"
			"a_sequence:sequence"
		"},"
		"points:float,"
		"line_width:float";

		info->out_param_spec = "render_out:render";
		info->component_class = "render";
		loading_done = true;
	}
  
	void declare_params(vsx_module_param_list& in_parameters, vsx_module_param_list& out_parameters) {
		seqs[0] = (vsx_module_param_sequence*)in_parameters.create(VSX_MODULE_PARAM_ID_SEQUENCE,"x_sequence");
		seqs[1] = (vsx_module_param_sequence*)in_parameters.create(VSX_MODULE_PARAM_ID_SEQUENCE,"y_sequence");
		seqs[2] = (vsx_module_param_sequence*)in_parameters.create(VSX_MODULE_PARAM_ID_SEQUENCE,"z_sequence");
		
		seqs[3] = (vsx_module_param_sequence*)in_parameters.create(VSX_MODULE_PARAM_ID_SEQUENCE,"r_sequence");
		seqs[4] = (vsx_module_param_sequence*)in_parameters.create(VSX_MODULE_PARAM_ID_SEQUENCE,"g_sequence");
		seqs[5] = (vsx_module_param_sequence*)in_parameters.create(VSX_MODULE_PARAM_ID_SEQUENCE,"b_sequence");
		seqs[6] = (vsx_module_param_sequence*)in_parameters.create(VSX_MODULE_PARAM_ID_SEQUENCE,"a_sequence");
		
		for (int i = 0; i < 7; i++) seqs[i]->set(seq_int);
		
		points = (vsx_module_param_float*)in_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,"points");
		points->set(100.0f);
		line_width = (vsx_module_param_float*)in_parameters.create(VSX_MODULE_PARAM_ID_FLOAT,"line_width");
		line_width->set(1.0f);

		render_out = (vsx_module_param_render*)out_parameters.create(VSX_MODULE_PARAM_ID_RENDER,"render_out");
		
  }
  
	void output(vsx_module_param_abs* param)
	{
		map();
		glLineWidth(line_width->get());
		glBegin(GL_LINE_LOOP);
			for(unsigned long i = 0; i < pts; i++)
			{
				glColor4f(vals[3][i], vals[4][i], vals[5][i], vals[6][i]);
				glVertex3f(vals[0][i], vals[1][i], vals[2][i]);
			}
		glEnd();
	    render_out->set(1);
		loading_done = true;
	}
	void on_delete() {
  }
  
  private:
	
	void map()
	{
		unsigned long new_pts = floor(abs(points->get()));
		if(pts != new_pts)
		{
			unsigned int res = 0;
			if(pts < new_pts ||  pts > 4 * new_pts) res = new_pts * 2;
			pts = new_pts;
			for(int i = 0; i < 7; i++)
			{
				map(i, res);
			}
		}
		else
		{
			for(int i = 0; i < 7; i++)
			{
				if(seqs[i]->updates)
				{
					seqs[i]->updates = 0;
					map(i);
				}
			}
		}
	}
	
	void map(int i, unsigned int resize = 0)
	{
		if(resize)
			vals[i].resize(pts, 0);
		seq_int = seqs[i]->get();
		seq_int.reset();
		float x = 1.0 / (float)pts;
		for(unsigned long j = 0; j < pts; j++)
		{
			vals[i][j]=seq_int.execute(x);
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
  // in here you load your module.
  // i suggest you make a cache on the first run (this is called once when the vsxu engine starts
  // as it's looping through the available modules and will call
  // this method to get one of each. If you cache the stuff you need (all the stuff
  // needed for the module_info method call, it'll be fast once the engine actually needs
  // one of these. You can do the actual loading/initialization in the run() method because
  // that will only be called when actually used.
  //
  switch(module) {
    case 0: return (vsx_module*)(new vsx_module_path_render_line);
  }
  return 0;
}

void MOD_DM(vsx_module* m,unsigned long module) {
  switch(module) {
    case 0: delete (vsx_module_path_render_line*)m; break;
  }
}

unsigned long MOD_NM() {
  // in here you have to find out how many salvation modules are available and return
  // the correct number. The create_new_module will then create one of each and run
  //   vsx_module_plugin::module_info(vsx_module_info* info)
  // on it. The aim here is to make VSXU think that each and one of the available
  // slavation cabinets are a unique module.

  return 1;
}
