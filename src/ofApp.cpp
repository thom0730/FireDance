#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    kinect.open(true, true, 0);
    kinect.start();
    
    mesh.setUsage(GL_DYNAMIC_DRAW);
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    cam.setAutoDistance(false);
    cam.setDistance(200);
    
    minDistance = 10;
    maxDistance = 150;
    gui.setup();
    gui.add(minDistance.setup("minDistance", minDistance, 0, 500));
    gui.add(maxDistance.setup("maxDistance", maxDistance, 0, 500));
    
    depthShader.setupShaderFromSource(GL_FRAGMENT_SHADER, depthFragmentShader);
    depthShader.linkProgram();
    irShader.setupShaderFromSource(GL_FRAGMENT_SHADER, irFragmentShader);
    irShader.linkProgram();

}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    
    if (kinect.isFrameNew()) {
        
        
        texture.loadData(kinect.getDepthPixelsRef());
      //  texture.loadData(kinect.getIrPixelsRef());

        
        /*
        mesh.clear();
        
        int st = 2;
        int w = kinect.getDepthPixelsRef().getWidth();
        int h = kinect.getDepthPixelsRef().getHeight();
        int minD = minDistance;
        int maxD = maxDistance;
        
        for (int x = 0; x < w; x+=2) {
            for (int y = 0; y < h; y+=2) {
                float d = kinect.getDistanceAt(x, y);
                if (d > minD && d < maxD) { // adjust depth range as you like
                    ofVec3f p = kinect.getWorldCoordinateAt(x, y, d);
                    mesh.addColor(ofColor(255));
                    mesh.addVertex(p);
                }
            }
         }*/
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
   // ofClear(0);
    
    /*
    if (mesh.getVertices().size()) {
        ofPushStyle();
        glPointSize(2);
        cam.begin();
        ofPushMatrix();
        ofTranslate(0, 0, -100);
        mesh.draw();
        ofPopMatrix();
        cam.end();
        ofPopStyle();
     }*/
    if(texture.isAllocated()){
        //depth
        depthShader.begin();
        texture.draw(0, 0, ofGetWidth(), ofGetHeight());
        depthShader.end();
        
        //ir
       // irShader.begin();
       // texture.draw(0, 0, ofGetWidth(), ofGetHeight());
       // irShader.end();

    }
    
    ofDrawBitmapStringHighlight("fps: " + ofToString(ofGetFrameRate()), ofGetWidth() - 120, 20);
    gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
