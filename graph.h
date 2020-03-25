#pragma once

#include <vector>
#include <iostream>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

class Graph {
private:
    int num_nodes;
    int num_edges;
    std::vector<cv::Point2i> node_locations;
    cv::Mat adjacency_matrix;

    void resizeAdjacencyMatrix();
public:
    // Constructors
    Graph();
    Graph(int _num_nodes);
    Graph(std::istream &is);

    // Getter functions
    int numNodes();
    int numEdges();
    std::vector<cv::Point2i> & getNodes();

    // User functions
    void addNode(cv::Point2i pt);
    void addNode(int x, int y);

    void addEdge(int node1, int node2);
    void addEdge(cv::Point2i pt1, cv::Point2i pt2);

    void load(std::istream &is);
    void save(std::ostream &os);

    int getClosestNodeIndex(cv::Point pt);
    int getClosestNodeIndex(int x, int y);
    cv::Point2i & getClosestNode(cv::Point pt);
    cv::Point2i & getClosestNode(int x, int y);

    // Drawing functions
    void drawNode(cv::Mat &img, cv::Point2i pt);
    void drawNode(cv::Mat &img, int x, int y);
    void drawEdge(cv::Mat &img, cv::Point2i pt1, cv::Point2i pt2);
    void drawEdge(cv::Mat &img, int x1, int y1, int x2, int y2);
    void drawNodes(cv::Mat &img);
    void drawEdges(cv::Mat &img);
};