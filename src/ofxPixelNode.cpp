#include "ofxPixelNode.h"

//#define NMODULOS 14
#define NMODULOS 2

// passar pra la
char yesData[3];
char brightnessData[3];
char sceneData[3];

vector <ofPoint> posFbo;
vector <string> ids;



//--------------------------------------------------------------
void ofxPixelNode::setup() {
	yesData[0] = 1;

	// TO RECEIVE INCOMING UDP BROADCAST FROM PIXELNODES
	udpConnection.Create();
	udpConnection.SetTTL(1);
	udpConnection.SetNonBlocking(true);
	udpConnection.Bind(4001);
	
	udpYes.Create();
	udpYes.SetEnableBroadcast(true);
	udpYes.Connect("192.168.0.255", 4002);
	udpYes.SetTTL(1);
	udpYes.SetNonBlocking(true);
//	udpYes.Bind(4002);

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

	
	
	for (int a=0; a<14; a++) {
		int x = (a/2) * 30;
		int y = (a%2) * 20;
		posFbo.push_back(ofPoint (x,y));
		//cout << posFbo.back() << endl;
	}
	
	if (ofFile::doesFileExist("pixelNodes.txt")) {
		
		ofBuffer buff2 = ofBufferFromFile("pixelNodes.txt");
		for(auto & i : buff2.getLines()) {
			ids.push_back(ofSplitString(i, " ")[0]);
		}
		cout << "=-=-=-=-= initializing pixelNodes by TXT ID =-=-=-=-=" << endl;
	} else {
		string idLines =
R"(
0.4.191.136
0.4.191.209 2
0.4.191.156 3
0.4.191.26 4
0.4.197.21 5
0.4.191.145 6
0.4.227.191 7
0.4.221.184 8
0.4.191.74 9
0.4.198.186 10
0.4.192.97
0.4.191.200
0.4.228.5
)";
		
		for (auto & i : ofSplitString(idLines, "\n")) {
			if (i != "") {
				ids.push_back(ofSplitString(i, " ")[0]);
			}
		}
	}
}
// end setup

int highIndex = 14;


void ofxPixelNode::createPixelNode(string ip, string id) {
	if ( pixelNodes.find(ip) == pixelNodes.end()) {
		pixelNodes[ip].ip = ip;
		pixelNodes[ip].id = id;
		
		//	};
	//if (!pixelNodes[ip].connected) {
		cout << ">>> ofxPixelNode connect to ip :: "+ip << endl;
		{
		std::shared_ptr<ofxUDPManager> udpRef = std::shared_ptr<ofxUDPManager>(new ofxUDPManager);
		udpRef->Create();
		udpRef->Connect(ip.c_str(), 4000);
		udpRef->SetTTL(1);
		udpRef->SetNonBlocking(true);
		udpConnections.push_back(udpRef);
		}

		
		// remover este depois
		{
		std::shared_ptr<ofxUDPManager> udpConfigRef = std::shared_ptr<ofxUDPManager>(new ofxUDPManager);
		udpConfigRef->Create();
		udpConfigRef->Connect(ip.c_str(), 4002);
		udpConfigRef->SetTTL(1);
		udpConfigRef->SetNonBlocking(true);
		udpConfigs.push_back(udpConfigRef);
		}
		
		
		int index = 0;

		// fazer aqui a posicao x e y de cada controlador
		for (auto & i : ids) {
			if (i==id) {
				pixelNodes[ip].index = index;
				pixelNodes[ip].posFbo = posFbo[index];
			}
			index++;
		}
		
		// temporario
		
		if (pixelNodes[ip].index < 0) {
			pixelNodes[ip].index = highIndex;
			highIndex++;
		}
		//pixelNodes[ip].index = pixelNodes.size()-1;
		
		pixelNodes[ip].udpConfig = udpConfigs.back();
		
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
		vector <string> msgs = ofSplitString(message, " ");
		if (msgs.size() >= 2) {
			string ip = msgs[1];
			string id = msgs[2];
			//cout << message << endl;
			//cout << "=-=-=-=-=" << endl;
			createPixelNode(ip, id);
		}
	}
}

void ofxPixelNode::yes() {
	udpYes.Send(yesData, 1);
}

void ofxPixelNode::send() {
	for (auto & p : pixelNodes) {
		p.second.prepare();
	}
	for (auto & p : pixelNodes) {
		p.second.send();
	}
	
	yes();
}


void ofxPixelNode::draw() {
	for (auto & p : pixelNodes) {
		p.second.draw();
	}
}

void ofxPixelNode::identifyPixelnode(int index) {
//	cout << "identifyPixelnode" << endl;
//	for (auto & p : pixelNodes) {
//		p.second.setScene(0);
//	}
	
	int i = 0;
	for (auto & p : pixelNodes) {
		if (i == index) {
			p.second.setScene(1);
			cout << "PixelNode " + p.second.ip << endl;
		} else {
			p.second.setScene(0);
		}
		i++;
	}

}

void ofxPixelNode::onExit(ofEventArgs &data) {
	cout << "ofxPixelNode Exit!" << endl;
}


void ofxPixelNode::setBrightness(int b) {
	cout << "setBrightness :: " << b <<  endl;
	brightnessData[0] = 2;
	brightnessData[1] = b;
	udpYes.Send(brightnessData,2);
}


void ofxPixelNode::setScene(int s) {
	cout << "setScene :: " << s <<  endl;
	sceneData[0] = 3;
	sceneData[1] = s;
	udpYes.Send(sceneData,2);
}

void ofxPixelNode::setFbo(ofFbo & f) {
	_fbo = &f;
	for (auto & p : pixelNodes) {
		p.second.setFbo(f);
	}
}
