#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>
#include <iomanip>
#include <chrono>

#define PRECISION 5

using namespace std;
using namespace std::chrono;

class Vertex {
	private:
		int id_;
		double lat_, lon_;
		vector<int> adja_;
		
	public:
		Vertex() { }
		Vertex(const int id, const double lat, const double lon) : id_(id), lat_(lat), lon_(lon) { }

		void adjaAdd(const int n) {
			adja_.push_back(n);
		}
		
		void printAdja() {
			cout << "Adjacency of " << id_ << " : " << endl;
			for(auto a:adja_)
				cout << "    " << a << endl;
		}
		
		vector<int> getAdja() const {
			return adja_;
		}
		
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

			map<pair<int,int>,Edge>::iterator it;
			int x;
			for(it=edges.begin();it!=edges.end();it++){
				x=0;
				while(vertexs[x++].getId()!=it->second.getSrcVid());
				vertexs[x-1].adjaAdd(it->second.getDstVid());
			}

		}
		
		void affD(const vector<vector<Line>> D) {
			for(auto a:D){
				for(auto b:a)
					cout << setw(5) << b << setw(5);
				cout << endl;
			} 
		}
		
		void Dijkstra(const int depart, const int arrivee) {
			vector<vector<Line>> D;
			vector<Line> l;

			l.push_back(Line(0,0,0));
				
			for(auto a:vertexs)
					//Si sur point de départ, alors tout initialisé à 0 vu que départ	
				if(a.getId()==depart)
					l.push_back(Line(0,0,0));
					//Sinon, pas point de départ donc point à l'infini d'où le -1
				else
					l.push_back(Line(0,-1,0));
			D.push_back(l);			
			
					//a correspond au point courant, b à la longeur entre le point courant et le point précédent, c correspond au point précédent
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
					//Si le point consécutif n'existe pas, on remet à jour la vertex courante avec un point qui existe
				do {
					a=vertexs[i-1].getId();
					b=D.back()[i].getB();
					c=D.back()[i].getC();
				}
				while(D.back()[i++].getB()<0);
					//On cherche la plus petite valeur de la ligne afin d'avoir le chemin le plus court
				for(x=2;x<=vlen;x++) {
					getB=D.back()[x].getB();
					if(getB<=b && getB>=0) {
						a=vertexs[x-1].getId();
						b=getB;
						c=D.back()[x].getC();
					}
				}
					//Une fois plus petite valeur obtenu, on la met dans la première case de la ligne
				l.push_back(Line(a,b,c));
				a=0;
					//On parcourt chaque ligne et on vérifie qu'on ne parcourt pas plusieurs fois la même vertex
				for(x=1;x<=vlen;x++) {
					getB=D.back()[x].getB();
					if(getB==-2 || l[0].getA()==vertexs[x-1].getId()) {
						b=-2;
						c=0;
					}
					//Sinon, on compare la vertex courante avec la vertex précédente, et on conserve uniquement celle qui a la plus petite longueur
					else {	
						t=true;
						q=edges[pair<int,int>(l[0].getA(),vertexs[x-1].getId())].getLength();
						if(q!=0 && (q+l[0].getB()<getB || getB==-1)) {
							b=q+l[0].getB();
							c=l[0].getA();
							t=!t;
						}
						//Si jamais la vertex courante est plus longue que la vertex précédente, on conserve la vertex précédente
						if(t){
							b=getB;
							c=D.back()[x].getC();
						}
					}
					//On ajoute la vertex avec la plus petite longueur dans le vecteur
					l.push_back(Line(a,b,c));
				}
				cout << l[0] << endl; 
				
				D.push_back(l);
			}
			auto stop = high_resolution_clock::now();
			cout << endl << endl << "Execution time = " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl << endl << endl;
			
			cout << "Total visited vertex = " << D.size()-1 << endl;
			
			//On parcourt de nouveau le graph, mais dans le sens inverse, afin de retrouver le chemin le plus court
			vector<Line> w;
			int j=(int) D.size()-1;
			w.push_back(D[j][0]);
			//cc : Variable qui permet dans la boucle for de parcourir uniquement les noeuds inclus dans le chemin
			int cc=D[j][0].getC();
			for(unsigned long int k=j-1;k>0;k--){
				if(D[k][0].getA()==cc){
					w.push_back(D[k][0]);
					cc=D[k][0].getC();
				}
			}
			
			cout << "Total vertex on path from start to end = " << w.size() << endl;
			
			//Affichage, mais on parcourt à l'envers pour afficher les vertex dans l'ordre, càd du premier au dernier
			int g=(int) w.size();
			for(int y=g-1;y>=0;y--)
				cout << "Vertex[" << setw(4) << g-y << "] = " << setw(6) << w[y].getA() << ", length = " << setw(10) << w[y].getB() << endl;
			
		}
		
		bool is_in_closed_set(const int v, const deque<Line> d) {
			for(auto a:d)
				if(a.getA()==v)
					return true;
			return false;
		}
		
		bool is_in_active_queue(const int v, const deque<Line> d) {
			for(auto a:d)
				if(a.getA()==v)
					return true;
			return false;
		}
		
		void Bfs(const int vstart, const int vend) {
			// On utilise notre classe Line afin de prendre en compte le noeud courant, le précédent ainsi que la distance
			deque<Line> active_queue;
			deque<Line> closed_set;
			
			active_queue.push_back(Line(vstart,0,0));
			
			do {
				
				auto vcurrent=active_queue.front();
				active_queue.pop_front();
				closed_set.push_back(vcurrent);
				
				// On déclare un vecteur qui contient la liste d'adjacence de chaque noeud courant
				vector<int> adja;
				for(auto a:vertexs) {
					if(a.getId()==vcurrent.getA()) {
						adja=a.getAdja();
						break;
					}
				}
				
				// On parcourt la liste d'adjacence
				for(auto vnext:adja) {
					if(is_in_closed_set(vnext,closed_set))
						continue;
					if(!is_in_active_queue(vnext,active_queue))
						active_queue.push_back(Line(vnext,edges[pair<int,int>(vcurrent.getA(),vnext)].getLength()+closed_set.back().getB(),vcurrent.getA()));
				}
				// On boucle tant que le noeud final n'a pas été atteint
			} while(closed_set.back().getA()!=vend);
			
			cout << "Total visited vertex = " << closed_set.size() << endl;

			// Comme pour Dijkstra, on parcourt le tableau en sens inverse pour retrouver le chemin
			vector<Line> w;
			int j=(int) closed_set.size()-1;
			w.push_back(closed_set[j]);
			int cc=closed_set[j].getC();
			for(unsigned long int k=j-1;k>0;k--) {
				if(closed_set[k].getA()==cc){
					w.push_back(closed_set[k]);
					cc=closed_set[k].getC();
				}
			}
			w.push_back(Line(vstart,0,0));
			
			cout << "Total vertex on path from start to end = " << w.size() << endl;
			
			int g=(int) w.size();
			for(int y=g-1;y>=0;y--)
				cout << "Vertex[" << setw(4) << g-y << "] = " << setw(6) << w[y].getA() << ", length = " << setw(10) << w[y].getB() << endl;
			
		}
		
};

