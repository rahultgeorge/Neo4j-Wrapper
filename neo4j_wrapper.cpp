#include "neo4j_wrapper.hpp"

int Neo4j_Wrapper::Neo4j_Connector::connect()
{

    connection = neo4j_connect("neo4j://neo4j:secret@localhost:7687", NULL, NEO4J_INSECURE);
    if (connection == NULL)
    {
        neo4j_perror(stderr, errno, "Connection failed");
        return 0;
    }
 return 1;
}

void Neo4j_Wrapper::Neo4j_Connector::close()
{
   neo4j_close(connection);
   neo4j_client_cleanup();
}


int Neo4j_Wrapper::Neo4j_Connector::insert(const char* node_type,int num_arguments, char** arguments)
{
     // TODO - Add generic insert and create methods
    return EXIT_SUCCESS;
}


int Neo4j_Wrapper::Neo4j_Connector::insert_pdg_edge(string node_source_type,string source_instruction, string edge_label, string node_dest_type, string dest_instruction)
{

    string statement="MERGE (:ProgramInstruction{type:\'"+node_source_type+"\', instruction:\'"+ source_instruction+"\'})-[:EDGE{label:\'"+edge_label+"\'}]->(:ProgramInstruction{type:\'"+node_dest_type+"\', instruction:\'"+ dest_instruction+"\'})";
    neo4j_result_stream_t *results = neo4j_run(connection,statement.c_str(),neo4j_null );
    if (results == NULL)
    {
        neo4j_perror(stderr, errno, "Failed to run statement");
        return 0;
    }
    if(results==NULL)
        neo4j_perror(stderr, errno, strerror(errno));

    neo4j_close_results(results);
    return 1;
}


int Neo4j_Wrapper::Neo4j_Connector::insert_pdg_node(string label, string node_type,string instruction)
{
    //  std::string label;
  string statement="CREATE (:ProgramInstruction{label:\'"+label+"\', type:\'"+node_type+"\', instruction:\'"+ instruction+"\'})";
  neo4j_result_stream_t *results = neo4j_run(connection,statement.c_str(),neo4j_null );
    if (results == NULL)
    {
        neo4j_perror(stderr, errno, "Failed to run statement");
        return 0;
    }
  neo4j_close_results(results);
  return 1;
}
