# 2opt-tour
Implementation of the 2-opt algorithm which finds approximate solutions for Traveling Salesman problemsets

Traveling Salesman problem-set files should be plain text & formatted as follows for each city:
[city number] [x-coordinate] [y-coordinate]
E.G.
1 30 50
2 0 10
3 25 15
...

The approximate best route is placed in a file titled [input file name].tour. The first line contains the total 
distance, with each following line indicating the numbers for each city in the order in which they would be 
visited to achieve the total distance.
