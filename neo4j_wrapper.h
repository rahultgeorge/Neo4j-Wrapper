#include <neo4j-client.h>
#include <errno.h>
#include <stdio.h>

namespace Neo4j_Wrapper
{
 class Neo4j_Connector
 {
   private:
            neo4j_connection_t *connection;
   public:
            Neo4j_Connector()
            {
                  neo4j_client_init();
            }
            int connect();
 };

}