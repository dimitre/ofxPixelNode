#pragma once
#include "ofMain.h"
#include "ofEvents.h"
#include "ofxNetwork.h"
#include "pixelNode.h"

class ofxPixelNode : public ofBaseApp {
public:
	void setup();
    void setBrightness(int b);
    void send();
	void draw();
	void setFbo(ofFbo & f);
    void onExit(ofEventArgs &data);
	void onUpdate(ofEventArgs &data);

	vector < std::shared_ptr<ofxUDPManager> > udpConnections;

	int brightness = 127;

	string info;

	int previewScale = 2;

	ofxUDPManager udpConnection;
	map <string, pixelNode> pixelNodes;
	
	
	void createPixelNode(string ip, string id);
	ofFbo * _fbo;
};
