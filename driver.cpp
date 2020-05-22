#include "neo4j_wrapper.hpp"

//Dummy test case
int main(int argc, char *argv[])
{

    Neo4j_Wrapper::Neo4j_Connector connector;
    connector.connect();
    connector.insert_pdg_node("012","DEFAULT","load i8*,i8** dummy_struct align 8, dbg! 651");
    connector.insert_attack_graph_node(Neo4j_Wrapper::AttackState,"CCured","load i8*,i8** dummy_struct align 8");
    connector.close();
//    neo4j_client_init();
//    /* use NEO4J_INSECURE when connecting to disable TLS  (7687 - ubuntu) (7474 - mac os x) */
//    neo4j_connection_t *connection = neo4j_connect("neo4j://neo4j:secret@localhost:7474", NULL, NEO4J_INSECURE);


//    if (connection == NULL)
//    {
//        neo4j_perror(stderr, errno, "Connection failed");
//        return EXIT_FAILURE;
//    }
//
//    neo4j_result_stream_t *results =
//            neo4j_run(connection, "RETURN 'hello world'", neo4j_null);
//    if (results == NULL)
//    {
//        neo4j_perror(stderr, errno, "Failed to run statement");
//        return EXIT_FAILURE;
//    }
//
//    neo4j_result_t *result = neo4j_fetch_next(results);
//    if (result == NULL)
//    {
//        neo4j_perror(stderr, errno, "Failed to fetch result");
//        return EXIT_FAILURE;
//    }
//
//    neo4j_value_t value = neo4j_result_field(result, 0);
//    char buf[128];
//    printf("%s\n", neo4j_tostring(value, buf, sizeof(buf)));
//
//    neo4j_close_results(results);
//    neo4j_close(connection);
//    neo4j_client_cleanup();
    return EXIT_SUCCESS;
}