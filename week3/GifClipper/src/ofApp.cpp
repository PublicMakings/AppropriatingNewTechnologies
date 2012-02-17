#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	    
    // These two gifs are the same size and same number of frames.
	ofVideoPlayer mask, content;
    mask.loadMovie("cc3-square.gif");
    content.loadMovie("stars.gif");
    
    // Just to make sure...
	int n = mask.getTotalNumFrames();
    int m = content.getTotalNumFrames();
    
    int maxFrames = n;
    if(n - m > 0) {
        maxFrames = m;
    }
    
	for(int i = 0; i < maxFrames; i++) {
        // This command gets the ith frame of the gif/movie and
        // makes it accessible by calling <video>.getPixelsRef()
		mask.setFrame(i);
        content.setFrame(i);

        // Add a reference to the pixels of the current frame to
        // the end of each vector of images. 
        displaySlices.push_back(content.getPixelsRef());
        clippingSlices.push_back(mask.getPixelsRef());
		
        // Convert the image type of the last element of the vector
        // to include an alpha channel.
        // I don't need to do this to the clipping mask as its 
        // brightness levels will set the alpha channel on the display
        // slices.
		displaySlices.back().setImageType(OF_IMAGE_COLOR_ALPHA); // RGB -> RGBA
        
		ofPixels& pix = displaySlices.back().getPixelsRef();
        ofPixels& clips = clippingSlices.back().getPixelsRef();
        
		for(int y = 0; y < pix.getHeight(); y++) {
			for(int x = 0; x < pix.getWidth(); x++) {
				ofColor cur = pix.getColor(x, y);
                ofColor clip = clips.getColor(x, y);

                cur.a = cur.getBrightness();
                if( clip.getBrightness() == 255 ) {
                    cur.a = 0;
                }
				pix.setColor(x, y, cur);
			}
		}
		// update the ofImage (upload ofPixels to ofTexture)
		displaySlices.back().update();
	}
	ofEnableAlphaBlending();
}  

void ofApp::update() {
}

void ofApp::draw() {
	ofBackground(0);
	cam.begin();
	ofSetColor(255, 64); // make everything more transparent
	float spacing = 8;
	ofTranslate(-displaySlices[0].getWidth() / 2, -displaySlices[0].getHeight() / 2, -spacing * displaySlices.size() / 2); // center things
	for(int i = 0; i < displaySlices.size(); i++) {
		ofPushMatrix();
		
		// space the slices out evenly on the z axis
		ofTranslate(0, 0, i * spacing);
		
		// jiggle things
		float noiseOffset = ofGetElapsedTimef() + i;
		float noiseScale = 10;
		ofTranslate(ofSignedNoise(noiseOffset, 0) * noiseScale, ofSignedNoise(0, noiseOffset) * noiseScale, 0);
		
		displaySlices[i].draw(0, 0);
		ofPopMatrix();
	}
	cam.end();
}
