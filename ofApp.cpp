#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp() {
	for (ofxBulletBox* tmp : this->boxes) {
		delete tmp;
	}
	this->boxes.clear();
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofEnableDepthTest();
	ofBackground(0);
	ofSetWindowTitle("Insta");

	this->world.setup();
	this->world.setGravity(ofVec3f(0.0, 0.0, 0.0));

	float size = 15;

	for (float x = -100; x < 100; x += size) {
		for (float y = -100; y < 100; y += size) {
			for (float z = -100; z < 100; z += size) {
				ofxBulletBox* box = new ofxBulletBox();
				box->create(this->world.world, ofVec3f(x, y, z), 1.0, size, size, size);
				box->setRestitution(1.0);
				box->add();

				this->boxes.push_back(box);

				ofColor color;
				color.setHsb(ofRandom(255), 255, 255);
				this->bxes_color.push_back(color);
				this->boxes_location.push_back(ofVec3f(x, y, z));
			}
		}
	}

	this->light.setPosition(ofVec3f(0, 0, 512));
	ofEnableLighting();
	this->light.enable();
}

//--------------------------------------------------------------
void ofApp::update() {

	ofVec3f diff;
	for (int i = 0; i < this->boxes.size(); i++) {
		diff = this->boxes_location[i] - this->boxes[i]->getPosition();
		diff *= 20;
		this->boxes[i]->applyCentralForce(diff);
	}

	float x = ofMap(ofNoise(ofGetFrameNum() * 0.01), 0, 1, -1000, 1000);
	float y = ofMap(ofNoise((ofGetFrameNum() + 100) * 0.01), 0, 1, -1000, 1000);
	float z = ofMap(ofNoise((ofGetFrameNum() + 200) * 0.01), 0, 1, -1000, 1000);

	this->hater_location = ofVec3f(x, y, z);
	for (int i = 0; i < this->boxes.size(); i++) {
		diff = this->boxes[i]->getPosition() - this->hater_location;
		if (diff.length() < 150) {
			diff *= ofMap(diff.length(), 0, 150, -300, 10);
			this->boxes[i]->applyCentralForce(diff);
		}
	}

	this->world.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (int i = 0; i < this->boxes.size(); i++) {
		ofSetColor(this->bxes_color[i]);
		this->boxes[i]->draw();
		//ofDrawLine(this->boxes[i]->getPosition(), this->boxes_location[i]);
	}

	ofSetColor(200);
	ofDrawSphere(this->hater_location, 30);

	this->cam.end();
}

//========================================================================
int main() {
	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}