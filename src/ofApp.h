#pragma once

#include "ofMain.h"
#include "ofShader_check.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"

class ofApp : public ofBaseApp
{

public:
	virtual void setup();
	virtual void update();
	virtual void draw();

	virtual void keyPressed(int key);
	virtual void keyReleased(int key);
	virtual void mouseMoved(int x, int y);
	virtual void mouseScrolled(int x, int y, float scrollX, float scrollY);
	virtual void mouseDragged(int x, int y, int button);
	virtual void mousePressed(int x, int y, int button);
	virtual void mouseReleased(int x, int y, int button);
	virtual void mouseEntered(int x, int y);
	virtual void mouseExited(int x, int y);
	virtual void windowResized(int w, int h);
	virtual void dragEvent(ofDragInfo dragInfo);
	virtual void gotMessage(ofMessage msg);
private:
	ofImage _image;
	ofShader_check _shader;
	ofFbo _fbo;

	ofVec2f _anchor;
	ofVec2f _size_image;

	//ofxPanel gui;
	ofParameterGroup _parameters;
	string _lastFileName;
	ofParameter<ofVec2f> _pos;
	ofParameter<float> _scale;
	ofParameter<bool> _edit;

	ofXml _settings;
	ofTrueTypeFont _font;

	bool _has_alt_press;
	ofVec2f _d_mouse;
	ofVec2f _prev_pos_image;
	ofVec2f _mouse_press;

	ofVec2f _point;

	ofDirectory _directory;
	map<string, string> _name_images;
	string _curr_dir;
	bool _has_start_cuts;
	ofVec2f _ids;
};
