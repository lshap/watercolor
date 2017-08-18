#include "ofApp.h"

static bool abs_compare(int a, int b)
{
    return (std::abs(a) < std::abs(b));
}

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
    float maxVelocity = fmax(getMax(u), getMax(v));
    float deltaT = 1.0/maxVelocity;
    for (int k=0; k < NUM_PIGMENTS; k++) {
        for (float t= 0; t < 1; t += deltaT) {
            float g_temp[PAPER_WIDTH][PAPER_HEIGHT];
            memcpy(g_temp, g[k], sizeof(float) * PAPER_WIDTH * PAPER_HEIGHT);
            for (int i =0; i < PAPER_WIDTH; i++) {
                for (int j =0; j < PAPER_HEIGHT; j++) {
                    if (i < PAPER_WIDTH - 1) {
                        g_temp[i+1][j] += fmax(0, u[i+1][j]*g[k][i][j]);
                    }
                    if (i > 0) {
                        g_temp[i-1][j] += fmax(0, -u[i][j] * g[k][i][j]);
                    }
                    if (j < PAPER_HEIGHT - 1) {
                        g_temp[i][j+1] += fmax(0, v[i][j+1] * g[k][i][j]);
                    }
                    if (j > 0) {
                        g_temp[i][j-1] += fmax(0, -v[i][j] * g[k][i][j]);
                    }
                    
                    g_temp[i][j] -= fmax(0, u[i+1][j] * g[k][i][j]) + fmax(0, -u[i][j] * g[k][i][j]) + fmax(0, v[i][j+1] * g[k][i][j]) + fmax(0, -v[i][j] * g[k][i][j]);
                }
            }
            
            memcpy(g[k], g_temp, sizeof(float) * PAPER_WIDTH * PAPER_HEIGHT);
        }
    }
}

void ofApp::transferPigment() {

}

void ofApp::simulateCapillaryFlow() {
    
}

float ofApp::getMax(float items[][101]) {
    float max = std::numeric_limits<float>::min();
    for (int i=0; i < PAPER_WIDTH; i++ ) {
        max = fmax(max, *std::max_element(items[i], items[i] + 101, abs_compare));
    }
    return max;
}

void ofApp::enforceBoundaryConditions() {
    for (int i = 0; i < PAPER_WIDTH - 1; i++) {
        for (int j = 0; j < PAPER_HEIGHT - 1; j++) {
            if (M(i, j)  == 0) {
                u[i][j] = 0;
                u[i + 1][j] = 0;
                v[i][j] = 0;
                v[i][j+ 1] = 0;
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
        for (int i = 0; i < PAPER_WIDTH; i++) {
            for (int j = 0; j < PAPER_HEIGHT; j++) {
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
    int MAX_ITER = 50;
    float tolerance = 0.01;
    float xi = 0.1;
    int t = 0;
    float d_max = 0;
    do {
        float u_temp[PAPER_WIDTH][PAPER_HEIGHT];
        float v_temp[PAPER_WIDTH][PAPER_HEIGHT];
        memcpy(u_temp, u, sizeof(float)*PAPER_WIDTH*PAPER_HEIGHT);
        memcpy(v_temp, v, sizeof(float)*PAPER_WIDTH*PAPER_HEIGHT);
        d_max = 0;
        for (int i = 0; i < PAPER_WIDTH; i++) {
            for (int j = 0; j < PAPER_HEIGHT; j++) {
                float d = xi * (u[i+1][j] - u[i][j] + v[i][j+1] - v[i][j]);
                p[i][j] += d;
                u_temp[i+1][j] += d;
                u_temp[i][j] -= d;
                v_temp[i+1][j] += d;
                v_temp[i][j] -= d;
                d_max = fmax(abs(d), d_max);
            }
        }
        memcpy(u, u_temp, sizeof(float)*PAPER_WIDTH*PAPER_HEIGHT);
        memcpy(v, v_temp, sizeof(float)*PAPER_WIDTH*PAPER_HEIGHT);
        t += 1;
    } while (d_max > tolerance && t < MAX_ITER);
}

float** ofApp::getGaussian(int width, int height, int K) {
    
}

void ofApp::flowOutward() {

}

void ofApp::moveWater() {
    updateVelocities();
    relaxDivergence();
    flowOutward();
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
