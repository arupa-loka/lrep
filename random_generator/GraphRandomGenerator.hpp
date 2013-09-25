#ifndef _GRAPH_RANDOM_GENERATOR_HPP
#define _GRAPH_RANDOM_GENERATOR_HPP

class GraphRandomGenerator
{
	public:
	GraphRandomGenerator(GraphM & iG);
	GraphRandomGenerator(GraphM & iG, int iSeed);
	void initProbGraph(int iEdges);
	void initExactGraph(int iEdges);
	void getRandomEdge(int & a, int & b);
	
	private:
	GraphM & _graph;
};

GraphRandomGenerator::GraphRandomGenerator(GraphM & iG): _graph(iG)
{
	int seed = time(NULL);
	std::cout << "seed= "<<seed<<std::endl; 
	srand(seed);
}

GraphRandomGenerator::GraphRandomGenerator(GraphM & iG, int iSeed): _graph(iG)
{
	std::cout << "seed= "<<iSeed<<std::endl; 
	srand(iSeed);
}

void GraphRandomGenerator::getRandomEdge(int & a, int & b)
{
	a = rand()%_graph.getVertexSize();
	b = rand()%_graph.getVertexSize();
	return;
}

void GraphRandomGenerator::initProbGraph(int iEdges)
{
	double p = 2.0*(double)iEdges/(double)((_graph.getVertexSize()-1)*_graph.getVertexSize());

	for (int i=0; i<_graph.getVertexSize(); ++i)
	{
		for (int j=0; j<_graph.getVertexSize(); ++j)
		{
			if (rand()<p*RAND_MAX)
			{
				int a,b;
				do {	
					getRandomEdge(a,b);
				}while(!_graph.addEdge(a,b));
			}
		}
	}

}

void GraphRandomGenerator::initExactGraph(int iEdges)
{
	for(int i=0; i<iEdges; ++i)
	{
		int a,b;
		do {	
			getRandomEdge(a,b);
		}while(!_graph.addEdge(a,b));
	}
}

#endif

