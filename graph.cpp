#include <iostream>
#include "graph.hpp"

using namespace std;

int main(int argc, char *argv[]) {

	int start=0,end=0;
	string algorithm="",file="";
	
	for(int x=1;x<argc;x++) {
		string s(argv[x]);
		if(s=="--start")
			start=stoi(argv[x+1]);
				
		else if(s=="--end")
			end=stoi(argv[x+1]);
				
		else if(s=="--algorithm")
			algorithm=argv[x+1];

		else if(s=="--file")
			file=argv[x+1];
		
	}
	
	Graph g(file);
	
	if(algorithm=="dijkstra")	
		g.Dijkstra(start, end);

	else if(algorithm=="bfs")
		g.Bfs(start, end);

	
	return EXIT_SUCCESS;
}

