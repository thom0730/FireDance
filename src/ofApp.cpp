#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofEnableSmoothing();
    
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    
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
    gui.add(threshold.set("Threshold", 40, 0, 255));
    gui.add(trackHs.set("Track Hue/Saturation", false));
    gui.add(r.set("r",255, 0, 255));
    gui.add(g.set("g",255, 0, 255));
    gui.add(b.set("b",255, 0, 255));
    gui.add(IRCamera.set("IRCamera", true));
    gui.add(circle.set("Circle for Debug", true));
    
    width = ofGetWidth();
    height = ofGetHeight();
    
    myCvImage.allocate(ofGetWidth(), ofGetHeight());
    frameBuffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    irPixels.allocate(ofGetWidth(), ofGetHeight(),OF_PIXELS_RGB);
    
    //FOR FIRE

    fluid.allocate(width, height, 0.5);

    fluid.dissipation = 0.99;
    fluid.velocityDissipation = 0.99;
        
    fluid.setGravity(ofVec2f(0.0,0.0));
    fluid.begin();
    ofSetColor(0,0);
    ofSetColor(255);
   // ofCircle(width*0.5, height*0.35, 40);
    fluid.end();
    fluid.setUseObstacles(false);
    
    //FOR POSTGLITCH
    GlitchFBO.allocate(ofGetWidth(), ofGetHeight());
    postGlitch.setup(&GlitchFBO);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    kinect.update();
    if (kinect.isFrameNew()) {
        irTexture.loadData(kinect.getIrPixelsRef());
    }
    
    //FOR FIRE
    fluid.update();

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
        //IRカメラの描画
        if(IRCamera){
            myCvImage.draw(0.0, 0.0, ofGetWidth(), ofGetHeight());
        }
        
        
        //openCV
        contourFinder.setTargetColor(targetColor, trackHs ? TRACK_COLOR_HS : TRACK_COLOR_RGB);
        contourFinder.setThreshold(threshold); // 閾値を設定
        cv::Mat imgMat = ofxCv::toCv(myCvImage);
        contourFinder.findContours(imgMat); //ビデオカメラから輪郭を検出
        
        //用意したoldMの要素数
        int oldMsize = sizeof(oldM)/sizeof(oldM[0]);
        
        //着火
        if(contourFinder.size() < oldMsize){
            for(int i = 0; i < contourFinder.size() ; i++){
                //デバッグ用の円
                if(circle){
                    ofDrawCircle(ofxCv::toOf(contourFinder.getCenter(i)),20);
                    //cout << contourFinder.getCenter(i)<<endl;
                }
                int x = contourFinder.getCenter(i).x;
                int y = contourFinder.getCenter(i).y;
                //FOR FIRE
                ofPoint m = ofPoint(x,y);
                ofPoint d = (m - oldM[i])*10.0;
                oldM[i] = m;
                ofPoint c = ofPoint(640*0.5, 480*0.5) - m;
                c.normalize();
                
                fluid.addTemporalForce(m, d, ofFloatColor(0.5,0.1,0.0),4.0f);
            }
        }else{
            for(int i = 0; i < oldMsize ; i++){
                //デバッグ用の円
                if(circle){
                    ofDrawCircle(ofxCv::toOf(contourFinder.getCenter(i)),20);
                    //cout << contourFinder.getCenter(i)<<endl;
                }
                int x = contourFinder.getCenter(i).x;
                int y = contourFinder.getCenter(i).y;
                //FOR FIRE
                ofPoint m = ofPoint(x,y);
                ofPoint d = (m - oldM[i])*10.0;
                oldM[i] = m;
                ofPoint c = ofPoint(640*0.5, 480*0.5) - m;
                c.normalize();
                
                fluid.addTemporalForce(m, d, ofFloatColor(0.5,0.1,0.0),4.0f);
            }
            
        }
        
      //  contourFinder.draw();
        GlitchFBO.begin();
        fluid.draw();
        GlitchFBO.end();
        if(release){
            postGlitch.generateFx();
        }
    }
    GlitchFBO.draw(0,0);
    
    ofDrawBitmapStringHighlight("fps: " + ofToString(ofGetFrameRate()), ofGetWidth() - 120, 20);
    mainOutputSyphonServer.publishScreen();
    gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    
    if (key == '1') postGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE    , true);
    if (key == '2') postGlitch.setFx(OFXPOSTGLITCH_GLOW            , true);
    if (key == '3') postGlitch.setFx(OFXPOSTGLITCH_SHAKER            , true);
    if (key == '4') postGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER        , true);
    if (key == '5') postGlitch.setFx(OFXPOSTGLITCH_TWIST            , true);
    if (key == '6') postGlitch.setFx(OFXPOSTGLITCH_OUTLINE        , true);
    if (key == '7') postGlitch.setFx(OFXPOSTGLITCH_NOISE            , true);
    if (key == '8') postGlitch.setFx(OFXPOSTGLITCH_SLITSCAN        , true);
    if (key == '9') postGlitch.setFx(OFXPOSTGLITCH_SWELL            , true);
    if (key == '0') postGlitch.setFx(OFXPOSTGLITCH_INVERT            , true);
    
    if (key == 'q') postGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    if (key == 'w') postGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE    , true);
    if (key == 'e') postGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE    , true);
    if (key == 'r') postGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE    , true);
    if (key == 't') postGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT    , true);
    if (key == 'y') postGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT    , true);
    if (key == 'u') postGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT    , true);
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == '1') postGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE    , false);
    if (key == '2') postGlitch.setFx(OFXPOSTGLITCH_GLOW            , false);
    if (key == '3') postGlitch.setFx(OFXPOSTGLITCH_SHAKER            , false);
    if (key == '4') postGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER        , false);
    if (key == '5') postGlitch.setFx(OFXPOSTGLITCH_TWIST            , false);
    if (key == '6') postGlitch.setFx(OFXPOSTGLITCH_OUTLINE        , false);
    if (key == '7') postGlitch.setFx(OFXPOSTGLITCH_NOISE            , false);
    if (key == '8') postGlitch.setFx(OFXPOSTGLITCH_SLITSCAN        , false);
    if (key == '9') postGlitch.setFx(OFXPOSTGLITCH_SWELL            , false);
    if (key == '0') postGlitch.setFx(OFXPOSTGLITCH_INVERT            , false);
    
    if (key == 'q') postGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    if (key == 'w') postGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE    , false);
    if (key == 'e') postGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE    , false);
    if (key == 'r') postGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE    , false);
    if (key == 't') postGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT    , false);
    if (key == 'y') postGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT    , false);
    if (key == 'u') postGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT    , false);
    
    //ALL reset
    if (key == 'z'){
        postGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE    , false);
        postGlitch.setFx(OFXPOSTGLITCH_GLOW            , false);
        postGlitch.setFx(OFXPOSTGLITCH_SHAKER            , false);
        postGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER        , false);
        postGlitch.setFx(OFXPOSTGLITCH_TWIST            , false);
        postGlitch.setFx(OFXPOSTGLITCH_OUTLINE        , false);
        postGlitch.setFx(OFXPOSTGLITCH_NOISE            , false);
        postGlitch.setFx(OFXPOSTGLITCH_SLITSCAN        , false);
        postGlitch.setFx(OFXPOSTGLITCH_SWELL            , false);
        postGlitch.setFx(OFXPOSTGLITCH_INVERT            , false);
        
        postGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
        postGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE    , false);
        postGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE    , false);
        postGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE    , false);
        postGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT    , false);
        postGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT    , false);
        postGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT    , false);
        
        release = false;
        
    }
    
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

