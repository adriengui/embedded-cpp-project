#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
#include <chrono>

#define PRECISION 5

using namespace std;
using namespace std::chrono;

class Vertex {
	private:
		int id_;
		double lat_, lon_;
		
	public:
		Vertex() { }
		Vertex(const int id, const double lat, const double lon) : id_(id), lat_(lat), lon_(lon) { }
		
		int getId() const {
			return id_;
		}
		
		double getLat() const {
			return lat_;
		}
		
		double getLon() const {
			return lon_;
		}
		
		const string toString() const {
			ostringstream s;
			
			s << setprecision(PRECISION) << "Id = " << getId() << endl << "Latitude = " << getLat() << endl << "Longitude = " << getLon() << endl;
			return s.str();
		}
		
		friend ostream& operator<<(ostream &f, const Vertex &v) {
			return f<<v.toString();
		}
};

class Edge {
	private:
		int src_vid_, dst_vid_;
		double length_;
		string name_;
	public:
	
		Edge() { }
	
		Edge(const int src_vid, const int dst_vid, const double length, const string name) : src_vid_(src_vid), dst_vid_(dst_vid), length_(length), name_(name) { }
		
		int getSrcVid() const {
			return src_vid_;
		}
		
		int getDstVid() const {
			return dst_vid_;
		}
		
		double getLength() const {
			return length_;
		}
		
		string getName() const {
			return name_;
		}
		
		string toString() const {
			ostringstream s;
			
			s << setprecision(PRECISION) << "Source Vertex Id = " << getSrcVid() << endl << "Dest Vertex Id = " << getDstVid() << endl << "Longueur = " << getLength() << endl << "Nom = " << getName() << endl;
			return s.str();
		}
		
		friend ostream& operator<<(ostream &f, const Edge &e) {
			return f<<e.toString();
		}
};

class Line {
	private:
		int a_;
		double b_;
		int c_;
	public:
		Line(const int a=0, const double b=0, const int c=0) : a_(a), b_(b), c_(c) { }
		
		int getA() const {
			return a_;
		}
		
		double getB() const {
			return b_;
		}
		
		int getC() const {
			return c_;
		}
		
		string toString() const {
			ostringstream s;
			if(a_)
				s << a_ << "(";
			if(b_==-1)
				s << "inf";
			else if(b_==-2)
				s << "x";
			else
				s << setprecision(PRECISION) << b_;
			if(c_)
				s << "," << c_;
			if(a_)
				s << ")";
			
			return s.str();
		}
		
		friend ostream& operator<<(ostream &f, const Line &l) {
			return f<<l.toString();
		}
};

class Graph {
	private:
		vector<Vertex> vertexs;
		map<pair<int,int>,Edge> edges;
		
	public:
		Graph(const string name) {
			vector<string> row;
			string line, word;
			
			fstream file (name, ios::in);

			if(file.is_open()) {
				while(getline(file, line)) {
					row.clear();
					stringstream str(line);
			
					while(getline(str, word, ','))
						row.push_back(word);
				
					if(row[0]=="V")			
						vertexs.push_back(Vertex(stoi(row[1]), stod(row[3]), stod(row[2])));
		
					else if(row[0]=="E")
						edges.insert(pair<pair<int,int>,Edge>(pair<int,int>(stoi(row[1]), stoi(row[2])),Edge(stoi(row[1]), stoi(row[2]), stod(row[3]), row[4])));
				
				}
			}
	
			else
				cout << "Could not open the file" << endl;
	
			file.close();
		}
		
		void Dijkstra(const int depart, const int arrivee) {
			vector<vector<Line>> D;
			vector<Line> l;

			l.push_back(Line(0,0,0));
				
			for(auto a:vertexs)
				if(a.getId()==depart)
					l.push_back(Line(0,0,0));
				else
					l.push_back(Line(0,-1,0));
			D.push_back(l);			
			
			int a,c,i,q;
			unsigned int x,vlen=vertexs.size();
			double b,getB;
			bool t;
			
			auto start = high_resolution_clock::now();
			
			while(D.back()[0].getA()!=arrivee){
				l.clear();
				a=1;
				c=0;
				i=1;
				do {
					a=vertexs[i-1].getId();
					b=D.back()[i].getB();
					c=D.back()[i].getC();
				}
				while(D.back()[i++].getB()<0);
								
				for(x=2;x<=vlen;x++) {
					getB=D.back()[x].getB();
					if(getB<=b && getB>=0) {
						a=vertexs[x-1].getId();
						b=getB;
						c=D.back()[x].getC();
					}
				}
				l.push_back(Line(a,b,c));
				a=0;
				for(x=1;x<=vlen;x++) {
					getB=D.back()[x].getB();
					if(getB==-2 || l[0].getA()==vertexs[x-1].getId()) {
						b=-2;
						c=0;
					}
					
					else {	
						t=true;
						q=edges[pair<int,int>(l[0].getA(),vertexs[x-1].getId())].getLength();
						if(q!=0 && (q+l[0].getB()<getB || getB==-1)) {
							b=q+l[0].getB();
							c=l[0].getA();
							t=!t;
						}
						if(t){
							b=getB;
							c=D.back()[x].getC();
						}
					}

					l.push_back(Line(a,b,c));
				}
				cout << l[0] << endl; 
				
				D.push_back(l);
			}
			auto stop = high_resolution_clock::now();
				cout << endl << endl << "Execution time = " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl << endl << endl;
			
			cout << "Nb visited = " << D.size() << endl << endl;

			vector<Line> w;
			int j=(int) D.size()-1;
			w.push_back(D[j][0]);
			int cc=D[j][0].getC();
			for(unsigned long int k=j-1;k>0;k--){
				if(D[k][0].getA()==cc){
					w.push_back(D[k][0]);
					cc=D[k][0].getC();
				}
			}
			
			cout << "Chemin le plus court = " << endl << endl;
			
			int g=(int) w.size();
			for(int y=g-1;y>=0;y--)
				cout << w[y] << endl;
			
		}
};

void affD(const vector<vector<Line>> D) {
	for(auto a:D){
		for(auto b:a)
			cout << setw(5) << b << setw(5);
		cout << endl;
	} 
}

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
	
	if(algorithm=="dijkstra"){
			
			g.Dijkstra(start, end);
			
	}
	
	
	return EXIT_SUCCESS;
}

