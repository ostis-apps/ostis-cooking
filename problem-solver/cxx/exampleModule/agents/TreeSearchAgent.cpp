#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "keynodes/keynodes.hpp"
#include "TreeSearchAgent.hpp"

using namespace exampleModule;

SC_AGENT_IMPLEMENTATION(TreeSearchAgent){
    ScAddr tests = otherAddr;
    ScMemoryContext *context = &m_memoryCtx;
    //ScAddr Graph = context->HelperFindBySystemIdtf("test5");
    SC_LOG_DEBUG("MY agent started");
    ScAddr Del = context->CreateNode(ScType::NodeConst);
    ScIterator3Ptr it = context->Iterator3(tests, ScType::EdgeDCommonConst,ScType::NodeConstStruct);
    while(it->Next())
    {
        ScAddr Graph=it->Get(2);
        context->HelperSetSystemIdtf("Delete", Del);
        SC_LOG_DEBUG("Created DelNode");

        std::vector<ScAddr> vertexes = utils::IteratorUtils::getAllWithType(&m_memoryCtx, Graph, ScType::NodeConst);
        std::vector<ScAddr> edges = utils::IteratorUtils::getAllWithType(&m_memoryCtx, Graph, ScType::EdgeDCommonConst);
        std::vector<ScAddr> checked(0);
        SC_LOG_DEBUG("Created vertexes and edges, vector for checked");

        std::pair<bool, bool> temp;
        bool BEGIN = 1;
        SC_LOG_DEBUG("Find the set of vertexes");
        while (true)
        {
            temp = checked_for_tree(context, Del, vertexes, edges);
            std::cout << temp.first << " " << temp.second << std::endl;

            if (temp.first == 1 && temp.second == 1) {
                SC_LOG_DEBUG("It's a tree");
                break;
            }
            if (temp.second == 0 && BEGIN) {
                SC_LOG_DEBUG("There's a vertex with degree = 0 \n End");
                break;
            }
            if (temp.first == 0 && temp.second == 1 && BEGIN) {
                SC_LOG_DEBUG("Starting to find vertexes");
                BEGIN = 0;
            }
            if (temp.second == 0 && !BEGIN)
                delete_connection(context, Del, checked.back());

            //find a vertex to delete
            checked.push_back(find_max_degree(context, checked, vertexes, Del));
            //try to delete it
            try_to_del(context, Del, checked.back());
        }
    }
    return SC_RESULT_OK;
}

