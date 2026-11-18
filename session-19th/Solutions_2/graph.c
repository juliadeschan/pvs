#define _CRT_SECURE_NO_WARNINGS

#include <limits.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

typedef struct GRAPH Graph;
typedef struct NODE Node;

struct NODE
{
	Graph* graph; // Graph containing the node
	char* label;  // A string containing a label for the node
	int adjacent_nodes_count;  // Amount of other nodes which are directly connected by edges to this node
	Node** adjacent_nodes;     // Other nodes which are directly connected by edges to this node
	                           // Those nodes are stored via an array of pointers
};

struct GRAPH {
	int node_count; // Amount of nodes in the graph
	Node** nodes;   // The nodes contained by this graph
	                // Those nodes are stored via an array of pointers
};

// a)
//
// Why is it necessary, that a node stores its adjacent nodes (or its edges 
// respectively) as an array of pointers on nodes (Node**), or respectively, 
// why is it impossible, that a node stores its adjacent
// nodes as an array of nodes (Node*)?

// 
// If a node referenced its adjacent nodes as a simple array of nodes (Node*),
// then this would require the adjacent nodes of any given node to occupy 
// subsequent memory addresses. However, since any given node should be able to 
// reference an  arbitrary set of adjacent nodes, it is impossible for this 
// condition to  be fulfilled in the general case. 
// 
// If a node references its adjacent nodes as an array of pointers on nodes 
// (Node**), then only the pointers will occupy subsequent memory adresses, 
// while the nodes may have arbitrary memory adresses. As a consequence, 
// any given node can reference an arbitrary set of nodes.

// Memory layout for a node storing its neigbors via an array
//
//		Memory layout of Node:
//		 .....
//		 8...11:  adjacent_nodes_count        Addresses in the array of nodes:
//		12...15:  Node* adjacent_nodes -->  0... 15:  16... 31:  32...47:   48...64:  ...
//											Node_0     Node_1     Node_2     Node_3
//
//
// Memory layout for a node stroing its neighbors via an array of pointers:
//
//		Memory layout of Node:
//		.....
//		8...11:  adjacent_nodes_count        Addresses in the array of pointers:
//		12...15:  Node** adjacent_nodes -->  0... 3:   4... 7:    8...11:   12...15:  ...
//										      ptr_0     ptr_1      ptr_2     ptr_3    ...
//											    |         |         |         |
//											    |         |         |         |
//		Addresses in the pointers:		       5096      7084      1932      1988     ...
//											    |         |         |         |
//										       \|/       \|/       \|/       \|/
//											    V         V		    V         V
//		Referenced Nodes:					  Node_0	Node_1    Node_2    Node_3    ...

// b)
//
// Why is it possible, that a graph stores its nodes as an array of 
// nodes (Node*), but highly advantageous, if a graph stores its nodes as an array
//  of pointer on nodes (Node**) ?
//
//
// Because there is a parent child relationship between a graph and its nodes, 
// it is possible for the graph to store its nodes in a array of nodes (Node*). 
// However, since the function graph_insert_node calls the realloc function all 
// pointers on this array of nodes would become invalid after inserting 
// a node by calling this function.
// -> Since each node stores its adjacent nodes as pointers on this array of nodes, 
//   those pointers would also needed to be corrected every time after a new node
//   has been insterted.
// -> In addition, the program using the graph library may store pointers on the 
//   nodes of this graph, which also would become invalid after inserting a new node.
// Both problems can be avoided by storing the nodes as an array of pointers on nodes 
// (Node**).

#include <limits.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>



/////////////////////////////////////////////////////////////////////////////////
// This function allocates an empty graph in the dynamic memory 
// and returns a pointer to this graph via its first argument
/////////////////////////////////////////////////////////////////////////////////

void graph_create(Graph** g)
{
	*g = (Graph*)malloc(sizeof(Graph));
	(*g)->node_count = 0;
	(*g)->nodes = NULL;
}


/////////////////////////////////////////////////////////////////////////////////
// This function is a helper function returning the id of a node
// in the graph of this node
/////////////////////////////////////////////////////////////////////////////////

int graph_get_node_id(Node* n)
{
	for (int n_id = 0; n_id < n->graph->node_count; n_id++)
		if (n->graph->nodes[n_id] == n)
			return n_id;
	printf("ERROR NODE NOT FOUND\n");
	return -1;
}

/////////////////////////////////////////////////////////////////////////////////
// This function creates a node in dynamic memory with a given 
// string as label, inserts the created node in the graph passed
// as argument and returns a pointer on the created node 
/////////////////////////////////////////////////////////////////////////////////

Node* graph_insert_node(Graph* g, char* label)
{
	Node* n = (Node*)malloc(sizeof(Node));

	n->adjacent_nodes_count= 0;
	n->adjacent_nodes = NULL;

	n->label = (char*)malloc((strlen(label) + 1) * sizeof(char));
	strcpy(n->label, label);

	n->graph = g;
	g->node_count++;
	g->nodes = (Node**)realloc(g->nodes, g->node_count * sizeof(Node*));
	g->nodes[g->node_count - 1] = n;
	return n;
}


/////////////////////////////////////////////////////////////////////////////////
// Inserts an edge between the two nodes passed as arguments
/////////////////////////////////////////////////////////////////////////////////

void graph_insert_edge(Node* n_1, Node* n_2)
{
	n_1->adjacent_nodes_count++;
	n_1->adjacent_nodes = (Node**)realloc(n_1->adjacent_nodes, n_1->adjacent_nodes_count * sizeof(Node*));
	n_1->adjacent_nodes[n_1->adjacent_nodes_count - 1] = n_2;

	n_2->adjacent_nodes_count++;
	n_2->adjacent_nodes = (Node**)realloc(n_2->adjacent_nodes, n_2->adjacent_nodes_count * sizeof(Node*));
	n_2->adjacent_nodes[n_2->adjacent_nodes_count - 1] = n_1;
}

/////////////////////////////////////////////////////////////////////////////////
// Deletes a graph passed as argument
/////////////////////////////////////////////////////////////////////////////////

void graph_delete(Graph* g)
{
	for (int n_id = 0; n_id < g->node_count; n_id++)
	{
        if(g->nodes[n_id]->adjacent_nodes_count != 0)
	        free(g->nodes[n_id]->adjacent_nodes); 

	        
	    free(g->nodes[n_id]);
	}

	free(g->nodes);
	free(g);
}

typedef struct
{
	int node_count;	 // Amount of nodes in the path                
	Node** nodes;    // The nodes contained in this graph
                     // Those nodes are stored via an array of pointers    
}Path;


/////////////////////////////////////////////////////////////////////////////////
// Prints the nodes in the path passed as argument
/////////////////////////////////////////////////////////////////////////////////

void path_print(Path* p)
{
	for (int n_id = 0; n_id < p->node_count; n_id++)
	{
		printf("%s", p->nodes[n_id]->label);
		if (n_id != p->node_count - 1)
			printf("->");
	}
	printf("\n");
}

/////////////////////////////////////////////////////////////////////////////////
// Deletes the path 
/////////////////////////////////////////////////////////////////////////////////

void path_delete(Path* p) 
{
	free(p->nodes);
	free(p);
}


/////////////////////////////////////////////////////////////////////////////////
// This function finds the shortest path between the two nodes passed
// as first and second arguments via Dijkstra, and returns a pointer to the 
// found path data structure via its third argument.
// The returned path does not contain any nodes if no path is found.
// If from == to then the returned path only contains from. 
// Since the function allocates the path in dynamic memory, the returned path 
// needs to be deleted via the path_delete function!
/////////////////////////////////////////////////////////////////////////////////

void graph_find_shortest_path(Node* from, Node* to, Path** p)
{
    // Allocates the path
	*p = (Path*)malloc(sizeof(Path));

	if (from == to) // From-Node == To-Node, returning path with only From-Node
	{
		(*p)->node_count =1;
		(*p)->nodes = (Node**)malloc(sizeof(Node*));		
		(*p)->nodes[0] = from;
		return;
	}

	Graph* g = from->graph;
	
	// Current cost to add the nodes to the shortest path
	unsigned int* nodes_current_cost = (unsigned int*)malloc(g->node_count * sizeof(unsigned int));
	for (int n_id = 0; n_id < g->node_count; n_id++)		
			nodes_current_cost[n_id] = UINT_MAX;
	nodes_current_cost[graph_get_node_id(from)] = 0;
	
	// Stores for each node, from which other node the cost for reaching this
	// node are currently lowest
	int* nodes_best_reachable_from = (int*)malloc(g->node_count * sizeof(int));
	for (int n_id = 0; n_id < g->node_count; n_id++)	
		nodes_best_reachable_from[n_id] = -1;

	
	// Nodes, to which the shortest path is known
	bool* nodes_added = (bool*)malloc(g->node_count * sizeof(bool));
	for (int n_id = 0; n_id < g->node_count; n_id++)	
			nodes_added[n_id] = false;
	nodes_added[graph_get_node_id(from)] = true;
	
	
    // Initializes the shortest path from From-Node to all adjacent nodes
	for (int e_id = 0; e_id < from->adjacent_nodes_count; e_id++)
	{
		Node* n = from->adjacent_nodes[e_id];
		nodes_best_reachable_from[graph_get_node_id(n)] = graph_get_node_id(from);
		nodes_current_cost[graph_get_node_id(n)] = 1;
	}
	
	
	bool node_found = false;
	while (true)
	{
		unsigned int next_min_cost = UINT_MAX;
		int next_node_id = -1;

        // Searches for the next node to add to the nodes to which
        // the shortest path is known
		for (int n_id = 0; n_id < g->node_count; n_id++)
			if (nodes_added[n_id] == false &&
				nodes_current_cost[n_id] < next_min_cost)
			{
				next_min_cost = nodes_current_cost[n_id];
				next_node_id = n_id;
			}

		if (next_min_cost == UINT_MAX)  // No more nodes reachable because graph 
			break;		                // is disconnected, To-Node has not been 
			                            // reached

        // Adding the newly found node to the nodes to which the shortest path is
        // known
		nodes_added[next_node_id] = true;
		Node* next_node = g->nodes[next_node_id];

		if (next_node == to) // Found the shortest path to To-node
		{
			node_found = true;
			break;
		}

        // Updates the cost to reach all other nodes if the path from the newly  
        // added node is cheaper than the path from an older node
		for (int e_id = 0; e_id < next_node->adjacent_nodes_count; e_id++)
		{
			int updated_node_id = graph_get_node_id(next_node->adjacent_nodes[e_id]);
			unsigned int cost_to_reach = 1 + nodes_current_cost[next_node_id];

			if (nodes_current_cost[updated_node_id] > cost_to_reach)
			{
				nodes_current_cost[updated_node_id] = cost_to_reach;
				nodes_best_reachable_from[updated_node_id] = next_node_id;
			}
		}
	}


	if (!node_found) // To-Node not reachable because graph is disconnected
	{
		(*p)->node_count = 0;
		(*p)->nodes = NULL;
	}
	else // Backtracking, building the path from To-Node to From-Node
	{

		int to_node_id = graph_get_node_id(to);
		int from_node_id = graph_get_node_id(from);


		(*p)->nodes = NULL;
		
		// Determining the amount of nodes on the path 
		(*p)->node_count = 1;	
		int backtrack_node_id = to_node_id;
		while (backtrack_node_id != from_node_id)
		{
			(*p)->node_count++;
			backtrack_node_id = nodes_best_reachable_from[backtrack_node_id];
		}

        // Allocating pointers for the nodes on the path
		(*p)->nodes = (Node**)malloc((*p)->node_count * sizeof(Node*));
		

        // Inserting the nodes on the path from To-Node to From-Node		
		(*p)->nodes[(*p)->node_count-1] = to;
		backtrack_node_id = to_node_id;
		for (int n_id = 1; n_id < (*p)->node_count; n_id++)
		{
			backtrack_node_id = nodes_best_reachable_from[backtrack_node_id];
			Node* current_backtrack_node = g->nodes[backtrack_node_id];
			(*p)->nodes[(*p)->node_count-1-n_id] = current_backtrack_node;
		}

	}

	//freeing all temporary arrays
	free(nodes_current_cost);
	free(nodes_best_reachable_from);
	free(nodes_added);
}



#define LABEL_BUFFER_SIZE 256
void graph_create_ring(Graph** g, int node_count)
{
	graph_create(g);
	// Temporary char array for storing the labels of the ring
	char label_buffer[LABEL_BUFFER_SIZE];
	
	// Inserting the nodes of the ring
	for (int n_id = 0; n_id < node_count; n_id++)
	{
		//Storing the label for a node in the temporary array
		sprintf(label_buffer, "%i", n_id);
		graph_insert_node(*g, label_buffer);
	}

	// Inserts an edge between each pair of subsequent nodes
	for (int n_id = 0; n_id < node_count; n_id++)
		graph_insert_edge((*g)->nodes[n_id], (*g)->nodes[(n_id + 1) % node_count]);
}



void graph_create_3d_torus(Graph** g, int depth, int height, int width)
{
	graph_create(g);
	// Temporary char array for storing the labels of the torus
	char label_buffer[LABEL_BUFFER_SIZE];
	
	// Inserting the nodes of the torus using the row major order
	for (int d_id = 0; d_id < depth; d_id++)
		for (int h_id = 0; h_id < height; h_id++)
			for (int w_id = 0; w_id < width; w_id++)
			{
				//Storing the label for a node in the temporary array				
				sprintf(label_buffer, "%i, %i, %i", d_id, h_id, w_id);
				graph_insert_node(*g, label_buffer);
			}
			
	// Inserting the edges of the torus respecting the row major order
	for (int d_id = 0; d_id < depth; d_id++)
		for (int h_id = 0; h_id < height; h_id++)
			for (int w_id = 0; w_id < width; w_id++)
			{
				// Each iteration of this loop inserts the following 3 edges 
				// to node (d_id, h_id, w_id):
				//
				//       top node
				// (d_id, h_id + 1, w_id)
				//         A                    
				//        /|\             front node
				//		   |    __ (d_id + 1, h_id, w_id)
				//         |     /|
				//         |    / 
				//         |   /     
				//        node  ------------------> right node
				// (d_id, h_id, w_id)       (d_id, h_id, w_id + 1)
				//
				// Cyclic edges at "border" of torus are achieved via 
				// (w_id + 1) % width, (h_id + 1) % height, and (d_id + 1) % depth
				
				
				int index		  =					d_id * height * width +
													h_id * width + 
													w_id;

				int right_index   =					d_id * height * width + 
													h_id * width + 
										(w_id + 1)	% width;

				int top_index     =					d_id * height * width + 
								   ((h_id + 1) % height) * width + 
													w_id;

				int front_index   = ((d_id +1) % depth) * height * width +
													h_id * width +
													w_id;

				graph_insert_edge((*g)->nodes[index], (*g)->nodes[right_index]);
				graph_insert_edge((*g)->nodes[index], (*g)->nodes[top_index]);
				graph_insert_edge((*g)->nodes[index], (*g)->nodes[front_index]);
			}
}

void graph_create_complete_graph(Graph** g, int node_count)
{
	graph_create(g);
	// Temporary char array for storing the labels of the graph
	char label_buffer[LABEL_BUFFER_SIZE];
	
	// Inserting the nodes of the complete graph
	for (int n_id = 0; n_id < node_count; n_id++)
	{
		sprintf(label_buffer, "%i", n_id);
		graph_insert_node(*g, label_buffer);
	}
	
	// Inserting the edges of the complete graph
	for (int n1_id = 0; n1_id < node_count; n1_id++)
	// Only inserting an edge if n2_id > n1_id so that edges are not inserted twice
		for (int n2_id = n1_id+1; n2_id < node_count; n2_id++)
			graph_insert_edge((*g)->nodes[n1_id], (*g)->nodes[n2_id]);
}

int graph_compute_degree(Graph* g)
{
	int degree = 0;
	for (int n_id = 0; n_id < g->node_count; n_id++)
		degree = degree > g->nodes[n_id]->adjacent_nodes_count ?
		degree : g->nodes[n_id]->adjacent_nodes_count;

	return degree;
}

unsigned int graph_compute_diamater(Graph* g)
{
	unsigned int diameter = 0;
	// Finding maximum distance between two arbitrary nodes in graph by iterating
	// over all possible pairs of nodes n1 and n2
	for (int n1_id = 0; n1_id < g->node_count; n1_id++)
		for (int n2_id = n1_id + 1; n2_id < g->node_count; n2_id++)
		// Since graph is not directed length of path from n_1 to n_2 is equal to 
		// the length of path from n_2 to n_1 -> only considering nodes with  
		// n2_id > n1_id saves redundant computation		
		{
			Path* p;
			graph_find_shortest_path(g->nodes[n1_id], g->nodes[n2_id], &p);
			diameter = diameter > p->node_count-1 ? diameter : p->node_count-1;
			path_delete(p);
		}
	return diameter;
}

int graph_compute_edge_count(Graph* g)
{
	int edge_ends = 0;
	for (int n_id = 0; n_id < g->node_count; n_id++)
		edge_ends += g->nodes[n_id]->adjacent_nodes_count;

	int edge_count = edge_ends / 2;
	return edge_count;
}


int main(int argc, char** args)
{
	// Testing ring graph
	
	int ring_node_count = 20;
	int ring_degree_expected = 2;
	int ring_diameter_expected = ring_node_count / 2;
	int ring_edge_count_expected = ring_node_count;

	Graph* ring;
	graph_create_ring(&ring, ring_node_count);
	int ring_degree_computed = graph_compute_degree(ring);
	int ring_diameter_computed = graph_compute_diamater(ring);
	int ring_edge_count_computed = graph_compute_edge_count(ring);

	printf("Ring:\n");
	printf("Degree expected: %i\n", ring_degree_expected);
	printf("Degree computed: %i\n", ring_degree_computed);
	printf("Diameter expected: %i\n", ring_diameter_expected);
	printf("Diameter computed: %i\n", ring_diameter_computed);
	printf("Node count expected: %i \n", ring_node_count);
	printf("Actual node count: %i\n", ring->node_count);
	printf("Edge count expected: %i\n", ring_edge_count_expected);
	printf("Actual edge count: %i\n\n", ring_edge_count_computed);
	graph_delete(ring);
	
	// Testing 3D torus
	 
	int torus_3d_width  = 5;
	int torus_3d_height = 4;
	int torus_3d_depth  = 5;

	int torus_3d_degree_expected = 6;
	int torus_3d_diameter_expected = torus_3d_width /2 + torus_3d_height/2 + torus_3d_depth/2;
	int torus_3d_node_count_expected = torus_3d_width * torus_3d_height * torus_3d_depth;
	int torus_3d_edge_count_expected = 3 * torus_3d_node_count_expected;
	
	Graph* torus_3d;
	graph_create_3d_torus(&torus_3d,torus_3d_depth, torus_3d_height, torus_3d_width);
	int torus_3d_degree_computed = graph_compute_degree(torus_3d);
	int torus_3d_diameter_computed = graph_compute_diamater(torus_3d);
	int torus_3d_edge_count_computed = graph_compute_edge_count(torus_3d);

	printf("3D torus:\n");
	printf("Degree expected: %i\n", torus_3d_degree_expected);
	printf("Degree computed: %i\n", torus_3d_degree_computed);
	printf("Diameter expected: %i\n", torus_3d_diameter_expected);
	printf("Diameter computed: %i\n", torus_3d_diameter_computed);
	printf("Node count expected: %i \n", torus_3d_node_count_expected);
	printf("Actual node count: %i\n",  torus_3d->node_count);
	printf("Edge count expected: %i\n", torus_3d_edge_count_expected);
	printf("Actual edge count: %i\n\n", torus_3d_edge_count_computed);
	graph_delete(torus_3d);

	int complete_graph_node_count = 20;
	int complete_graph_diameter_expected = 1;
	int complete_graph_degree_expected = complete_graph_node_count - 1;
	int complete_graph_edge_count_expected = (complete_graph_node_count *
		(complete_graph_node_count - 1))/2;
		
	// Testing complete graph
	
	Graph* complete_graph;
	graph_create_complete_graph(&complete_graph, complete_graph_node_count);
	int complete_graph_degree_computed     = graph_compute_degree(complete_graph);
	int complete_graph_diameter_computed   = graph_compute_diamater(complete_graph);
	int complete_graph_edge_count_computed = graph_compute_edge_count(complete_graph);

	printf("Complete graph:\n");
	printf("Degree expected: %i\n", complete_graph_degree_expected);
	printf("Degree computed: %i\n", complete_graph_degree_computed);
	printf("Diameter expected: %i\n", complete_graph_diameter_expected);
	printf("Diameter computed: %i\n", complete_graph_diameter_computed);
	printf("Node count expected: %i \n", complete_graph_node_count);
	printf("Actual node count: %i\n", complete_graph->node_count);
	printf("Edge count expected: %i\n", complete_graph_edge_count_expected);
	printf("Actual edge count: %i\n\n", complete_graph_edge_count_computed);
	graph_delete(complete_graph);
}
