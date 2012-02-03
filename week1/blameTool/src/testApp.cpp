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
    ofSetVerticalSync(true);
	
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
        image.setImageType(OF_IMAGE_COLOR);
        image.update();
        finder.findHaarObjects(image);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
    image.draw(0,0);

    ofNoFill();
    
	//for each face "blob" we found, draw a rectangle around the face
	for(int i = 0; i < finder.blobs.size(); i++) {

        ofRectangle face = finder.blobs[i].boundingRect;
        
        ofRect(face);
        
        ofColor blame;
        blame.r = 255;
        blame.b = 0;
        blame.g = 0;
        
        ofSetColor(blame.r, blame.g, blame.b);
        ofFill();
        ofEllipse(face.x, face.y, 10, 10);
        ofNoFill();
        ofSetColor(255,255,255);
        
        ofEnableAlphaBlending();
        blameText.draw(face.x - blameText.width, face.y);
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