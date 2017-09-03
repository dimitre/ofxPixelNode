#include "ofxPixelNode.h"

//#define NMODULOS 14
#define NMODULOS 2

//--------------------------------------------------------------
void ofxPixelNode::setup() {
	// TO RECEIVE INCOMING UDP BROADCAST FROM PIXELNODES
	udpConnection.Create();
	udpConnection.SetTTL(1);
	udpConnection.SetNonBlocking(true);
	udpConnection.Bind(4001);

	cout << "ofxPixelNode INIT -+-+-+-+-+-+-+-+<<<>>>" << endl;
	string informationSociety = R"(
 ________           __
 \______ \   ______/  |________   ___________  ____
 |     |  \ /     \   __\_  __ \ /  _ \_  __ \/ ___\
 |     `   \  Y Y  \  |  |  | \/(  <_> )  | \/ /_/  >
 /_______  /__|_|  /__|  |__| /\ \____/|__|  \___  /
         \/      \/           \/            /_____/
)";

	cout << informationSociety << endl;
	ofAddListener(ofEvents().exit, this,   &ofxPixelNode::onExit);
	ofAddListener(ofEvents().update, this,   &ofxPixelNode::onUpdate);
	
	
//	for (int a=0; a<NMODULOS; a++) {
//		createPixelNode("192.168.0." + ofToString(200+a), a);
//	}
}
// end setup

void ofxPixelNode::createPixelNode(string ip, string id) {
	pixelNodes[ip].ip = ip;
	pixelNodes[ip].id = id;
	
	
	string ids[] = { "0.4.228.5" };
	
	if (!pixelNodes[ip].connected) {
		std::shared_ptr<ofxUDPManager> udpRef = std::shared_ptr<ofxUDPManager>(new ofxUDPManager);
		udpRef->Create();
		udpRef->Connect(ip.c_str(), 4000);
		udpRef->SetTTL(1);
		udpRef->SetNonBlocking(true);
		udpConnections.push_back(udpRef);
		
		
		//int ids[] = { 768572, 9725796, 9724665, 14405023, 9724601, 14405244, 14409397, 9726602, 14408232, 9725841 };
		//map <string, posX>;
		
		int index = 0;

//		for (auto & i : ids) {
//			if (i==pixelNodes[ip].id) {
//				pixelNodes[ip].pos = index;
//			}
//			index++;
//		}
		
		
		// temporario
		//pixelNodes[ip].pos = id;
		
		pixelNodes[ip].pos = 0;
		pixelNodes[ip].id = id;
		
//		if (id == 1) {
//			//pixelNodes[ip].fboPosX =
//			pixelNodes[ip].fboPosY = 20;
//		}
		pixelNodes[ip].udpRef = udpConnections.back();
		pixelNodes[ip].init();
		pixelNodes[ip].setFbo(*_fbo);
		pixelNodes[ip].connected = true;
	}

}

void ofxPixelNode::onUpdate(ofEventArgs &data) {
	// GATHER MORE PIXELNODE DATA FROM NETWORK, ONCE A SECOND
	//cout << "update pixelnode" << endl;
	char udpMessage[1000];
	udpConnection.Receive(udpMessage,1000);
	string message=udpMessage;
	
	if (message != "") {
		cout << message << endl;
		vector <string> msgs = ofSplitString(message, " ");
		string ip = msgs[1];
		string id = msgs[2];
		cout << "=-=-=-=-=" << endl;
		
		createPixelNode(ip, id);
	}
}

void ofxPixelNode::send() {
	for (auto & p : pixelNodes) {
		p.second.prepare();
	}
	for (auto & p : pixelNodes) {
		p.second.send();
	}
}


void ofxPixelNode::draw() {
	for (auto & p : pixelNodes) {
		p.second.draw();
	}
}

void ofxPixelNode::onExit(ofEventArgs &data) {
	cout << "ofxPixelNode Exit!" << endl;
}

void ofxPixelNode::setBrightness(int b) {
	cout << "setBrightness" << endl;
	cout << b << endl;
	
	for (auto & p : pixelNodes) {
		p.second.setBrightness(b);
	}
}

void ofxPixelNode::setFbo(ofFbo & f) {
	_fbo = &f;
	for (auto & p : pixelNodes) {
		p.second.setFbo(f);
	}
}
