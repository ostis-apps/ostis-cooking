
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "keynodes/keynodes.hpp"
#include "CyclicGraphCheckAgent.hpp"

using namespace utils;
using namespace exampleModule;

SC_AGENT_IMPLEMENTATION(CyclicGraphCheckAgent)
{
    ScMemoryContext* context = &m_memoryCtx;

    if (!edgeAddr.IsValid())
        SC_LOG_INFO("Error");

    SC_LOG_DEBUG("CyclicGraphCheckAgent started");

    ScAddr questionNode = otherAddr;
    ScAddr param = utils::IteratorUtils::getAnyByOutRelation(context, questionNode, scAgentsCommon::CoreKeynodes::nrel_inclusion);
    if (!param.IsValid())
    {
        SC_LOG_INFO("Error: invalid params");
        return SC_RESULT_ERROR_INVALID_PARAMS;
    }


    checkCyclicGraph(param);

    return SC_RESULT_OK;
}



bool CyclicGraphCheckAgent::exist(ScAddr nodeNow, std::vector<ScAddr> nodes){

    for(ScAddr node : nodes){
        if (node==nodeNow)
        return true;
    }

    return false;
}


std::vector<ScAddr> CyclicGraphCheckAgent::getAllNodes(ScAddr startNode){
    ScMemoryContext* context = &m_memoryCtx;
    ScIterator3Ptr graphIterator = context->Iterator3(
        startNode,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst
    );
    std::vector<ScAddr> nodes;


    while (graphIterator->Next())
    {
        nodes.push_back(graphIterator->Get(2));
    }
    return nodes;
}


void CyclicGraphCheckAgent::DFS(std::vector<ScAddr> container, std::vector<std::vector<ScAddr>>& cycle_list, std::vector<ScAddr> id_list, ScAddr initial_id, ScAddr current_id){
    ScMemoryContext* context = &m_memoryCtx;

    id_list.push_back(current_id);

    ScIterator3Ptr graphIterator = context->Iterator3(
        current_id,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst
    );

    while(graphIterator->Next()){
        if (prevNode!=graphIterator->Get(2)){
            if(graphIterator->Get(2) == initial_id && id_list.size() > 2){
                cycle_list.push_back(id_list);
            }
            else{
                if(!exist(graphIterator->Get(2), id_list)){
                    prevNode=current_id;                
                    DFS(container, cycle_list, id_list, initial_id, graphIterator->Get(2));
                }
            }
        }
    }

}

    std::vector<std::vector<ScAddr>> CyclicGraphCheckAgent::getCycleList(ScAddr startNode){
        std::vector<std::vector<ScAddr>> res;
        std::vector<ScAddr> id_list;
        std::vector <ScAddr> allNodes = getAllNodes(startNode);
        for(ScAddr node : allNodes){
            DFS(getAllNodes(startNode), res, id_list, node, node);
        }

        return res;

    }

    void CyclicGraphCheckAgent::checkCyclicGraph(ScAddr startNode){

        if (getCycleList(startNode).size()==0){
            SC_LOG_DEBUG("Graph is acyclic");
            exit(0);
        }
        else {
            SC_LOG_DEBUG("Graph is cyclic");
        }
       
    }
