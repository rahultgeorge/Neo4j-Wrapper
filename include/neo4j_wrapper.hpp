#include <neo4j-client.h>
#include <errno.h>
#include <stdio.h>
#include <string>
#include <iostream>


using namespace std;


namespace Neo4j_Wrapper {

    enum AttackGraphNode
    {
        AttackSurface,
        AttackState,
        AttackAction,
    };

    class Neo4j_Connector {
    private:
        neo4j_connection_t *connection;

    public:

        Neo4j_Connector() {
            neo4j_client_init();
        }

        //Connect to the db (Return 1 on success)
        int connect(string uri="neo4j://neo4j:secret@localhost:7687");

        // Generic function to insert data
        int insert(const char *node_type, int num_arguments, char **arguments);

        int insert_pdg_edge(string node_source_label, string edge_label,
                            string node_dest_label);

        int insert_pdg_node(string label, string node_type, string instruction, string function_name, string program_name);

        int insert_attack_graph_node(AttackGraphNode attackGraphNode, string description, string instruction,string function_name ="NULL");


        //Close the connection and clean up
        void close();

    };

    string getStringEquivalent(AttackGraphNode attackGraphNode);

}