#pragma once
#include "ofMain.h"
#include "ofEvents.h"
#include "ofxNetwork.h"
#include "pixelNode.h"

class ofxPixelNode : public ofBaseApp {
public:
	void setup();
    void send();
	void draw();
    void onExit(ofEventArgs &data);
	void onUpdate(ofEventArgs &data);

	void setFbo(ofFbo & f);
	void setBrightness(int v);
	void setScene(int v);
	void yes();

	vector < std::shared_ptr<ofxUDPManager> > udpConnections;
	ofxUDPManager udpConnection;
	ofxUDPManager udpYes;

	int brightness = 127;

	string info;

	int previewScale = 2;

	void createPixelNode(string ip, string id);
	ofFbo * _fbo;
	
	map <string, pixelNode> pixelNodes;
	
	
	vector < std::shared_ptr<ofxUDPManager> > udpConfigs;
	
	
	void identifyPixelnode(int i);
};
