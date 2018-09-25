#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofEnableSmoothing();
    
    //FOR KINECT INIT
    kinect.open(true, true, 0);
    kinect.start();
    
    
    //FOR SHADE
    irShader.setupShaderFromSource(GL_FRAGMENT_SHADER, irFragmentShader);
    irShader.linkProgram(); // シェーダのリンクが正しく行なわれたかチェック
    
    //syphon
    mainOutputSyphonServer.setName("Screen Output");
    mClient.setup();
    mClient.setApplicationName("Simple Server");
    mClient.setServerName("");
    
    //FOR CV
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(150);
    
    //FOR GUI
    gui.setup();
    gui.add(threshold.set("Threshold", 128, 0, 255));
    gui.add(trackHs.set("Track Hue/Saturation", false));
    gui.add(r.set("r",255, 0, 255));
    gui.add(g.set("g",255, 0, 255));
    gui.add(b.set("b",255, 0, 255));
    
    width = 2000;
    height = 2000;
    
    myCvImage.allocate(ofGetWidth(), ofGetHeight());
    frameBuffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    irPixels.allocate(ofGetWidth(), ofGetHeight(),OF_PIXELS_RGB);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    kinect.update();
    if (kinect.isFrameNew()) {
        irTexture.loadData(kinect.getIrPixelsRef());
//        float *data = kinect.getIrPixelsRef().getData();
        
        
        
        

    

        


        

        
//        contourFinder.setTargetColor(targetColor, trackHs ? TRACK_COLOR_HS : TRACK_COLOR_RGB);
//        cv::Mat imgMat = ofxCv::toCv(kinect.getIrPixelsRef());
//        contourFinder.findContours(imgMat); //ビデオカメラから輪郭を検出
        
        
        //        ofxCvGrayscaleImage  grayImage;
        //
        //        grayImage.allocate(kinect.getIrPixelsRef().getWidth(),kinect.getIrPixelsRef().getHeight());
        //
        //        img.setFromPixels(kinect.getIrPixelsRef());
        //        ofxCv::convertColor(img.getPixelsRef(), grayImage.getPixels() , CV_RGBA2GRAY);
        //        grayImage.flagImageChanged();
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    ofClear(0);
    
    
    
    if(irTexture.isAllocated()){
        frameBuffer.begin();
        irShader.begin();
        irTexture.draw(0, 0, ofGetWidth(), ofGetHeight());
        irShader.end();
        frameBuffer.end();
        frameBuffer.readToPixels(irPixels);
        myCvImage.setFromPixels(irPixels);
        myCvImage.draw(0.0, 0.0, ofGetWidth(), ofGetHeight());
        
        //openCV
        contourFinder.setTargetColor(targetColor, trackHs ? TRACK_COLOR_HS : TRACK_COLOR_RGB);
        contourFinder.setThreshold(threshold); // 閾値を設定
        cv::Mat imgMat = ofxCv::toCv(myCvImage);
        contourFinder.findContours(imgMat); //ビデオカメラから輪郭を検出
        
        for(int i = 0; i < contourFinder.size() ; i++){
            ofDrawCircle(ofxCv::toOf(contourFinder.getCenter(i)),50);
            
        }
        contourFinder.draw();
        
//        ofPixels pixels;
//        irTexture.readToPixels(pixels);
//        //unsigned char *data = pixels.getData();
//        float *data = kinect.getIrPixelsRef().getData();
//
//        int w = kinect.getIrPixelsRef().getWidth();
//        int h = kinect.getIrPixelsRef().getHeight();
//
//
//
//        for(int y = 0; y < h; y+=8){
//            for(int x = 0; x < w; x+=8){
//
//
//                int r = pixels[y * 3 * w + x * 3];  //それぞれのピクセルのr, g, bを抽出
//                int g = pixels[y * 3 * w + x * 3 + 1];
//                int b = pixels[y * 3 * w + x * 3 + 2];
//
//                float value = r / 65535.0;
//
//                ofSetColor(r, r, r, 255);   //透過度50%で描画
//                ofCircle(20 + x, 20 + y, 5);   //円を描く
//            }
//        }
        
//        irShader.begin();
//        irTexture.draw(0, 0, ofGetWidth(), ofGetHeight());
//        irShader.end();
        
        //img.draw(0,0);
        //
//        for(int i = 0; i < contourFinder.size() ; i++){
//            ofDrawCircle(ofxCv::toOf(contourFinder.getCenter(i)),10);
//        }
//        contourFinder.draw();
    }
    
    ofDrawBitmapStringHighlight("fps: " + ofToString(ofGetFrameRate()), ofGetWidth() - 120, 20);
    mainOutputSyphonServer.publishScreen();
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
    targetColor = myCvImage.getPixels().getColor(x,y);
    cout <<targetColor<< endl;
    
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

