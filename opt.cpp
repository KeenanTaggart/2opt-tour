/*********************************************************************                                                                                                               
 *** Author: Keenan Taggart
 *** Date: 2017
 *** Program: opt.cpp 
 *** Description: An implementation of the 2opt algorithm in order to 
 *** approximate the Traveling Salesman problem. Nearest Neighbor
 *** algorithm is used to generate a rudimentary route very quickly
 *** which is then optimized with 2opt. 
 **********************************************************************/

#include <fstream>
#include <iostream>
#include <vector>
#include <climits>
#include <cstring>
#include <set>
#include <cmath>
#include <algorithm>		                            


// params: the complete tour (tour) and the 2D vector of cities and their coordinates (c)
// Arguments passed by reference to avoid copying overhead
int tourDistance(std::vector<int> &tour, std::vector<std::vector<int> > &c) {	
  int distance = 0;
  std::vector<int>::iterator it = tour.begin();
  for (; it != tour.end() - 1; it++) {
    distance += round(sqrt(pow(((double)c[*it][0] - (double)c[*(it+1)][0]), 2.0) + pow(((double)c[*it][1] - (double)c[*(it+1)][1]), 2.0)));
  }
  // Return trip
  distance += round(sqrt(pow(((double)c[*it][0] - (double)c[*(tour.begin())][0]), 2.0) + pow(((double)c[*it][1] - (double)c[*(tour.begin())][1]), 2.0)));  
  return distance;
}

// params: the complete tour (tour) and the 2D vector of cities and their coordinates (c)
void opt2(std::vector<int> &tour, std::vector<std::vector<int> > &c) {
  int improvement = 0;
  int standard = 10;
  if (c.size() > 5000) {
    standard = 2;
  }
  while (improvement < standard) {
    int bestDistance = tourDistance(tour, c);
    int margin = 1;
    if (c.size() > 5000) {
      margin = 9;
    }
    for (int i = 1; i < tour.size() - 1; i += margin) {
      if (i % 10 == 0) {
        std::cout << "***** i is at " << i << " *****" << std::endl;
      }
      for (int k = i + 1; k < tour.size(); k += margin) {
	std::vector<int> newTour = tour;
	std::reverse(newTour.begin() + i, newTour.begin() + k + 1);		                              // Righthand bound of reverse() is exclusive, so we add 1 to k
	int newDistance = tourDistance(newTour, c);
	if (newDistance < bestDistance) {
	  improvement = 0;
	  tour = newTour;
	  bestDistance = newDistance;
	}
      }
    }
    improvement++;
  }
}

int main(int argc, char* argv[]) {

  if (argc != 2) {
    std::cout << "Usage: \"" << argv[0] << "  filename\"" << std::endl;
    return 0;
  }

  std::ifstream inputFile;
  inputFile.open(argv[1]);
  if (!inputFile) {
    std::cout << "File \"" << argv[1] << "\" not found!" << std::endl;
    return 0;
  }

  std::ofstream outputFile;
  char* outName = argv[1];
  std::strcat(outName, ".tour");
  outputFile.open(outName);
  
  std::vector<std::vector<int> > cities;
  int cityNum, xcoord, ycoord;
  while (inputFile >> cityNum) {						                                                  // cityNum is not used, but this conveniently skips over it while checking whether we're done
    inputFile >> xcoord;
    inputFile >> ycoord;
    std::vector<int> newCity;
    newCity.push_back(xcoord);
    newCity.push_back(ycoord);
    cities.push_back(newCity);
  } 

  std::set<int> not_visited;
  std::vector<int> visited;							                                                      // Holds the cities in the order in which they're visited
  for (int i = 0; i < cities.size(); i++) {					                                          // Populate 'not_visited' with all cities
    not_visited.emplace(i);
  }  

  not_visited.erase(0);								                                                        // Arbitrarily start with city 0
  visited.push_back(0);
  int currentXCoord, currentYCoord, distanceTraveled;
  currentXCoord = cities[0][0];	
  currentYCoord = cities[0][1];
  distanceTraveled = 0;								                                                        // Cumulatively holds distance to find eventual length of route
  while (!not_visited.empty()) {
    int dist = INT_MAX;								                                                        // This will hold the minimum distance we find
    int destination;								                                                          // Number of the city which leads to minimum distance
    for (std::set<int>::iterator it = not_visited.begin(); it != not_visited.end(); it++) {
      int tempDist = round(sqrt(pow(((double)currentXCoord - (double)cities[*it][0]), 2.0) + pow(((double)currentYCoord - (double)cities[*it][1]), 2.0)));
      if (tempDist < dist) {
	    dist = tempDist;
        destination = *it;
      }      
    }
    not_visited.erase(destination);
    visited.push_back(destination);
    currentXCoord = cities[destination][0];
    currentYCoord = cities[destination][1];
    distanceTraveled += dist;
  }

  opt2(visited, cities); 
  distanceTraveled = tourDistance(visited, cities);

  outputFile << distanceTraveled << "\n";
  for (std::vector<int>::iterator it = visited.begin(); it != visited.end(); it++) {
    outputFile << *it << "\n";
  }

  inputFile.close();
  outputFile.close();

  return 0;
}


