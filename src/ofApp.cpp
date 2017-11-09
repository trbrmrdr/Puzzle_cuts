#include "ofApp.h"


#define IMAGE_FILENAME "кактус/finish.png"

//in shader
#define COUNT_L 5.
#define PYZZ_D .285
//--------------------------------------------------------------
void ofApp::setup(){
	if(ofIsGLProgrammableRenderer()){
		//shader.load("shadersGL3/shader");
		//todo
		DebugBreak();
	}
	else{
		_shader.load("shadersGL2/shader");

	}

	_has_start_cuts = false;
	_has_alt_press = false;
	_d_mouse = ofVec2f(0);
	//-------------------------------
	_parameters.setName("settings");
	_parameters.add(_pos.set("pos", ofVec2f(0), ofVec2f(-2000.), ofVec2f(2000.)));
	_parameters.add(_scale.set("scale", 1., .1, 3.));
	_parameters.add(_edit.set("isEdit", true));

	//parameters.add(size_box.set("size_box", 1., 1., 100.));
	//gui.setup(parameters);
	//gui.loadFromFile(SETTING_FILE);

	_font.loadFont(OF_TTF_SANS, 20, true, true);
	ofEnableAlphaBlending();
	ofEnableAntiAliasing();
	ofSetVerticalSync(true);

	_anchor = ofVec2f(.5);


	/*
	if(_image.loadImage(IMAGE_FILENAME)){
		_size_image = ofVec2f(_image.getWidth(), _image.getHeight());
	}
	else{
		DebugBreak();
	}

	_fbo.allocate(_size_image.x, _size_image.y, GL_RGBA);
	_fbo.setAnchorPercent(_anchor.x, _anchor.y);
	*/
}

void ofApp::read_needed_files(){
	ofDirectory directory;
	_name_images.clear();

	directory.open("./");
	vector<ofFile> t_dirs = directory.getFiles();
	for(auto& it : t_dirs){
		if(!it.isDirectory())continue;

		string dirs_name = "./" + it.getBaseName();
		directory.open(dirs_name);
		for(auto& file : directory.getFiles()){
			string name = file.getBaseName();
			if(0 == name.compare("finish")){
				_name_images.insert(pair<string, string>(dirs_name + "/", dirs_name + "/finish.png"));
				break;
			}
		}

		cout << "___________" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	_shader.update();
	if(_image.isAllocated()){
		_image.update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);
	//--------------

	if(_fbo.isAllocated()){
		ofEnableAntiAliasing();
		ofEnableAlphaBlending();
		_fbo.begin();
		ofClear(255, 255, 255, 0);

		_shader.begin();
		_shader.setUniform2f("u_resolution", _size_image);
		ofVec2f t_u_point = _point / _size_image + _anchor;
		if(!( t_u_point.x<.0 || t_u_point.x>1. || t_u_point.y<.0 || t_u_point.y>1. ))
			t_u_point = t_u_point;
		_shader.setUniform2f("u_mouse", t_u_point * _size_image);
		//shader.setUniform1i("u_press", mousePress.x == -1 ? 0 : 1);
		_shader.setUniform1f("u_time", (float) ofGetSystemTime() / 5000.f);
		_shader.setUniform2f("u_ids", _ids);

		if(_image.isAllocated()){
			_image.draw(0, 0);
		}

		_shader.end();
		_fbo.end();

		//--------------

		ofPushMatrix();

		ofTranslate(_pos.get());
		ofScale(_scale.get(), _scale.get());

		//-----------------------------------
		ofSetColor(ofColor::white);
		ofDrawLine(ofVec2f(.0, -10000.f), ofVec2f(.0, +10000.f));
		ofDrawLine(ofVec2f(-10000.f, .0f), ofVec2f(+10000.f, .0f));

		//-----------------------------------
		ofEnableAlphaBlending();
		_fbo.draw(0, 0);
	}

	//-----------------------------------
	if(_has_alt_press){
		ofSetColor(ofColor::green);
		ofDrawCircle(_point, 10);
	}

	ofPopMatrix();
	//-----------------------------------	
	//gui.draw();
	ofSetColor(ofColor::green);
	_font.drawString("fps :" + ofToString((int) ofGetFrameRate()), ofGetWidth() - 300, 40);

	//______________________________
	if(_has_start_cuts){
		if(_ids.y > COUNT_L - 1){
			if(!( _has_start_cuts = !_name_images.empty() ))return;
			map<string, string>::iterator it = _name_images.begin();
			_curr_dir = it->first;


			if(_image.loadImage(it->second)){
				_size_image = ofVec2f(_image.getWidth(), _image.getHeight());
			}
			else{
				DebugBreak();
			}
			_name_images.erase(it);

			_fbo.clear();
			_fbo.allocate(_size_image.x, _size_image.y, GL_RGBA);
			_fbo.setAnchorPercent(_anchor.x, _anchor.y);

			_ids = ofVec2f(0., 0.);
		}
		else{

			ofPixels t_pixels;
			_fbo.readToPixels(t_pixels);
			ofImage t_image(t_pixels);

			string new_file_name = ofVAArgsToString("%s%i_%i.png", _curr_dir.c_str(), (int) _ids.x, (int) _ids.y);
			ofVec2f new_size = _size_image / ( COUNT_L - 1. );
			new_size += new_size*PYZZ_D;

			t_image.resize(new_size.x, new_size.y);

			ofFile::removeFile(new_file_name);
			//if(_ids.y == 4.)
				t_image.saveImage(new_file_name, OF_IMAGE_QUALITY_BEST);
			//ofSaveImage(t_pixels, );

			cout << "saves _ids.x = " << _ids.x << " _ids.y = " << _ids.y << endl;
			if(++_ids.x == COUNT_L){
				_ids.y++;
				_ids.x = 0.;
			}
			//_name_images.clear();
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case OF_KEY_ALT:
			_has_alt_press = true;
			break;
		case ' ':
			/*
			if(!hasCaptureFrame){
				gifEncoder.reset();
				hasCaptureFrame = true;
				gifFrame = 0;
				lastGifTime = -1.;
			}
			*/
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key){
		case OF_KEY_ALT:
			_has_alt_press = false;
			break;
		case ' ':
			//hasCaptureFrame = false;
			break;
		case 's':
			cout << "start saving\n" << endl;
			read_needed_files();
			_ids.y = COUNT_L + 1;
			_has_start_cuts = true;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){
	if(_has_alt_press && _edit.get()){
		_scale.set(_scale.get() + .1*scrollY);
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if(_mouse_press.x == -1) return;

	if(_has_alt_press && _edit.get()){
		_d_mouse = _mouse_press - ofVec2f(x, y) - _prev_pos_image;
		_pos.set(ofVec2f(-1.) * _d_mouse);
	}
	else{
		_point = ( ofVec2f(x, y) - _pos.get() ) / _scale.get();
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	_mouse_press = ofVec2f(x, y);
	_prev_pos_image = _pos.get();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	_mouse_press.x = -1.f;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
