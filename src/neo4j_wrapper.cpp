#include "neo4j_wrapper.hpp"

int Neo4j_Wrapper::Neo4j_Connector::connect(string uri) {
    connection = neo4j_connect(uri.c_str(), NULL, NEO4J_INSECURE);
    if (connection == NULL) {
        neo4j_perror(stderr, errno, "Connection failed");
        return 0;
    }
    return 1;
}

void Neo4j_Wrapper::Neo4j_Connector::close() {
    neo4j_close(connection);
    neo4j_client_cleanup();
}


int Neo4j_Wrapper::Neo4j_Connector::insert(const char *node_type, int num_arguments, char **arguments) {
    // TODO - Add generic insert and create methods
    return EXIT_SUCCESS;
}


int Neo4j_Wrapper::Neo4j_Connector::insert_pdg_edge(string node_source_label, string edge_label,
                                                    string node_dest_label)
{
    // Only using label to find the nodes (assuming nodes have been created)
    string statement="MERGE (p1:ProgramInstruction{label:\'"+node_source_label+"\'}) MERGE(p2:ProgramInstruction{label:\'"+node_dest_label+"\'}) MERGE((p1)-[:EDGE{label:\'"+edge_label+"\'}]->(p2))";
    neo4j_result_stream_t *results = neo4j_run(connection,statement.c_str(),neo4j_null );
    if (results == NULL)
    {
        neo4j_perror(stderr, errno, "Failed to run statement");
        return 0;
    }
    neo4j_close_results(results);
    return 1;
}


int Neo4j_Wrapper::Neo4j_Connector::insert_pdg_node(string label, string node_type,
                                                    string instruction, string function_name, string program_name) {
    string debugLocation="-1";
    size_t pos = instruction.find("!dbg !");
    if(pos!=std::string::npos)
        debugLocation=instruction.substr(pos+6);
        string statement="MERGE (n:ProgramInstruction{label:\'"+label+"\'}) ON MATCH SET n.type=\'"+node_type+"\', n.instruction=\'"+ instruction +"\', n.debug_location=\'"+debugLocation+"\', n.function_name=\'"+function_name +"\', n.program_name=\'"+program_name+ +"\' ON CREATE SET n.type=\'" +node_type+"\', n.instruction=\'"+ instruction+"\', n.debug_location=\'"+debugLocation+"\', n.function_name=\'"+function_name+"\', n.program_name=\'"+program_name+"\'";
    neo4j_result_stream_t *results = neo4j_run(connection,statement.c_str(),neo4j_null );
    if (results == NULL)
    {
        neo4j_perror(stderr, errno, "Failed to run statement");
        return 0;
    }
    neo4j_close_results(results);
    return 1;
}


bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


int Neo4j_Wrapper::Neo4j_Connector::insert_attack_graph_node(AttackGraphNode attackGraphNode, string description, string instruction, string function_name)
{
    // Step 1: Find the underlying PDG node - Use the debug location if provided
    string statement;
    if(attackGraphNode==AttackState) {
        if (instruction.find("alloca") != std::string::npos)
            statement = " MATCH (p:ProgramInstruction) WHERE p.instruction CONTAINS  \'" + instruction +
                        "\' AND p.function_name =\'" + function_name + "\'";
        else //Regex instruction pattern
            statement = " MATCH (p:ProgramInstruction) WHERE p.instruction =~\'" + instruction + "\'";
    }
    else
        statement = " MATCH (p:ProgramInstruction) WHERE p.instruction CONTAINS  \'" + instruction + "\'";

    /* Step 2: Create the attack state
       Key assumptions here
       1) Each attack state is only related to one data node
     */
    statement += " CREATE ((node:AttackGraphNode{type:\""+getStringEquivalent(attackGraphNode)+"\",label : \'"+ description+" - "+instruction +"\', program_name : \'p.program_name\'})-[:EDGE]->(p)) RETURN node";
    neo4j_result_stream_t *results = neo4j_run(connection,statement.c_str(),neo4j_null );
    if (results == NULL)
    {
        neo4j_perror(stderr, errno, "Failed to run statement");
        return 0;
    }
    neo4j_close_results(results);
    return 0;
}




string Neo4j_Wrapper::getStringEquivalent(AttackGraphNode attackGraphNode)
{
    string node;
    switch(attackGraphNode)
    {
        case AttackSurface:
            node="AttackSurface";
            break;
        case AttackState:
            node="AttackState";
            break;
        case AttackAction:
            node="AttackAction";
            break;
    }
    return node;
}
