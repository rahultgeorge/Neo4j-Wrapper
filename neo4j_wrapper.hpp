#include <neo4j-client.h>
#include <errno.h>
#include <stdio.h>
#include <string>
#include <iostream>


using namespace std;


namespace Neo4j_Wrapper {
    class Neo4j_Connector {
    private:
        neo4j_connection_t *connection;

    public:

        Neo4j_Connector() {
            neo4j_client_init();
        }

        //Connect to the db (Return 1 on success)
        int connect();

        // Generic function to insert data
        int insert(const char *node_type, int num_arguments, char **arguments);

        int insert_pdg_edge(string node_source_label, string edge_label,
                            string node_dest_label);

        int insert_pdg_node(string label, string node_type, string instruction);

        //Close the connection and clean up
        void close();
    };

}