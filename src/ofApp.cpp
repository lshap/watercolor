#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    initPaper(h);
    /*for (int i =0; i < PAPER_WIDTH; i++) {
        for (int j = 0; j < PAPER_HEIGHT; j++) {
            cout << h[i][j] << endl;
        }
    }*/
}

//--------------------------------------------------------------
void ofApp::update(){

}


//--------------BEGIN WATERCOLOR METHODS------------------------

void ofApp::initPaper(float paper[][100]) {
    for (int i =0; i < PAPER_WIDTH; i++) {
        for (int j = 0; j < PAPER_HEIGHT; j++) {
            paper[i][j] = float(rand() % 100)/100.0f;
        }
    }
}

void ofApp::movePigment() {
    
}

void ofApp::transferPigment() {

}

void ofApp::simulateCapillaryFlow() {
    
}

float ofApp::getMax(GridItem items[][100]) {
    float max = std::numeric_limits<float>::min();
    for (int i=0; i < PAPER_WIDTH; i++ ) {
        for (int j =0; j < PAPER_HEIGHT; j++) {
            GridItem item = items[i][j];
            max = fmax(max, item.top);
            max = fmax(max, item.bottom);
            max = fmax(max, item.left);
            max = fmax(max, item.right);
        }
    }
    return max;
}

void ofApp::enforceBoundaryConditions() {
    for (int i = 0; i < PAPER_WIDTH; i++) {
        for (int j = 0; j < PAPER_HEIGHT; j++) {
            if (M[i][j]  == 0) {
                u[i][j].top = 0;
                u[i][j].bottom = 0;
                u[i][j].left = 0;
                u[i][j].right = 0;
                
                v[i][j].top = 0;
                v[i][j].bottom = 0;
                v[i][j].left = 0;
                v[i][j].right = 0;
            }
        }
    }
}

void ofApp::updateVelocities() {
    // (u, v) <-- (u, v) - grad(h)
    for (int i=0; i < PAPER_WIDTH; i++) {
        for (int j=0; j < PAPER_HEIGHT; j++) {
            float height_diffs[4];
            // left
            height_diffs[0] = (i > 0) ? h[i][j] - h[i - 1][j] : 0.0f;
            // right
            height_diffs[1] = (i < PAPER_WIDTH - 1) ? h[i][j] - h[i + 1][j] : 0.0f;
            // above
            height_diffs[2] = (j > 0) ? h[i][j] - h[i][j - 1] : 0.0f;
            // below
            height_diffs[3] = (j < PAPER_HEIGHT - 1) ? h[i][j] - h[i][j + 1] : 0.0f;
            
            float dh = *std::max_element(height_diffs, height_diffs + 4);
            u[i][j].top -= dh;
            u[i][j].bottom -= dh;
            u[i][j].left -= dh;
            u[i][j].right -= dh;
            
            v[i][j].top -= dh;
            v[i][j].bottom -= dh;
            v[i][j].left -= dh;
            v[i][j].right -= dh;
        }
    }
    
    //float maxVelocity = fmax(getMax(u), getMax(v));
    //float deltaT = 1.0/maxVelocity;
    float t = 0;
    float u_temp[PAPER_WIDTH][PAPER_HEIGHT];
    float v_temp[PAPER_WIDTH][PAPER_HEIGHT];
    while (t < 1) {
        //t += deltaT;
    }
    
    enforceBoundaryConditions();
}

void ofApp::relaxDivergence() {
    
}

void ofApp::flowOutward() {

}

void ofApp::moveWater() {

}

//----------------END WATERCOLOR METHODS------------------------

//--------------------------------------------------------------
void ofApp::draw(){

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
