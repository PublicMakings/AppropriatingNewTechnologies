#include "testApp.h"

extern "C" {
#include "macGlutfix.h"
}

//--------------------------------------------------------------
void testApp::setup(){

    
    captureWidth = ofGetWidth();
    captureHeight = ofGetHeight();
    
    // try to grab web cam at this size
    camWidth = 400;
    camHeight = 300;
    
    // get ready to grab the video at the desired size.
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);
    
    // I don't know what this line does, just copying it from the movieGrabberExample
    videoTexture.allocate(camWidth, camHeight, GL_RGB);
    
    // setup the Haar face-finder
	finder.setup("haarcascade_frontalface_default.xml");

    // why not?
//    ofSetVerticalSync(true);
	
    image.allocate(captureWidth, captureHeight, OF_IMAGE_COLOR);
	//pixels.allocate(captureWidth, captureHeight, OF_IMAGE_COLOR);
	
    blameText.loadImage("blame.png");
	ofSetFrameRate(30);

}

//--------------------------------------------------------------
void testApp::update(){
	
    captureWidth = ofGetWidth();
    captureHeight = ofGetHeight();
    
    vidGrabber.grabFrame();
	
    if (vidGrabber.isFrameNew()){
		unsigned char * pixels = vidGrabber.getPixels();
        // now, let's get the R and B data swapped, so that it's all OK:
        image.setFromPixels(pixels, camWidth, camHeight, OF_IMAGE_COLOR, true);
        image.update();
        // send this frame of video through haar detection
        finder.findHaarObjects(image);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
    image.draw(0,0);

    ofNoFill();
    
	// for each face "blob" we found, draw a rectangle around the face
	for(int i = 0; i < finder.blobs.size(); i++) {

        ofRectangle face = finder.blobs[i].boundingRect;
        
        ofSetColor(255, 0, 0);
        ofFill();

        ofNoFill();
        ofSetColor(255,255,255);

        float blamex;
        float blamey;
        int padding = 10;

        if(face.x + face.width/2 < ofGetWidth()/2) {
            blamex = face.x + face.width + padding; }
        else {
            blamex = face.x - blameText.width - padding; }
        
        if(face.y + face.height/2 < ofGetHeight()/2) {
            blamey = face.y; }
        else {
            blamey = face.y - blameText.height; }
        
        ofEnableAlphaBlending();
        blameText.draw(blamex, blamey);
        ofDisableAlphaBlending();
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}