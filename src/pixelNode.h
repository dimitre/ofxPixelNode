#define NBYTES 300*3
#define NBYTESCONFIG 10

class pixelNode {
public:
	string ip;
	string id;
	//int id = 0;
	int pos;
	
	std::shared_ptr<ofxUDPManager> udpRef;
	std::shared_ptr<ofxUDPManager> udpConfig;
	char pixelData[NBYTES]; // 300 pixels x 3 bytes rgb
	char dataConfig[NBYTESCONFIG];
	

	int w = 30;
	int h = 20;
	int fboPosX = 0;
	int fboPosY = 0;
	
	vector <int> pixelOrder; // nubank
	ofFbo * _fbo = NULL;
	ofFbo fbo;
	ofPixels pixels;
	

	bool connected = false;
	void init() {
		connected = true;
		fbo.allocate(w, h, GL_RGB);
		fbo.begin();
		ofClear(0,255);
		fbo.end();
		fbo.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		pixels.allocate(w, h, OF_IMAGE_COLOR);
		createPixelPosition();
	}
	
	void setFbo(ofFbo & f) {
		_fbo = &f;
	}

	void prepare() {
		fbo.begin();
		ofSetColor(255);
		if (_fbo != NULL) {
			_fbo->draw(-fboPosX, -fboPosY);
		} else {
			cout << "fbo is null" << endl;
		}
		fbo.end();
		fbo.readToPixels(pixels);
		for (int a=0; a<pixelOrder.size(); a++) {
			int cursor = a*3;
			//void* memcpy( void* dest, const void* src, std::size_t count );
			memcpy(&pixelData[cursor], &pixels.getData()[cursor], 3);
			//memcpy aqui pra 3 bytes? pode ser.
//			pixelData[a * 3 + 0] = pixels.getData()[pixelOrder[a] * 3 + 0];
//			pixelData[a * 3 + 1] = pixels.getData()[pixelOrder[a] * 3 + 1];
//			pixelData[a * 3 + 2] = pixels.getData()[pixelOrder[a] * 3 + 2];
		}
	}
	
	void send() {
		udpRef->Send(pixelData, NBYTES);
	}

	
	void setBrightness(int b) {
		dataConfig[1] = b;
		udpConfig->Send(dataConfig, NBYTESCONFIG);
	}

	void draw() {
		if (connected) {
			ofSetColor(0, 200, 0);
		} else {
			ofSetColor(255,0,0);
		}

		ofPushMatrix();
//		ofTranslate(pos * 200, 0);
		int altura = 48;
		ofTranslate( 1200, pos * 50);
		ofDrawRectangle(0, 0, 190, altura - 2);
		
		ofSetColor(255);
		fbo.draw(0,0, fbo.getWidth() * 2, fbo.getHeight()*2);
		
		string s = ip
		+ "\n" + id;
		ofDrawBitmapString(s, 10, 20);
		ofPopMatrix();
	}
	
	
	
	void createPixelPosition() {
		int offImpar = -1;
		int offPar = 1;
		
		for (int y = 10; y>0; y--) {
			int yPar = 20;
			yPar = y*2;
			int yImpar = 19;
			yImpar = yPar -2;
			
			pixelOrder.push_back(30*yPar-1 + offImpar);
			pixelOrder.push_back(30*yPar-3 + offImpar);
			pixelOrder.push_back(30*yPar-5 + offImpar);
			pixelOrder.push_back(30*yPar-7 + offImpar);
			pixelOrder.push_back(30*yPar-9 + offImpar);
			
			pixelOrder.push_back(30*yPar-11 + offImpar);
			pixelOrder.push_back(30*yPar-13 + offImpar);
			pixelOrder.push_back(30*yPar-15 + offImpar);
			pixelOrder.push_back(30*yPar-17 + offImpar);
			pixelOrder.push_back(30*yPar-19 + offImpar);
			
			pixelOrder.push_back(30*yPar-21 + offImpar);
			pixelOrder.push_back(30*yPar-23 + offImpar);
			pixelOrder.push_back(30*yPar-25 + offImpar);
			pixelOrder.push_back(30*yPar-27 + offImpar);
			pixelOrder.push_back(30*yPar-29 + offImpar);
			
			
			pixelOrder.push_back(30*yImpar+0 + offPar);
			pixelOrder.push_back(30*yImpar+2 + offPar);
			pixelOrder.push_back(30*yImpar+4 + offPar);
			pixelOrder.push_back(30*yImpar+6 + offPar);
			pixelOrder.push_back(30*yImpar+8 + offPar);
			
			pixelOrder.push_back(30*yImpar+10 + offPar);
			pixelOrder.push_back(30*yImpar+12 + offPar);
			pixelOrder.push_back(30*yImpar+14 + offPar);
			pixelOrder.push_back(30*yImpar+16 + offPar);
			pixelOrder.push_back(30*yImpar+18 + offPar);
			
			pixelOrder.push_back(30*yImpar+20 + offPar);
			pixelOrder.push_back(30*yImpar+22 + offPar);
			pixelOrder.push_back(30*yImpar+24 + offPar);
			pixelOrder.push_back(30*yImpar+26 + offPar);
			pixelOrder.push_back(30*yImpar+28 + offPar);
		}
	}
};
