#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMultiKinectV2.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxSyphon.h"


#define STRINGIFY(x) #x

static string irFragmentShader =
STRINGIFY(
          uniform sampler2DRect tex;
          void main()
          {
              vec4 col = texture2DRect(tex, gl_TexCoord[0].xy);
              float value = col.r / 65535.0;
              gl_FragColor = vec4(vec3(value), 1.0);
          }
);

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    
    ofxMultiKinectV2 kinect;
    ofEasyCam cam;
    ofVboMesh mesh;
    
    ofTexture irTexture;
    ofxCvGrayscaleImage irImage;
    ofxCvColorImage myCvImage;
    ofFbo frameBuffer;
    ofPixels irPixels;
    int width;
    int height;
    
    ofImage kinectImage;
    ofImage depthImage;
    ofImage threshImage; 
    ofxCv::ContourFinder contourFinder; //CV輪郭抽出
    //int threshold;
    
    //FOR GUI
    ofxPanel gui;
    ofParameter<float> threshold;
    ofParameter<bool> trackHs;
    ofParameter<int> r;
    ofParameter<int> g;
    ofParameter<int> b;
    ofColor targetColor;
    ofxFloatSlider minDistance;
    ofxFloatSlider maxDistance;

    ofxFloatSlider thresh;
    ofxFloatSlider minRadius;
    ofxFloatSlider maxRadius;
    
    ofTexture texture;
    ofShader depthShader;
    ofShader irShader;
    
    ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonServer individualTextureSyphonServer;
    ofxSyphonClient mClient;
    
    ofImage img;
};
