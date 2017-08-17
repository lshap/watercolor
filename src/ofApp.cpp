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

float ofApp::getMax(float items[][100]) {
    float max = std::numeric_limits<float>::min();
    for (int i=0; i < PAPER_WIDTH; i++ ) {
        max = fmax(max, *std::max_element(items[i], items[i] + 100));
    }
    return max;
}

void ofApp::enforceBoundaryConditions() {
    for (int i = 0; i < PAPER_WIDTH; i++) {
        for (int j = 0; j < PAPER_HEIGHT; j++) {
            if (M[i][j]  == 0) {
                u[i][j] = 0;
                if ( i < PAPER_WIDTH - 1) {
                    u[i + 1][j] = 0;
                }
                v[i][j] = 0;
                if ( j < PAPER_HEIGHT - 1) {
                    v[i][j+ 1] = 0;
                }
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
            // TODO: figure out which should be updated
            u[i][j] -= dh;
            v[i][j] -= dh;
        }
    }
    
    float maxVelocity = fmax(getMax(u), getMax(v));
    float deltaT = 1.0/maxVelocity;
    float t = 0;
    float u_temp[PAPER_WIDTH][PAPER_HEIGHT];
    float v_temp[PAPER_WIDTH][PAPER_HEIGHT];
    while (t < 1) {
        for (int i = 0; i < PAPER_WIDTH - 1; i++) {
            for (int j = 0; j < PAPER_HEIGHT - 1; j++) {
                float u_i_j = (u[i][j] + u[i + 1][j])/2;
                float u_i_1_j = (i < PAPER_WIDTH - 2) ? (u[i+1][j] + u[i+2][j])/2 : 0;
                float A = u_i_j * u_i_j - u_i_1_j * u_i_1_j + u[i+1][j]*v[i][j] - u[i+1][j]*v[i][j+1];
                float B = u[i][j] + u[i+1][j+1] - 4 * u[i+1][j];
                if (i < PAPER_WIDTH - 2) {
                    B += u[i+2][j];
                }
                if (j > 0) {
                    B += u[i+1][j-1];
                }
                u_temp[i+1][j] = u[i+1][j] + deltaT * (A - VISCOSITY * B + p[i][j] - p[i+1][j] - VISCOUS_DRAG * u[i+1][j]);
                float v_i_j = (v[i][j] + v[i][j + 1])/2;
                float v_i_1_j = (j < PAPER_HEIGHT - 2) ? (v[i][j+1] + v[i][j+2])/2 : 0;
                A = v_i_j * v_i_j - v_i_1_j * v_i_1_j + u[i+1][j]*v[i][j] - u[i+1][j]*v[i][j+1];
                B = v[i+1][j+1] + v[i][j] -4* v[i][j+1];
                if (j < PAPER_HEIGHT - 2) {
                    B += v[i][j+2];
                }
                if (i > 0) {
                    B += v[i-1][j+1];
                }
                v_temp[i][j+1] = v[i][j+1] + deltaT *(A - VISCOSITY * B + p[i][j] - p[i+1][j] - VISCOUS_DRAG * v[i][j+1]);
            }
        }
        t += deltaT;
    }
    memcpy(u, u_temp, sizeof(float) * PAPER_WIDTH * PAPER_HEIGHT);
    memcpy(v, v_temp, sizeof(float) * PAPER_WIDTH * PAPER_HEIGHT);
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
