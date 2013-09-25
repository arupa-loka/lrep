#include "GraphReader.hpp"
#include "GraphT.hpp"
#include <fstream>
#include <string.h>
#include <stdlib.h>


int GraphReader::readGraphT( const char *iFileName, GraphT<Node*,int>** oGraph ) {
	
	std::FILE * aFile = NULL;
	char aLine[MAXLINE];
	char * aToken;
	int oNumberOfNodes = 0;
	
	aFile = fopen(iFileName, "r");
	
	if( ! aFile )
	{
		printf("Error: in file opening %s\n", iFileName);
		return -1;
	}
	
	*oGraph = new GraphT<Node*, int>();
	
	while( fgets(aLine, MAXLINE, aFile) )
	{
		aToken = strtok (aLine, "->");
		if ( aToken == NULL)
		{
			break;
		}
		++oNumberOfNodes;
	}
	printf("Number of Nodes: %d\n", oNumberOfNodes);
	fseek( aFile, 0, SEEK_SET );
	
	int i=0;
	int aFromNode=0;
	int aToNode=0;
	while ( fgets(aLine, MAXLINE, aFile) )
	{	
		//printf("%s\n", aLine);
		aToken = strtok (aLine, "-");
		if ( aToken == NULL)
		{
			break;
		}
		aFromNode = strtol( aToken, NULL, 10);
		printf("%d -> ", aFromNode);	
		Node * aNewNode = new Node(aFromNode);
		(*oGraph)->addNode(aNewNode);

		while ( aToken=strtok(NULL, ",") )
		{
			//printf("Token: #%s#\n", aToken);
			if (strcmp(aToken,"\n")!=0) {
				aToNode = strtol( aToken, NULL, 10);
				Node * aNewNode02 = new Node(aToNode);
				(*oGraph)->addEdge(aNewNode,aNewNode02,0);
		
				printf("%d, ", aToNode);
			}
		}
		printf("\n");
	}

	return 0;
}

