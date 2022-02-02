// C++ program for Implementation of
// Reverse Cuthill Mckee Algorithm

#include <bits/stdc++.h>
using namespace std;

using namespace std;
template <typename T>
vector<vector<T>> fileIn( const char* filename )
{
   vector< vector<T> > arr;
   ifstream in( filename );
   if ( in )
   {
      for ( string line; getline( in, line ); )
      {
	  stringstream ss( line );
	  vector<int> row;
	  for ( int i; ss >> i; )
	      row.push_back( i );
	  arr.push_back( row );
      }
   }
   else
   {
      cerr << "Boo! No such file\n";
   }
   return arr;
}


vector<double> globalDegree;

int findIndex(vector<pair<int, double> > a, int x)
{
	for (int i = 0; i < a.size(); i++)
		if (a[i].first == x)
			return i;
	return -1;
}

bool compareDegree(int i, int j)
{
	return ::globalDegree[i] < ::globalDegree[j];
}

template <typename T>
ostream& operator<<(ostream& out, vector<T> const& v)
{
	for (int i = 0; i < v.size(); i++)
		out << v[i] << ' ';
	return out;
}

class ReorderingSSM {
private:
	vector<vector<double> > _matrix;

public:
	// Constructor and Destructor
	ReorderingSSM(vector<vector<double> > m)
	{
		_matrix = m;
	}

	ReorderingSSM() {}
	~ReorderingSSM() {}

	// class methods

	// Function to generate degree of all the nodes
	vector<double> degreeGenerator()
	{

		vector<double> degrees;

		for (int i = 0; i < _matrix.size(); i++) {
			double count = 0;
			for (int j = 0; j < _matrix[0].size(); j++) {
				count += _matrix[i][j];
			}

			degrees.push_back(count);
		}

		return degrees;
	}

	// Implementation of Cuthill-Mckee algorithm
	vector<int> CuthillMckee()
	{
		vector<double> degrees = degreeGenerator();

		::globalDegree = degrees;

		queue<int> Q;
		vector<int> R;
		vector<pair<int, double> > notVisited;

		for (int i = 0; i < degrees.size(); i++)
			notVisited.push_back(make_pair(i, degrees[i]));

		// Vector notVisited helps in running BFS
		// even when there are dijoind graphs
		while (notVisited.size()) {

			int minNodeIndex = 0;

			for (int i = 0; i < notVisited.size(); i++)
				if (notVisited[i].second < notVisited[minNodeIndex].second)
					minNodeIndex = i;

			Q.push(notVisited[minNodeIndex].first);

			notVisited.erase(notVisited.begin()
									+ findIndex(notVisited,
												notVisited[Q.front()].first));

			// Simple BFS
			while (!Q.empty()) {

				vector<int> toSort;

				for (int i = 0; i < _matrix[0].size(); i++) {
					if (i != Q.front() && _matrix[Q.front()][i] == 1
						&& findIndex(notVisited, i) != -1) {
						toSort.push_back(i);
						notVisited.erase(notVisited.begin()
										+ findIndex(notVisited, i));
					}
				}

				sort(toSort.begin(), toSort.end(), compareDegree);

				for (int i = 0; i < toSort.size(); i++)
					Q.push(toSort[i]);

				R.push_back(Q.front());
				Q.pop();
			}
		}

		return R;
	}

	// Implementation of reverse Cuthill-Mckee algorithm
	vector<int> ReverseCuthillMckee()
	{

		vector<int> cuthill = CuthillMckee();

		int n = cuthill.size();

		if (n % 2 == 0)
			n -= 1;

		n = n / 2;

		for (int i = 0; i <= n; i++) {
			int j = cuthill[cuthill.size() - 1 - i];
			cuthill[cuthill.size() - 1 - i] = cuthill[i];
			cuthill[i] = j;
		}

		return cuthill;
	}
};

// Driver Code
int main(int argc, char **argv)
{
	auto matr = fileIn<double>(argv[1]);

	ReorderingSSM m(matr);

	vector<int> r = m.CuthillMckee();

	cout << "Permutation order of objects: " << r << endl;

	return 0;
}
