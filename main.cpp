#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

#include "parser.h"
#include "graph.h"

using namespace cv;
using std::cout;
using std::endl;
using std::string;

namespace Mouse {
    bool pressed = false, clicking = false, dragging = false;
    int x = -1, y = -1;

    void callback(int event, int x, int y, int flags, void* userdata){
        Mouse::x = x;
        Mouse::y = y;
        switch (event){
            case EVENT_LBUTTONDOWN:
                Mouse::pressed = true;
                break;
            case EVENT_LBUTTONUP:
                Mouse::pressed = false;
                Mouse::clicking = true;
                Mouse::dragging = false;
                break;
            case EVENT_MOUSEMOVE:
                Mouse::dragging = Mouse::pressed;
                break;
            default:
                break;
        }
    }
};

int main( int argc, char** argv ) {
    // Get command-line options
    Settings settings = parseOptions(argc, argv);

    // Read image from file
    Mat img = imread(settings.image_path);
    if (img.empty()){
        cout << "Error reading image from: " << settings.image_path << endl;
        return 1;
    }

    // Load graph from file if necessary
    Graph graph;
    if (settings.load){
        std::ifstream input_file(settings.graph_path);
        graph.load(input_file);
        input_file.close();
        cout << "Loaded " << graph.numNodes() << " nodes and ";
        cout << graph.numEdges() << " edges from " << settings.graph_path << endl;
    }

    // Create window with callback and draw image
    string winname = "My Window";
    namedWindow(winname);
    setMouseCallback(winname, Mouse::callback);

    // Draw all the initial nodes
    graph.drawNodes(img);
    graph.drawEdges(img);
    imshow(winname, img);
    bool changed = false;

    // Some variables used for CREATE_EDGES mode
    bool pt_picked = false;
    Point pt1;

    // Exit when user presses 'q' key
    while (true){
        if (Mouse::clicking && settings.mode == Mode::CREATE_NODES){
            graph.addNode(Mouse::x, Mouse::y);
            graph.drawNode(img, Mouse::x, Mouse::y);
            imshow(winname, img);
            changed = true;
        } else if (Mouse::clicking && settings.mode == Mode::CREATE_EDGES){
            if (!pt_picked){ // Need to pick starting point
                pt1 = Point(Mouse::x, Mouse::y);
                pt_picked = true;
            } else { // Need to pick ending point
                Point pt2 = Point(Mouse::x, Mouse::y);
                graph.addEdge(pt1, pt2);
                graph.drawEdge(img, graph.getClosestNode(pt1), graph.getClosestNode(pt2));
                pt_picked = false;
                changed = true;
                imshow(winname, img);
            }
        }

        Mouse::clicking = false;
        if (waitKey(5) == 'q') break;
    }

    if (settings.save){
        if (!changed){
            cout << "No changes made." << endl;
            return 0;
        }
        std::ofstream output_file(settings.graph_path);
        graph.save(output_file);
        output_file.close();
        cout << "Saved " << graph.numNodes() << " nodes and ";
        cout << graph.numEdges() << " edges to " << settings.graph_path << endl;
    }

    return 0;
}