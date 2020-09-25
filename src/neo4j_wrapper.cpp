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
                                                    string node_dest_label, string program_name) {
    // Only using label to find the nodes (assuming nodes have been created)

    //Parameterized queries
    neo4j_value_t nodeSourceLabelValue, edgeLabelValue, nodeDestLabelValue, programNameValue;
    neo4j_map_entry_t mapEntries[5];
    nodeSourceLabelValue = neo4j_string(node_source_label.c_str());
    nodeDestLabelValue = neo4j_string(node_dest_label.c_str());
    edgeLabelValue = neo4j_string(edge_label.c_str());
    programNameValue = neo4j_string(program_name.c_str());

    mapEntries[0] = neo4j_map_entry("node_source_label", nodeSourceLabelValue);
    mapEntries[1] = neo4j_map_entry("source_program_name", programNameValue);
    mapEntries[2] = neo4j_map_entry("node_dest_label", nodeDestLabelValue);
    mapEntries[3] = neo4j_map_entry("dest_program_name", programNameValue);
    mapEntries[4] = neo4j_map_entry("edge_label", edgeLabelValue);

    neo4j_value_t params = neo4j_map(mapEntries, 5);

    //TODO - Is it beneficial to merge only using label?
    string statement = "MERGE (p1:ProgramInstruction{label:$node_source_label, program_name:$source_program_name}) MERGE(p2:ProgramInstruction{label:$node_dest_label, program_name:$dest_program_name}) MERGE((p1)-[:EDGE{label:$edge_label}]->(p2))";
    neo4j_result_stream_t *results = neo4j_run(connection, statement.c_str(), params);
    if (results == NULL) {
        neo4j_perror(stderr, errno, "Failed to run statement");
        cout<<"ERROR - FAILED TO INSERT NODE\n";
        return 0;
    }
    neo4j_close_results(results);
    return 1;
}


int Neo4j_Wrapper::Neo4j_Connector::insert_pdg_node(string label, string node_type,
                                                    string instruction, string function_name, string program_name) {

    //Parameterized queries
    neo4j_value_t labelValue, nodeTypeValue, instructionValue, functionNameValue, programNameValue, debugLocationValue;
    neo4j_map_entry_t mapEntries[11];
    labelValue = neo4j_string(label.c_str());
    nodeTypeValue = neo4j_string(node_type.c_str());
    instructionValue = neo4j_string(instruction.c_str());
    functionNameValue = neo4j_string(function_name.c_str());
    programNameValue = neo4j_string(program_name.c_str());
    string debugLocation = "-1";
    size_t pos = instruction.find("!dbg !");
    if (pos != std::string::npos)
        debugLocation = instruction.substr(pos + 6);
    debugLocationValue = neo4j_string(debugLocation.c_str());

    mapEntries[0] = neo4j_map_entry("label", labelValue);
    mapEntries[1] = neo4j_map_entry("node_type", nodeTypeValue);
    mapEntries[2] = neo4j_map_entry("instruction", instructionValue);
    mapEntries[3] = neo4j_map_entry("debug_location", debugLocationValue);
    mapEntries[4] = neo4j_map_entry("function_name", functionNameValue);
    mapEntries[5] = neo4j_map_entry("program_name", programNameValue);

    mapEntries[6] = neo4j_map_entry("node_type_2", nodeTypeValue);
    mapEntries[7] = neo4j_map_entry("instruction_2", instructionValue);
    mapEntries[8] = neo4j_map_entry("debug_location_2", debugLocationValue);
    mapEntries[9] = neo4j_map_entry("function_name_2", functionNameValue);
    mapEntries[10] = neo4j_map_entry("program_name_2", programNameValue);

    //TODO - Is it beneficial to merge only using label?
    string statement = "MERGE (n:ProgramInstruction{label:$label}) ON MATCH SET n.type=$node_type, n.instruction=$instruction , n.debug_location=$debug_location, n.function_name=$function_name, n.program_name= $program_name ON CREATE SET  n.type=$node_type_2, n.instruction=$instruction_2 , n.debug_location=$debug_location_2, n.function_name=$function_name_2, n.program_name= $program_name_2";

    neo4j_value_t params = neo4j_map(mapEntries, 11);

    neo4j_result_stream_t *results = neo4j_run(connection, statement.c_str(), params);
    if (results == NULL) {
        neo4j_perror(stderr, errno, "Failed to run statement");
        cout<<"ERROR - FAILED TO INSERT NODE\n";
        return 0;
    }
    neo4j_close_results(results);
    return 1;
}


bool is_number(const std::string &s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


int Neo4j_Wrapper::Neo4j_Connector::insert_attack_graph_node(AttackGraphNode attackGraphNode, string description,
                                                             string instruction, string function_name,
                                                             string programName) {
    // Step 1: Find the underlying PDG node - Use the debug location if provided
    string statement;

    //Parameterized queries
    neo4j_value_t labelValue, nodeTypeValue, instructionValue, functionNameValue, programNameValue, debugLocationValue;
    neo4j_map_entry_t mapEntries[6];
    labelValue = neo4j_string(description.c_str());
    nodeTypeValue = neo4j_string(getStringEquivalent(attackGraphNode).c_str());
    instructionValue = neo4j_string(instruction.c_str());
    functionNameValue = neo4j_string(function_name.c_str());
    programNameValue = neo4j_string(programName.c_str());
    string debugLocation = "-1";
    size_t pos = instruction.find("!dbg !");
    if (pos != std::string::npos)
        debugLocation = instruction.substr(pos + 6);
    debugLocationValue = neo4j_string(debugLocation.c_str());

    mapEntries[0] = neo4j_map_entry("label", labelValue);
    mapEntries[1] = neo4j_map_entry("node_type", nodeTypeValue);
    mapEntries[2] = neo4j_map_entry("instruction", instructionValue);
    mapEntries[3] = neo4j_map_entry("debug_location", debugLocationValue);
    mapEntries[4] = neo4j_map_entry("function_name", functionNameValue);
    mapEntries[5] = neo4j_map_entry("program_name", programNameValue);


    if (attackGraphNode == AttackState) {
        if (instruction.find("alloca") != std::string::npos)
            statement = " MATCH (p:ProgramInstruction) WHERE p.instruction CONTAINS  \'" + instruction +
                        "\' AND p.function_name =\'" + function_name + "\' AND p.program_name =\'" + programName + "\'";
        else //Regex instruction pattern
            statement = " MATCH (p:ProgramInstruction) WHERE p.instruction =~\'" + instruction +
                        "\' AND p.program_name =\'" + programName + "\'";
    } else
        statement = " MATCH (p:ProgramInstruction) WHERE p.instruction CONTAINS \'" + instruction +
                    "\' AND p.program_name =\'" + programName + "\'";

    /* Step 2: Create the attack state
       Key assumptions here
       1) Each attack state is only related to one data node
     */
    statement += " CREATE ((node:AttackGraphNode{type:\"" + getStringEquivalent(attackGraphNode) + "\",label : \'" +
                 description + " - " + instruction + "\', program_name : p.program_name})-[:EDGE]->(p)) RETURN node";
    neo4j_result_stream_t *results = neo4j_run(connection, statement.c_str(), neo4j_null);
    if (results == NULL) {
        neo4j_perror(stderr, errno, "Failed to run statement");
        return 0;
    }
    neo4j_close_results(results);
    return 0;
}


string Neo4j_Wrapper::getStringEquivalent(AttackGraphNode attackGraphNode) {
    string node;
    switch (attackGraphNode) {
        case AttackSurface:
            node = "AttackSurface";
            break;
        case AttackState:
            node = "AttackState";
            break;
        case AttackAction:
            node = "AttackAction";
            break;
    }
    return node;
}
