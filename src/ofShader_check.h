#pragma once

#include "ofMain.h"

class EditFile
{
	long last_edit;
	string file_name;
public:
	void init(const string& file){
		last_edit = -1;
		file_name = ofFilePath::getAbsolutePath(file, true);
		isEdit();
	}

	bool isEdit(){
		long t_last_Edit = GetLastEdit(file_name.c_str());
		if(t_last_Edit != last_edit){
			last_edit = t_last_Edit;
			return true;
		}
		return false;
	}

	static long GetLastEdit(const char* path_to_file){
		struct tm* clock;
		struct stat attrib;
		stat(path_to_file, &attrib);
#if 0
		time_t rawtime;
		time(&rawtime);
		clock = gmtime(&rawtime);
#endif
		clock = gmtime(&( attrib.st_mtime ));
		return  clock->tm_hour * 10000
			+ clock->tm_min * 100
			+ clock->tm_sec;
	}
};

class ofShader_check :public ofShader
{
	EditFile EditFile_v;
	EditFile EditFile_f;
	ofShader t_shader;
	std::string file_name;
public:
	virtual bool load(string shaderName){
		file_name = shaderName;
		bool ret = ofShader::load(file_name);

		EditFile_v.init(file_name + ".vert");
		EditFile_f.init(file_name + ".frag");
		return ret;
	}

	void update(){
		bool isEdit = EditFile_v.isEdit() + EditFile_f.isEdit();
		if(isEdit && isLoaded()){
			t_shader.load(file_name);
			if(t_shader.isLoaded() && t_shader.checkProgramLinkStatus(t_shader.getProgram())){
				t_shader.unload();
				ofLogNotice("ofShader") << "\n" << "##########################\n" << "reload shader";
				ofShader::load(file_name);
			}
		}
	}
};
