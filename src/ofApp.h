#pragma once

#include "ofMain.h"
#include "ofxMatrixEigen.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
    private:
        void initPaper(float paper[][100]);
        void moveWater();
        void movePigment();
        void transferPigment();
        void simulateCapillaryFlow();
        void updateVelocities();
        void relaxDivergence();
        void flowOutward();
        void enforceBoundaryConditions();
        float getMax(float items[][101]);
        float** getGaussian(int width, int height, int K);
    
        const int PAPER_WIDTH = 100;
        const int PAPER_HEIGHT = 100;
        const int NUM_PIGMENTS = 10;
    
        const float VISCOSITY = 0.1;
        const float VISCOUS_DRAG = 0.01;
    
        Eigen::Matrix<int, 100, 100> M;
        float h[100][100];
    
        // staggered grid--size = (m-1)*(n-1)*4 - ((m-2)*(n-1) + (n-2)*(m-1)) + 2*m + 2*n
        float u[101][101]; // x velocities
        float v[101][101]; // y velocities
    
        float p[100][100]; // water pressure
        float g[10][100][100]; // shallow pigment 
        float d[10][100][100]; // deposited pigment
};
