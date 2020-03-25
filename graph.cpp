#include "graph.h"
#include <utility>
#include <algorithm>
#include <opencv2/opencv.hpp>

#define GRAPH_MAT_TYPE CV_32SC1

using namespace cv;

void printMat(std::ostream &os, const Mat &mat){
	for (int i = 0; i < mat.rows; ++i){
		for (int j = 0; j < mat.cols; ++j){
			os << mat.at<int>(i,j) << ' ';
		}
		os << '\n';
	}
}

Graph::Graph(){

}

Graph::Graph(int _num_nodes) : num_nodes(_num_nodes) {
	node_locations.reserve(num_nodes);
	adjacency_matrix = Mat::zeros(num_nodes, num_nodes, GRAPH_MAT_TYPE);
}

Graph::Graph(std::istream &is){
	load(is);
}

void Graph::load(std::istream &is){
	is >> num_nodes;
	node_locations.reserve(num_nodes);
	adjacency_matrix = Mat::zeros(num_nodes, num_nodes, GRAPH_MAT_TYPE);

	for (int i = 0; i < num_nodes; ++i){
		int x, y;
		is >> x >> y;
		node_locations.emplace_back(x,y);
	}

	for (int i = 0; i < num_nodes; ++i){
		for (int j = 0; j < num_nodes; ++j){
			int val;
			is >> val;
			adjacency_matrix.at<int>(i, j) = val;
		}
	}
}

void Graph::save(std::ostream &os){
	os << num_nodes << '\n' << '\n';
	for (Point2i &p : node_locations){
		os << p.x << ' ' << p.y << '\n';
	}
	os << '\n';
	if (adjacency_matrix.rows != num_nodes) resizeAdjacencyMatrix();
	printMat(os, adjacency_matrix);
}

int Graph::numNodes(){
	return num_nodes;
}

int Graph::numEdges(){
	return num_edges;
}

std::vector<Point2i> & Graph::getNodes(){
	return node_locations;
}

void Graph::addNode(Point2i pt){
	node_locations.push_back(pt);
	++num_nodes;
}

void Graph::addNode(int x, int y){
	node_locations.emplace_back(x, y);
	++num_nodes;
}

void Graph::resizeAdjacencyMatrix(){
	Mat mat2 = Mat::zeros(num_nodes, num_nodes, GRAPH_MAT_TYPE);
	Rect roi(Point(0,0), adjacency_matrix.size()); // Create ROI on the big image
	adjacency_matrix.copyTo(mat2(roi));
	std::swap(adjacency_matrix, mat2);
	//adjacency_matrix = mat2;
}

void Graph::addEdge(int node1, int node2){
	if (adjacency_matrix.rows != num_nodes) resizeAdjacencyMatrix();
	adjacency_matrix.at<int>(Point(node1, node2)) = 1;
	adjacency_matrix.at<int>(Point(node2, node1)) = 1;
	++num_edges;
}

void Graph::addEdge(Point2i pt1, Point2i pt2){
	addEdge(getClosestNodeIndex(pt1), getClosestNodeIndex(pt2));
}

class ClosestToComparator {
private:
	Point2i ref;
	int squared_dist(const Point2i &p) const {
		int dx = p.x - ref.x;
		int dy = p.y - ref.y;
		return dx*dx + dy*dy;
	}
public:
	ClosestToComparator(Point2i _ref) : ref(_ref) {}
	bool operator()(const Point2i &p1, const Point2i &p2) const {
		return squared_dist(p1) < squared_dist(p2);
	}
};

int Graph::getClosestNodeIndex(Point pt){
	return std::distance(
		node_locations.begin(),
		std::min_element(
			node_locations.begin(),
			node_locations.end(),
			ClosestToComparator(pt)
		)
	);
}

int Graph::getClosestNodeIndex(int x, int y){
	return getClosestNodeIndex(Point(x, y));
}

Point2i & Graph::getClosestNode(Point pt){
	return *std::min_element(
		node_locations.begin(),
		node_locations.end(),
		ClosestToComparator(pt)
	);
}

Point2i & Graph::getClosestNode(int x, int y){
	return getClosestNode(Point(x, y));
}

void Graph::drawNode(Mat &img, Point2i pt){
	circle(img, pt, 3, Scalar(0,0,255), -1);
}

void Graph::drawNode(Mat &img, int x, int y){
	drawNode(img, Point(x,y));
}

void Graph::drawEdge(Mat &img, Point2i pt1, Point2i pt2){
	line(img, pt1, pt2, Scalar(255,0,0), 2);
}

void Graph::drawEdge(Mat &img, int x1, int y1, int x2, int y2){
	drawEdge(img, Point(x1,y1), Point(x2,y2));
}

void Graph::drawNodes(Mat &img){
	for (Point2i &p : node_locations) drawNode(img, p);
}

void Graph::drawEdges(Mat &img){
	for (int i = 0; i < adjacency_matrix.rows; ++i){
		for (int j = 0; j < i; ++j){
			if (adjacency_matrix.at<int>(i,j) > 0){
				++num_edges;
				drawEdge(img, node_locations[i], node_locations[j]);
			}
		}
	}
}

/*
 * References
 * https://www.softwaretestinghelp.com/graph-implementation-cpp/
 * https://www.tutorialspoint.com/cplusplus-program-to-implement-adjacency-matrix
 */