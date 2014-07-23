require 'gecode_composition_management'

include GECODE_COMPOSITION_MANAGEMENT

compA = Component.new(0, "a")
compB = Component.new(1, "b")
compC = Component.new(2, "c")
# And their ports
compA.pushBackOutPort(OutgoingPort.new("std::string","a_out_1"))
compA.pushBackOutPort(OutgoingPort.new("int","a_out_2"))
compA.pushBackInPort(IncomingPort.new("std::string","a_in_1"))
compB.pushBackInPort(IncomingPort.new("std::string","b_in_1"))
compB.pushBackInPort(IncomingPort.new("float","b_in_2"))
compC.pushBackOutPort(OutgoingPort.new("std::string","c_out_1"))
compC.pushBackOutPort(OutgoingPort.new("float","c_out_2"))
compC.pushBackInPort(IncomingPort.new("int","c_in_1"))

# Query components
queryCompA = compA.copy
queryCompB = compB.copy
queryCompC = compC.copy
queryCompAnotherC = compC.copy
queryCompAnotherC.setName("anotherC")
# Configure query components
compAConf = ["3", "1"]
#queryCompA.setConfiguration(compAConf) TODO
# No constraint on B's config
compCConf = ["2"]
# queryCompC.setConfiguration(compCConf) TODO
# Construct query and subquery
query = Query.new("query")
subquery = Query.new("subquery")
query.addComponent(queryCompA)
query.addComponent(queryCompAnotherC)
subquery.addComponent(queryCompB)
subquery.addComponent(queryCompC)


# Configure connections
subquery.addConnection(queryCompC.getName(), subquery.getComponent(queryCompC.getName()).getOutPorts().at(1),
                       queryCompB.getName(), subquery.getComponent(queryCompB.getName()).getInPorts().at(1));
query.integrateSubQuery(subquery)

query.addConnection(queryCompA.getName(), query.getComponent(queryCompA.getName()).getOutPorts().at(0),
                    queryCompB.getName(), query.getComponent(queryCompB.getName()).getInPorts().at(0));
query.addConnection(queryCompA.getName(), query.getComponent(queryCompA.getName()).getOutPorts().at(1),
                    queryCompAnotherC.getName(), query.getComponent(queryCompAnotherC.getName()).getInPorts().at(0));
query.addConnection(queryCompAnotherC.getName(), query.getComponent(queryCompAnotherC.getName()).getOutPorts().at(0),
                    queryCompA.getName(), query.getComponent(queryCompA.getName()).getInPorts().at(0));

# Pool components
poolCompA0 = compA.copy
poolCompA0.setName("a0")
poolCompA1 = compA.copy
poolCompA1.setName("a1")
poolCompB0 = compB.copy
poolCompB0.setName("b0")
poolCompB1 = compB.copy
poolCompB1.setName("b1")
poolCompC0 = compC.copy
poolCompC0.setName("c0")
poolCompC1 = compC.copy
poolCompC1.setName("c1")
# Configure pool components
#compA0Conf = ["3", "3"]
#poolCompA0.setConfiguration(compA0Conf)
#compB0Conf = ["2"]
#compB0Conf.push_back("2")
#poolCompB0.setConfiguration(compB0Conf)
#compC1Conf = ["2"]
#poolCompC1.setConfiguration(compC1Conf)
# Construct pool
pool = Query.new("pool")
pool.addComponent(poolCompA0)
pool.addComponent(poolCompA1)
pool.addComponent(poolCompB0)
pool.addComponent(poolCompB1)
pool.addComponent(poolCompC0)
pool.addComponent(poolCompC1)

# Configure connections
pool.addConnection(poolCompA0.getName(), pool.getComponent(poolCompA0.getName()).getOutPorts().at(0), 
                   poolCompB0.getName(), pool.getComponent(poolCompB0.getName()).getInPorts().at(0));
pool.addConnection(poolCompA0.getName(), pool.getComponent(poolCompA0.getName()).getOutPorts().at(1), 
                   poolCompC1.getName(), pool.getComponent(poolCompC1.getName()).getInPorts().at(0));
pool.addConnection(poolCompC0.getName(), pool.getComponent(poolCompC0.getName()).getOutPorts().at(0), 
                   poolCompA0.getName(), pool.getComponent(poolCompA0.getName()).getInPorts().at(0));
pool.addConnection(poolCompC1.getName(), pool.getComponent(poolCompC1.getName()).getOutPorts().at(1), 
                   poolCompB0.getName(), pool.getComponent(poolCompB0.getName()).getInPorts().at(1));
# Optional:
pool.addConnection(poolCompA0.getName(), pool.getComponent(poolCompA0.getName()).getOutPorts().at(0), 
                   poolCompB1.getName(), pool.getComponent(poolCompB1.getName()).getInPorts().at(0));

# Print pool and query
print "Pool: " + pool.toString()
print "Query: " + query.toString()

s = Solution.babSearch(query, pool)
s.print()