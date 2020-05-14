#!/bin/bash

# Script to setup and install Neo4j driver
echo "Installing boost"
sudo apt install libboost-all-dev


# Install velocypack
git clone https://github.com/arangodb/velocypack.git
cd velocypack
mkdir build
cd build
cmake ..
sudo make install


sudo add-apt-repository ppa:cleishm/neo4j
sudo apt-get update
sudo apt-get install neo4j-client libneo4j-client-dev



# Install neo4j (3.5 cause of bolt dependency)
wget -O - https://debian.neo4j.com/neotechnology.gpg.key | sudo apt-key add -
echo 'deb https://debian.neo4j.com stable 3.5' | sudo tee /etc/apt/sources.list.d/neo4j.list
sudo apt-get update
sudo apt-get install neo4j=1:3.5.9

